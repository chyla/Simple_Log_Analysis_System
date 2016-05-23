import json
import copy

from network import Network
from server_config import SERVER_ADDRESS, SERVER_PORT
from exception import CommandException


def connect_and_get_result_from_command(command, args=[]):
    n = Network.connect_ipv4(SERVER_ADDRESS, SERVER_PORT)

    json_object = {}
    json_object["command"] = command
    json_object["args"] = args

    json_string = json.dumps(json_object)

    Network.send_text(n, json_string)

    response = Network.receive_text(n)

    json_response = json.loads(response)
    if json_response['status'] == "ok":
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
