#include "database.h"

#include <utility>
#include <boost/log/trivial.hpp>
#include <patlms/type/bash_log_entry.h>

#include "detail/sqlite.h"
#include "src/database/exception/detail/cant_open_database_exception.h"
#include "src/database/exception/detail/cant_close_database_exception.h"
#include "src/database/exception/detail/cant_execute_sql_statement_exception.h"


namespace database
{

DatabasePtr Database::Create() {
  std::unique_ptr<detail::SQLite> sqlite(new detail::SQLite());
  return Database::Create(std::move(sqlite));
}

DatabasePtr Database::Create(std::unique_ptr<detail::SQLiteInterface> sqlite) {
  DatabasePtr db(new Database(move(sqlite)));
  return db;
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
    "  UTC_HOUR integer, "
    "  UTC_MINUTE integer, "
    "  UTC_SECOND integer, "
    "  UTC_DAY integer, "
    "  UTC_MONTH integer, "
    "  UTC_YEAR integer, "
    "  USER_ID integer, "
    "  COMMAND text "
    ");";
  int ret = sqlite_interface_->Exec(db_handle_, sql, nullptr, nullptr, nullptr);
  if (ret != SQLITE_OK) {
    BOOST_LOG_TRIVIAL(error) << "database::Database::CreateBashLogsTable: Create table error: " << ret;
    throw exception::detail::CantExecuteSqlStatementException();
  }
}

void Database::CreateApacheLogsTable() {
  BOOST_LOG_TRIVIAL(debug) << "database::Database::CreateApacheLogsTable: Function call";
  
  if (is_open_ == false) {
    BOOST_LOG_TRIVIAL(error) << "database::Database::CreateBashLogsTable: Database is not open.";
    throw exception::detail::CantExecuteSqlStatementException();
  }
  
  const char *sql =
    "create table if not exists APACHE_LOGS_TABLE("
    "  ID integer primary key, "
    "  VIRTUALHOST text, "
    "  CLIENT_IP text, "
    "  UTC_HOUR integer, "
    "  UTC_MINUTE integer, "
    "  UTC_SECOND integer, "
    "  UTC_DAY integer, "
    "  UTC_MONTH integer, "
    "  UTC_YEAR integer, "
    "  REQUEST text, "
    "  STATUS_CODE integer, "
    "  BYTES integer, "
    "  USER_AGENT text "
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
    BOOST_LOG_TRIVIAL(debug) << "database::Database::AddBashLogs: Processing: " << entry.hostname << " ; " << entry.utc_time << " ; " << entry.user_id << " ; " << entry.command;

    const char *sql = "insert into BASH_LOGS_TABLE(HOSTNAME, UTC_HOUR, UTC_MINUTE, UTC_SECOND, UTC_DAY, UTC_MONTH, UTC_YEAR, USER_ID, COMMAND) values(?, ?, ?, ?, ?, ?, ?, ?, ?)";
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

    ret = sqlite_interface_->BindInt(statement, 2, entry.utc_time.GetHour());
    if (ret != SQLITE_OK) {
      BOOST_LOG_TRIVIAL(error) << "database::Database::AddBashLogs: Bind hour error: " << ret;
      rollback();
      throw exception::detail::CantExecuteSqlStatementException();
    }

    ret = sqlite_interface_->BindInt(statement, 3, entry.utc_time.GetMinute());
    if (ret != SQLITE_OK) {
      BOOST_LOG_TRIVIAL(error) << "database::Database::AddBashLogs: Bind minute error: " << ret;
      rollback();
      throw exception::detail::CantExecuteSqlStatementException();
    }

    ret = sqlite_interface_->BindInt(statement, 4, entry.utc_time.GetSecond());
    if (ret != SQLITE_OK) {
      BOOST_LOG_TRIVIAL(error) << "database::Database::AddBashLogs: Bind second error: " << ret;
      rollback();
      throw exception::detail::CantExecuteSqlStatementException();
    }

    ret = sqlite_interface_->BindInt(statement, 5, entry.utc_time.GetDay());
    if (ret != SQLITE_OK) {
      BOOST_LOG_TRIVIAL(error) << "database::Database::AddBashLogs: Bind day error: " << ret;
      rollback();
      throw exception::detail::CantExecuteSqlStatementException();
    }

    ret = sqlite_interface_->BindInt(statement, 6, entry.utc_time.GetMonth());
    if (ret != SQLITE_OK) {
      BOOST_LOG_TRIVIAL(error) << "database::Database::AddBashLogs: Bind month error: " << ret;
      rollback();
      throw exception::detail::CantExecuteSqlStatementException();
    }

    ret = sqlite_interface_->BindInt(statement, 7, entry.utc_time.GetYear());
    if (ret != SQLITE_OK) {
      BOOST_LOG_TRIVIAL(error) << "database::Database::AddBashLogs: Bind year error: " << ret;
      rollback();
      throw exception::detail::CantExecuteSqlStatementException();
    }

    ret = sqlite_interface_->BindInt(statement, 8, entry.user_id);
    if (ret != SQLITE_OK) {
      BOOST_LOG_TRIVIAL(error) << "database::Database::AddBashLogs: Bind user id error: " << ret;
      rollback();
      throw exception::detail::CantExecuteSqlStatementException();
    }

    ret = sqlite_interface_->BindText(statement, 9, entry.command.c_str(), -1, nullptr);
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

Database::Database(std::unique_ptr<database::detail::SQLiteInterface> sqlite)
  : is_open_(false),
  db_handle_(nullptr),
  sqlite_interface_(std::move(sqlite)) {
}

}
