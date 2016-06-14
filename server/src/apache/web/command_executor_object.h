#ifndef SRC_APACHE_WEB_COMMAND_EXECUTOR_OBJECT_H
#define SRC_APACHE_WEB_COMMAND_EXECUTOR_OBJECT_H

#include "src/web/type/command_executor_object_interface.h"

#include <memory>

#include "src/database/database.h"
#include "src/database/type/agent_name.h"
#include "src/database/type/virtualhost_name.h"
#include "src/database/detail/general_database_functions_interface.h"
#include "src/apache/database/detail/database_functions_interface.h"

namespace apache
{

namespace web
{

class CommandExecutorObject;
typedef std::shared_ptr<CommandExecutorObject> CommandExecutorObjectPtr;

class CommandExecutorObject : public ::web::type::CommandExecutorObjectInterface {
 public:
  virtual ~CommandExecutorObject() = default;

  static CommandExecutorObjectPtr Create(::database::DatabasePtr database,
                                         ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
                                         ::apache::database::detail::DatabaseFunctionsInterfacePtr apache_database_functions);

  const ::web::type::JsonMessage Execute(const ::web::type::JsonMessage &message);

  bool IsCommandSupported(const ::web::type::Command &command);

 private:
  CommandExecutorObject(::database::DatabasePtr database,
                        ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions,
                        ::apache::database::detail::DatabaseFunctionsInterfacePtr apache_database_functions);

  ::database::DatabasePtr database_;
  ::database::detail::GeneralDatabaseFunctionsInterfacePtr general_database_functions_;
  ::apache::database::detail::DatabaseFunctionsInterfacePtr apache_database_functions_;

  const ::web::type::JsonMessage GetHostnames();
  const ::web::type::JsonMessage GetVirtualhostsNames(const std::string &agent_name);
  const ::web::type::JsonMessage GetSessions(const std::string &agent_name,
                                             const std::string &virtualhost_name,
                                             const std::string &begin_date,
                                             const std::string &end_date);
  const ::web::type::JsonMessage SetApacheSessionsAsAnomaly(const ::database::type::AgentName &agent_name,
                                                            const ::database::type::VirtualhostName &virtualhost_name,
                                                            const std::vector<long long> &all,
                                                            const std::vector<long long> &anomalies);
  const ::web::type::JsonMessage GetApacheAnomalyDetectionConfiguration();
  const ::web::type::JsonMessage SetApacheAnomalyDetectionConfiguration(const std::string &agent_name,
                                                                        const std::string &virtualhost_name,
                                                                        const std::string &begin_date,
                                                                        const std::string &end_date);
  const ::web::type::JsonMessage GetSessionsWithoutLearningSet(const std::string &agent_name,
                                                               const std::string &virtualhost_name,
                                                               const std::string &begin_date,
                                                               const std::string &end_date);
  const ::web::type::JsonMessage MarkLearningSetWithIqrMethod(const std::string &agent_name,
                                                              const std::string &virtualhost_name);
  const ::web::type::JsonMessage GetAgentsAndVirtualhostsNamesFilteredBySessionsClassificationExists();
  const ::web::type::JsonMessage GetLearningSetSessions(const std::string &agent_name,
                                                        const std::string &virtualhost_name);
};

}

}

#endif /* PROGRAM_OPTIONS_COMMAND_EXECUTOR_OBJECT_H */
