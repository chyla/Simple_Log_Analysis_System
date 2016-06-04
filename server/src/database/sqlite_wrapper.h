#ifndef SQLITE_WRAPPER_H
#define SQLITE_WRAPPER_H

#include <memory>
#include <string>

#include "detail/sqlite_interface.h"
#include "detail/sqlite_wrapper_interface.h"

namespace database
{

class SQLiteWrapper;
typedef std::shared_ptr<SQLiteWrapper> SQLiteWrapperPtr;

class SQLiteWrapper : public detail::SQLiteWrapperInterface {
 public:
  static SQLiteWrapperPtr Create();
  static SQLiteWrapperPtr Create(detail::SQLiteInterfacePtr sqlite_interface);

  void Open(const std::string &file_path) override;
  bool IsOpen() const override;
  bool Close() override;

  void Prepare(const std::string &sql, sqlite3_stmt **ppStmt) override;

  void BindDouble(sqlite3_stmt *pStmt, int pos, double value) override;
  void BindInt(sqlite3_stmt *pStmt, int pos, int value) override;
  void BindInt64(sqlite3_stmt *pStmt, int pos, sqlite3_int64 value) override;
  void BindText(sqlite3_stmt *pStmt, int pos, const std::string &value) override;

  double ColumnDouble(sqlite3_stmt *pStmt, int iCol) override;
  int ColumnInt(sqlite3_stmt *pStmt, int iCol) override;
  sqlite3_int64 ColumnInt64(sqlite3_stmt *pStmt, int iCol) override;
  const std::string ColumnText(sqlite3_stmt *pStmt, int iCol) override;

  int Step(sqlite3_stmt *pStmt) override;
  void Finalize(sqlite3_stmt *pStmt) override;

  void Exec(const std::string &sql, int (*callback) (void *, int, char **, char **) = nullptr, void *arg = nullptr) override;
  long long GetFirstInt64Column(const std::string &sql) override;
  long long GetFirstInt64Column(const std::string &sql, long long default_return_value) override;

  sqlite3* GetSQLiteHandle() override;

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
