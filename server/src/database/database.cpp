#include "database.h"

#include <utility>
#include <boost/log/trivial.hpp>
#include <patlms/type/bash_log_entry.h>
#include <patlms/type/apache_log_entry.h>

#include "detail/sqlite.h"
#include "src/database/exception/detail/cant_open_database_exception.h"
#include "src/database/exception/detail/cant_close_database_exception.h"
#include "src/database/exception/detail/cant_execute_sql_statement_exception.h"

using namespace std;

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
      "  AGENT_NAME text, "
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
  StatementCheckForError(ret, "Create BASH_LOGS_TABLE error");
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
      "  AGENT_NAME text,"
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
  StatementCheckForError(ret, "Create APACHE_LOGS_TABLE error");
}

void Database::CreateApacheSessionTable() {
  BOOST_LOG_TRIVIAL(debug) << "database::Database::CreateApacheSessionTable: Function call";

  if (is_open_ == false) {
    BOOST_LOG_TRIVIAL(error) << "database::Database::CreateApacheSessionTable: Database is not open.";
    throw exception::detail::CantExecuteSqlStatementException();
  }

  const char *sql =
      "create table if not exists APACHE_SESSION_TABLE("
      "  ID integer primary key, "
      "  AGENT_NAME text,"
      "  VIRTUALHOST text, "
      "  CLIENT_IP text, "
      "  SESSION_LENGTH integer, "
      "  BANDWIDTH_USAGE integer, "
      "  REQUESTS_COUNT integer, "
      "  ERROR_PERCENTAGE integer, "
      "  USER_AGENT text "
      ");";
  int ret = sqlite_interface_->Exec(db_handle_, sql, nullptr, nullptr, nullptr);
  StatementCheckForError(ret, "Create APACHE_SESSION_TABLE error");
}

bool Database::AddBashLogs(const type::BashLogs &log_entries) {
  BOOST_LOG_TRIVIAL(debug) << "database::Database::AddBashLogs: Function call";

  if (is_open_ == false) {
    BOOST_LOG_TRIVIAL(error) << "database::Database::AddBashLogs: Database is not open.";
    throw exception::detail::CantExecuteSqlStatementException();
  }

  int ret;
  ret = sqlite_interface_->Exec(db_handle_, "begin transaction", nullptr, nullptr, nullptr);
  StatementCheckForError(ret, "Begin transaction error");

  for (const type::BashLogEntry &entry : log_entries) {
    BOOST_LOG_TRIVIAL(debug) << "database::Database::AddBashLogs: Processing: " << entry.agent_name << " ; " << entry.utc_time << " ; " << entry.user_id << " ; " << entry.command;

    const char *sql = "insert into BASH_LOGS_TABLE(AGENT_NAME, UTC_HOUR, UTC_MINUTE, UTC_SECOND, UTC_DAY, UTC_MONTH, UTC_YEAR, USER_ID, COMMAND) values(?, ?, ?, ?, ?, ?, ?, ?, ?)";
    sqlite3_stmt *statement;
    ret = sqlite_interface_->Prepare(db_handle_, sql, -1, &statement, nullptr);
    StatementCheckForErrorAndRollback(ret, "Prepare insert error");

    ret = sqlite_interface_->BindText(statement, 1, entry.agent_name.c_str(), -1, nullptr);
    StatementCheckForErrorAndRollback(ret, "Bind agentname error");

    ret = sqlite_interface_->BindInt(statement, 2, entry.utc_time.GetHour());
    StatementCheckForErrorAndRollback(ret, "Bind hour error");

    ret = sqlite_interface_->BindInt(statement, 3, entry.utc_time.GetMinute());
    StatementCheckForErrorAndRollback(ret, "Bind minute error");

    ret = sqlite_interface_->BindInt(statement, 4, entry.utc_time.GetSecond());
    StatementCheckForErrorAndRollback(ret, "Bind second error");

    ret = sqlite_interface_->BindInt(statement, 5, entry.utc_time.GetDay());
    StatementCheckForErrorAndRollback(ret, "Bind day error");

    ret = sqlite_interface_->BindInt(statement, 6, entry.utc_time.GetMonth());
    StatementCheckForErrorAndRollback(ret, "Bind month error");

    ret = sqlite_interface_->BindInt(statement, 7, entry.utc_time.GetYear());
    StatementCheckForErrorAndRollback(ret, "Bind year error");

    ret = sqlite_interface_->BindInt(statement, 8, entry.user_id);
    StatementCheckForErrorAndRollback(ret, "Bind user id error");

    ret = sqlite_interface_->BindText(statement, 9, entry.command.c_str(), -1, nullptr);
    StatementCheckForErrorAndRollback(ret, "Bind command error");

    ret = sqlite_interface_->Step(statement);
    if (ret == SQLITE_BUSY) {
      BOOST_LOG_TRIVIAL(error) << "database::Database::AddBashLogs: Step error - SQLite is busy";
      Rollback();
      return false;
    }
    StatementCheckForErrorAndRollback(ret, "Step error");

    ret = sqlite_interface_->Finalize(statement);
    StatementCheckForErrorAndRollback(ret, "Finalize error");
  }

  ret = sqlite_interface_->Exec(db_handle_, "end transaction", nullptr, nullptr, nullptr);
  if (ret == SQLITE_BUSY) {
    BOOST_LOG_TRIVIAL(error) << "database::Database::AddBashLogs: End transaction error - SQLite is busy";
    Rollback();
    return false;
  }
  StatementCheckForErrorAndRollback(ret, "End transaction error");

  return true;
}

bool Database::AddApacheLogs(const type::ApacheLogs &log_entries) {
  BOOST_LOG_TRIVIAL(debug) << "database::Database::AddApacheLogs: Function call";

  if (is_open_ == false) {
    BOOST_LOG_TRIVIAL(error) << "database::Database::AddApacheLogs: Database is not open.";
    throw exception::detail::CantExecuteSqlStatementException();
  }

  int ret;
  ret = sqlite_interface_->Exec(db_handle_, "begin transaction", nullptr, nullptr, nullptr);
  StatementCheckForError(ret, "Begin transaction error");

  for (const type::ApacheLogEntry &entry : log_entries) {
    const char *sql = "insert into APACHE_LOGS_TABLE(AGENT_NAME, VIRTUALHOST, CLIENT_IP, UTC_HOUR, UTC_MINUTE, UTC_SECOND, UTC_DAY, UTC_MONTH, UTC_YEAR, REQUEST, STATUS_CODE, BYTES, USER_AGENT) values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)";
    sqlite3_stmt *statement;
    ret = sqlite_interface_->Prepare(db_handle_, sql, -1, &statement, nullptr);
    StatementCheckForErrorAndRollback(ret, "Prepare insert error");

    ret = sqlite_interface_->BindText(statement, 1, entry.agent_name.c_str(), -1, nullptr);
    StatementCheckForErrorAndRollback(ret, "Bind agent_name error");

    ret = sqlite_interface_->BindText(statement, 2, entry.virtualhost.c_str(), -1, nullptr);
    StatementCheckForErrorAndRollback(ret, "Bind virtualhost error");

    ret = sqlite_interface_->BindText(statement, 3, entry.client_ip.c_str(), -1, nullptr);
    StatementCheckForErrorAndRollback(ret, "Bind client_ip error");

    ret = sqlite_interface_->BindInt(statement, 4, entry.time.GetHour());
    StatementCheckForErrorAndRollback(ret, "Bind hour error");

    ret = sqlite_interface_->BindInt(statement, 5, entry.time.GetMinute());
    StatementCheckForErrorAndRollback(ret, "Bind minute error");

    ret = sqlite_interface_->BindInt(statement, 6, entry.time.GetSecond());
    StatementCheckForErrorAndRollback(ret, "Bind second error");

    ret = sqlite_interface_->BindInt(statement, 7, entry.time.GetDay());
    StatementCheckForErrorAndRollback(ret, "Bind day error");

    ret = sqlite_interface_->BindInt(statement, 8, entry.time.GetMonth());
    StatementCheckForErrorAndRollback(ret, "Bind month error");

    ret = sqlite_interface_->BindInt(statement, 9, entry.time.GetYear());
    StatementCheckForErrorAndRollback(ret, "Bind year error");

    ret = sqlite_interface_->BindText(statement, 10, entry.request.c_str(), -1, nullptr);
    StatementCheckForErrorAndRollback(ret, "Bind request error");

    ret = sqlite_interface_->BindInt(statement, 11, entry.status_code);
    StatementCheckForErrorAndRollback(ret, "Bind status_code id error");

    ret = sqlite_interface_->BindInt(statement, 12, entry.bytes);
    StatementCheckForErrorAndRollback(ret, "Bind bytes error");

    ret = sqlite_interface_->BindText(statement, 13, entry.user_agent.c_str(), -1, nullptr);
    StatementCheckForErrorAndRollback(ret, "Bind user_agent error");

    ret = sqlite_interface_->Step(statement);
    if (ret == SQLITE_BUSY) {
      BOOST_LOG_TRIVIAL(error) << "database::Database::AddBashLogs: Step error - SQLite is busy";
      Rollback();
      return false;
    }
    StatementCheckForErrorAndRollback(ret, "Step error");

    ret = sqlite_interface_->Finalize(statement);
    StatementCheckForErrorAndRollback(ret, "Finalize error");
  }

  ret = sqlite_interface_->Exec(db_handle_, "end transaction", nullptr, nullptr, nullptr);
  if (ret == SQLITE_BUSY) {
    BOOST_LOG_TRIVIAL(error) << "database::Database::AddBashLogs: End transaction error - SQLite is busy";
    Rollback();
    return false;
  }
  StatementCheckForErrorAndRollback(ret, "End transaction error");

  return true;
}

bool Database::AddApacheSessionStatistics(const analyzer::ApacheSessions &sessions) {
  BOOST_LOG_TRIVIAL(debug) << "database::Database::AddApacheSessionStatistics: Function call";

  if (is_open_ == false) {
    BOOST_LOG_TRIVIAL(error) << "database::Database::AddApacheSessionStatistics: Database is not open.";
    throw exception::detail::CantExecuteSqlStatementException();
  }

  int ret;
  ret = sqlite_interface_->Exec(db_handle_, "begin transaction", nullptr, nullptr, nullptr);
  StatementCheckForError(ret, "Begin transaction error");

  for (const analyzer::ApacheSessionEntry &entry : sessions) {
    const char *sql = "insert into APACHE_SESSION_TABLE(AGENT_NAME, VIRTUALHOST, CLIENT_IP, SESSION_LENGTH, BANDWIDTH_USAGE, REQUESTS_COUNT, ERROR_PERCENTAGE, USER_AGENT) values(?, ?, ?, ?, ?, ?, ?, ?)";
    sqlite3_stmt *statement;
    ret = sqlite_interface_->Prepare(db_handle_, sql, -1, &statement, nullptr);
    StatementCheckForErrorAndRollback(ret, "Prepare insert error");

    ret = sqlite_interface_->BindText(statement, 1, entry.agent_name.c_str(), -1, nullptr);
    StatementCheckForErrorAndRollback(ret, "Bind agent_name error");

    ret = sqlite_interface_->BindText(statement, 2, entry.virtualhost.c_str(), -1, nullptr);
    StatementCheckForErrorAndRollback(ret, "Bind virtualhost error");

    ret = sqlite_interface_->BindText(statement, 3, entry.client_ip.c_str(), -1, nullptr);
    StatementCheckForErrorAndRollback(ret, "Bind client_ip error");

    ret = sqlite_interface_->BindInt64(statement, 4, entry.session_length);
    StatementCheckForErrorAndRollback(ret, "Bind session_length error");

    ret = sqlite_interface_->BindInt64(statement, 5, entry.bandwidth_usage);
    StatementCheckForErrorAndRollback(ret, "Bind bandwidth_usage error");

    ret = sqlite_interface_->BindInt64(statement, 6, entry.requests_count);
    StatementCheckForErrorAndRollback(ret, "Bind requests_count error");

    ret = sqlite_interface_->BindInt(statement, 7, entry.error_percentage);
    StatementCheckForErrorAndRollback(ret, "Bind error_percentage error");

    ret = sqlite_interface_->BindText(statement, 8, entry.useragent.c_str(), -1, nullptr);
    StatementCheckForErrorAndRollback(ret, "Bind useragent error");

    ret = sqlite_interface_->Step(statement);
    if (ret == SQLITE_BUSY) {
      BOOST_LOG_TRIVIAL(error) << "database::Database::AddApacheSessionStatistics: Step error - SQLite is busy";
      Rollback();
      return false;
    }
    StatementCheckForErrorAndRollback(ret, "Step error");

    ret = sqlite_interface_->Finalize(statement);
    StatementCheckForErrorAndRollback(ret, "Finalize error");
  }

  ret = sqlite_interface_->Exec(db_handle_, "end transaction", nullptr, nullptr, nullptr);
  if (ret == SQLITE_BUSY) {
    BOOST_LOG_TRIVIAL(error) << "database::Database::AddBashLogs: End transaction error - SQLite is busy";
    Rollback();
    return false;
  }
  StatementCheckForErrorAndRollback(ret, "End transaction error");

  return true;
}

long long Database::GetApacheLogsCount(string agent_name, string virtualhost_name, type::Time from, type::Time to) {
  BOOST_LOG_TRIVIAL(debug) << "database::Database::GetApacheLogsCount: Function call";
  int ret;
  long long count = 0;

  if (is_open_ == false) {
    BOOST_LOG_TRIVIAL(error) << "database::Database::GetApacheLogsCount: Database is not open";
    throw exception::detail::CantExecuteSqlStatementException();
  }

  string from_day = to_string(from.GetDay()),
      from_month = to_string(from.GetMonth()),
      from_year = to_string(from.GetYear()),
      from_hour = to_string(from.GetHour()),
      from_minute = to_string(from.GetMinute()),
      from_second = to_string(from.GetSecond()),
      to_day = to_string(to.GetDay()),
      to_month = to_string(to.GetMonth()),
      to_year = to_string(to.GetYear()),
      to_hour = to_string(to.GetHour()),
      to_minute = to_string(to.GetMinute()),
      to_second = to_string(to.GetSecond());

  string sql =
      "select * from APACHE_LOGS_TABLE"
      "  where"
      "    ("
      "      AGENT_NAME=?"
      "      and"
      "      VIRTUALHOST=?"
      "    )"
      "  and"
      "    ("
      "      ("
      "        (UTC_HOUR > " + from_hour + ")"
      "        or (UTC_HOUR = " + from_hour + " and UTC_MINUTE > " + from_minute + ")"
      "        or (UTC_HOUR = " + from_hour + " and UTC_MINUTE = " + from_minute + " and UTC_SECOND >= " + from_second + ")"
      "      )"
      "      and"
      "      ("
      "        (UTC_YEAR > " + from_year + ")"
      "        or (UTC_YEAR = " + from_year + " and UTC_MONTH > " + from_month + ")"
      "        or (UTC_YEAR = " + from_year + " and UTC_MONTH = " + from_month + " and UTC_DAY >= " + from_day + ")"
      "      )"
      "    )"
      "  and"
      "    ("
      "      ("
      "        (UTC_HOUR < " + to_hour + ")"
      "        or (UTC_HOUR = " + to_hour + " and UTC_MINUTE < " + to_minute + ")"
      "        or (UTC_HOUR = " + to_hour + " and UTC_MINUTE = " + to_minute + " and UTC_SECOND <= " + to_second + ")"
      "      )"
      "      and"
      "      ("
      "        (UTC_YEAR < " + to_year + ")"
      "        or (UTC_YEAR = " + to_year + " and UTC_MONTH < " + to_month + ")"
      "        or (UTC_YEAR = " + to_year + " and UTC_MONTH = " + to_month + " and UTC_DAY <= " + to_day + ")"
      "      )"
      "    )"
      ";";

  sqlite3_stmt *statement;
  ret = sqlite_interface_->Prepare(db_handle_, sql.c_str(), -1, &statement, nullptr);
  StatementCheckForError(ret, "Prepare insert error");

  ret = sqlite_interface_->BindText(statement, 1, agent_name.c_str(), -1, nullptr);
  StatementCheckForError(ret, "Bind useragent error");

  ret = sqlite_interface_->BindText(statement, 2, virtualhost_name.c_str(), -1, nullptr);
  StatementCheckForError(ret, "Bind useragent error");

  ret = sqlite_interface_->Step(statement);
  StatementCheckForError(ret, "Step error");

  count = sqlite_interface_->ColumnInt64(statement, 0);

  ret = sqlite_interface_->Finalize(statement);
  StatementCheckForError(ret, "Finalize error");

  return count;
}

Database::AgentNames Database::GetApacheAgentNames() {
  BOOST_LOG_TRIVIAL(debug) << "database::Database::GetApacheAgentNames: Function call";

  int ret;
  AgentNames names;

  const char *sql = "select DISTINCT AGENT_NAME from APACHE_SESSION_TABLE;";

  sqlite3_stmt *statement;
  ret = sqlite_interface_->Exec(db_handle_, sql, GetApacheAgentNamesCallback, static_cast<void*> (&names), nullptr);
  StatementCheckForError(ret, "Exec error");

  BOOST_LOG_TRIVIAL(debug) << "database::Database::GetApacheAgentNames: Found " << names.size() << " names";
  
  return names;
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
  }
  else {
    BOOST_LOG_TRIVIAL(warning) << "database::Database::Close: Database already closed";
    return false;
  }
}

Database::Database(std::unique_ptr<database::detail::SQLiteInterface> sqlite)
: is_open_(false),
db_handle_(nullptr),
sqlite_interface_(std::move(sqlite)) {
}

void Database::StatementCheckForError(int return_value, const char *description) {
  if (return_value != SQLITE_OK && return_value != SQLITE_DONE && return_value != SQLITE_ROW) {
    BOOST_LOG_TRIVIAL(error) << "database::Database: " << description << ": " << return_value;
    throw exception::detail::CantExecuteSqlStatementException();
  }
}

void Database::StatementCheckForErrorAndRollback(int return_value, const char *description) {
  if (return_value != SQLITE_OK && return_value != SQLITE_DONE && return_value != SQLITE_ROW) {
    BOOST_LOG_TRIVIAL(error) << "database::Database: " << description << ": " << return_value;
    Rollback();
    throw exception::detail::CantExecuteSqlStatementException();
  }
}

void Database::Rollback() {
  int ret = sqlite_interface_->Exec(db_handle_, "rollback", nullptr, nullptr, nullptr);
  if (ret != SQLITE_OK) {
    BOOST_LOG_TRIVIAL(error) << "database::Database: Rollback error: " << ret;
    throw exception::detail::CantExecuteSqlStatementException();
  }
}

int Database::GetApacheAgentNamesCallback(void *names_vptr, int argc, char **argv, char **azColName) {
  AgentNames *names_ptr = static_cast<AgentNames*> (names_vptr);

  for (int i = 0; i < argc; i++) {
    if (argv[i] != nullptr) {
      names_ptr->push_back(argv[i]);
    }
  }

  return 0;
}

}
