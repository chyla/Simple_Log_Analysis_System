import json
import copy

from network import Network
from server_config import SERVER_ADDRESS, SERVER_PORT
from exception import CommandException


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
