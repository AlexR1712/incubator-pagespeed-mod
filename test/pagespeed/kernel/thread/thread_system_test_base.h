/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *   http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#ifndef PAGESPEED_KERNEL_THREAD_THREAD_SYSTEM_TEST_BASE_H_
#define PAGESPEED_KERNEL_THREAD_THREAD_SYSTEM_TEST_BASE_H_

#include "pagespeed/kernel/base/basictypes.h"
#include "pagespeed/kernel/base/scoped_ptr.h"
#include "pagespeed/kernel/base/thread_system.h"
#include "test/pagespeed/kernel/base/gtest.h"
#include "test/pagespeed/kernel/base/mock_message_handler.h"

namespace net_instaweb {

class ThreadSystemTestBase : public testing::Test {
 public:
  // Data transfer between thread & main.
  void set_ok_flag(bool ok) { ok_flag_ = ok; }
  bool ok_flag() const { return ok_flag_; }

  ThreadSystem* thread_system() const { return thread_system_.get(); }

 protected:
  // Takes ownership of 'thread_system'
  explicit ThreadSystemTestBase(ThreadSystem* thread_system);

  // Test simple start & join.
  void TestStartJoin();

  // Very basic use of synchronization --- waiting for thread
  // to notify us. Also tests detached execution.
  void TestSync();

 private:
  bool ok_flag_;
  std::unique_ptr<ThreadSystem> thread_system_;
  MockMessageHandler handler_;

  DISALLOW_COPY_AND_ASSIGN(ThreadSystemTestBase);
};

// Passes in the appropriate ThreadSystem to ThreadSystemTestBase via a template
// param to help glue us to the test framework
template <typename ToTest>
class ThreadSystemTestTemplate : public ThreadSystemTestBase {
 public:
  ThreadSystemTestTemplate() : ThreadSystemTestBase(new ToTest) {}
};

TYPED_TEST_SUITE_P(ThreadSystemTestTemplate);

TYPED_TEST_P(ThreadSystemTestTemplate, TestStartJoin) {
  ThreadSystemTestBase::TestStartJoin();
}

TYPED_TEST_P(ThreadSystemTestTemplate, TestSync) {
  ThreadSystemTestBase::TestSync();
}

REGISTER_TYPED_TEST_SUITE_P(ThreadSystemTestTemplate, TestStartJoin, TestSync);

}  // namespace net_instaweb

#endif  // PAGESPEED_KERNEL_THREAD_THREAD_SYSTEM_TEST_BASE_H_
