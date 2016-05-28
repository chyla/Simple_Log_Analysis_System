#include <memory>
#include <utility>

#include <patlms/type/time.h>
#include <patlms/type/bash_log_entry.h>
#include <patlms/type/apache_log_entry.h>

#include "src/database/database.h"
#include "src/database/exception/detail/cant_open_database_exception.h"
#include "src/database/exception/detail/cant_close_database_exception.h"
#include "src/database/exception/detail/cant_execute_sql_statement_exception.h"

#include "mock/database/sqlite.h"
#include "src/apache/type/anomaly_detection_configuration_entry.h"

using std::unique_ptr;
using std::move;
using namespace testing;
using namespace database;

#define DB_HANDLE_EXAMPLE_PTR_VALUE (reinterpret_cast<sqlite3*>(0x000001))
#define DB_STATEMENT_EXAMPLE_PTR_VALUE (reinterpret_cast<sqlite3_stmt*>(0x000002))

type::Time MY_DEFAULT_TIME1() {
  type::Time t;
  t.Set(21, 12, 3, 22, 4, 2011);
  return t;
}

type::Time MY_DEFAULT_TIME2() {
  type::Time t;
  t.Set(22, 13, 4, 23, 5, 2012);
  return t;
}

void MY_EXPECT_DEFAULT_TIME1(unique_ptr<mock::database::SQLite> &sqlite_mock) {
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, 21)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 3, 12)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 4, 3)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 5, 22)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 6, 4)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 7, 2011)).WillOnce(Return(SQLITE_OK));
}

void MY_EXPECT_DEFAULT_TIME2(unique_ptr<mock::database::SQLite> &sqlite_mock) {
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, 22)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 3, 13)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 4, 4)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 5, 23)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 6, 5)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 7, 2012)).WillOnce(Return(SQLITE_OK));
}

void MY_EXPECT_OPEN(unique_ptr<mock::database::SQLite> &sqlite_mock, int return_value = SQLITE_OK) {
  EXPECT_CALL(*sqlite_mock, Open(NotNull(), NotNull(), SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, IsNull()))
      .WillOnce(
                DoAll(SetArgPointee<1>(DB_HANDLE_EXAMPLE_PTR_VALUE),
                      Return(return_value)
                      )
                );
}

void MY_EXPECT_CLOSE(unique_ptr<mock::database::SQLite> &sqlite_mock, int return_value = SQLITE_OK) {
  EXPECT_CALL(*sqlite_mock, Close(DB_HANDLE_EXAMPLE_PTR_VALUE))
      .WillOnce(Return(return_value));
}

void MY_EXPECT_PREPARE(unique_ptr<mock::database::SQLite> &sqlite_mock, int times = 1, int return_value = SQLITE_OK) {
  EXPECT_CALL(*sqlite_mock, Prepare(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), -1, NotNull(), nullptr))
      .Times(times)
      .WillRepeatedly(
                      DoAll(SetArgPointee<3>(DB_STATEMENT_EXAMPLE_PTR_VALUE),
                            Return(return_value)
                            )
                      );
}

TEST(DatabaseTest, ConstructorTest) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  DatabasePtr database = Database::Create(move(sqlite_mock));

  EXPECT_FALSE(database->IsOpen());
}

TEST(DatabaseTest, OpenDatabase) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  EXPECT_TRUE(database->IsOpen());
}

TEST(DatabaseTest, OpenDatabaseWhenFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock, SQLITE_NOMEM);

  DatabasePtr database = Database::Create(move(sqlite_mock));

  EXPECT_THROW(database->Open("sqlite.db"), database::exception::detail::CantOpenDatabaseException);
  EXPECT_FALSE(database->IsOpen());
}

TEST(DatabaseTest, CloseDatabaseWhenOpened) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_CLOSE(sqlite_mock);

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  EXPECT_TRUE(database->IsOpen());
  EXPECT_TRUE(database->Close());
}

TEST(DatabaseTest, CloseDatabaseDoubleClose) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_CLOSE(sqlite_mock);

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  EXPECT_TRUE(database->IsOpen());
  EXPECT_TRUE(database->Close());
  EXPECT_FALSE(database->Close());
  EXPECT_FALSE(database->Close());
}

TEST(DatabaseTest, CloseDatabaseWhenFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_CLOSE(sqlite_mock, SQLITE_NOMEM);

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  EXPECT_TRUE(database->IsOpen());
  EXPECT_THROW(database->Close(), database::exception::detail::CantCloseDatabaseException);
}

TEST(DatabaseTest, CloseDatabaseWhenDatabaseIsClosed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());

  DatabasePtr database = Database::Create(move(sqlite_mock));

  EXPECT_FALSE(database->IsOpen());
  EXPECT_FALSE(database->Close());
}

TEST(DatabaseTest, CreateBashLogsTable) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).WillOnce(Return(SQLITE_OK));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  database->CreateBashLogsTable();
}

TEST(DatabaseTest, CreateBashLogsTableWhenDatabaseIsClosed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());

  DatabasePtr database = Database::Create(move(sqlite_mock));

  EXPECT_FALSE(database->IsOpen());
  EXPECT_THROW(database->CreateBashLogsTable(), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, CreateBashLogsTableWhenFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  EXPECT_THROW(database->CreateBashLogsTable(), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, CreateApacheSessionTable) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).WillOnce(Return(SQLITE_OK));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  database->CreateApacheSessionTable();
}

TEST(DatabaseTest, CreateApacheSessionTable_WhenDatabaseIsClosed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());

  DatabasePtr database = Database::Create(move(sqlite_mock));

  EXPECT_FALSE(database->IsOpen());
  EXPECT_THROW(database->CreateApacheSessionTable(), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, CreateApacheSessionTable_WhenFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  EXPECT_THROW(database->CreateApacheSessionTable(), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, CreateApacheLogsTable) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).WillOnce(Return(SQLITE_OK));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  database->CreateApacheLogsTable();
}

TEST(DatabaseTest, CreateApacheLogsTableWhenDatabaseIsClosed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());

  DatabasePtr database = Database::Create(move(sqlite_mock));

  EXPECT_FALSE(database->IsOpen());
  EXPECT_THROW(database->CreateApacheLogsTable(), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, CreateApacheLogsTableWhenFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  EXPECT_THROW(database->CreateApacheLogsTable(), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddBashLogsWithEmptyLogList) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  type::BashLogs logs;

  EXPECT_TRUE(database->AddBashLogs(logs));
}

TEST(DatabaseTest, AddBashLogsOneLogEntry) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  MY_EXPECT_DEFAULT_TIME1(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 8, 11)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 9, StrEq("command"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_DONE));
  EXPECT_CALL(*sqlite_mock, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_OK));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  type::BashLogs logs;
  logs.push_back({0, "agentname", MY_DEFAULT_TIME1(), 11, "command"});

  EXPECT_TRUE(database->AddBashLogs(logs));
}

TEST(DatabaseTest, AddBashLogsTwoLogsEntries) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));
  MY_EXPECT_PREPARE(sqlite_mock, 2);

  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  MY_EXPECT_DEFAULT_TIME1(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 8, 11)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 9, StrEq("command"), -1, nullptr)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname2"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  MY_EXPECT_DEFAULT_TIME2(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 8, 21)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 9, StrEq("command2"), -1, nullptr)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).Times(2).WillRepeatedly(Return(SQLITE_DONE));
  EXPECT_CALL(*sqlite_mock, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE)).Times(2).WillRepeatedly(Return(SQLITE_OK));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  type::BashLogs logs;
  logs.push_back({0, "agentname", MY_DEFAULT_TIME1(), 11, "command"});
  logs.push_back({0, "agentname2", MY_DEFAULT_TIME2(), 21, "command2"});

  EXPECT_TRUE(database->AddBashLogs(logs));
}

TEST(DatabaseTest, AddBashLogsOneLogEntryWhenPrepareFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));
  MY_EXPECT_PREPARE(sqlite_mock, 1, SQLITE_NOMEM);

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  type::BashLogs logs;
  logs.push_back({0, "agentname", MY_DEFAULT_TIME1(), 11, "command"});

  EXPECT_THROW(database->AddBashLogs(logs), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddBashLogsOneLogEntryWhenBindHostnameFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  type::BashLogs logs;
  logs.push_back({0, "agentname", MY_DEFAULT_TIME1(), 11, "command"});

  EXPECT_THROW(database->AddBashLogs(logs), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddBashLogsOneLogEntryWhenBindUidFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  MY_EXPECT_DEFAULT_TIME1(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 8, 11)).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  type::BashLogs logs;
  logs.push_back({0, "agentname", MY_DEFAULT_TIME1(), 11, "command"});

  EXPECT_THROW(database->AddBashLogs(logs), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddBashLogsOneLogEntryWhenBindCommandFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  MY_EXPECT_DEFAULT_TIME1(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 8, 11)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 9, StrEq("command"), -1, nullptr)).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  type::BashLogs logs;
  logs.push_back({0, "agentname", MY_DEFAULT_TIME1(), 11, "command"});

  EXPECT_THROW(database->AddBashLogs(logs), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddBashLogsOneLogEntryWhenStepFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  MY_EXPECT_DEFAULT_TIME1(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 8, 11)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 9, StrEq("command"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  type::BashLogs logs;
  logs.push_back({0, "agentname", MY_DEFAULT_TIME1(), 11, "command"});

  EXPECT_THROW(database->AddBashLogs(logs), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddBashLogsOneLogEntryWhenStepBusy) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  MY_EXPECT_DEFAULT_TIME1(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 8, 11)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 9, StrEq("command"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_BUSY));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  type::BashLogs logs;
  logs.push_back({0, "agentname", MY_DEFAULT_TIME1(), 11, "command"});

  EXPECT_FALSE(database->AddBashLogs(logs));
}

TEST(DatabaseTest, AddBashLogsOneLogEntryWhenFinalizeFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  MY_EXPECT_DEFAULT_TIME1(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 8, 11)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 9, StrEq("command"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_DONE));
  EXPECT_CALL(*sqlite_mock, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  type::BashLogs logs;
  logs.push_back({0, "agentname", MY_DEFAULT_TIME1(), 11, "command"});

  EXPECT_THROW(database->AddBashLogs(logs), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddBashLogsWhenDatabaseIsClosed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());

  DatabasePtr database = Database::Create(move(sqlite_mock));
  type::BashLogs logs;

  EXPECT_FALSE(database->IsOpen());
  EXPECT_THROW(database->AddBashLogs(logs), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddBashLogsWhenBeginTransactionFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull()))
      .WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  type::BashLogs logs;

  EXPECT_THROW(database->AddBashLogs(logs), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddBashLogsWhenEndTransactionFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull()))
      .WillOnce(Return(SQLITE_OK))
      .WillOnce(Return(SQLITE_NOMEM))
      .WillOnce(Return(SQLITE_OK));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  type::BashLogs logs;

  EXPECT_THROW(database->AddBashLogs(logs), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddBashLogsWhenEndTransactionFailedAndRollbackFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull()))
      .WillOnce(Return(SQLITE_OK))
      .WillOnce(Return(SQLITE_NOMEM))
      .WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  type::BashLogs logs;

  EXPECT_THROW(database->AddBashLogs(logs), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddBashLogsWhenEndTransactionBusy) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull()))
      .WillOnce(Return(SQLITE_OK))
      .WillOnce(Return(SQLITE_BUSY))
      .WillOnce(Return(SQLITE_OK));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  type::BashLogs logs;

  EXPECT_FALSE(database->AddBashLogs(logs));
}

TEST(DatabaseTest, AddBashLogsWhenEndTransactionBusyAndRollbackFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull()))
      .WillOnce(Return(SQLITE_OK))
      .WillOnce(Return(SQLITE_BUSY))
      .WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  type::BashLogs logs;

  EXPECT_THROW(database->AddBashLogs(logs), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddApacheSessionStatistics_WithEmptySessionsList) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  ::apache::type::ApacheSessions sessions;

  EXPECT_TRUE(database->AddApacheSessionStatistics(sessions));
}

TEST(DatabaseTest, AddApacheSessionStatistics_WithOneEntry) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq("vh1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 3, StrEq("127.0.0.1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 4, 21)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 5, 12)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 6, 3)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 7, 22)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 8, 4)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 9, 2011)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 10, 23)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 11, 941)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 12, 13)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 13, 44)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 14, StrEq("User-Agent Example"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 15, 1)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_DONE));
  EXPECT_CALL(*sqlite_mock, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_OK));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  ::apache::type::ApacheSessions sessions;
  sessions.push_back({0, "agentname", "vh1", "127.0.0.1", MY_DEFAULT_TIME1(), 23, 941, 13, 44, "User-Agent Example", true});

  EXPECT_TRUE(database->AddApacheSessionStatistics(sessions));
}

TEST(DatabaseTest, AddApacheSessionStatistics_WithTwoEntries) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));
  MY_EXPECT_PREPARE(sqlite_mock, 2);

  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq("vh1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 3, StrEq("127.0.0.1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 4, 21)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 5, 12)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 6, 3)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 7, 22)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 8, 4)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 9, 2011)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 10, 23)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 11, 941)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 12, 13)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 13, 44)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 14, StrEq("User-Agent Example"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 15, 0)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname2"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq("vh2"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 3, StrEq("127.0.0.12"), -1, nullptr)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 4, 22)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 5, 13)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 6, 4)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 7, 23)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 8, 5)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 9, 2012)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 10, 232)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 11, 9412)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 12, 132)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 13, 442)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 14, StrEq("User-Agent Example 2"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 15, 1)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).Times(2).WillRepeatedly(Return(SQLITE_DONE));
  EXPECT_CALL(*sqlite_mock, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE)).Times(2).WillRepeatedly(Return(SQLITE_OK));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  ::apache::type::ApacheSessions sessions;
  sessions.push_back({0, "agentname", "vh1", "127.0.0.1", MY_DEFAULT_TIME1(), 23, 941, 13, 44, "User-Agent Example", false});
  sessions.push_back({0, "agentname2", "vh2", "127.0.0.12", MY_DEFAULT_TIME2(), 232, 9412, 132, 442, "User-Agent Example 2", true});

  EXPECT_TRUE(database->AddApacheSessionStatistics(sessions));
}

TEST(DatabaseTest, AddApacheSessionStatistics_WhenPrepareFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));
  MY_EXPECT_PREPARE(sqlite_mock, 1, SQLITE_NOMEM);

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  ::apache::type::ApacheSessions sessions;
  sessions.push_back({0, "agentname", "vh1", "127.0.0.1", MY_DEFAULT_TIME1(), 23, 941, 13, 44, "User-Agent Example"});

  EXPECT_THROW(database->AddApacheSessionStatistics(sessions), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddApacheSessionStatistics_WhenFinalizeFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq("vh1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 3, StrEq("127.0.0.1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 4, 21)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 5, 12)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 6, 3)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 7, 22)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 8, 4)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 9, 2011)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 10, 23)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 11, 941)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 12, 13)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 13, 44)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 14, StrEq("User-Agent Example"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 15, 0)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_DONE));
  EXPECT_CALL(*sqlite_mock, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  ::apache::type::ApacheSessions sessions;
  sessions.push_back({0, "agentname", "vh1", "127.0.0.1", MY_DEFAULT_TIME1(), 23, 941, 13, 44, "User-Agent Example", false});

  EXPECT_THROW(database->AddApacheSessionStatistics(sessions), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddApacheSessionStatistics_WhenStepFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq("vh1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 3, StrEq("127.0.0.1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 4, 21)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 5, 12)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 6, 3)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 7, 22)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 8, 4)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 9, 2011)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 10, 23)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 11, 941)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 12, 13)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 13, 44)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 14, StrEq("User-Agent Example"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 15, 1)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  ::apache::type::ApacheSessions sessions;
  sessions.push_back({0, "agentname", "vh1", "127.0.0.1", MY_DEFAULT_TIME1(), 23, 941, 13, 44, "User-Agent Example", true});

  EXPECT_THROW(database->AddApacheSessionStatistics(sessions), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddApacheSessionStatistics_StepWhenDatabaseIsBusy) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq("vh1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 3, StrEq("127.0.0.1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 4, 21)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 5, 12)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 6, 3)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 7, 22)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 8, 4)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 9, 2011)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 10, 23)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 11, 941)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 12, 13)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 13, 44)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 14, StrEq("User-Agent Example"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 15, 1)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_BUSY));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  ::apache::type::ApacheSessions sessions;
  sessions.push_back({0, "agentname", "vh1", "127.0.0.1", MY_DEFAULT_TIME1(), 23, 941, 13, 44, "User-Agent Example", true});

  EXPECT_FALSE(database->AddApacheSessionStatistics(sessions));
}

TEST(DatabaseTest, AddApacheSessionStatistics_WhenBindUserAgentFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq("vh1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 3, StrEq("127.0.0.1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 4, 21)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 5, 12)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 6, 3)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 7, 22)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 8, 4)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 9, 2011)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 10, 23)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 11, 941)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 12, 13)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 13, 44)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 14, StrEq("User-Agent Example"), -1, nullptr)).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  ::apache::type::ApacheSessions sessions;
  sessions.push_back({0, "agentname", "vh1", "127.0.0.1", MY_DEFAULT_TIME1(), 23, 941, 13, 44, "User-Agent Example"});

  EXPECT_THROW(database->AddApacheSessionStatistics(sessions), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddApacheSessionStatistics_WhenBindErrorPercentageFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq("vh1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 3, StrEq("127.0.0.1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 4, 21)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 5, 12)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 6, 3)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 7, 22)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 8, 4)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 9, 2011)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 10, 23)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 11, 941)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 12, 13)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 13, 44)).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  ::apache::type::ApacheSessions sessions;
  sessions.push_back({0, "agentname", "vh1", "127.0.0.1", MY_DEFAULT_TIME1(), 23, 941, 13, 44, "User-Agent Example"});

  EXPECT_THROW(database->AddApacheSessionStatistics(sessions), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddApacheSessionStatistics_WhenBindRequestsCountFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq("vh1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 3, StrEq("127.0.0.1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 4, 21)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 5, 12)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 6, 3)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 7, 22)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 8, 4)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 9, 2011)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 10, 23)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 11, 941)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 12, 13)).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  ::apache::type::ApacheSessions sessions;
  sessions.push_back({0, "agentname", "vh1", "127.0.0.1", MY_DEFAULT_TIME1(), 23, 941, 13, 44, "User-Agent Example"});

  EXPECT_THROW(database->AddApacheSessionStatistics(sessions), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddApacheSessionStatistics_WhenBindBandwidthUsageFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq("vh1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 3, StrEq("127.0.0.1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 4, 21)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 5, 12)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 6, 3)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 7, 22)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 8, 4)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 9, 2011)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 10, 23)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 11, 941)).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  ::apache::type::ApacheSessions sessions;
  sessions.push_back({0, "agentname", "vh1", "127.0.0.1", MY_DEFAULT_TIME1(), 23, 941, 13, 44, "User-Agent Example"});

  EXPECT_THROW(database->AddApacheSessionStatistics(sessions), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddApacheSessionStatistics_WhenBindSessionLengthFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq("vh1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 3, StrEq("127.0.0.1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 4, 21)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 5, 12)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 6, 3)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 7, 22)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 8, 4)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 9, 2011)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 10, 23)).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  ::apache::type::ApacheSessions sessions;
  sessions.push_back({0, "agentname", "vh1", "127.0.0.1", MY_DEFAULT_TIME1(), 23, 941, 13, 44, "User-Agent Example"});

  EXPECT_THROW(database->AddApacheSessionStatistics(sessions), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddApacheSessionStatistics_WhenBindYearFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq("vh1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 3, StrEq("127.0.0.1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 4, 21)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 5, 12)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 6, 3)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 7, 22)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 8, 4)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 9, 2011)).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  ::apache::type::ApacheSessions sessions;
  sessions.push_back({0, "agentname", "vh1", "127.0.0.1", MY_DEFAULT_TIME1(), 23, 941, 13, 44, "User-Agent Example"});

  EXPECT_THROW(database->AddApacheSessionStatistics(sessions), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddApacheSessionStatistics_WhenBindMonthFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq("vh1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 3, StrEq("127.0.0.1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 4, 21)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 5, 12)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 6, 3)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 7, 22)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 8, 4)).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  ::apache::type::ApacheSessions sessions;
  sessions.push_back({0, "agentname", "vh1", "127.0.0.1", MY_DEFAULT_TIME1(), 23, 941, 13, 44, "User-Agent Example"});

  EXPECT_THROW(database->AddApacheSessionStatistics(sessions), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddApacheSessionStatistics_WhenBindDayFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq("vh1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 3, StrEq("127.0.0.1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 4, 21)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 5, 12)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 6, 3)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 7, 22)).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  ::apache::type::ApacheSessions sessions;
  sessions.push_back({0, "agentname", "vh1", "127.0.0.1", MY_DEFAULT_TIME1(), 23, 941, 13, 44, "User-Agent Example"});

  EXPECT_THROW(database->AddApacheSessionStatistics(sessions), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddApacheSessionStatistics_WhenBindSecondFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq("vh1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 3, StrEq("127.0.0.1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 4, 21)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 5, 12)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 6, 3)).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  ::apache::type::ApacheSessions sessions;
  sessions.push_back({0, "agentname", "vh1", "127.0.0.1", MY_DEFAULT_TIME1(), 23, 941, 13, 44, "User-Agent Example"});

  EXPECT_THROW(database->AddApacheSessionStatistics(sessions), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddApacheSessionStatistics_WhenBindMinuteFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq("vh1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 3, StrEq("127.0.0.1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 4, 21)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 5, 12)).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  ::apache::type::ApacheSessions sessions;
  sessions.push_back({0, "agentname", "vh1", "127.0.0.1", MY_DEFAULT_TIME1(), 23, 941, 13, 44, "User-Agent Example"});

  EXPECT_THROW(database->AddApacheSessionStatistics(sessions), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddApacheSessionStatistics_WhenBindHourFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq("vh1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 3, StrEq("127.0.0.1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 4, 21)).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  ::apache::type::ApacheSessions sessions;
  sessions.push_back({0, "agentname", "vh1", "127.0.0.1", MY_DEFAULT_TIME1(), 23, 941, 13, 44, "User-Agent Example"});

  EXPECT_THROW(database->AddApacheSessionStatistics(sessions), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddApacheSessionStatistics_WhenBindClientIPFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq("vh1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 3, StrEq("127.0.0.1"), -1, nullptr)).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  ::apache::type::ApacheSessions sessions;
  sessions.push_back({0, "agentname", "vh1", "127.0.0.1", MY_DEFAULT_TIME1(), 23, 941, 13, 44, "User-Agent Example"});

  EXPECT_THROW(database->AddApacheSessionStatistics(sessions), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddApacheSessionStatistics_WhenBindVirtualhostFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq("vh1"), -1, nullptr)).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  ::apache::type::ApacheSessions sessions;
  sessions.push_back({0, "agentname", "vh1", "127.0.0.1", MY_DEFAULT_TIME1(), 23, 941, 13, 44, "User-Agent Example"});

  EXPECT_THROW(database->AddApacheSessionStatistics(sessions), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddApacheSessionStatistics_WhenBindAgentNameFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  ::apache::type::ApacheSessions sessions;
  sessions.push_back({0, "agentname", "vh1", "127.0.0.1", MY_DEFAULT_TIME1(), 23, 941, 13, 44, "User-Agent Example"});

  EXPECT_THROW(database->AddApacheSessionStatistics(sessions), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, GetApacheLogsCount) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock, 1);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq("vh1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_DONE));
  EXPECT_CALL(*sqlite_mock, ColumnInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0)).WillOnce(Return(43));
  EXPECT_CALL(*sqlite_mock, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_OK));

  type::Time from, to;
  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  EXPECT_EQ(43, database->GetApacheLogsCount("agentname", "vh1", from, to));
}

TEST(DatabaseTest, GetApacheLogsCount_WhenFinalizeFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock, 1);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq("vh1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_DONE));
  EXPECT_CALL(*sqlite_mock, ColumnInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0)).WillOnce(Return(43));
  EXPECT_CALL(*sqlite_mock, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_NOMEM));

  type::Time from, to;
  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  EXPECT_THROW(database->GetApacheLogsCount("agentname", "vh1", from, to), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, GetApacheLogsCount_WhenStepFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock, 1);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq("vh1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_NOMEM));

  type::Time from, to;
  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  EXPECT_THROW(database->GetApacheLogsCount("agentname", "vh1", from, to), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, GetApacheLogsCount_WhenBindVirtualhostFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock, 1);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq("vh1"), -1, nullptr)).WillOnce(Return(SQLITE_NOMEM));

  type::Time from, to;
  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  EXPECT_THROW(database->GetApacheLogsCount("agentname", "vh1", from, to), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, GetApacheLogsCount_WhenBindAgentNameFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock, 1);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_NOMEM));

  type::Time from, to;
  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  EXPECT_THROW(database->GetApacheLogsCount("agentname", "vh1", from, to), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, GetApacheLogsCount_WhenPrepareFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock, 1, SQLITE_NOMEM);

  type::Time from, to;
  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  EXPECT_THROW(database->GetApacheLogsCount("agentname", "vh1", from, to), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, GetApacheAgentNames) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), NotNull(), NotNull(), IsNull())).Times(1).WillRepeatedly(Return(SQLITE_OK));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  auto names = database->GetApacheAgentNames();
  EXPECT_EQ(static_cast<unsigned>(0), names.size());
}

TEST(DatabaseTest, GetApacheAgentNames_WhenExecFail) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), NotNull(), NotNull(), IsNull())).Times(1).WillRepeatedly(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  EXPECT_THROW(database->GetApacheAgentNames(), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, GetApacheVirtualhostNames) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname2"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).Times(2).WillOnce(Return(SQLITE_ROW)).WillOnce(Return(SQLITE_DONE));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("virtualhost2")));
  EXPECT_CALL(*sqlite_mock, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_OK));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  auto names = database->GetApacheVirtualhostNames("agentname2");
  EXPECT_EQ(static_cast<unsigned>(1), names.size());
  EXPECT_STREQ("virtualhost2", names.at(0).c_str());
}

TEST(DatabaseTest, GetApacheVirtualhostNames_WhenTwoNamesAreAvailable) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname2"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).Times(3).WillOnce(Return(SQLITE_ROW)).WillOnce(Return(SQLITE_ROW)).WillOnce(Return(SQLITE_DONE));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0)).Times(2)
      .WillOnce(Return(reinterpret_cast<unsigned const char *> ("virtualhost2")))
      .WillOnce(Return(reinterpret_cast<unsigned const char *> ("virtualhost1")));
  EXPECT_CALL(*sqlite_mock, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_OK));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  auto names = database->GetApacheVirtualhostNames("agentname2");
  EXPECT_EQ(static_cast<unsigned>(2), names.size());
  EXPECT_STREQ("virtualhost2", names.at(0).c_str());
  EXPECT_STREQ("virtualhost1", names.at(1).c_str());
}

TEST(DatabaseTest, GetApacheVirtualhostNames_WhenColumnTextReturnsNull) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname2"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).Times(3).WillOnce(Return(SQLITE_ROW)).WillOnce(Return(SQLITE_ROW)).WillOnce(Return(SQLITE_DONE));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0)).Times(2)
      .WillOnce(Return(reinterpret_cast<unsigned const char *> ("virtualhost2")))
      .WillOnce(Return(nullptr));
  EXPECT_CALL(*sqlite_mock, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_OK));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  auto names = database->GetApacheVirtualhostNames("agentname2");
  EXPECT_EQ(static_cast<unsigned>(1), names.size());
  EXPECT_STREQ("virtualhost2", names.at(0).c_str());
}

TEST(DatabaseTest, GetApacheVirtualhostNames_WhenFinalizeFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname2"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).Times(2).WillOnce(Return(SQLITE_ROW)).WillOnce(Return(SQLITE_DONE));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("virtualhost2")));
  EXPECT_CALL(*sqlite_mock, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  EXPECT_THROW(database->GetApacheVirtualhostNames("agentname2"), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, GetApacheVirtualhostNames_WhenStepFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname2"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  EXPECT_THROW(database->GetApacheVirtualhostNames("agentname2"), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, GetApacheVirtualhostNames_WhenBindAgentNameFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname2"), -1, nullptr)).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  EXPECT_THROW(database->GetApacheVirtualhostNames("agentname2"), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, GetApacheVirtualhostNames_WhenPrepareFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock, 1, SQLITE_NOMEM);

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  EXPECT_THROW(database->GetApacheVirtualhostNames("agentname2"), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, GetApacheLogs) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq("vh1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).Times(2).WillOnce(Return(SQLITE_ROW)).WillOnce(Return(SQLITE_DONE));

  EXPECT_CALL(*sqlite_mock, ColumnInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0)).WillOnce(Return(1));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("agentname")));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("vh1")));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 3)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("127.0.0.1")));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 4)).WillOnce(Return(10));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 5)).WillOnce(Return(6));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 6)).WillOnce(Return(7));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 7)).WillOnce(Return(2));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 8)).WillOnce(Return(9));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 9)).WillOnce(Return(2013));

  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 10)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("request")));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 11)).WillOnce(Return(402));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 12)).WillOnce(Return(23));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 13)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("useragent")));

  EXPECT_CALL(*sqlite_mock, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_OK));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  type::Time from, to;
  from.Set(10, 0, 0, 1, 1, 2015);
  to.Set(10, 0, 0, 1, 1, 2017);
  auto names = database->GetApacheLogs("agentname", "vh1", from, to, 100, 0);
  EXPECT_EQ(static_cast<unsigned>(1), names.size());

  auto log_entry = names.at(0);
  EXPECT_EQ(1, log_entry.id);
  EXPECT_STREQ("agentname", log_entry.agent_name.c_str());
  EXPECT_STREQ("vh1", log_entry.virtualhost.c_str());
  EXPECT_STREQ("127.0.0.1", log_entry.client_ip.c_str());
  EXPECT_EQ(10, log_entry.time.GetHour());
  EXPECT_EQ(6, log_entry.time.GetMinute());
  EXPECT_EQ(7, log_entry.time.GetSecond());
  EXPECT_EQ(2, log_entry.time.GetDay());
  EXPECT_EQ(9, log_entry.time.GetMonth());
  EXPECT_EQ(2013, log_entry.time.GetYear());
  EXPECT_STREQ("request", log_entry.request.c_str());
  EXPECT_EQ(402, log_entry.status_code);
  EXPECT_EQ(23, log_entry.bytes);
  EXPECT_STREQ("useragent", log_entry.user_agent.c_str());
}

TEST(DatabaseTest, GetApacheLogs_WhenFinalizeFail) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq("vh1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).Times(2).WillOnce(Return(SQLITE_ROW)).WillOnce(Return(SQLITE_DONE));

  EXPECT_CALL(*sqlite_mock, ColumnInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0)).WillOnce(Return(1));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("agentname")));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("vh1")));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 3)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("127.0.0.1")));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 4)).WillOnce(Return(10));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 5)).WillOnce(Return(6));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 6)).WillOnce(Return(7));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 7)).WillOnce(Return(2));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 8)).WillOnce(Return(9));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 9)).WillOnce(Return(2013));

  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 10)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("request")));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 11)).WillOnce(Return(402));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 12)).WillOnce(Return(23));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 13)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("useragent")));

  EXPECT_CALL(*sqlite_mock, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  type::Time from, to;
  from.Set(10, 0, 0, 1, 1, 2015);
  to.Set(10, 0, 0, 1, 1, 2017);
  EXPECT_THROW(database->GetApacheLogs("agentname", "vh1", from, to, 100, 0), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, GetApacheLogs_WhenUserAgentReturnsNull) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq("vh1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).Times(2).WillOnce(Return(SQLITE_ROW)).WillOnce(Return(SQLITE_DONE));

  EXPECT_CALL(*sqlite_mock, ColumnInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0)).WillOnce(Return(1));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("agentname")));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("vh1")));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 3)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("127.0.0.1")));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 4)).WillOnce(Return(10));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 5)).WillOnce(Return(6));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 6)).WillOnce(Return(7));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 7)).WillOnce(Return(2));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 8)).WillOnce(Return(9));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 9)).WillOnce(Return(2013));

  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 10)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("request")));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 11)).WillOnce(Return(402));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 12)).WillOnce(Return(23));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 13)).WillOnce(Return(nullptr));

  EXPECT_CALL(*sqlite_mock, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_OK));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  type::Time from, to;
  from.Set(10, 0, 0, 1, 1, 2015);
  to.Set(10, 0, 0, 1, 1, 2017);
  auto names = database->GetApacheLogs("agentname", "vh1", from, to, 100, 0);
  EXPECT_EQ(static_cast<unsigned>(1), names.size());

  auto log_entry = names.at(0);
  EXPECT_EQ(1, log_entry.id);
  EXPECT_STREQ("agentname", log_entry.agent_name.c_str());
  EXPECT_STREQ("vh1", log_entry.virtualhost.c_str());
  EXPECT_STREQ("127.0.0.1", log_entry.client_ip.c_str());
  EXPECT_EQ(10, log_entry.time.GetHour());
  EXPECT_EQ(6, log_entry.time.GetMinute());
  EXPECT_EQ(7, log_entry.time.GetSecond());
  EXPECT_EQ(2, log_entry.time.GetDay());
  EXPECT_EQ(9, log_entry.time.GetMonth());
  EXPECT_EQ(2013, log_entry.time.GetYear());
  EXPECT_STREQ("request", log_entry.request.c_str());
  EXPECT_EQ(402, log_entry.status_code);
  EXPECT_EQ(23, log_entry.bytes);
  EXPECT_STREQ("", log_entry.user_agent.c_str());
}

TEST(DatabaseTest, GetApacheLogs_WhenRequestReturnsNull) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq("vh1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).Times(2).WillOnce(Return(SQLITE_ROW)).WillOnce(Return(SQLITE_DONE));

  EXPECT_CALL(*sqlite_mock, ColumnInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0)).WillOnce(Return(1));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("agentname")));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("vh1")));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 3)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("127.0.0.1")));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 4)).WillOnce(Return(10));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 5)).WillOnce(Return(6));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 6)).WillOnce(Return(7));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 7)).WillOnce(Return(2));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 8)).WillOnce(Return(9));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 9)).WillOnce(Return(2013));

  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 10)).WillOnce(Return(nullptr));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 11)).WillOnce(Return(402));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 12)).WillOnce(Return(23));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 13)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("useragent")));

  EXPECT_CALL(*sqlite_mock, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_OK));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  type::Time from, to;
  from.Set(10, 0, 0, 1, 1, 2015);
  to.Set(10, 0, 0, 1, 1, 2017);
  auto names = database->GetApacheLogs("agentname", "vh1", from, to, 100, 0);
  EXPECT_EQ(static_cast<unsigned>(1), names.size());

  auto log_entry = names.at(0);
  EXPECT_EQ(1, log_entry.id);
  EXPECT_STREQ("agentname", log_entry.agent_name.c_str());
  EXPECT_STREQ("vh1", log_entry.virtualhost.c_str());
  EXPECT_STREQ("127.0.0.1", log_entry.client_ip.c_str());
  EXPECT_EQ(10, log_entry.time.GetHour());
  EXPECT_EQ(6, log_entry.time.GetMinute());
  EXPECT_EQ(7, log_entry.time.GetSecond());
  EXPECT_EQ(2, log_entry.time.GetDay());
  EXPECT_EQ(9, log_entry.time.GetMonth());
  EXPECT_EQ(2013, log_entry.time.GetYear());
  EXPECT_STREQ("", log_entry.request.c_str());
  EXPECT_EQ(402, log_entry.status_code);
  EXPECT_EQ(23, log_entry.bytes);
  EXPECT_STREQ("useragent", log_entry.user_agent.c_str());
}

TEST(DatabaseTest, GetApacheLogs_WhenClientIPReturnsNull) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq("vh1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).Times(2).WillOnce(Return(SQLITE_ROW)).WillOnce(Return(SQLITE_DONE));

  EXPECT_CALL(*sqlite_mock, ColumnInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0)).WillOnce(Return(1));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("agentname")));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("vh1")));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 3)).WillOnce(Return(nullptr));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 4)).WillOnce(Return(10));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 5)).WillOnce(Return(6));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 6)).WillOnce(Return(7));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 7)).WillOnce(Return(2));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 8)).WillOnce(Return(9));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 9)).WillOnce(Return(2013));

  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 10)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("request")));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 11)).WillOnce(Return(402));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 12)).WillOnce(Return(23));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 13)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("useragent")));

  EXPECT_CALL(*sqlite_mock, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_OK));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  type::Time from, to;
  from.Set(10, 0, 0, 1, 1, 2015);
  to.Set(10, 0, 0, 1, 1, 2017);
  auto names = database->GetApacheLogs("agentname", "vh1", from, to, 100, 0);
  EXPECT_EQ(static_cast<unsigned>(1), names.size());

  auto log_entry = names.at(0);
  EXPECT_EQ(1, log_entry.id);
  EXPECT_STREQ("agentname", log_entry.agent_name.c_str());
  EXPECT_STREQ("vh1", log_entry.virtualhost.c_str());
  EXPECT_STREQ("", log_entry.client_ip.c_str());
  EXPECT_EQ(10, log_entry.time.GetHour());
  EXPECT_EQ(6, log_entry.time.GetMinute());
  EXPECT_EQ(7, log_entry.time.GetSecond());
  EXPECT_EQ(2, log_entry.time.GetDay());
  EXPECT_EQ(9, log_entry.time.GetMonth());
  EXPECT_EQ(2013, log_entry.time.GetYear());
  EXPECT_STREQ("request", log_entry.request.c_str());
  EXPECT_EQ(402, log_entry.status_code);
  EXPECT_EQ(23, log_entry.bytes);
  EXPECT_STREQ("useragent", log_entry.user_agent.c_str());
}

TEST(DatabaseTest, GetApacheLogs_WhenVirtualhostReturnsNull) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq("vh1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).Times(2).WillOnce(Return(SQLITE_ROW)).WillOnce(Return(SQLITE_DONE));

  EXPECT_CALL(*sqlite_mock, ColumnInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0)).WillOnce(Return(1));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("agentname")));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2)).WillOnce(Return(nullptr));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 3)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("127.0.0.1")));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 4)).WillOnce(Return(10));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 5)).WillOnce(Return(6));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 6)).WillOnce(Return(7));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 7)).WillOnce(Return(2));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 8)).WillOnce(Return(9));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 9)).WillOnce(Return(2013));

  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 10)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("request")));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 11)).WillOnce(Return(402));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 12)).WillOnce(Return(23));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 13)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("useragent")));

  EXPECT_CALL(*sqlite_mock, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_OK));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  type::Time from, to;
  from.Set(10, 0, 0, 1, 1, 2015);
  to.Set(10, 0, 0, 1, 1, 2017);
  auto names = database->GetApacheLogs("agentname", "vh1", from, to, 100, 0);
  EXPECT_EQ(static_cast<unsigned>(1), names.size());

  auto log_entry = names.at(0);
  EXPECT_EQ(1, log_entry.id);
  EXPECT_STREQ("agentname", log_entry.agent_name.c_str());
  EXPECT_STREQ("", log_entry.virtualhost.c_str());
  EXPECT_STREQ("127.0.0.1", log_entry.client_ip.c_str());
  EXPECT_EQ(10, log_entry.time.GetHour());
  EXPECT_EQ(6, log_entry.time.GetMinute());
  EXPECT_EQ(7, log_entry.time.GetSecond());
  EXPECT_EQ(2, log_entry.time.GetDay());
  EXPECT_EQ(9, log_entry.time.GetMonth());
  EXPECT_EQ(2013, log_entry.time.GetYear());
  EXPECT_STREQ("request", log_entry.request.c_str());
  EXPECT_EQ(402, log_entry.status_code);
  EXPECT_EQ(23, log_entry.bytes);
  EXPECT_STREQ("useragent", log_entry.user_agent.c_str());
}

TEST(DatabaseTest, GetApacheLogs_WhenAgentNameReturnsNull) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq("vh1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).Times(2).WillOnce(Return(SQLITE_ROW)).WillOnce(Return(SQLITE_DONE));

  EXPECT_CALL(*sqlite_mock, ColumnInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0)).WillOnce(Return(1));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1)).WillOnce(Return(nullptr));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("vh1")));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 3)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("127.0.0.1")));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 4)).WillOnce(Return(10));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 5)).WillOnce(Return(6));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 6)).WillOnce(Return(7));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 7)).WillOnce(Return(2));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 8)).WillOnce(Return(9));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 9)).WillOnce(Return(2013));

  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 10)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("request")));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 11)).WillOnce(Return(402));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 12)).WillOnce(Return(23));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 13)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("useragent")));

  EXPECT_CALL(*sqlite_mock, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_OK));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  type::Time from, to;
  from.Set(10, 0, 0, 1, 1, 2015);
  to.Set(10, 0, 0, 1, 1, 2017);
  auto names = database->GetApacheLogs("agentname", "vh1", from, to, 100, 0);
  EXPECT_EQ(static_cast<unsigned>(1), names.size());

  auto log_entry = names.at(0);
  EXPECT_EQ(1, log_entry.id);
  EXPECT_STREQ("", log_entry.agent_name.c_str());
  EXPECT_STREQ("vh1", log_entry.virtualhost.c_str());
  EXPECT_STREQ("127.0.0.1", log_entry.client_ip.c_str());
  EXPECT_EQ(10, log_entry.time.GetHour());
  EXPECT_EQ(6, log_entry.time.GetMinute());
  EXPECT_EQ(7, log_entry.time.GetSecond());
  EXPECT_EQ(2, log_entry.time.GetDay());
  EXPECT_EQ(9, log_entry.time.GetMonth());
  EXPECT_EQ(2013, log_entry.time.GetYear());
  EXPECT_STREQ("request", log_entry.request.c_str());
  EXPECT_EQ(402, log_entry.status_code);
  EXPECT_EQ(23, log_entry.bytes);
  EXPECT_STREQ("useragent", log_entry.user_agent.c_str());
}

TEST(DatabaseTest, GetApacheLogs_WhenStepFail) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq("vh1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  type::Time from, to;
  from.Set(10, 0, 0, 1, 1, 2015);
  to.Set(10, 0, 0, 1, 1, 2017);
  EXPECT_THROW(database->GetApacheLogs("agentname", "vh1", from, to, 100, 0), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, GetApacheLogs_WhenBindVirtualhostFail) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq("vh1"), -1, nullptr)).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  type::Time from, to;
  from.Set(10, 0, 0, 1, 1, 2015);
  to.Set(10, 0, 0, 1, 1, 2017);
  EXPECT_THROW(database->GetApacheLogs("agentname", "vh1", from, to, 100, 0), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, GetApacheLogs_WhenBindAgentNameFail) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  type::Time from, to;
  from.Set(10, 0, 0, 1, 1, 2015);
  to.Set(10, 0, 0, 1, 1, 2017);
  EXPECT_THROW(database->GetApacheLogs("agentname", "vh1", from, to, 100, 0), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, GetApacheLogs_WhenBindPrepareFail) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock, 1, SQLITE_NOMEM);

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  type::Time from, to;
  from.Set(10, 0, 0, 1, 1, 2015);
  to.Set(10, 0, 0, 1, 1, 2017);
  EXPECT_THROW(database->GetApacheLogs("agentname", "vh1", from, to, 100, 0), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, GetApacheSessionStatisticsCount) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock, 1);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq("vh1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_DONE));
  EXPECT_CALL(*sqlite_mock, ColumnInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0)).WillOnce(Return(43));
  EXPECT_CALL(*sqlite_mock, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_OK));

  type::Time from, to;
  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  EXPECT_EQ(43, database->GetApacheSessionStatisticsCount("agentname", "vh1", from, to));
}

TEST(DatabaseTest, GetApacheSessionStatisticsCount_WhenFinalizeFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock, 1);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq("vh1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_DONE));
  EXPECT_CALL(*sqlite_mock, ColumnInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0)).WillOnce(Return(43));
  EXPECT_CALL(*sqlite_mock, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_NOMEM));

  type::Time from, to;
  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  EXPECT_THROW(database->GetApacheSessionStatisticsCount("agentname", "vh1", from, to), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, GetApacheSessionStatisticsCount_WhenStepFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock, 1);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq("vh1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_NOMEM));

  type::Time from, to;
  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  EXPECT_THROW(database->GetApacheSessionStatisticsCount("agentname", "vh1", from, to), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, GetApacheSessionStatisticsCount_WhenBindVirtualhostFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock, 1);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq("vh1"), -1, nullptr)).WillOnce(Return(SQLITE_NOMEM));

  type::Time from, to;
  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  EXPECT_THROW(database->GetApacheSessionStatisticsCount("agentname", "vh1", from, to), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, GetApacheSessionStatisticsCount_WhenBindAgentNameFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock, 1);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_NOMEM));

  type::Time from, to;
  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  EXPECT_THROW(database->GetApacheSessionStatisticsCount("agentname", "vh1", from, to), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, GetApacheSessionStatisticsCount_WhenPrepareFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock, 1, SQLITE_NOMEM);

  type::Time from, to;
  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  EXPECT_THROW(database->GetApacheSessionStatisticsCount("agentname", "vh1", from, to), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, GetApacheSessionStatistics) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq("vh1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).Times(2).WillOnce(Return(SQLITE_ROW)).WillOnce(Return(SQLITE_DONE));

  EXPECT_CALL(*sqlite_mock, ColumnInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0)).WillOnce(Return(1));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("agentname")));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("vh1")));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 3)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("127.0.0.1")));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 4)).WillOnce(Return(10));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 5)).WillOnce(Return(6));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 6)).WillOnce(Return(7));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 7)).WillOnce(Return(2));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 8)).WillOnce(Return(9));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 9)).WillOnce(Return(2013));

  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 10)).WillOnce(Return(2));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 11)).WillOnce(Return(3));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 12)).WillOnce(Return(4));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 13)).WillOnce(Return(5));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 14)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("useragent")));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 15)).WillOnce(Return(1));

  EXPECT_CALL(*sqlite_mock, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_OK));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  type::Time from, to;
  from.Set(10, 0, 0, 1, 1, 2015);
  to.Set(10, 0, 0, 1, 1, 2017);
  auto names = database->GetApacheSessionStatistics("agentname", "vh1", from, to, 100, 0);
  EXPECT_EQ(static_cast<unsigned>(1), names.size());

  auto session = names.at(0);
  EXPECT_EQ(1, session.id);
  EXPECT_STREQ("agentname", session.agent_name.c_str());
  EXPECT_STREQ("vh1", session.virtualhost.c_str());
  EXPECT_STREQ("127.0.0.1", session.client_ip.c_str());
  EXPECT_EQ(10, session.session_start.GetHour());
  EXPECT_EQ(6, session.session_start.GetMinute());
  EXPECT_EQ(7, session.session_start.GetSecond());
  EXPECT_EQ(2, session.session_start.GetDay());
  EXPECT_EQ(9, session.session_start.GetMonth());
  EXPECT_EQ(2013, session.session_start.GetYear());
  EXPECT_EQ(2, session.session_length);
  EXPECT_EQ(3, session.bandwidth_usage);
  EXPECT_EQ(4, session.requests_count);
  EXPECT_EQ(5, session.error_percentage);
  EXPECT_STREQ("useragent", session.useragent.c_str());
  EXPECT_EQ(true, session.is_anomaly);
}

TEST(DatabaseTest, GetApacheSessionStatistics_WhenFinalizeFail) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq("vh1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).Times(2).WillOnce(Return(SQLITE_ROW)).WillOnce(Return(SQLITE_DONE));

  EXPECT_CALL(*sqlite_mock, ColumnInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0)).WillOnce(Return(1));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("agentname")));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("vh1")));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 3)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("127.0.0.1")));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 4)).WillOnce(Return(10));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 5)).WillOnce(Return(6));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 6)).WillOnce(Return(7));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 7)).WillOnce(Return(2));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 8)).WillOnce(Return(9));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 9)).WillOnce(Return(2013));

  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 10)).WillOnce(Return(2));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 11)).WillOnce(Return(3));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 12)).WillOnce(Return(4));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 13)).WillOnce(Return(5));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 14)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("useragent")));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 15)).WillOnce(Return(1));

  EXPECT_CALL(*sqlite_mock, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  type::Time from, to;
  from.Set(10, 0, 0, 1, 1, 2015);
  to.Set(10, 0, 0, 1, 1, 2017);
  EXPECT_THROW(database->GetApacheSessionStatistics("agentname", "vh1", from, to, 100, 0), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, GetApacheSessionStatistics_WhenStepFail) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq("vh1"), -1, nullptr)).WillOnce(Return(SQLITE_OK));

  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  type::Time from, to;
  from.Set(10, 0, 0, 1, 1, 2015);
  to.Set(10, 0, 0, 1, 1, 2017);
  EXPECT_THROW(database->GetApacheSessionStatistics("agentname", "vh1", from, to, 100, 0), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, GetApacheSessionStatistics_WhenBindVirtualhostFail) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq("vh1"), -1, nullptr)).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  type::Time from, to;
  from.Set(10, 0, 0, 1, 1, 2015);
  to.Set(10, 0, 0, 1, 1, 2017);
  EXPECT_THROW(database->GetApacheSessionStatistics("agentname", "vh1", from, to, 100, 0), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, GetApacheSessionStatistics_WhenBindAgentNameFail) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("agentname"), -1, nullptr)).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  type::Time from, to;
  from.Set(10, 0, 0, 1, 1, 2015);
  to.Set(10, 0, 0, 1, 1, 2017);
  EXPECT_THROW(database->GetApacheSessionStatistics("agentname", "vh1", from, to, 100, 0), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, GetApacheSessionStatistics_WhenPrepareFail) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock, 1, SQLITE_NOMEM);

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  type::Time from, to;
  from.Set(10, 0, 0, 1, 1, 2015);
  to.Set(10, 0, 0, 1, 1, 2017);
  EXPECT_THROW(database->GetApacheSessionStatistics("agentname", "vh1", from, to, 100, 0), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, CreateDateTable) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).WillOnce(Return(SQLITE_OK));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  database->CreateDateTable();
}

TEST(DatabaseTest, CreateDateTable_WhenExecFail) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  EXPECT_THROW(database->CreateDateTable(), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddDate) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).WillOnce(Return(SQLITE_OK));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  database->AddDate(1, 1, 2016);
}

TEST(DatabaseTest, AddDate_WhenExecFail) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  EXPECT_THROW(database->AddDate(1, 1, 2016), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, GetDateId) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).Times(1).WillOnce(Return(SQLITE_ROW));
  EXPECT_CALL(*sqlite_mock, ColumnInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0)).WillOnce(Return(7));
  EXPECT_CALL(*sqlite_mock, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_OK));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  EXPECT_EQ(7, database->GetDateId(1, 1, 2016));
}

TEST(DatabaseTest, GetDateId_WhenItemNotExist) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).Times(1).WillOnce(Return(SQLITE_DONE));
  EXPECT_CALL(*sqlite_mock, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_OK));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  EXPECT_EQ(-1, database->GetDateId(1, 1, 2016));
}

TEST(DatabaseTest, GetDateId_WhenFinalizeFail) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).Times(1).WillOnce(Return(SQLITE_ROW));
  EXPECT_CALL(*sqlite_mock, ColumnInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0)).WillOnce(Return(7));
  EXPECT_CALL(*sqlite_mock, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  EXPECT_THROW(database->GetDateId(1, 1, 2016), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, GetDateId_WhenStepFail) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).Times(1).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  EXPECT_THROW(database->GetDateId(1, 1, 2016), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, GetDateId_WhenPrepareFail) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock, 1, SQLITE_NOMEM);

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  EXPECT_THROW(database->GetDateId(1, 1, 2016), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, CreateApacheSessionExistsTable) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).WillOnce(Return(SQLITE_OK));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  database->CreateApacheSessionExistsTable();
}

TEST(DatabaseTest, CreateApacheSessionExistsTable_WhenExecFail) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  EXPECT_THROW(database->CreateApacheSessionExistsTable(), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, MarkApacheStatisticsAsCreatedFor) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).WillOnce(Return(SQLITE_OK));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  database->MarkApacheStatisticsAsCreatedFor(10, 10, 2016);
}

TEST(DatabaseTest, MarkApacheStatisticsAsCreatedFor_WhenExecFail) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  EXPECT_THROW(database->MarkApacheStatisticsAsCreatedFor(10, 10, 2016), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, IsApacheStatisticsCreatedFor_WhenExist) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_ROW));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0)).WillOnce(Return(1));
  EXPECT_CALL(*sqlite_mock, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_OK));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  EXPECT_EQ(true, database->IsApacheStatisticsCreatedFor(1, 1, 2016));
}

TEST(DatabaseTest, IsApacheStatisticsCreatedFor_WhenNotExist) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_ROW));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0)).WillOnce(Return(0));
  EXPECT_CALL(*sqlite_mock, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_OK));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  EXPECT_FALSE(database->IsApacheStatisticsCreatedFor(1, 1, 2016));
}

TEST(DatabaseTest, IsApacheStatisticsCreatedFor_WhenFinalizeFail) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_ROW));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 0)).WillOnce(Return(0));
  EXPECT_CALL(*sqlite_mock, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  EXPECT_THROW(database->IsApacheStatisticsCreatedFor(1, 1, 2016), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, IsApacheStatisticsCreatedFor_WhenStepFail) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  EXPECT_THROW(database->IsApacheStatisticsCreatedFor(1, 1, 2016), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, IsApacheStatisticsCreatedFor_WhenPrepareFail) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock, 1, SQLITE_NOMEM);

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  EXPECT_THROW(database->IsApacheStatisticsCreatedFor(1, 1, 2016), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, GetApacheOneSessionStatistic) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).Times(1).WillOnce(Return(SQLITE_ROW));

  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("agentname")));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("vh1")));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 3)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("127.0.0.1")));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 4)).WillOnce(Return(10));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 5)).WillOnce(Return(6));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 6)).WillOnce(Return(7));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 7)).WillOnce(Return(2));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 8)).WillOnce(Return(9));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 9)).WillOnce(Return(2013));

  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 10)).WillOnce(Return(2));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 11)).WillOnce(Return(3));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 12)).WillOnce(Return(4));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 13)).WillOnce(Return(5));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 14)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("useragent")));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 15)).WillOnce(Return(1));

  EXPECT_CALL(*sqlite_mock, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_OK));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  type::Time from, to;
  from.Set(10, 0, 0, 1, 1, 2015);
  to.Set(10, 0, 0, 1, 1, 2017);
  auto session = database->GetApacheOneSessionStatistic(1);
  EXPECT_EQ(1, session.id);
  EXPECT_STREQ("agentname", session.agent_name.c_str());
  EXPECT_STREQ("vh1", session.virtualhost.c_str());
  EXPECT_STREQ("127.0.0.1", session.client_ip.c_str());
  EXPECT_EQ(10, session.session_start.GetHour());
  EXPECT_EQ(6, session.session_start.GetMinute());
  EXPECT_EQ(7, session.session_start.GetSecond());
  EXPECT_EQ(2, session.session_start.GetDay());
  EXPECT_EQ(9, session.session_start.GetMonth());
  EXPECT_EQ(2013, session.session_start.GetYear());
  EXPECT_EQ(2, session.session_length);
  EXPECT_EQ(3, session.bandwidth_usage);
  EXPECT_EQ(4, session.requests_count);
  EXPECT_EQ(5, session.error_percentage);
  EXPECT_STREQ("useragent", session.useragent.c_str());
  EXPECT_EQ(true, session.is_anomaly);
}

TEST(DatabaseTest, GetApacheOneSessionStatistic_WhenFinalizeFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).Times(1).WillOnce(Return(SQLITE_ROW));

  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("agentname")));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("vh1")));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 3)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("127.0.0.1")));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 4)).WillOnce(Return(10));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 5)).WillOnce(Return(6));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 6)).WillOnce(Return(7));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 7)).WillOnce(Return(2));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 8)).WillOnce(Return(9));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 9)).WillOnce(Return(2013));

  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 10)).WillOnce(Return(2));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 11)).WillOnce(Return(3));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 12)).WillOnce(Return(4));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 13)).WillOnce(Return(5));
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 14)).WillOnce(Return(reinterpret_cast<unsigned const char *> ("useragent")));
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 15)).WillOnce(Return(1));

  EXPECT_CALL(*sqlite_mock, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  type::Time from, to;
  from.Set(10, 0, 0, 1, 1, 2015);
  to.Set(10, 0, 0, 1, 1, 2017);
  EXPECT_THROW(database->GetApacheOneSessionStatistic(1), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, GetApacheOneSessionStatistic_WhenStepFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).Times(1).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  type::Time from, to;
  from.Set(10, 0, 0, 1, 1, 2015);
  to.Set(10, 0, 0, 1, 1, 2017);
  EXPECT_THROW(database->GetApacheOneSessionStatistic(1), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, GetApacheOneSessionStatistic_WhenPrepareFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_PREPARE(sqlite_mock, 1, SQLITE_NOMEM);

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");

  type::Time from, to;
  from.Set(10, 0, 0, 1, 1, 2015);
  to.Set(10, 0, 0, 1, 1, 2017);
  EXPECT_THROW(database->GetApacheOneSessionStatistic(1), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, SetApacheSessionAsAnomaly) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).WillOnce(Return(SQLITE_OK));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  Database::RowIds ids{1};
  database->SetApacheSessionAsAnomaly(ids, ids);
}

TEST(DatabaseTest, SetApacheSessionAsAnomaly_WhenExecFail) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  Database::RowIds ids{1};
  EXPECT_THROW(database->SetApacheSessionAsAnomaly(ids, ids), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, CreateApacheAnomalyDetectionConfigurationTable) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).WillOnce(Return(SQLITE_OK));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  database->CreateApacheAnomalyDetectionConfigurationTable();
}

TEST(DatabaseTest, CreateApacheAnomalyDetectionConfigurationTable_WhenExecFail) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).WillOnce(Return(SQLITE_NOMEM));

  DatabasePtr database = Database::Create(move(sqlite_mock));
  database->Open("sqlite.db");
  EXPECT_THROW(database->CreateApacheAnomalyDetectionConfigurationTable(), database::exception::detail::CantExecuteSqlStatementException);
}
