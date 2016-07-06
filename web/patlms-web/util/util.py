import json
import copy
import datetime

from network import Network
from server_config import SERVER_ADDRESS, SERVER_PORT
from exception import CommandException

def get_default_begin_date():
    past = datetime.datetime.now() - datetime.timedelta(30)
    return past.strftime("%Y-%m-%d")

def get_default_end_date():
    past = datetime.datetime.now()
    return past.strftime("%Y-%m-%d")

def connect_and_get_result_from_command(command, args=None):
    n = Network.connect_ipv4(SERVER_ADDRESS, SERVER_PORT)

    json_object = {}
    json_object["command"] = command
    if args:
        json_object["args"] = args

    json_string = json.dumps(json_object)

    Network.send_text(n, json_string)

    response = Network.receive_text(n)

    json_response = json.loads(response)
    if json_response['status'] == "ok":
        if 'result' in json_response:
            return json_response['result']
    else:
        raise CommandException(json_response['message'])

def get_process_id_from_server():
    return connect_and_get_result_from_command('get_server_process_pid')

def get_process_pidfile_path_from_server():
    return connect_and_get_result_from_command('get_server_process_pidfile')

def get_server_version():
    return connect_and_get_result_from_command('get_server_version')

def get_apache_agent_names():
    return connect_and_get_result_from_command('get_apache_agent_names')

def get_apache_virtualhost_names(agent_name):
    return connect_and_get_result_from_command('get_apache_virtualhosts_names', [agent_name, ])

def get_apache_sessions(agent_name, virtualhost_name, begin_date, end_date):
    return connect_and_get_result_from_command('get_apache_sessions', [agent_name, virtualhost_name, begin_date, end_date])

def set_apache_sessions_as_anomaly(agent_name, virtualhost_name, all_rows_ids, anomalies_ids):
    return connect_and_get_result_from_command('set_apache_sessions_as_anomaly', [agent_name, virtualhost_name, all_rows_ids, anomalies_ids])

def get_apache_anomaly_detection_configuration():
    return connect_and_get_result_from_command('get_apache_anomaly_detection_configuration')

def set_apache_anomaly_detection_configuration(agent_name, virtualhost_name, begin_date, end_date):
    return connect_and_get_result_from_command('set_apache_anomaly_detection_configuration', [agent_name, virtualhost_name, begin_date, end_date])

def get_apache_sessions_without_learning_set(agent_name, virtualhost_name, begin_date, end_date):
    return connect_and_get_result_from_command('get_apache_sessions_without_learning_set', [agent_name, virtualhost_name, begin_date, end_date])

def mark_learning_set_with_iqr_method(agent_name, virtualhost_name):
    return connect_and_get_result_from_command('mark_learning_set_with_iqr_method', [agent_name, virtualhost_name])

def get_agents_and_virtualhosts_names_filtered_by_sessions_classification_exists():
    return connect_and_get_result_from_command('get_agents_and_virtualhosts_names_filtered_by_sessions_classification_exists')

def get_learning_set_sessions(agent_name, virtualhost_name):
    return connect_and_get_result_from_command('get_learning_set_sessions', [agent_name, virtualhost_name])

def remove_configuration(configuration_id):
    return connect_and_get_result_from_command('remove_configuration', [configuration_id])

def bash_get_unconfigured_agents():
    return connect_and_get_result_from_command('bash.get_unconfigured_agents')

def bash_get_anomaly_detection_configurations():
    return connect_and_get_result_from_command('bash.get_anomaly_detection_configurations')

def bash_remove_anomaly_detection_configurations(configuration_id):
    return connect_and_get_result_from_command('bash.remove_anomaly_detection_configurations', [int(configuration_id)])

def bash_get_commands_statistics(agent_name_id, begin_date, end_date):
    return connect_and_get_result_from_command('bash.get_commands_statistics', [int(agent_name_id), begin_date, end_date])

def bash_save_anomaly_detection_configuration(agent_name_id, begin_date, end_date):
    return connect_and_get_result_from_command('bash.save_anomaly_detection_configuration', [int(agent_name_id), begin_date, end_date])
