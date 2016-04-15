#ifndef PATLMS_DATABASE_DETAIL_SQLITE_INTERFACE_H
#define PATLMS_DATABASE_DETAIL_SQLITE_INTERFACE_H

#include <sqlite3.h>

namespace database
{

namespace detail
{

class SQLiteInterface {
 public:
  virtual inline ~SQLiteInterface();

  virtual int Open(const char *filename, sqlite3 **ppDb, int flags, const char *zVfs) = 0;

  virtual int Prepare(sqlite3 *pDb, const char *zSql, int nByte, sqlite3_stmt **ppStmt, const char **pzTail) = 0;

  virtual int BindDouble(sqlite3_stmt *pStmt, int pos, double value) = 0;

  virtual int BindInt(sqlite3_stmt *pStmt, int pos, int value) = 0;

  virtual int BindInt64(sqlite3_stmt *pStmt, int pos, sqlite3_int64 value) = 0;

  virtual int BindText(sqlite3_stmt *pStmt, int pos, const char *value, int num, void (*destructor) (void*)) = 0;

  virtual int Step(sqlite3_stmt *pStmt) = 0;

  virtual double ColumnDouble(sqlite3_stmt *pStmt, int iCol) = 0;

  virtual int ColumnInt(sqlite3_stmt *pStmt, int iCol) = 0;

  virtual sqlite3_int64 ColumnInt64(sqlite3_stmt *pStmt, int iCol) = 0;

  virtual const unsigned char* ColumnText(sqlite3_stmt *pStmt, int iCol) = 0;

  virtual int Finalize(sqlite3_stmt *pStmt) = 0;

  virtual int Close(sqlite3 *pDb) = 0;

  virtual int Exec(sqlite3 *pDb, const char *sql, int (*callback) (void *, int, char **, char **), void *arg, char **errmsg) = 0;
};

SQLiteInterface::~SQLiteInterface() {
}

}

}

#endif // PATLMS_DATABASE_DETAIL_SQLITE_INTERFACE_H
