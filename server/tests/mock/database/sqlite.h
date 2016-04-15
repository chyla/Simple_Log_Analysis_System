#ifndef PATLMS_TESTS_MOCK_DATABASE_SQLITE_H
#define PATLMS_TESTS_MOCK_DATABASE_SQLITE_H

#include <gmock/gmock.h>

#include "src/database/detail/sqlite_interface.h"


namespace mock
{

namespace database
{

class SQLite : public ::database::detail::SQLiteInterface {
 public:
  MOCK_METHOD4(Open, int (const char *filename, sqlite3 **ppDb, int flags, const char *zVfs));

  MOCK_METHOD5(Prepare, int (sqlite3 *pDb, const char *zSql, int nByte, sqlite3_stmt **ppStmt, const char **pzTail));

  MOCK_METHOD3(BindDouble, int (sqlite3_stmt *pStmt, int pos, double value));

  MOCK_METHOD3(BindInt, int (sqlite3_stmt *pStmt, int pos, int value));

  MOCK_METHOD3(BindInt64, int (sqlite3_stmt *pStmt, int pos, sqlite3_int64 value));

  MOCK_METHOD5(BindText, int (sqlite3_stmt *pStmt, int pos, const char *value, int num, void (*destructor) (void*)));

  MOCK_METHOD1(Step, int (sqlite3_stmt *pStmt));

  MOCK_METHOD2(ColumnDouble, double (sqlite3_stmt *pStmt, int iCol));

  MOCK_METHOD2(ColumnInt, int (sqlite3_stmt *pStmt, int iCol));

  MOCK_METHOD2(ColumnInt64, sqlite3_int64 (sqlite3_stmt *pStmt, int iCol));

  MOCK_METHOD2(ColumnText, const unsigned char* (sqlite3_stmt *pStmt, int iCol));

  MOCK_METHOD1(Finalize, int (sqlite3_stmt *pStmt));

  MOCK_METHOD1(Close, int (sqlite3 *pDb));

  MOCK_METHOD5(Exec, int (sqlite3 *pDb, const char *sql, int (*callback) (void *, int, char **, char **), void *arg, char **errmsg));
};

}

}

#endif /* PATLMS_TESTS_MOCK_DATABASE_SQLITE_H */

