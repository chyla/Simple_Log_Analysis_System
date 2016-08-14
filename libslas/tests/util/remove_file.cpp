#include <gtest/gtest.h>

#include <slas/util/remove_file.h>
#include <slas/util/detail/cant_remove_file_exception.h>

#include "tests/mock/util/detail/system.h"

using namespace testing;
using namespace util;
using namespace std;

class UtilRemoveFile : public ::testing::Test {
 public:

  UtilRemoveFile() {
    path = "/var/run/test.pid";
  }

  void SetUp() {
    system = make_shared<mock::util::detail::System>();
  }

  void TearDown() {
  }

  virtual ~UtilRemoveFile() {
  }

  shared_ptr<mock::util::detail::System> system;
  const char *path;
};

TEST_F(UtilRemoveFile, RemoveFile_CorrectRemoval) {
  EXPECT_CALL(*system, unlink(StrEq(path))).WillOnce(Return(0));

  RemoveFile(path, system);
}

TEST_F(UtilRemoveFile, RemoveFile_WhenUnlinkFail) {
  EXPECT_CALL(*system, unlink(StrEq(path))).WillOnce(Return(-1));

  EXPECT_THROW(RemoveFile(path, system), util::detail::CantRemoveFileException);
}
