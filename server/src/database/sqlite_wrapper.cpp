#include "sqlite_wrapper.h"

#include <boost/log/trivial.hpp>

#include "detail/sqlite.h"
#include "src/database/exception/detail/cant_open_database_exception.h"
#include "src/database/exception/detail/cant_close_database_exception.h"
#include "src/database/exception/detail/cant_execute_sql_statement_exception.h"
#include "src/database/exception/detail/item_not_found_exception.h"

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
  BOOST_LOG_TRIVIAL(debug) << "database::SQLiteWrapper::Open: Function call";

  int ret = sqlite_interface_->Open(file_path.c_str(), &db_handle_, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
  if (ret != SQLITE_OK) {
    BOOST_LOG_TRIVIAL(error) << "database::SQLiteWrapper::Open: Open error: " << ret;
    throw exception::detail::CantOpenDatabaseException();
  }

  is_open_ = true;
}

bool SQLiteWrapper::IsOpen() const {
  return is_open_;
}

bool SQLiteWrapper::Close() {
  BOOST_LOG_TRIVIAL(debug) << "database::SQLiteWrapper::Close: Function call";

  if (is_open_) {
    int ret = sqlite_interface_->Close(db_handle_);
    if (ret != SQLITE_OK) {
      BOOST_LOG_TRIVIAL(error) << "database::SQLiteWrapper::Close: Failed to close database: " << ret;
      throw exception::detail::CantCloseDatabaseException();
    }

    is_open_ = false;
    return true;
  }
  else {
    BOOST_LOG_TRIVIAL(warning) << "database::SQLiteWrapper::Close: Database already closed";
    return false;
  }
}

void SQLiteWrapper::Prepare(const std::string &sql, sqlite3_stmt **ppStmt) {
  BOOST_LOG_TRIVIAL(debug) << "database::SQLiteWrapper::Prepare: Function call";

  CheckIsOpen();

  int ret = sqlite_interface_->Prepare(db_handle_, sql.c_str(), -1, ppStmt, nullptr);
  CheckForError(ret, "Prepare function error");
}

void SQLiteWrapper::BindDouble(sqlite3_stmt* pStmt, int pos, double value) {
  BOOST_LOG_TRIVIAL(debug) << "database::SQLiteWrapper::BindDouble: Function call";

  CheckIsOpen();

  int ret = sqlite_interface_->BindDouble(pStmt, pos, value);
  CheckForError(ret, "BindDouble function error");
}

void SQLiteWrapper::BindInt(sqlite3_stmt *pStmt, int pos, int value) {
  BOOST_LOG_TRIVIAL(debug) << "database::SQLiteWrapper::BindInt: Function call";

  CheckIsOpen();

  int ret = sqlite_interface_->BindInt(pStmt, pos, value);
  CheckForError(ret, "BindInt function error");
}

void SQLiteWrapper::BindInt64(sqlite3_stmt *pStmt, int pos, sqlite3_int64 value) {
  BOOST_LOG_TRIVIAL(debug) << "database::SQLiteWrapper::BindInt64: Function call";

  CheckIsOpen();

  int ret = sqlite_interface_->BindInt64(pStmt, pos, value);
  CheckForError(ret, "BindInt64 function error");
}

void SQLiteWrapper::BindText(sqlite3_stmt *pStmt, int pos, const std::string &value) {
  BOOST_LOG_TRIVIAL(debug) << "database::SQLiteWrapper::BindText: Function call";

  CheckIsOpen();

  int ret = sqlite_interface_->BindText(pStmt, pos, value.c_str(), -1, SQLITE_STATIC);
  CheckForError(ret, "BindText function error");
}

double SQLiteWrapper::ColumnDouble(sqlite3_stmt *pStmt, int iCol) {
  BOOST_LOG_TRIVIAL(debug) << "database::SQLiteWrapper::ColumnDouble: Function call";

  CheckIsOpen();

  return sqlite_interface_->ColumnDouble(pStmt, iCol);
}

int SQLiteWrapper::ColumnInt(sqlite3_stmt *pStmt, int iCol) {
  BOOST_LOG_TRIVIAL(debug) << "database::SQLiteWrapper::ColumnInt: Function call";

  CheckIsOpen();

  return sqlite_interface_->ColumnInt(pStmt, iCol);
}

sqlite3_int64 SQLiteWrapper::ColumnInt64(sqlite3_stmt *pStmt, int iCol) {
  BOOST_LOG_TRIVIAL(debug) << "database::SQLiteWrapper::ColumnInt64: Function call";

  CheckIsOpen();

  return sqlite_interface_->ColumnInt64(pStmt, iCol);
}

const std::string SQLiteWrapper::ColumnText(sqlite3_stmt *pStmt, int iCol) {
  BOOST_LOG_TRIVIAL(debug) << "database::SQLiteWrapper::ColumnText: Function call";

  CheckIsOpen();

  const unsigned char *text = sqlite_interface_->ColumnText(pStmt, iCol);
  const char *ctext = reinterpret_cast<const char*> (text);

  if (ctext == nullptr)
    return "";
  else
    return ctext;
}

int SQLiteWrapper::Step(sqlite3_stmt *pStmt) {
  BOOST_LOG_TRIVIAL(debug) << "database::SQLiteWrapper::Step: Function call";

  CheckIsOpen();

  int ret = sqlite_interface_->Step(pStmt);
  CheckForError(ret, "Step function error");

  return ret;
}

void SQLiteWrapper::Finalize(sqlite3_stmt *pStmt) {
  BOOST_LOG_TRIVIAL(debug) << "database::SQLiteWrapper::Finalize: Function call";

  CheckIsOpen();

  int ret = sqlite_interface_->Finalize(pStmt);
  CheckForError(ret, "Finalize function error");
}

void SQLiteWrapper::Exec(const std::string &sql, int (*callback) (void *, int, char **, char **), void *arg) {
  BOOST_LOG_TRIVIAL(debug) << "database::SQLiteWrapper::Exec: Function call";

  CheckIsOpen();

  int ret = sqlite_interface_->Exec(db_handle_, sql.c_str(), callback, arg, nullptr);
  CheckForError(ret, "Exec function error");
}

long long SQLiteWrapper::GetFirstInt64Column(const std::string &sql) {
  BOOST_LOG_TRIVIAL(debug) << "database::SQLiteWrapper::GetFirstInt64Column: Function call";
  long long value;
  bool found = false;

  sqlite3_stmt *statement = nullptr;
  Prepare(sql, &statement);

  try {
    auto ret = Step(statement);
    if (ret == SQLITE_ROW) {
      value = ColumnInt64(statement, 0);
      found = true;
    }
  }
  catch (exception::DatabaseException &ex) {
    Finalize(statement);
    throw;
  }

  Finalize(statement);

  if (!found) {
    BOOST_LOG_TRIVIAL(error) << "database::SQLiteWrapper::GetFirstInt64Column: Item not found";
    throw exception::detail::ItemNotFoundException();
  }

  return value;
}

SQLiteWrapper::SQLiteWrapper(detail::SQLiteInterfacePtr sqlite_interface) :
sqlite_interface_(move(sqlite_interface)),
is_open_(false) {
}

sqlite3* SQLiteWrapper::GetSQLiteHandle() {
  return db_handle_;
}

void SQLiteWrapper::CheckIsOpen() {
  BOOST_LOG_TRIVIAL(debug) << "database::SQLiteWrapper::CheckIsOpen: Function call";

  if (is_open_ == false) {
    BOOST_LOG_TRIVIAL(error) << "database::SQLiteWrapper::CheckIsOpen: Database is not open.";
    throw exception::detail::CantExecuteSqlStatementException();
  }
}

void SQLiteWrapper::CheckForError(int return_value, const char *description) {
  BOOST_LOG_TRIVIAL(debug) << "database::SQLiteWrapper::CheckForError: Function call";

  if (return_value != SQLITE_OK && return_value != SQLITE_DONE && return_value != SQLITE_ROW) {
    BOOST_LOG_TRIVIAL(error) << "database::SQLiteWrapper::CheckForError: " << description << ": " << return_value;
    throw exception::detail::CantExecuteSqlStatementException();
  }
}

}
