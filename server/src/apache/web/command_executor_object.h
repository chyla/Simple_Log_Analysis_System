#ifndef SRC_APACHE_WEB_COMMAND_EXECUTOR_OBJECT_H
#define SRC_APACHE_WEB_COMMAND_EXECUTOR_OBJECT_H

#include "src/database/database.h"
#include "src/web/type/command_executor_object_interface.h"

#include <memory>

namespace apache
{

namespace web
{

class CommandExecutorObject;
typedef std::shared_ptr<CommandExecutorObject> CommandExecutorObjectPtr;

class CommandExecutorObject : public ::web::type::CommandExecutorObjectInterface {
 public:
  virtual ~CommandExecutorObject() = default;

  static CommandExecutorObjectPtr Create(::database::DatabasePtr database);

  const ::web::type::JsonMessage Execute(const ::web::type::JsonMessage &message);

  bool IsCommandSupported(const ::web::type::Command &command);

 private:
  CommandExecutorObject(::database::DatabasePtr database);

  ::database::DatabasePtr database_;

  const ::web::type::JsonMessage GetHostnames();
  const ::web::type::JsonMessage GetVirtualhostsNames(const std::string &agent_name);
  const ::web::type::JsonMessage GetSessions(const std::string &agent_name,
                                             const std::string &virtualhost_name,
                                             const std::string &begin_date,
                                             const std::string &end_date);
  const ::web::type::JsonMessage SetApacheSessionsAsAnomaly(const std::vector<long long> &all,
                                                            const std::vector<long long> &anomalies);
  const ::web::type::JsonMessage GetApacheAnomalyDetectionConfiguration();
  const ::web::type::JsonMessage SetApacheAnomalyDetectionConfiguration(const std::string &agent_name,
                                                                        const std::string &virtualhost_name,
                                                                        const std::string &begin_date,
                                                                        const std::string &end_date);
};

}

}

#endif /* PROGRAM_OPTIONS_COMMAND_EXECUTOR_OBJECT_H */
