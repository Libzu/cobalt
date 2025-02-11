// Copyright 2015 The Cobalt Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "starboard/common/socket.h"
#include "starboard/nplb/socket_helpers.h"
#include "starboard/socket_waiter.h"
#include "testing/gtest/include/gtest/gtest.h"

namespace starboard {
namespace nplb {
namespace {

void NoOpSocketWaiterCallback(SbSocketWaiter waiter,
                              int socket,
                              void* context,
                              int ready_interests) {}

TEST(SbPosixSocketWaiterRemoveTest, RainyDayInvalidSocket) {
  SbSocketWaiter waiter = SbSocketWaiterCreate();
  EXPECT_TRUE(SbSocketWaiterIsValid(waiter));

  EXPECT_FALSE(SbPosixSocketWaiterRemove(waiter, -1));

  EXPECT_TRUE(SbSocketWaiterDestroy(waiter));
}

TEST(SbPosixSocketWaiterRemoveTest, RainyDayInvalidWaiter) {
  int socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  ASSERT_TRUE(socket >= 0);

  EXPECT_FALSE(SbPosixSocketWaiterRemove(kSbSocketWaiterInvalid, socket));

  EXPECT_TRUE(close(socket) == 0);
}

TEST(SbPosixSocketWaiterRemoveTest, RainyDayNotAdded) {
  SbSocketWaiter waiter = SbSocketWaiterCreate();
  EXPECT_TRUE(SbSocketWaiterIsValid(waiter));
  int socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  ASSERT_TRUE(socket >= 0);

  EXPECT_FALSE(SbPosixSocketWaiterRemove(waiter, socket));

  EXPECT_TRUE(close(socket) == 0);
  EXPECT_TRUE(SbSocketWaiterDestroy(waiter));
}

TEST(SbPosixSocketWaiterRemoveTest, RainyDayAlreadyRemoved) {
  SbSocketWaiter waiter = SbSocketWaiterCreate();
  EXPECT_TRUE(SbSocketWaiterIsValid(waiter));
  int socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  ASSERT_TRUE(socket >= 0);
  EXPECT_TRUE(SbPosixSocketWaiterAdd(
      waiter, socket, NULL, &NoOpSocketWaiterCallback,
      kSbSocketWaiterInterestRead | kSbSocketWaiterInterestWrite, false));

  EXPECT_TRUE(SbPosixSocketWaiterRemove(waiter, socket));
  EXPECT_FALSE(SbPosixSocketWaiterRemove(waiter, socket));

  EXPECT_TRUE(close(socket) == 0);
  EXPECT_TRUE(SbSocketWaiterDestroy(waiter));
}

}  // namespace
}  // namespace nplb
}  // namespace starboard
