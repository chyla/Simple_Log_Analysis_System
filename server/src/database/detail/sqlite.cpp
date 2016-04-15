#include "sqlite.h"

#include <boost/log/trivial.hpp>


namespace database
{

namespace detail
{

int SQLite::Open(const char *filename, sqlite3 **ppDb, int flags, const char *zVfs) {
  BOOST_LOG_TRIVIAL(debug) << "database::SQLite::Open: Function call";
  return sqlite3_open_v2(filename, ppDb, flags, zVfs);
}

int SQLite::Prepare(sqlite3 *pDb, const char *zSql, int nByte, sqlite3_stmt **ppStmt, const char **pzTail) {
  BOOST_LOG_TRIVIAL(debug) << "database::SQLite::Prepare: Function call";
  return sqlite3_prepare_v2(pDb, zSql, nByte, ppStmt, pzTail);
}

int SQLite::BindDouble(sqlite3_stmt *pStmt, int pos, double value) {
  BOOST_LOG_TRIVIAL(debug) << "database::SQLite::BindDouble: Function call";
  return sqlite3_bind_double(pStmt, pos, value);
}

int SQLite::BindInt(sqlite3_stmt *pStmt, int pos, int value) {
  BOOST_LOG_TRIVIAL(debug) << "database::SQLite::BindInt: Function call";
  return sqlite3_bind_int(pStmt, pos, value);
}

int SQLite::BindInt64(sqlite3_stmt *pStmt, int pos, sqlite3_int64 value) {
  BOOST_LOG_TRIVIAL(debug) << "database::SQLite::BindInt64: Function call";
  return sqlite3_bind_int64(pStmt, pos, value);
}

int SQLite::BindText(sqlite3_stmt *pStmt, int pos, const char *value, int num, void (*destructor) (void*)) {
  BOOST_LOG_TRIVIAL(debug) << "database::SQLite::BindText: Function call";
  return sqlite3_bind_text(pStmt, pos, value, num, destructor);
}

int SQLite::Step(sqlite3_stmt *pStmt) {
  BOOST_LOG_TRIVIAL(debug) << "database::SQLite::Step: Function call";
  return sqlite3_step(pStmt);
}

double SQLite::ColumnDouble(sqlite3_stmt *pStmt, int iCol) {
  BOOST_LOG_TRIVIAL(debug) << "database::SQLite::ColumnDouble: Function call";
  return sqlite3_column_double(pStmt, iCol);
}

int SQLite::ColumnInt(sqlite3_stmt *pStmt, int iCol) {
  BOOST_LOG_TRIVIAL(debug) << "database::SQLite::ColumnInt: Function call";
  return sqlite3_column_int(pStmt, iCol);
}

sqlite3_int64 SQLite::ColumnInt64(sqlite3_stmt *pStmt, int iCol) {
  BOOST_LOG_TRIVIAL(debug) << "database::SQLite::ColumnInt64: Function call";
  return sqlite3_column_int64(pStmt, iCol);
}

const unsigned char* SQLite::ColumnText(sqlite3_stmt *pStmt, int iCol) {
  BOOST_LOG_TRIVIAL(debug) << "database::SQLite::ColumnText: Function call";
  return sqlite3_column_text(pStmt, iCol);
}

int SQLite::Finalize(sqlite3_stmt *pStmt) {
  BOOST_LOG_TRIVIAL(debug) << "database::SQLite::Finalize: Function call";
  return sqlite3_finalize(pStmt);
}

int SQLite::Close(sqlite3 *pDb) {
  BOOST_LOG_TRIVIAL(debug) << "database::SQLite::Close: Function call";
  return sqlite3_close_v2(pDb);
}

int SQLite::Exec(sqlite3 *pDb, const char *sql, int (*callback) (void *, int, char **, char **), void *arg, char **errmsg) {
  BOOST_LOG_TRIVIAL(debug) << "database::SQLite::Exec: Function call";
  return sqlite3_exec(pDb, sql, callback, arg, errmsg);
}

}

}
