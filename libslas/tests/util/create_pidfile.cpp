#include <gtest/gtest.h>

#include <slas/util/create_pidfile.h>
#include <slas/util/detail/cant_create_pidfile_exception.h>

#include "tests/mock/util/detail/system.h"

using namespace testing;
using namespace std;
using namespace util;

constexpr FILE* FILE_POINTER = reinterpret_cast<FILE*> (0x03);
constexpr int PID = 213;

class UtilCreatePidfileTest : public ::testing::Test {
 public:

  UtilCreatePidfileTest() {
    pidfile_path = "/var/run/test.pid";
  }

  void SetUp() {
    system = make_shared<mock::util::detail::System>();
  }

  void TearDown() {
  }

  virtual ~UtilCreatePidfileTest() {
  }

  shared_ptr<mock::util::detail::System> system;
  const char *pidfile_path;
};

TEST_F(UtilCreatePidfileTest, CreatePidFile_CorrectPidCreation) {
  EXPECT_CALL(*system, access(StrEq(pidfile_path), F_OK)).WillOnce(Return(-1));
  EXPECT_CALL(*system, fopen(StrEq(pidfile_path), StrEq("w"))).WillOnce(Return(FILE_POINTER));
  EXPECT_CALL(*system, getpid()).WillOnce(Return(PID));
  EXPECT_CALL(*system, fwrite(NotNull(), 1, 3, FILE_POINTER)).WillOnce(Return(3));
  EXPECT_CALL(*system, fclose(FILE_POINTER)).WillOnce(Return(0));

  CreatePidFile(pidfile_path, system);
}

TEST_F(UtilCreatePidfileTest, CreatePidFile_WhenPidFileExists) {
  EXPECT_CALL(*system, access(StrEq(pidfile_path), F_OK)).WillOnce(Return(0));

  EXPECT_THROW(CreatePidFile(pidfile_path, system), util::detail::CantCreatePidfileException);
}

TEST_F(UtilCreatePidfileTest, CreatePidFile_WhenFopenFail) {
  EXPECT_CALL(*system, access(StrEq(pidfile_path), F_OK)).WillOnce(Return(-1));
  EXPECT_CALL(*system, fopen(StrEq(pidfile_path), StrEq("w"))).WillOnce(Return(nullptr));

  EXPECT_THROW(CreatePidFile(pidfile_path, system), util::detail::CantCreatePidfileException);
}

TEST_F(UtilCreatePidfileTest, CreatePidFile_WhenFwriteFail) {
  EXPECT_CALL(*system, access(StrEq(pidfile_path), F_OK)).WillOnce(Return(-1));
  EXPECT_CALL(*system, fopen(StrEq(pidfile_path), StrEq("w"))).WillOnce(Return(FILE_POINTER));
  EXPECT_CALL(*system, getpid()).WillOnce(Return(PID));
  EXPECT_CALL(*system, fwrite(NotNull(), 1, 3, FILE_POINTER)).WillOnce(Return(-1));
  EXPECT_CALL(*system, fclose(FILE_POINTER)).WillOnce(Return(0));

  EXPECT_THROW(CreatePidFile(pidfile_path, system), util::detail::CantCreatePidfileException);
}

TEST_F(UtilCreatePidfileTest, CreatePidFile_WhenFwriteAndCloseFail) {
  EXPECT_CALL(*system, access(StrEq(pidfile_path), F_OK)).WillOnce(Return(-1));
  EXPECT_CALL(*system, fopen(StrEq(pidfile_path), StrEq("w"))).WillOnce(Return(FILE_POINTER));
  EXPECT_CALL(*system, getpid()).WillOnce(Return(PID));
  EXPECT_CALL(*system, fwrite(NotNull(), 1, 3, FILE_POINTER)).WillOnce(Return(-1));
  EXPECT_CALL(*system, fclose(FILE_POINTER)).WillOnce(Return(-1));

  EXPECT_THROW(CreatePidFile(pidfile_path, system), util::detail::CantCreatePidfileException);
}

TEST_F(UtilCreatePidfileTest, CreatePidFile_WhenFwriteWritesTextPartially) {
  EXPECT_CALL(*system, access(StrEq(pidfile_path), F_OK)).WillOnce(Return(-1));
  EXPECT_CALL(*system, fopen(StrEq(pidfile_path), StrEq("w"))).WillOnce(Return(FILE_POINTER));
  EXPECT_CALL(*system, getpid()).WillOnce(Return(PID));
  EXPECT_CALL(*system, fwrite(NotNull(), 1, 3, FILE_POINTER)).WillOnce(Return(2));
  EXPECT_CALL(*system, fclose(FILE_POINTER)).WillOnce(Return(0));

  EXPECT_THROW(CreatePidFile(pidfile_path, system), util::detail::CantCreatePidfileException);
}

TEST_F(UtilCreatePidfileTest, CreatePidFile_WhenFwriteWritesTextPartiallyAndCloseFail) {
  EXPECT_CALL(*system, access(StrEq(pidfile_path), F_OK)).WillOnce(Return(-1));
  EXPECT_CALL(*system, fopen(StrEq(pidfile_path), StrEq("w"))).WillOnce(Return(FILE_POINTER));
  EXPECT_CALL(*system, getpid()).WillOnce(Return(PID));
  EXPECT_CALL(*system, fwrite(NotNull(), 1, 3, FILE_POINTER)).WillOnce(Return(2));
  EXPECT_CALL(*system, fclose(FILE_POINTER)).WillOnce(Return(-1));

  EXPECT_THROW(CreatePidFile(pidfile_path, system), util::detail::CantCreatePidfileException);
}
