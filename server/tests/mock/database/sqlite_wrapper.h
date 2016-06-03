/*
 * Copyright 2016 Adam Chyła, adam@chyla.org
 * All rights reserved. Distributed under the terms of the MIT License.
 */

#ifndef TESTS_MOCK_DATABASE_SQLITE_WRAPPER_H
#define TESTS_MOCK_DATABASE_SQLITE_WRAPPER_H

#include "src/database/detail/sqlite_wrapper_interface.h"

#include <memory>
#include <gmock/gmock.h>

namespace mock
{

namespace database
{

class SQLiteWrapper;
typedef std::shared_ptr<SQLiteWrapper> SQLiteWrapperPtr;

class SQLiteWrapper : public ::database::detail::SQLiteWrapperInterface {
 public:
  virtual ~SQLiteWrapper() = default;

  static SQLiteWrapperPtr Create() {
    return std::make_shared<SQLiteWrapper>();
  }

  MOCK_METHOD1(Open, void(const std::string &file_path));
  MOCK_METHOD0(IsOpen, bool());
  MOCK_METHOD0(Close, bool());

  MOCK_METHOD2(Prepare, void(const std::string &sql, sqlite3_stmt **ppStmt));

  MOCK_METHOD3(BindDouble, void(sqlite3_stmt *pStmt, int pos, double value));
  MOCK_METHOD3(BindInt, void(sqlite3_stmt *pStmt, int pos, int value));
  MOCK_METHOD3(BindInt64, void(sqlite3_stmt *pStmt, int pos, sqlite3_int64 value));
  MOCK_METHOD3(BindText, void(sqlite3_stmt *pStmt, int pos, const std::string &value));

  MOCK_METHOD2(ColumnDouble, double(sqlite3_stmt *pStmt, int iCol));
  MOCK_METHOD2(ColumnInt, int(sqlite3_stmt *pStmt, int iCol));
  MOCK_METHOD2(ColumnInt64, sqlite3_int64(sqlite3_stmt *pStmt, int iCol));
  MOCK_METHOD2(ColumnText, const std::string(sqlite3_stmt *pStmt, int iCol));

  MOCK_METHOD1(Step, int(sqlite3_stmt *pStmt));
  MOCK_METHOD1(Finalize, void(sqlite3_stmt *pStmt));

  MOCK_METHOD3(Exec, void(const std::string &sql, int (*callback) (void *, int, char **, char **), void *arg));

  MOCK_METHOD0(GetSQLiteHandle, sqlite3*());
};

}

}

#endif /* TESTS_MOCK_DATABASE_SQLITE_WRAPPER_H */
