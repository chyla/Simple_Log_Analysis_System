#include <gtest/gtest.h>
#include <string>

#include <slas/util/path.h>

using namespace testing;
using namespace std;
using namespace util;

TEST(UtilTest, IsAbsoluteUnixPath_WithoutEndingShlash) {
  string path = "/var/log/file";
  
  EXPECT_TRUE(IsAbsoluteUnixPath(path));
}

TEST(UtilTest, IsAbsoluteUnixPath_WithUnderscore) {
  string path = "/va_r/l_og/fi_le";
  
  EXPECT_TRUE(IsAbsoluteUnixPath(path));
}

TEST(UtilTest, IsAbsoluteUnixPath_WithMinus) {
  string path = "/v-ar/lo-g/fi-le";
  
  EXPECT_TRUE(IsAbsoluteUnixPath(path));
}

TEST(UtilTest, IsAbsoluteUnixPath_NameWithDot) {
  string path = "/var/log/file.dot";
  
  EXPECT_TRUE(IsAbsoluteUnixPath(path));
}

TEST(UtilTest, IsAbsoluteUnixPath_DirectoryWithDot) {
  string path = "/var/l.og/filedot";
  
  EXPECT_TRUE(IsAbsoluteUnixPath(path));
}

TEST(UtilTest, IsAbsoluteUnixPath_WithEndingShlash) {
  string path = "/var/log/";
  
  EXPECT_TRUE(IsAbsoluteUnixPath(path));
}

TEST(UtilTest, IsAbsoluteUnixPath_WithTwoShlashes) {
  string path = "/var/log//file";
  
  EXPECT_TRUE(IsAbsoluteUnixPath(path));
}

TEST(UtilTest, IsAbsoluteUnixPath_WithThreeShlashes) {
  string path = "/var///log//file";
  
  EXPECT_TRUE(IsAbsoluteUnixPath(path));
}

TEST(UtilTest, IsAbsoluteUnixPath_WithThreeBeginingShlashes) {
  string path = "///var/log//file";
  
  EXPECT_TRUE(IsAbsoluteUnixPath(path));
}

TEST(UtilTest, IsAbsoluteUnixPath_RelativePath) {
  string path = "log/file";
  
  EXPECT_FALSE(IsAbsoluteUnixPath(path));
}

TEST(UtilTest, IsAbsoluteUnixPath_RelativePathOneDot) {
  string path = ".";
  
  EXPECT_FALSE(IsAbsoluteUnixPath(path));
}

TEST(UtilTest, IsAbsoluteUnixPath_RelativePathTwoDots) {
  string path = "..";
  
  EXPECT_FALSE(IsAbsoluteUnixPath(path));
}

TEST(UtilTest, IsAbsoluteUnixPath_RelativePathWithDots) {
  string path = "../log/file";
  
  EXPECT_FALSE(IsAbsoluteUnixPath(path));
}

TEST(UtilTest, IsAbsoluteUnixPath_EmptyPath) {
  string path = "";
  
  EXPECT_FALSE(IsAbsoluteUnixPath(path));
}
