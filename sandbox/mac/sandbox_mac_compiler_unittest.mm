// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <fcntl.h>
#include <stdint.h>
#include <sys/stat.h>
#include <unistd.h>

#include "base/process/kill.h"
#include "base/test/multiprocess_test.h"
#include "base/test/test_timeouts.h"
#include "sandbox/mac/sandbox_compiler.h"
#include "testing/gtest/include/gtest/gtest.h"
#include "testing/multiprocess_func_list.h"

namespace sandbox {

class SandboxMacCompilerTest : public base::MultiProcessTest {};

MULTIPROCESS_TEST_MAIN(BasicProfileProcess) {
  std::string profile =
      "(version 1)"
      "(allow file-read* file-write* (literal \"/\"))";

  SandboxCompiler compiler(profile);

  std::string error;
  CHECK(compiler.CompileAndApplyProfile(&error));

  return 0;
}

TEST_F(SandboxMacCompilerTest, BasicProfileTest) {
  base::SpawnChildResult spawn_child = SpawnChild("BasicProfileProcess");
  ASSERT_TRUE(spawn_child.process.IsValid());
  int exit_code = 42;
  EXPECT_TRUE(spawn_child.process.WaitForExitWithTimeout(
      TestTimeouts::action_max_timeout(), &exit_code));
  EXPECT_EQ(exit_code, 0);
}

MULTIPROCESS_TEST_MAIN(BasicProfileWithParamProcess) {
  std::string profile =
      "(version 1)"
      "(allow file-read* file-write* (literal (param \"DIR\")))";

  SandboxCompiler compiler(profile);
  CHECK(compiler.InsertStringParam("DIR", "/"));

  std::string error;
  CHECK(compiler.CompileAndApplyProfile(&error));

  return 0;
}

TEST_F(SandboxMacCompilerTest, BasicProfileTestWithParam) {
  base::SpawnChildResult spawn_child =
      SpawnChild("BasicProfileWithParamProcess");
  ASSERT_TRUE(spawn_child.process.IsValid());
  int exit_code = 42;
  EXPECT_TRUE(spawn_child.process.WaitForExitWithTimeout(
      TestTimeouts::action_max_timeout(), &exit_code));
  EXPECT_EQ(exit_code, 0);
}

MULTIPROCESS_TEST_MAIN(ProfileFunctionalProcess) {
  std::string profile =
      "(version 1)"
      "(debug deny)"
      "(allow file-read-data file-read-metadata (literal \"/dev/urandom\"))";

  SandboxCompiler compiler(profile);

  std::string error;
  CHECK(compiler.CompileAndApplyProfile(&error));

  // The profile compiled and applied successfully, now try and read 1 byte from
  // /dev/urandom.
  uint8_t byte;
  int fd = open("/dev/urandom", O_RDONLY);
  CHECK_NE(fd, -1);

  EXPECT_TRUE(read(fd, &byte, sizeof(byte)) == sizeof(byte));

  return 0;
}

TEST_F(SandboxMacCompilerTest, ProfileFunctionalityTest) {
  base::SpawnChildResult spawn_child = SpawnChild("ProfileFunctionalProcess");
  ASSERT_TRUE(spawn_child.process.IsValid());
  int exit_code = 42;
  EXPECT_TRUE(spawn_child.process.WaitForExitWithTimeout(
      TestTimeouts::action_max_timeout(), &exit_code));
  EXPECT_EQ(exit_code, 0);
}

MULTIPROCESS_TEST_MAIN(ProfileFunctionalTestWithParamsProcess) {
  std::string profile =
      "(version 1)"
      "(debug deny)"
      "(if (string=? (param \"ALLOW_FILE\") \"TRUE\")"
      "    (allow file-read-data file-read-metadata (literal (param "
      "\"URANDOM\"))))";

  SandboxCompiler compiler(profile);

  CHECK(compiler.InsertBooleanParam("ALLOW_FILE", true));
  CHECK(compiler.InsertStringParam("URANDOM", "/dev/urandom"));

  std::string error;
  CHECK(compiler.CompileAndApplyProfile(&error));

  // The profile compiled and applied successfully, now try and read 1 byte from
  // /dev/urandom.
  uint8_t byte;
  int fd = open("/dev/urandom", O_RDONLY);
  CHECK_NE(fd, -1);

  EXPECT_TRUE(read(fd, &byte, sizeof(byte)) == sizeof(byte));

  // Make sure the sandbox isn't overly permissive.
  struct stat st;
  EXPECT_EQ(stat("/", &st), -1);

  return 0;
}

TEST_F(SandboxMacCompilerTest, ProfileFunctionalityTestWithParams) {
  base::SpawnChildResult spawn_child =
      SpawnChild("ProfileFunctionalTestWithParamsProcess");
  ASSERT_TRUE(spawn_child.process.IsValid());
  int exit_code = 42;
  EXPECT_TRUE(spawn_child.process.WaitForExitWithTimeout(
      TestTimeouts::action_max_timeout(), &exit_code));
  EXPECT_EQ(exit_code, 0);
}

MULTIPROCESS_TEST_MAIN(ProfileFunctionalityTestErrorProcess) {
  std::string profile = "(+ 5 a)";

  SandboxCompiler compiler(profile);

  // Make sure that this invalid profile results in an error returned.
  std::string error;
  CHECK_EQ(error, "");
  CHECK(!compiler.CompileAndApplyProfile(&error));
  CHECK_NE(error, "");

  return 0;
}

TEST_F(SandboxMacCompilerTest, ProfileFunctionalityTestError) {
  base::SpawnChildResult spawn_child =
      SpawnChild("ProfileFunctionalityTestErrorProcess");
  ASSERT_TRUE(spawn_child.process.IsValid());
  int exit_code = 42;
  EXPECT_TRUE(spawn_child.process.WaitForExitWithTimeout(
      TestTimeouts::action_max_timeout(), &exit_code));
  EXPECT_EQ(exit_code, 0);
}

}  // namespace sandbox
