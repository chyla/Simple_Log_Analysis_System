#include <gmock/gmock.h>

#include "src/database/sqlite_wrapper.h"
#include "src/database/exception/detail/cant_open_database_exception.h"
#include "src/database/exception/detail/cant_close_database_exception.h"
#include "src/database/exception/detail/cant_execute_sql_statement_exception.h"

#include "mock/database/sqlite.h"

using namespace testing;
using namespace database;
using namespace std;

#define DB_HANDLE_EXAMPLE_PTR_VALUE (reinterpret_cast<sqlite3*>(0x000001))
#define DB_STATEMENT_EXAMPLE_PTR_VALUE (reinterpret_cast<sqlite3_stmt*>(0x000002))

class SQLiteWrapperTest : public ::testing::Test {
 public:
  ::mock::database::SQLitePtr sqlite_mock;
  const string example_text;

  SQLiteWrapperTest() :
  example_text("example text") {
  }
  virtual ~SQLiteWrapperTest() = default;

  void SetUp() {
    sqlite_mock = ::mock::database::SQLitePtr(new mock::database::SQLite());
  }

  void TearDown() {
  }

  void MY_EXPECT_OPEN(::mock::database::SQLitePtr &sqlite_mock, int return_value = SQLITE_OK) {
    EXPECT_CALL(*sqlite_mock, Open(NotNull(), NotNull(), SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, IsNull()))
        .WillOnce(
                  DoAll(SetArgPointee<1>(DB_HANDLE_EXAMPLE_PTR_VALUE),
                        Return(return_value)
                        )
                  );
  }

  void MY_EXPECT_CLOSE(::mock::database::SQLitePtr &sqlite_mock, int return_value = SQLITE_OK) {
    EXPECT_CALL(*sqlite_mock, Close(DB_HANDLE_EXAMPLE_PTR_VALUE))
        .WillOnce(Return(return_value));
  }
};

TEST_F(SQLiteWrapperTest, Create) {
  SQLiteWrapperPtr wrapper = SQLiteWrapper::Create(move(sqlite_mock));

  EXPECT_FALSE(wrapper->IsOpen());
}

TEST_F(SQLiteWrapperTest, Open) {
  MY_EXPECT_OPEN(sqlite_mock);

  SQLiteWrapperPtr wrapper = SQLiteWrapper::Create(move(sqlite_mock));
  wrapper->Open("sqlite.db");

  EXPECT_TRUE(wrapper->IsOpen());
}

TEST_F(SQLiteWrapperTest, Open_WhenOpenFail) {
  MY_EXPECT_OPEN(sqlite_mock, SQLITE_NOMEM);

  SQLiteWrapperPtr wrapper = SQLiteWrapper::Create(move(sqlite_mock));

  EXPECT_THROW(wrapper->Open("sqlite.db"), database::exception::detail::CantOpenDatabaseException);
  EXPECT_FALSE(wrapper->IsOpen());
}

TEST_F(SQLiteWrapperTest, Close_WhenDatabaseIsOpen) {
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_CLOSE(sqlite_mock);

  SQLiteWrapperPtr wrapper = SQLiteWrapper::Create(move(sqlite_mock));
  wrapper->Open("sqlite.db");

  EXPECT_TRUE(wrapper->IsOpen());
  EXPECT_TRUE(wrapper->Close());
}

TEST_F(SQLiteWrapperTest, Close_TripleCloseCall) {
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_CLOSE(sqlite_mock);

  SQLiteWrapperPtr wrapper = SQLiteWrapper::Create(move(sqlite_mock));
  wrapper->Open("sqlite.db");

  EXPECT_TRUE(wrapper->IsOpen());
  EXPECT_TRUE(wrapper->Close());
  EXPECT_FALSE(wrapper->Close());
  EXPECT_FALSE(wrapper->Close());
}

TEST_F(SQLiteWrapperTest, Close_WhenCloseFailed) {
  MY_EXPECT_OPEN(sqlite_mock);
  MY_EXPECT_CLOSE(sqlite_mock, SQLITE_NOMEM);

  SQLiteWrapperPtr wrapper = SQLiteWrapper::Create(move(sqlite_mock));
  wrapper->Open("sqlite.db");

  EXPECT_TRUE(wrapper->IsOpen());
  EXPECT_THROW(wrapper->Close(), database::exception::detail::CantCloseDatabaseException);
}

TEST_F(SQLiteWrapperTest, Prepare) {
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Prepare(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), -1, NotNull(), nullptr))
      .WillOnce(
                DoAll(SetArgPointee<3>(DB_STATEMENT_EXAMPLE_PTR_VALUE),
                      Return(SQLITE_OK))
                );
  MY_EXPECT_CLOSE(sqlite_mock);

  sqlite3_stmt *stmt;
  SQLiteWrapperPtr wrapper = SQLiteWrapper::Create(move(sqlite_mock));
  wrapper->Open("sqlite.db");

  EXPECT_TRUE(wrapper->IsOpen());
  wrapper->Prepare("sql query", &stmt);
  EXPECT_TRUE(wrapper->Close());
}

TEST_F(SQLiteWrapperTest, Prepare_WhenDatabaseIsClosed) {
  SQLiteWrapperPtr wrapper = SQLiteWrapper::Create(move(sqlite_mock));
  sqlite3_stmt *stmt;

  EXPECT_FALSE(wrapper->IsOpen());
  EXPECT_THROW(wrapper->Prepare("sql query", &stmt), database::exception::detail::CantExecuteSqlStatementException);
}

TEST_F(SQLiteWrapperTest, Prepare_WhenPrepareFail) {
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Prepare(DB_HANDLE_EXAMPLE_PTR_VALUE, NotNull(), -1, NotNull(), nullptr))
      .WillOnce(
                DoAll(SetArgPointee<3>(DB_STATEMENT_EXAMPLE_PTR_VALUE),
                      Return(SQLITE_NOMEM))
                );
  MY_EXPECT_CLOSE(sqlite_mock);

  sqlite3_stmt *stmt;
  SQLiteWrapperPtr wrapper = SQLiteWrapper::Create(move(sqlite_mock));
  wrapper->Open("sqlite.db");

  EXPECT_TRUE(wrapper->IsOpen());
  EXPECT_THROW(wrapper->Prepare("sql query", &stmt), database::exception::detail::CantExecuteSqlStatementException);
  EXPECT_TRUE(wrapper->Close());
}

TEST_F(SQLiteWrapperTest, BindDouble) {
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindDouble(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, 3.1)).WillOnce(Return(SQLITE_OK));
  MY_EXPECT_CLOSE(sqlite_mock);

  sqlite3_stmt *stmt = DB_STATEMENT_EXAMPLE_PTR_VALUE;
  SQLiteWrapperPtr wrapper = SQLiteWrapper::Create(move(sqlite_mock));
  wrapper->Open("sqlite.db");

  EXPECT_TRUE(wrapper->IsOpen());
  wrapper->BindDouble(stmt, 2, 3.1);
  EXPECT_TRUE(wrapper->Close());
}

TEST_F(SQLiteWrapperTest, BindDouble_WhenDatabaseIsClosed) {
  SQLiteWrapperPtr wrapper = SQLiteWrapper::Create(move(sqlite_mock));
  sqlite3_stmt *stmt = DB_STATEMENT_EXAMPLE_PTR_VALUE;

  EXPECT_FALSE(wrapper->IsOpen());
  EXPECT_THROW(wrapper->BindDouble(stmt, 2, 3.1), database::exception::detail::CantExecuteSqlStatementException);
}

TEST_F(SQLiteWrapperTest, BindDouble_WhenBindDoubleFail) {
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindDouble(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, 3.1)).WillOnce(Return(SQLITE_NOMEM));
  MY_EXPECT_CLOSE(sqlite_mock);

  sqlite3_stmt *stmt = DB_STATEMENT_EXAMPLE_PTR_VALUE;
  SQLiteWrapperPtr wrapper = SQLiteWrapper::Create(move(sqlite_mock));
  wrapper->Open("sqlite.db");

  EXPECT_TRUE(wrapper->IsOpen());
  EXPECT_THROW(wrapper->BindDouble(stmt, 2, 3.1), database::exception::detail::CantExecuteSqlStatementException);
  EXPECT_TRUE(wrapper->Close());
}

TEST_F(SQLiteWrapperTest, BindInt) {
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, 5)).WillOnce(Return(SQLITE_OK));
  MY_EXPECT_CLOSE(sqlite_mock);

  sqlite3_stmt *stmt = DB_STATEMENT_EXAMPLE_PTR_VALUE;
  SQLiteWrapperPtr wrapper = SQLiteWrapper::Create(move(sqlite_mock));
  wrapper->Open("sqlite.db");

  EXPECT_TRUE(wrapper->IsOpen());
  wrapper->BindInt(stmt, 2, 5);
  EXPECT_TRUE(wrapper->Close());
}

TEST_F(SQLiteWrapperTest, BindInt_WhenDatabaseIsClosed) {
  SQLiteWrapperPtr wrapper = SQLiteWrapper::Create(move(sqlite_mock));
  sqlite3_stmt *stmt = DB_STATEMENT_EXAMPLE_PTR_VALUE;

  EXPECT_FALSE(wrapper->IsOpen());
  EXPECT_THROW(wrapper->BindInt(stmt, 2, 5), database::exception::detail::CantExecuteSqlStatementException);
}

TEST_F(SQLiteWrapperTest, BindInt_WhenBindIntFail) {
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, 5)).WillOnce(Return(SQLITE_NOMEM));
  MY_EXPECT_CLOSE(sqlite_mock);

  sqlite3_stmt *stmt = DB_STATEMENT_EXAMPLE_PTR_VALUE;
  SQLiteWrapperPtr wrapper = SQLiteWrapper::Create(move(sqlite_mock));
  wrapper->Open("sqlite.db");

  EXPECT_TRUE(wrapper->IsOpen());
  EXPECT_THROW(wrapper->BindInt(stmt, 2, 5), database::exception::detail::CantExecuteSqlStatementException);
  EXPECT_TRUE(wrapper->Close());
}

TEST_F(SQLiteWrapperTest, BindInt64) {
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, 5)).WillOnce(Return(SQLITE_OK));
  MY_EXPECT_CLOSE(sqlite_mock);

  sqlite3_stmt *stmt = DB_STATEMENT_EXAMPLE_PTR_VALUE;
  SQLiteWrapperPtr wrapper = SQLiteWrapper::Create(move(sqlite_mock));
  wrapper->Open("sqlite.db");

  EXPECT_TRUE(wrapper->IsOpen());
  wrapper->BindInt64(stmt, 2, 5);
  EXPECT_TRUE(wrapper->Close());
}

TEST_F(SQLiteWrapperTest, BindInt64_WhenDatabaseIsClosed) {
  SQLiteWrapperPtr wrapper = SQLiteWrapper::Create(move(sqlite_mock));
  sqlite3_stmt *stmt = DB_STATEMENT_EXAMPLE_PTR_VALUE;

  EXPECT_FALSE(wrapper->IsOpen());
  EXPECT_THROW(wrapper->BindInt64(stmt, 2, 5), database::exception::detail::CantExecuteSqlStatementException);
}

TEST_F(SQLiteWrapperTest, BindInt64_WhenBindInt64Fail) {
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, 5)).WillOnce(Return(SQLITE_NOMEM));
  MY_EXPECT_CLOSE(sqlite_mock);

  sqlite3_stmt *stmt = DB_STATEMENT_EXAMPLE_PTR_VALUE;
  SQLiteWrapperPtr wrapper = SQLiteWrapper::Create(move(sqlite_mock));
  wrapper->Open("sqlite.db");

  EXPECT_TRUE(wrapper->IsOpen());
  EXPECT_THROW(wrapper->BindInt64(stmt, 2, 5), database::exception::detail::CantExecuteSqlStatementException);
  EXPECT_TRUE(wrapper->Close());
}

TEST_F(SQLiteWrapperTest, BindText) {
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq(example_text.c_str()), -1, SQLITE_STATIC)).WillOnce(Return(SQLITE_OK));
  MY_EXPECT_CLOSE(sqlite_mock);

  sqlite3_stmt *stmt = DB_STATEMENT_EXAMPLE_PTR_VALUE;
  SQLiteWrapperPtr wrapper = SQLiteWrapper::Create(move(sqlite_mock));
  wrapper->Open("sqlite.db");

  EXPECT_TRUE(wrapper->IsOpen());
  wrapper->BindText(stmt, 2, example_text);
  EXPECT_TRUE(wrapper->Close());
}

TEST_F(SQLiteWrapperTest, BindText_WhenDatabaseIsClosed) {
  SQLiteWrapperPtr wrapper = SQLiteWrapper::Create(move(sqlite_mock));
  sqlite3_stmt *stmt = DB_STATEMENT_EXAMPLE_PTR_VALUE;

  EXPECT_FALSE(wrapper->IsOpen());
  EXPECT_THROW(wrapper->BindText(stmt, 2, example_text), database::exception::detail::CantExecuteSqlStatementException);
}

TEST_F(SQLiteWrapperTest, BindText_WhenBindTextFail) {
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, BindText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 2, StrEq(example_text.c_str()), -1, SQLITE_STATIC)).WillOnce(Return(SQLITE_NOMEM));
  MY_EXPECT_CLOSE(sqlite_mock);

  sqlite3_stmt *stmt = DB_STATEMENT_EXAMPLE_PTR_VALUE;
  SQLiteWrapperPtr wrapper = SQLiteWrapper::Create(move(sqlite_mock));
  wrapper->Open("sqlite.db");

  EXPECT_TRUE(wrapper->IsOpen());
  EXPECT_THROW(wrapper->BindText(stmt, 2, example_text), database::exception::detail::CantExecuteSqlStatementException);
  EXPECT_TRUE(wrapper->Close());
}

TEST_F(SQLiteWrapperTest, ColumnDouble) {
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, ColumnDouble(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1)).WillOnce(Return(4.3));
  MY_EXPECT_CLOSE(sqlite_mock);

  sqlite3_stmt *stmt = DB_STATEMENT_EXAMPLE_PTR_VALUE;
  SQLiteWrapperPtr wrapper = SQLiteWrapper::Create(move(sqlite_mock));
  wrapper->Open("sqlite.db");

  EXPECT_TRUE(wrapper->IsOpen());
  EXPECT_EQ(4.3, wrapper->ColumnDouble(stmt, 1));
  EXPECT_TRUE(wrapper->Close());
}

TEST_F(SQLiteWrapperTest, ColumnDouble_WhenDatabaseIsClosed) {
  sqlite3_stmt *stmt = DB_STATEMENT_EXAMPLE_PTR_VALUE;
  SQLiteWrapperPtr wrapper = SQLiteWrapper::Create(move(sqlite_mock));

  EXPECT_FALSE(wrapper->IsOpen());
  EXPECT_THROW(wrapper->ColumnDouble(stmt, 1), database::exception::detail::CantExecuteSqlStatementException);
}

TEST_F(SQLiteWrapperTest, ColumnInt) {
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, ColumnInt(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1)).WillOnce(Return(4));
  MY_EXPECT_CLOSE(sqlite_mock);

  sqlite3_stmt *stmt = DB_STATEMENT_EXAMPLE_PTR_VALUE;
  SQLiteWrapperPtr wrapper = SQLiteWrapper::Create(move(sqlite_mock));
  wrapper->Open("sqlite.db");

  EXPECT_TRUE(wrapper->IsOpen());
  EXPECT_EQ(4, wrapper->ColumnInt(stmt, 1));
  EXPECT_TRUE(wrapper->Close());
}

TEST_F(SQLiteWrapperTest, ColumnInt_WhenDatabaseIsClosed) {
  sqlite3_stmt *stmt = DB_STATEMENT_EXAMPLE_PTR_VALUE;
  SQLiteWrapperPtr wrapper = SQLiteWrapper::Create(move(sqlite_mock));

  EXPECT_FALSE(wrapper->IsOpen());
  EXPECT_THROW(wrapper->ColumnInt(stmt, 1), database::exception::detail::CantExecuteSqlStatementException);
}

TEST_F(SQLiteWrapperTest, ColumnInt64) {
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, ColumnInt64(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1)).WillOnce(Return(4));
  MY_EXPECT_CLOSE(sqlite_mock);

  sqlite3_stmt *stmt = DB_STATEMENT_EXAMPLE_PTR_VALUE;
  SQLiteWrapperPtr wrapper = SQLiteWrapper::Create(move(sqlite_mock));
  wrapper->Open("sqlite.db");

  EXPECT_TRUE(wrapper->IsOpen());
  EXPECT_EQ(4, wrapper->ColumnInt64(stmt, 1));
  EXPECT_TRUE(wrapper->Close());
}

TEST_F(SQLiteWrapperTest, ColumnInt64_WhenDatabaseIsClosed) {
  sqlite3_stmt *stmt = DB_STATEMENT_EXAMPLE_PTR_VALUE;
  SQLiteWrapperPtr wrapper = SQLiteWrapper::Create(move(sqlite_mock));

  EXPECT_FALSE(wrapper->IsOpen());
  EXPECT_THROW(wrapper->ColumnInt64(stmt, 1), database::exception::detail::CantExecuteSqlStatementException);
}

TEST_F(SQLiteWrapperTest, ColumnText) {
  const unsigned char *uexample_text = reinterpret_cast<const unsigned char*> (example_text.c_str());
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1)).WillOnce(Return(uexample_text));
  MY_EXPECT_CLOSE(sqlite_mock);

  sqlite3_stmt *stmt = DB_STATEMENT_EXAMPLE_PTR_VALUE;
  SQLiteWrapperPtr wrapper = SQLiteWrapper::Create(move(sqlite_mock));
  wrapper->Open("sqlite.db");

  EXPECT_TRUE(wrapper->IsOpen());

  EXPECT_EQ(example_text, wrapper->ColumnText(stmt, 1));
  EXPECT_TRUE(wrapper->Close());
}

TEST_F(SQLiteWrapperTest, ColumnText_WhenFieldIsNull) {
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, ColumnText(DB_STATEMENT_EXAMPLE_PTR_VALUE, 1)).WillOnce(Return(nullptr));
  MY_EXPECT_CLOSE(sqlite_mock);

  sqlite3_stmt *stmt = DB_STATEMENT_EXAMPLE_PTR_VALUE;
  SQLiteWrapperPtr wrapper = SQLiteWrapper::Create(move(sqlite_mock));
  wrapper->Open("sqlite.db");

  EXPECT_TRUE(wrapper->IsOpen());

  EXPECT_EQ(string(""), wrapper->ColumnText(stmt, 1));
  EXPECT_TRUE(wrapper->Close());
}

TEST_F(SQLiteWrapperTest, ColumnText_WhenDatabaseIsClosed) {
  SQLiteWrapperPtr wrapper = SQLiteWrapper::Create(move(sqlite_mock));
  sqlite3_stmt *stmt = DB_STATEMENT_EXAMPLE_PTR_VALUE;

  EXPECT_FALSE(wrapper->IsOpen());
  EXPECT_THROW(wrapper->ColumnText(stmt, 2), database::exception::detail::CantExecuteSqlStatementException);
}

TEST_F(SQLiteWrapperTest, Step) {
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_OK));
  MY_EXPECT_CLOSE(sqlite_mock);

  sqlite3_stmt *stmt = DB_STATEMENT_EXAMPLE_PTR_VALUE;
  SQLiteWrapperPtr wrapper = SQLiteWrapper::Create(move(sqlite_mock));
  wrapper->Open("sqlite.db");

  EXPECT_TRUE(wrapper->IsOpen());
  EXPECT_EQ(SQLITE_OK, wrapper->Step(stmt));
  EXPECT_TRUE(wrapper->Close());
}

TEST_F(SQLiteWrapperTest, Step_WhenDatabaseIsClosed) {
  sqlite3_stmt *stmt = DB_STATEMENT_EXAMPLE_PTR_VALUE;
  SQLiteWrapperPtr wrapper = SQLiteWrapper::Create(move(sqlite_mock));

  EXPECT_FALSE(wrapper->IsOpen());
  EXPECT_THROW(wrapper->Step(stmt), database::exception::detail::CantExecuteSqlStatementException);
}

TEST_F(SQLiteWrapperTest, Step_WhenSqliteRowReturned) {
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_ROW));
  MY_EXPECT_CLOSE(sqlite_mock);

  sqlite3_stmt *stmt = DB_STATEMENT_EXAMPLE_PTR_VALUE;
  SQLiteWrapperPtr wrapper = SQLiteWrapper::Create(move(sqlite_mock));
  wrapper->Open("sqlite.db");

  EXPECT_TRUE(wrapper->IsOpen());
  EXPECT_EQ(SQLITE_ROW, wrapper->Step(stmt));
  EXPECT_TRUE(wrapper->Close());
}

TEST_F(SQLiteWrapperTest, Step_WhenStepFail) {
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Step(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_NOMEM));
  MY_EXPECT_CLOSE(sqlite_mock);

  sqlite3_stmt *stmt = DB_STATEMENT_EXAMPLE_PTR_VALUE;
  SQLiteWrapperPtr wrapper = SQLiteWrapper::Create(move(sqlite_mock));
  wrapper->Open("sqlite.db");

  EXPECT_TRUE(wrapper->IsOpen());
  EXPECT_THROW(wrapper->Step(stmt), database::exception::detail::CantExecuteSqlStatementException);
  EXPECT_TRUE(wrapper->Close());
}

TEST_F(SQLiteWrapperTest, Finalize) {
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_OK));
  MY_EXPECT_CLOSE(sqlite_mock);

  sqlite3_stmt *stmt = DB_STATEMENT_EXAMPLE_PTR_VALUE;
  SQLiteWrapperPtr wrapper = SQLiteWrapper::Create(move(sqlite_mock));
  wrapper->Open("sqlite.db");

  EXPECT_TRUE(wrapper->IsOpen());
  wrapper->Finalize(stmt);
  EXPECT_TRUE(wrapper->Close());
}

TEST_F(SQLiteWrapperTest, Finalize_WhenDatabaseIsClosed) {
  sqlite3_stmt *stmt = DB_STATEMENT_EXAMPLE_PTR_VALUE;
  SQLiteWrapperPtr wrapper = SQLiteWrapper::Create(move(sqlite_mock));

  EXPECT_FALSE(wrapper->IsOpen());
  EXPECT_THROW(wrapper->Finalize(stmt), database::exception::detail::CantExecuteSqlStatementException);
}

TEST_F(SQLiteWrapperTest, Finalize_WhenFinalizeFail) {
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Finalize(DB_STATEMENT_EXAMPLE_PTR_VALUE)).WillOnce(Return(SQLITE_NOMEM));
  MY_EXPECT_CLOSE(sqlite_mock);

  sqlite3_stmt *stmt = DB_STATEMENT_EXAMPLE_PTR_VALUE;
  SQLiteWrapperPtr wrapper = SQLiteWrapper::Create(move(sqlite_mock));
  wrapper->Open("sqlite.db");

  EXPECT_TRUE(wrapper->IsOpen());
  EXPECT_THROW(wrapper->Finalize(stmt), database::exception::detail::CantExecuteSqlStatementException);
  EXPECT_TRUE(wrapper->Close());
}

TEST_F(SQLiteWrapperTest, Exec) {
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, StrEq(example_text), nullptr, nullptr, nullptr)).WillOnce(Return(SQLITE_OK));
  MY_EXPECT_CLOSE(sqlite_mock);

  SQLiteWrapperPtr wrapper = SQLiteWrapper::Create(move(sqlite_mock));
  wrapper->Open("sqlite.db");

  EXPECT_TRUE(wrapper->IsOpen());
  wrapper->Exec(example_text, nullptr, nullptr);
  EXPECT_TRUE(wrapper->Close());
}

TEST_F(SQLiteWrapperTest, Exec_WhenDatabaseIsClosed) {
  SQLiteWrapperPtr wrapper = SQLiteWrapper::Create(move(sqlite_mock));

  EXPECT_FALSE(wrapper->IsOpen());
  EXPECT_THROW(wrapper->Exec(example_text, nullptr, nullptr), database::exception::detail::CantExecuteSqlStatementException);
}

TEST_F(SQLiteWrapperTest, Exec_WhenFinalizeFail) {
  MY_EXPECT_OPEN(sqlite_mock);
  EXPECT_CALL(*sqlite_mock, Exec(DB_HANDLE_EXAMPLE_PTR_VALUE, StrEq(example_text), nullptr, nullptr, nullptr)).WillOnce(Return(SQLITE_NOMEM));
  MY_EXPECT_CLOSE(sqlite_mock);

  SQLiteWrapperPtr wrapper = SQLiteWrapper::Create(move(sqlite_mock));
  wrapper->Open("sqlite.db");

  EXPECT_TRUE(wrapper->IsOpen());
  EXPECT_THROW(wrapper->Exec(example_text, nullptr, nullptr), database::exception::detail::CantExecuteSqlStatementException);
  EXPECT_TRUE(wrapper->Close());
}
