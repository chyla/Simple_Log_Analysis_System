#include "sqlite_wrapper.h"

#include <boost/log/trivial.hpp>

#include "detail/sqlite.h"
#include "src/database/exception/detail/cant_open_database_exception.h"
#include "src/database/exception/detail/cant_close_database_exception.h"
#include "src/database/exception/detail/cant_execute_sql_statement_exception.h"

namespace database
{

SQLiteWrapperPtr SQLiteWrapper::Create() {
  detail::SQLiteInterfacePtr sqlite_interface(new detail::SQLite());
  return SQLiteWrapper::Create(std::move(sqlite_interface));
}

SQLiteWrapperPtr SQLiteWrapper::Create(detail::SQLiteInterfacePtr sqlite_interface) {
  SQLiteWrapperPtr db(new SQLiteWrapper(move(sqlite_interface)));
  return db;
}

void SQLiteWrapper::Open(const std::string &file_path) {
  BOOST_LOG_TRIVIAL(debug) << "database::detail::SQLiteWrapper::Open: Function call";

  int ret = sqlite_interface_->Open(file_path.c_str(), &db_handle_, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
  if (ret != SQLITE_OK) {
    BOOST_LOG_TRIVIAL(error) << "database::detail::SQLiteWrapper::Open: Open error: " << ret;
    throw exception::detail::CantOpenDatabaseException();
  }

  is_open_ = true;
}

bool SQLiteWrapper::IsOpen() const {
  return is_open_;
}

bool SQLiteWrapper::Close() {
  BOOST_LOG_TRIVIAL(debug) << "database::detail::SQLiteWrapper::Close: Function call";

  if (is_open_) {
    int ret = sqlite_interface_->Close(db_handle_);
    if (ret != SQLITE_OK) {
      BOOST_LOG_TRIVIAL(error) << "database::detail::SQLiteWrapper::Close: Failed to close database: " << ret;
      throw exception::detail::CantCloseDatabaseException();
    }

    is_open_ = false;
    return true;
  }
  else {
    BOOST_LOG_TRIVIAL(warning) << "database::detail::SQLiteWrapper::Close: Database already closed";
    return false;
  }
}

void SQLiteWrapper::Prepare(const std::string &sql, sqlite3_stmt **ppStmt) {
  CheckIsOpen();

  int ret = sqlite_interface_->Prepare(db_handle_, sql.c_str(), -1, ppStmt, nullptr);
  CheckForError(ret, "Prepare function error");
}

void SQLiteWrapper::BindDouble(sqlite3_stmt* pStmt, int pos, double value) {
  CheckIsOpen();

  int ret = sqlite_interface_->BindDouble(pStmt, pos, value);
  CheckForError(ret, "BindDouble function error");
}

void SQLiteWrapper::BindInt(sqlite3_stmt *pStmt, int pos, int value) {
  CheckIsOpen();

  int ret = sqlite_interface_->BindInt(pStmt, pos, value);
  CheckForError(ret, "BindInt function error");
}

void SQLiteWrapper::BindInt64(sqlite3_stmt *pStmt, int pos, sqlite3_int64 value) {
  CheckIsOpen();

  int ret = sqlite_interface_->BindInt64(pStmt, pos, value);
  CheckForError(ret, "BindInt64 function error");
}

void SQLiteWrapper::BindText(sqlite3_stmt *pStmt, int pos, const std::string &value) {
  CheckIsOpen();

  int ret = sqlite_interface_->BindText(pStmt, pos, value.c_str(), -1, SQLITE_STATIC);
  CheckForError(ret, "BindText function error");
}

double SQLiteWrapper::ColumnDouble(sqlite3_stmt *pStmt, int iCol) {
  CheckIsOpen();

  return sqlite_interface_->ColumnDouble(pStmt, iCol);
}

int SQLiteWrapper::ColumnInt(sqlite3_stmt *pStmt, int iCol) {
  CheckIsOpen();

  return sqlite_interface_->ColumnInt(pStmt, iCol);
}

sqlite3_int64 SQLiteWrapper::ColumnInt64(sqlite3_stmt *pStmt, int iCol) {
  CheckIsOpen();

  return sqlite_interface_->ColumnInt64(pStmt, iCol);
}

const std::string SQLiteWrapper::ColumnText(sqlite3_stmt *pStmt, int iCol) {
  CheckIsOpen();

  const unsigned char *text = sqlite_interface_->ColumnText(pStmt, iCol);
  const char *ctext = reinterpret_cast<const char*> (text);

  if (ctext == nullptr)
    return "";
  else
    return ctext;
}

int SQLiteWrapper::Step(sqlite3_stmt *pStmt) {
  CheckIsOpen();

  int ret = sqlite_interface_->Step(pStmt);
  CheckForError(ret, "Step function error");

  return ret;
}

void SQLiteWrapper::Finalize(sqlite3_stmt *pStmt) {
  CheckIsOpen();

  int ret = sqlite_interface_->Finalize(pStmt);
  CheckForError(ret, "Finalize function error");
}

void SQLiteWrapper::Exec(const std::string &sql, int (*callback) (void *, int, char **, char **), void *arg) {
  CheckIsOpen();
  
  int ret = sqlite_interface_->Exec(db_handle_, sql.c_str(), callback, arg, nullptr);
  CheckForError(ret, "Exec function error");
}

SQLiteWrapper::SQLiteWrapper(detail::SQLiteInterfacePtr sqlite_interface) :
sqlite_interface_(move(sqlite_interface)),
is_open_(false) {
}

void SQLiteWrapper::CheckIsOpen() {
  if (is_open_ == false) {
    BOOST_LOG_TRIVIAL(error) << "database::SQLiteWrapper: Database is not open.";
    throw exception::detail::CantExecuteSqlStatementException();
  }
}

void SQLiteWrapper::CheckForError(int return_value, const char *description) {
  if (return_value != SQLITE_OK && return_value != SQLITE_DONE && return_value != SQLITE_ROW) {
    BOOST_LOG_TRIVIAL(error) << "database::SQLiteWrapper: " << description << ": " << return_value;
    throw exception::detail::CantExecuteSqlStatementException();
  }
}

}
