#include <memory>
#include <utility>

#include "src/database/database.h"
#include "src/database/exception/detail/cant_open_database_exception.h"
#include "src/database/exception/detail/cant_close_database_exception.h"
#include "src/database/exception/detail/cant_execute_sql_statement_exception.h"
#include "src/type/all.h"

#include "mock/database/sqlite.h"

using std::unique_ptr;
using std::move;
using namespace testing;

#define DB_HANDLE_EXAMPLE_PTR_VALUE (reinterpret_cast<sqlite3*>(0x000001))
#define DB_STATEMENT_EXAMPLE_PTR_VALUE (reinterpret_cast<sqlite3_stmt*>(0x000002))

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
  database::Database database(move(sqlite_mock));

  EXPECT_FALSE(database.IsOpen());
}

TEST(DatabaseTest, OpenDatabase) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);

  database::Database database(move(sqlite_mock));
  database.Open("sqlite.db");

  EXPECT_TRUE(database.IsOpen());
}

TEST(DatabaseTest, OpenDatabaseWhenFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock, SQLITE_NOMEM);

  database::Database database(move(sqlite_mock));

  EXPECT_THROW(database.Open("sqlite.db"), database::exception::detail::CantOpenDatabaseException);
  EXPECT_FALSE(database.IsOpen());
}

TEST(DatabaseTest, CloseDatabaseWhenOpened) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_CLOSE(sqlite_mock);

  database::Database database(move(sqlite_mock));
  database.Open("sqlite.db");

  EXPECT_TRUE(database.IsOpen());
  EXPECT_TRUE(database.Close());
}

TEST(DatabaseTest, CloseDatabaseDoubleClose) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_CLOSE(sqlite_mock);

  database::Database database(move(sqlite_mock));
  database.Open("sqlite.db");

  EXPECT_TRUE(database.IsOpen());
  EXPECT_TRUE(database.Close());
  EXPECT_FALSE(database.Close());
  EXPECT_FALSE(database.Close());
}

TEST(DatabaseTest, CloseDatabaseWhenFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_CLOSE(sqlite_mock, SQLITE_NOMEM);

  database::Database database(move(sqlite_mock));
  database.Open("sqlite.db");

  EXPECT_TRUE(database.IsOpen());
  EXPECT_THROW(database.Close(), database::exception::detail::CantCloseDatabaseException);
}

TEST(DatabaseTest, CloseDatabaseWhenDatabaseIsClosed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());

  database::Database database(move(sqlite_mock));

  EXPECT_FALSE(database.IsOpen());
  EXPECT_FALSE(database.Close());
}

TEST(DatabaseTest, CreateBashLogsTable) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).WillOnce(Return(SQLITE_OK));

  database::Database database(move(sqlite_mock));
  database.Open("sqlite.db");
  database.CreateBashLogsTable();
}

TEST(DatabaseTest, CreateBashLogsTableWhenDatabaseIsClosed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());

  database::Database database(move(sqlite_mock));

  EXPECT_FALSE(database.IsOpen());
  EXPECT_THROW(database.CreateBashLogsTable(), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, CreateBashLogsTableWhenFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).WillOnce(Return(SQLITE_NOMEM));

  database::Database database(move(sqlite_mock));
  database.Open("sqlite.db");
  EXPECT_THROW(database.CreateBashLogsTable(), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddBashLogsWithEmptyLogList) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));

  database::Database database(move(sqlite_mock));
  database.Open("sqlite.db");
  type::BashLogs logs;

  EXPECT_TRUE(database.AddBashLogs(logs));
}

TEST(DatabaseTest, AddBashLogsOneLogEntry) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("hostname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, 10)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 3, 11)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 4, StrEq("command"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_DONE));
  EXPECT_CALL(*sqlite_mock, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_OK));

  database::Database database(move(sqlite_mock));
  database.Open("sqlite.db");
  type::BashLogs logs;
  logs.push_back({"hostname", 10, 11, "command"});

  EXPECT_TRUE(database.AddBashLogs(logs));
}

TEST(DatabaseTest, AddBashLogsTwoLogsEntries) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));
  MY_EXPECT_PREPARE(sqlite_mock, 2);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("hostname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, 10)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 3, 11)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 4, StrEq("command"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("hostname2"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, 20)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 3, 21)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 4, StrEq("command2"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).Times(2).WillRepeatedly(Return(SQLITE_DONE));
  EXPECT_CALL(*sqlite_mock, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE)).Times(2).WillRepeatedly(Return(SQLITE_OK));

  database::Database database(move(sqlite_mock));
  database.Open("sqlite.db");
  type::BashLogs logs;
  logs.push_back({"hostname", 10, 11, "command"});
  logs.push_back({"hostname2", 20, 21, "command2"});

  EXPECT_TRUE(database.AddBashLogs(logs));
}

TEST(DatabaseTest, AddBashLogsOneLogEntryWhenPrepareFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));
  MY_EXPECT_PREPARE(sqlite_mock, 1, SQLITE_NOMEM);

  database::Database database(move(sqlite_mock));
  database.Open("sqlite.db");
  type::BashLogs logs;
  logs.push_back({"hostname", 10, 11, "command"});

  EXPECT_THROW(database.AddBashLogs(logs), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddBashLogsOneLogEntryWhenBindHostnameFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("hostname"), -1, nullptr)).WillOnce(Return(SQLITE_NOMEM));

  database::Database database(move(sqlite_mock));
  database.Open("sqlite.db");
  type::BashLogs logs;
  logs.push_back({"hostname", 10, 11, "command"});

  EXPECT_THROW(database.AddBashLogs(logs), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddBashLogsOneLogEntryWhenBindTimeFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("hostname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, 10)).WillOnce(Return(SQLITE_NOMEM));

  database::Database database(move(sqlite_mock));
  database.Open("sqlite.db");
  type::BashLogs logs;
  logs.push_back({"hostname", 10, 11, "command"});

  EXPECT_THROW(database.AddBashLogs(logs), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddBashLogsOneLogEntryWhenBindUidFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("hostname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, 10)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 3, 11)).WillOnce(Return(SQLITE_NOMEM));

  database::Database database(move(sqlite_mock));
  database.Open("sqlite.db");
  type::BashLogs logs;
  logs.push_back({"hostname", 10, 11, "command"});

  EXPECT_THROW(database.AddBashLogs(logs), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddBashLogsOneLogEntryWhenBindCommandFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("hostname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, 10)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 3, 11)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 4, StrEq("command"), -1, nullptr)).WillOnce(Return(SQLITE_NOMEM));

  database::Database database(move(sqlite_mock));
  database.Open("sqlite.db");
  type::BashLogs logs;
  logs.push_back({"hostname", 10, 11, "command"});

  EXPECT_THROW(database.AddBashLogs(logs), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddBashLogsOneLogEntryWhenStepFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("hostname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, 10)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 3, 11)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 4, StrEq("command"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_NOMEM));

  database::Database database(move(sqlite_mock));
  database.Open("sqlite.db");
  type::BashLogs logs;
  logs.push_back({"hostname", 10, 11, "command"});

  EXPECT_THROW(database.AddBashLogs(logs), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddBashLogsOneLogEntryWhenStepBusy) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("hostname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, 10)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 3, 11)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 4, StrEq("command"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_BUSY));

  database::Database database(move(sqlite_mock));
  database.Open("sqlite.db");
  type::BashLogs logs;
  logs.push_back({"hostname", 10, 11, "command"});

  EXPECT_FALSE(database.AddBashLogs(logs));
}

TEST(DatabaseTest, AddBashLogsOneLogEntryWhenFinalizeFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull())).Times(2).WillRepeatedly(Return(SQLITE_OK));
  MY_EXPECT_PREPARE(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1, StrEq("hostname"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, 10)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 3, 11)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 4, StrEq("command"), -1, nullptr)).WillOnce(Return(SQLITE_OK));
  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_DONE));
  EXPECT_CALL(*sqlite_mock, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_NOMEM));

  database::Database database(move(sqlite_mock));
  database.Open("sqlite.db");
  type::BashLogs logs;
  logs.push_back({"hostname", 10, 11, "command"});

  EXPECT_THROW(database.AddBashLogs(logs), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddBashLogsWhenDatabaseIsClosed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());

  database::Database database(move(sqlite_mock));
  type::BashLogs logs;

  EXPECT_FALSE(database.IsOpen());
  EXPECT_THROW(database.AddBashLogs(logs), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddBashLogsWhenBeginTransactionFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull()))
    .WillOnce(Return(SQLITE_NOMEM));

  database::Database database(move(sqlite_mock));
  database.Open("sqlite.db");
  type::BashLogs logs;

  EXPECT_THROW(database.AddBashLogs(logs), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddBashLogsWhenEndTransactionFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull()))
    .WillOnce(Return(SQLITE_OK))
    .WillOnce(Return(SQLITE_NOMEM))
    .WillOnce(Return(SQLITE_OK));

  database::Database database(move(sqlite_mock));
  database.Open("sqlite.db");
  type::BashLogs logs;

  EXPECT_THROW(database.AddBashLogs(logs), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddBashLogsWhenEndTransactionFailedAndRollbackFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull()))
    .WillOnce(Return(SQLITE_OK))
    .WillOnce(Return(SQLITE_NOMEM))
    .WillOnce(Return(SQLITE_NOMEM));

  database::Database database(move(sqlite_mock));
  database.Open("sqlite.db");
  type::BashLogs logs;

  EXPECT_THROW(database.AddBashLogs(logs), database::exception::detail::CantExecuteSqlStatementException);
}

TEST(DatabaseTest, AddBashLogsWhenEndTransactionBusy) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull()))
    .WillOnce(Return(SQLITE_OK))
    .WillOnce(Return(SQLITE_BUSY))
    .WillOnce(Return(SQLITE_OK));

  database::Database database(move(sqlite_mock));
  database.Open("sqlite.db");
  type::BashLogs logs;

  EXPECT_FALSE(database.AddBashLogs(logs));
}

TEST(DatabaseTest, AddBashLogsWhenEndTransactionBusyAndRollbackFailed) {
  unique_ptr<mock::database::SQLite> sqlite_mock(new mock::database::SQLite());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), IsNull(), IsNull(), IsNull()))
    .WillOnce(Return(SQLITE_OK))
    .WillOnce(Return(SQLITE_BUSY))
    .WillOnce(Return(SQLITE_NOMEM));

  database::Database database(move(sqlite_mock));
  database.Open("sqlite.db");
  type::BashLogs logs;

  EXPECT_THROW(database.AddBashLogs(logs), database::exception::detail::CantExecuteSqlStatementException);
}
