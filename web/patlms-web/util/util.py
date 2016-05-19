import json
import copy

from network import Network
from server_config import SERVER_ADDRESS, SERVER_PORT

def connect_and_get_result_from_command(command):
    n = Network.connect_ipv4(SERVER_ADDRESS, SERVER_PORT)
    Network.send_text(n, ' { "command" : "' + command + '" } ')

    response = Network.receive_text(n)

    json_response = json.loads(response)

    return json_response['result']

def get_process_id_from_server():
    return connect_and_get_result_from_command('get_server_process_pid')

def get_process_pidfile_path_from_server():
    return connect_and_get_result_from_command('get_server_process_pidfile')
