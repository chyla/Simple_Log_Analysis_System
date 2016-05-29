#ifndef SQLITE_WRAPPER_H
#define SQLITE_WRAPPER_H

#include <memory>
#include <string>

#include "detail/sqlite_interface.h"

namespace database
{

class SQLiteWrapper;
typedef std::shared_ptr<SQLiteWrapper> SQLiteWrapperPtr;

class SQLiteWrapper {
 public:
  static SQLiteWrapperPtr Create();
  static SQLiteWrapperPtr Create(detail::SQLiteInterfacePtr sqlite_interface);

  void Open(const std::string &file_path);
  bool IsOpen() const;
  bool Close();

  void Prepare(const std::string &sql, sqlite3_stmt **ppStmt);

  void BindDouble(sqlite3_stmt *pStmt, int pos, double value);
  void BindInt(sqlite3_stmt *pStmt, int pos, int value);
  void BindInt64(sqlite3_stmt *pStmt, int pos, sqlite3_int64 value);
  void BindText(sqlite3_stmt *pStmt, int pos, const std::string &value);

  double ColumnDouble(sqlite3_stmt *pStmt, int iCol);
  int ColumnInt(sqlite3_stmt *pStmt, int iCol);
  sqlite3_int64 ColumnInt64(sqlite3_stmt *pStmt, int iCol);
  const std::string ColumnText(sqlite3_stmt *pStmt, int iCol);

  int Step(sqlite3_stmt *pStmt);
  void Finalize(sqlite3_stmt *pStmt);

  void Exec(const std::string &sql, int (*callback) (void *, int, char **, char **), void *arg);

 private:
  detail::SQLiteInterfacePtr sqlite_interface_;
  bool is_open_;
  sqlite3 *db_handle_;

  SQLiteWrapper(detail::SQLiteInterfacePtr sqlite_interface);

  void CheckIsOpen();
  void CheckForError(int return_value, const char *description);
};

}

#endif /* SQLITE_WRAPPER_H */
