#ifndef PATLMS_DATABASE_DATABASE_H
#define PATLMS_DATABASE_DATABASE_H

#include <string>
#include <memory>
#include <vector>

#include "detail/sqlite_interface.h"
#include "src/type/all.h"


namespace database
{

class Database {
 public:
  Database(std::unique_ptr<detail::SQLiteInterface> sqlite);

  void Open(const std::string &file_path);

  bool IsOpen() const;
  
  void CreateBashLogsTable();

  bool AddBashLogs(const type::BashLogs &log_entries);

  bool Close();

 private:
  bool is_open_;
  sqlite3 *db_handle_;
  std::unique_ptr<detail::SQLiteInterface> sqlite_interface_;
};

}

#endif /* PATLMS_DATABASE_DATABASE_H */
