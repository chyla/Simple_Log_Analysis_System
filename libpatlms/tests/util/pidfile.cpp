#include <gtest/gtest.h>

#include <patlms/util/pidfile.h>
#include <patlms/util/detail/cant_create_pidfile_exception.h>
#include <patlms/util/detail/cant_remove_pidfile_exception.h>

#include "tests/mock/util/detail/system.h"

using namespace testing;
using namespace std;
using namespace util;

constexpr FILE* FILE_POINTER = reinterpret_cast<FILE*> (0x03);
constexpr int PID = 213;

class UtilPidfileTest : public ::testing::Test {
 public:

  UtilPidfileTest() {
    pidfile_path = "/var/run/test.pid";
  }

  void SetUp() {
    system = make_shared<mock::util::detail::System>();
  }

  void TearDown() {
  }

  virtual ~UtilPidfileTest() {
  }

  shared_ptr<mock::util::detail::System> system;
  const char *pidfile_path;
};

TEST_F(UtilPidfileTest, CreatePidFile_CorrectPidCreation) {
  EXPECT_CALL(*system, access(StrEq(pidfile_path), F_OK)).WillOnce(Return(-1));
  EXPECT_CALL(*system, fopen(StrEq(pidfile_path), StrEq("w"))).WillOnce(Return(FILE_POINTER));
  EXPECT_CALL(*system, getpid()).WillOnce(Return(PID));
  EXPECT_CALL(*system, fwrite(NotNull(), 3, 1, FILE_POINTER)).WillOnce(Return(3));
  EXPECT_CALL(*system, fclose(FILE_POINTER)).WillOnce(Return(0));

  CreatePidFile(pidfile_path, system);
}

TEST_F(UtilPidfileTest, CreatePidFile_WhenPidFileExists) {
  EXPECT_CALL(*system, access(StrEq(pidfile_path), F_OK)).WillOnce(Return(0));

  EXPECT_THROW(CreatePidFile(pidfile_path, system), util::detail::CantCreatePidfileException);
}

TEST_F(UtilPidfileTest, CreatePidFile_WhenFopenFail) {
  EXPECT_CALL(*system, access(StrEq(pidfile_path), F_OK)).WillOnce(Return(-1));
  EXPECT_CALL(*system, fopen(StrEq(pidfile_path), StrEq("w"))).WillOnce(Return(nullptr));

  EXPECT_THROW(CreatePidFile(pidfile_path, system), util::detail::CantCreatePidfileException);
}

TEST_F(UtilPidfileTest, CreatePidFile_WhenFwriteFail) {
  EXPECT_CALL(*system, access(StrEq(pidfile_path), F_OK)).WillOnce(Return(-1));
  EXPECT_CALL(*system, fopen(StrEq(pidfile_path), StrEq("w"))).WillOnce(Return(FILE_POINTER));
  EXPECT_CALL(*system, getpid()).WillOnce(Return(PID));
  EXPECT_CALL(*system, fwrite(NotNull(), 3, 1, FILE_POINTER)).WillOnce(Return(-1));
  EXPECT_CALL(*system, fclose(FILE_POINTER)).WillOnce(Return(0));

  EXPECT_THROW(CreatePidFile(pidfile_path, system), util::detail::CantCreatePidfileException);
}

TEST_F(UtilPidfileTest, CreatePidFile_WhenFwriteAndCloseFail) {
  EXPECT_CALL(*system, access(StrEq(pidfile_path), F_OK)).WillOnce(Return(-1));
  EXPECT_CALL(*system, fopen(StrEq(pidfile_path), StrEq("w"))).WillOnce(Return(FILE_POINTER));
  EXPECT_CALL(*system, getpid()).WillOnce(Return(PID));
  EXPECT_CALL(*system, fwrite(NotNull(), 3, 1, FILE_POINTER)).WillOnce(Return(-1));
  EXPECT_CALL(*system, fclose(FILE_POINTER)).WillOnce(Return(-1));

  EXPECT_THROW(CreatePidFile(pidfile_path, system), util::detail::CantCreatePidfileException);
}

TEST_F(UtilPidfileTest, CreatePidFile_WhenFwriteWritesTextPartially) {
  EXPECT_CALL(*system, access(StrEq(pidfile_path), F_OK)).WillOnce(Return(-1));
  EXPECT_CALL(*system, fopen(StrEq(pidfile_path), StrEq("w"))).WillOnce(Return(FILE_POINTER));
  EXPECT_CALL(*system, getpid()).WillOnce(Return(PID));
  EXPECT_CALL(*system, fwrite(NotNull(), 3, 1, FILE_POINTER)).WillOnce(Return(2));
  EXPECT_CALL(*system, fclose(FILE_POINTER)).WillOnce(Return(0));

  EXPECT_THROW(CreatePidFile(pidfile_path, system), util::detail::CantCreatePidfileException);
}

TEST_F(UtilPidfileTest, CreatePidFile_WhenFwriteWritesTextPartiallyAndCloseFail) {
  EXPECT_CALL(*system, access(StrEq(pidfile_path), F_OK)).WillOnce(Return(-1));
  EXPECT_CALL(*system, fopen(StrEq(pidfile_path), StrEq("w"))).WillOnce(Return(FILE_POINTER));
  EXPECT_CALL(*system, getpid()).WillOnce(Return(PID));
  EXPECT_CALL(*system, fwrite(NotNull(), 3, 1, FILE_POINTER)).WillOnce(Return(2));
  EXPECT_CALL(*system, fclose(FILE_POINTER)).WillOnce(Return(-1));

  EXPECT_THROW(CreatePidFile(pidfile_path, system), util::detail::CantCreatePidfileException);
}

TEST_F(UtilPidfileTest, RemovePidFile_CorrectRemoval) {
  EXPECT_CALL(*system, unlink(StrEq(pidfile_path))).WillOnce(Return(0));

  RemovePidFile(pidfile_path, system);
}

TEST_F(UtilPidfileTest, RemovePidFile_WhenUnlinkFail) {
  EXPECT_CALL(*system, unlink(StrEq(pidfile_path))).WillOnce(Return(-1));

  EXPECT_THROW(RemovePidFile(pidfile_path, system), util::detail::CantRemovePidfileException);
}
