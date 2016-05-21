#ifndef PATLMS_DATABASE_DATABASE_H
#define PATLMS_DATABASE_DATABASE_H

#include <string>
#include <memory>
#include <vector>

#include <patlms/type/time.h>

#include "detail/sqlite_interface.h"
#include "src/type/all.h"
#include "src/analyzer/apache_session_entry.h"

namespace database
{

class Database;
typedef std::shared_ptr<Database> DatabasePtr;

class Database {
 public:
  typedef std::vector<std::string> AgentNames;
  typedef std::vector<std::string> VirtualhostNames;

  static DatabasePtr Create();
  static DatabasePtr Create(std::unique_ptr<detail::SQLiteInterface> sqlite);

  void Open(const std::string &file_path);

  bool IsOpen() const;

  void CreateBashLogsTable();

  void CreateApacheLogsTable();

  void CreateApacheSessionTable();

  bool AddBashLogs(const type::BashLogs &log_entries);

  bool AddApacheLogs(const type::ApacheLogs &log_entries);

  bool AddApacheSessionStatistics(const analyzer::ApacheSessions &sessions);

  long long GetApacheSessionStatisticsCount(const std::string &agent_name, const std::string &virtualhost_name,
                                            const type::Time &from, const type::Time &to);

  long long GetApacheLogsCount(std::string agent_name, std::string virtualhost_name, type::Time from, type::Time to);

  type::ApacheLogs GetApacheLogs(std::string agent_name, std::string virtualhost_name, type::Time from, type::Time to, unsigned limit, long long offset);

  AgentNames GetApacheAgentNames();

  VirtualhostNames GetApacheVirtualhostNames(std::string agent_name);

  bool Close();

 private:
  bool is_open_;
  sqlite3 *db_handle_;
  std::unique_ptr<detail::SQLiteInterface> sqlite_interface_;

  Database(std::unique_ptr<detail::SQLiteInterface> sqlite);

  void StatementCheckForError(int return_value, const char *description);
  void StatementCheckForErrorAndRollback(int return_value, const char *description);
  void Rollback();
  static int GetApacheAgentNamesCallback(void *names_vptr, int argc, char **argv, char **azColName);
  std::string GetTimeRule(const type::Time &from, const type::Time &to) const;
  std::string TextHelper(unsigned const char *text) const;
  long long GetApacheCount(const std::string &table, const std::string &agent_name,
                           const std::string &virtualhost_name, const type::Time &from,
                           const type::Time &to);
};

}

#endif /* PATLMS_DATABASE_DATABASE_H */
