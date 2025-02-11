// Copyright 2021 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ASH_QUICK_PAIR_FAST_PAIR_HANDSHAKE_FAKE_FAST_PAIR_DATA_ENCRYPTOR_H_
#define ASH_QUICK_PAIR_FAST_PAIR_HANDSHAKE_FAKE_FAST_PAIR_DATA_ENCRYPTOR_H_

#include "ash/quick_pair/fast_pair_handshake/fast_pair_data_encryptor.h"

namespace ash {
namespace quick_pair {

class FakeFastPairDataEncryptor : public FastPairDataEncryptor {
 public:
  FakeFastPairDataEncryptor();
  FakeFastPairDataEncryptor(const FakeFastPairDataEncryptor&) = delete;
  FakeFastPairDataEncryptor& operator=(const FakeFastPairDataEncryptor&) =
      delete;
  ~FakeFastPairDataEncryptor() override;

  void public_key(absl::optional<std::array<uint8_t, 64>> public_key) {
    public_key_ = std::move(public_key);
  }

  void encrypted_bytes(std::array<uint8_t, kBlockSizeBytes> encrypted_bytes) {
    encrypted_bytes_ = std::move(encrypted_bytes);
  }

  void response(absl::optional<DecryptedResponse> response) {
    response_ = std::move(response);
  }

  void passkey(absl::optional<DecryptedPasskey> passkey) {
    passkey_ = std::move(passkey);
  }

  void additional_encrypted_bytes(
      std::vector<uint8_t> additional_encrypted_bytes) {
    additional_data_packet_encrypted_bytes_ =
        std::move(additional_encrypted_bytes);
  }

  // FastPairDataEncryptor:
  const std::array<uint8_t, kBlockSizeBytes> EncryptBytes(
      const std::array<uint8_t, kBlockSizeBytes>& bytes_to_encrypt) override;
  const absl::optional<std::array<uint8_t, 64>>& GetPublicKey() override;
  void ParseDecryptedResponse(
      const std::vector<uint8_t>& encrypted_response_bytes,
      base::OnceCallback<void(const absl::optional<DecryptedResponse>&)>
          callback) override;
  void ParseDecryptedPasskey(
      const std::vector<uint8_t>& encrypted_passkey_bytes,
      base::OnceCallback<void(const absl::optional<DecryptedPasskey>&)>
          callback) override;
  std::vector<uint8_t> CreateAdditionalDataPacket(
      std::array<uint8_t, kNonceSizeBytes> nonce,
      const std::vector<uint8_t>& additional_data) override;

 private:
  absl::optional<std::array<uint8_t, 64>> public_key_ = absl::nullopt;
  std::array<uint8_t, kBlockSizeBytes> encrypted_bytes_ = {};
  std::vector<uint8_t> additional_data_packet_encrypted_bytes_ = {};
  absl::optional<DecryptedResponse> response_ = absl::nullopt;
  absl::optional<DecryptedPasskey> passkey_ = absl::nullopt;
};

}  // namespace quick_pair
}  // namespace ash

#endif  // ASH_QUICK_PAIR_FAST_PAIR_HANDSHAKE_FAKE_FAST_PAIR_DATA_ENCRYPTOR_H_
