#ifndef PATLMS_DATABASE_DETAIL_SQLITE_H
#define PATLMS_DATABASE_DETAIL_SQLITE_H

#include "sqlite_interface.h"


namespace database
{

namespace detail
{

class SQLite : public SQLiteInterface {
 public:
  int Open(const char *filename, sqlite3 **ppDb, int flags, const char *zVfs) override;

  int Prepare(sqlite3 *pDb, const char *zSql, int nByte, sqlite3_stmt **ppStmt, const char **pzTail) override;

  int BindDouble(sqlite3_stmt *pStmt, int pos, double value) override;

  int BindInt(sqlite3_stmt *pStmt, int pos, int value) override;

  int BindInt64(sqlite3_stmt *pStmt, int pos, sqlite3_int64 value) override;

  int BindText(sqlite3_stmt *pStmt, int pos, const char *value, int num, void (*destructor) (void*)) override;

  int Step(sqlite3_stmt *pStmt) override;

  double ColumnDouble(sqlite3_stmt *pStmt, int iCol) override;

  int ColumnInt(sqlite3_stmt *pStmt, int iCol) override;

  sqlite3_int64 ColumnInt64(sqlite3_stmt *pStmt, int iCol) override;

  const unsigned char* ColumnText(sqlite3_stmt *pStmt, int iCol) override;

  int Finalize(sqlite3_stmt *pStmt) override;

  int Close(sqlite3 *pDb) override;

  int Exec(sqlite3 *pDb, const char *sql, int (*callback) (void *, int, char **, char **), void *arg, char **errmsg) override;
};

}

}

#endif // PATLMS_DATABASE_DETAIL_SQLITE_H
