#include "database.h"

#include <utility>
#include <boost/log/trivial.hpp>

#include "src/database/exception/detail/cant_open_database_exception.h"
#include "src/database/exception/detail/cant_close_database_exception.h"
#include "src/database/exception/detail/cant_execute_sql_statement_exception.h"


namespace database
{

Database::Database(std::unique_ptr<database::detail::SQLiteInterface> sqlite)
  : is_open_(false),
  db_handle_(nullptr),
  sqlite_interface_(std::move(sqlite)) {
}

void Database::Open(const std::string &file_path) {
  BOOST_LOG_TRIVIAL(debug) << "database::Database::Open: Function call";

  int ret = sqlite_interface_->Open(file_path.c_str(), &db_handle_, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr);
  if (ret != SQLITE_OK) {
    BOOST_LOG_TRIVIAL(error) << "database::Database::Open: Open error: " << ret;
    throw exception::detail::CantOpenDatabaseException();
  }

  is_open_ = true;
}

bool Database::IsOpen() const {
  return is_open_;
}

void Database::CreateBashLogsTable() {
  BOOST_LOG_TRIVIAL(debug) << "database::Database::CreateBashLogsTable: Function call";

  if (is_open_ == false) {
    BOOST_LOG_TRIVIAL(error) << "database::Database::CreateBashLogsTable: Database is not open.";
    throw exception::detail::CantExecuteSqlStatementException();
  }

  const char *sql =
    "create table if not exists BASH_LOGS_TABLE("
    "  ID integer primary key, "
    "  HOSTNAME text, "
    "  TIME integer, "
    "  USER_ID integer, "
    "  COMMAND text "
    ");";
  int ret = sqlite_interface_->Exec(db_handle_, sql, nullptr, nullptr, nullptr);
  if (ret != SQLITE_OK) {
    BOOST_LOG_TRIVIAL(error) << "database::Database::CreateBashLogsTable: Create table error: " << ret;
    throw exception::detail::CantExecuteSqlStatementException();
  }
}

bool Database::AddBashLogs(const type::BashLogs &log_entries) {
  BOOST_LOG_TRIVIAL(debug) << "database::Database::AddBashLogs: Function call";

  if (is_open_ == false) {
    BOOST_LOG_TRIVIAL(error) << "database::Database::AddBashLogs: Database is not open.";
    throw exception::detail::CantExecuteSqlStatementException();
  }

  auto rollback = [this]()
  {
    int ret = sqlite_interface_->Exec(db_handle_, "rollback", nullptr, nullptr, nullptr);
    if (ret != SQLITE_OK) {
      BOOST_LOG_TRIVIAL(error) << "database::Database::AddBashLogs: Rollback error: " << ret;
      throw exception::detail::CantExecuteSqlStatementException();
    }
  };

  int ret;
  ret = sqlite_interface_->Exec(db_handle_, "begin transaction", nullptr, nullptr, nullptr);
  if (ret != SQLITE_OK) {
    BOOST_LOG_TRIVIAL(error) << "database::Database::AddBashLogs: Begin transaction error: " << ret;
    throw exception::detail::CantExecuteSqlStatementException();
  }

  for (const type::BashLogEntry &entry : log_entries) {
    BOOST_LOG_TRIVIAL(debug) << "database::Database::AddBashLogs: Processing: " << entry.hostname << " ; " << entry.time << " ; " << entry.user_id << " ; " << entry.command;

    const char *sql = "insert into BASH_LOGS_TABLE(HOSTNAME, TIME, USER_ID, COMMAND) values(?, ?, ?, ?)";
    sqlite3_stmt *statement;
    ret = sqlite_interface_->Prepare(db_handle_, sql, -1, &statement, nullptr);
    if (ret != SQLITE_OK) {
      BOOST_LOG_TRIVIAL(error) << "database::Database::AddBashLogs: Prepare insert error: " << ret;
      rollback();
      throw exception::detail::CantExecuteSqlStatementException();
    }

    ret = sqlite_interface_->BindText(statement, 1, entry.hostname.c_str(), -1, nullptr);
    if (ret != SQLITE_OK) {
      BOOST_LOG_TRIVIAL(error) << "database::Database::AddBashLogs: Bind hostname error: " << ret;
      rollback();
      throw exception::detail::CantExecuteSqlStatementException();
    }

    ret = sqlite_interface_->BindInt64(statement, 2, entry.time);
    if (ret != SQLITE_OK) {
      BOOST_LOG_TRIVIAL(error) << "database::Database::AddBashLogs: Bind time error: " << ret;
      rollback();
      throw exception::detail::CantExecuteSqlStatementException();
    }

    ret = sqlite_interface_->BindInt(statement, 3, entry.user_id);
    if (ret != SQLITE_OK) {
      BOOST_LOG_TRIVIAL(error) << "database::Database::AddBashLogs: Bind user id error: " << ret;
      rollback();
      throw exception::detail::CantExecuteSqlStatementException();
    }

    ret = sqlite_interface_->BindText(statement, 4, entry.command.c_str(), -1, nullptr);
    if (ret != SQLITE_OK) {
      BOOST_LOG_TRIVIAL(error) << "database::Database::AddBashLogs: Bind command error: " << ret;
      rollback();
      throw exception::detail::CantExecuteSqlStatementException();
    }

    ret = sqlite_interface_->Step(statement);
    if (ret == SQLITE_BUSY) {
      BOOST_LOG_TRIVIAL(error) << "database::Database::AddBashLogs: Step error - SQLite is busy";
      rollback();
      return false;
    } else if (ret != SQLITE_DONE) {
      BOOST_LOG_TRIVIAL(error) << "database::Database::AddBashLogs: Step error:" << ret;
      rollback();
      throw exception::detail::CantExecuteSqlStatementException();
    }

    ret = sqlite_interface_->Finalize(statement);
    if (ret != SQLITE_OK) {
      BOOST_LOG_TRIVIAL(error) << "database::Database::AddBashLogs: Finalize error:" << ret;
      rollback();
      throw exception::detail::CantExecuteSqlStatementException();
    }
  }

  ret = sqlite_interface_->Exec(db_handle_, "end transaction", nullptr, nullptr, nullptr);
  if (ret == SQLITE_BUSY) {
    BOOST_LOG_TRIVIAL(error) << "database::Database::AddBashLogs: End transaction error - SQLite is busy";
    rollback();
    return false;
  } else if (ret != SQLITE_OK) {
    BOOST_LOG_TRIVIAL(error) << "database::Database::AddBashLogs: End transaction error:" << ret;
    rollback();
    throw exception::detail::CantExecuteSqlStatementException();
  }

  return true;
}

bool Database::Close() {
  BOOST_LOG_TRIVIAL(debug) << "database::Database::Close: Function call";

  if (is_open_) {
    int ret = sqlite_interface_->Close(db_handle_);
    if (ret != SQLITE_OK) {
      BOOST_LOG_TRIVIAL(error) << "database::Database::Close: Failed to close database: " << ret;
      throw exception::detail::CantCloseDatabaseException();
    }

    is_open_ = false;
    return true;
  } else {
    BOOST_LOG_TRIVIAL(warning) << "database::Database::Close: Database already closed";
    return false;
  }
}

}
