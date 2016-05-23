#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import os
sys.path.insert(1, os.path.join(sys.path[0], '..'))
sys.path.insert(1, os.path.join(sys.path[0], '../..'))

from mock import patch, MagicMock, call
import pytest

import json
import util
from util.exception import CommandException


class TestUtil(object):

    SOCKET_TEST = 5

    @patch('network.Network.connect_ipv4')
    @patch('network.Network.send_text')
    @patch('network.Network.receive_text')
    def test_connect_and_get_result_from_command_with_no_args(self, receive_text, send_text, connect_ipv4):
        connect_ipv4.return_value = TestUtil.SOCKET_TEST
        receive_text.return_value = ' { "status" : "ok", "result" : "my_test_answer" } '

        result = util.connect_and_get_result_from_command("my_test_command")

        assert connect_ipv4.call_count == 1
        json_object = {}
        json_object["command"] = "my_test_command"
        json_object["args"] = []
        send_text.assert_called_once_with(TestUtil.SOCKET_TEST, json.dumps(json_object))
        receive_text.assert_called_once_with(TestUtil.SOCKET_TEST)
        assert result == "my_test_answer"

    @patch('network.Network.connect_ipv4')
    @patch('network.Network.send_text')
    @patch('network.Network.receive_text')
    def test_connect_and_get_result_from_command_with_one_arg(self, receive_text, send_text, connect_ipv4):
        connect_ipv4.return_value = TestUtil.SOCKET_TEST
        receive_text.return_value = ' { "status" : "ok", "result" : "my_test_answer" } '

        result = util.connect_and_get_result_from_command("my_test_command", ["arg1_example"])

        assert connect_ipv4.call_count == 1
        json_object = {}
        json_object["command"] = "my_test_command"
        json_object["args"] = ["arg1_example", ]
        send_text.assert_called_once_with(TestUtil.SOCKET_TEST, json.dumps(json_object))
        receive_text.assert_called_once_with(TestUtil.SOCKET_TEST)
        assert result == "my_test_answer"

    @patch('network.Network.connect_ipv4')
    @patch('network.Network.send_text')
    @patch('network.Network.receive_text')
    def test_connect_and_get_result_from_command_with_two_args(self, receive_text, send_text, connect_ipv4):
        connect_ipv4.return_value = TestUtil.SOCKET_TEST
        receive_text.return_value = ' { "status" : "ok", "result" : "my_test_answer" } '

        result = util.connect_and_get_result_from_command("my_test_command", ["arg1_example", "arg2_example"])

        assert connect_ipv4.call_count == 1
        json_object = {}
        json_object["command"] = "my_test_command"
        json_object["args"] = ["arg1_example", "arg2_example"]
        send_text.assert_called_once_with(TestUtil.SOCKET_TEST, json.dumps(json_object))
        receive_text.assert_called_once_with(TestUtil.SOCKET_TEST)
        assert result == "my_test_answer"

    @patch('network.Network.connect_ipv4')
    @patch('network.Network.send_text')
    @patch('network.Network.receive_text')
    def test_connect_and_get_result_from_command_when_error_returned(self, receive_text, send_text, connect_ipv4):
        connect_ipv4.return_value = TestUtil.SOCKET_TEST
        receive_text.return_value = ' { "status" : "error", "message" : "My error." } '

        with pytest.raises(CommandException) as ex_info:
            result = util.connect_and_get_result_from_command("my_test_command")

        assert connect_ipv4.call_count == 1
        json_object = {}
        json_object["command"] = "my_test_command"
        json_object["args"] = []
        send_text.assert_called_once_with(TestUtil.SOCKET_TEST, json.dumps(json_object))
        receive_text.assert_called_once_with(TestUtil.SOCKET_TEST)
        assert ex_info.value.message == "My error."
