#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import os
sys.path.insert(1, os.path.join(sys.path[0], '..'))

from network import Network
import socket
import struct
from mock import patch, MagicMock, call
import pytest

class TestNetwork(object):

    def mock_socket(self):
        s = MagicMock()

        s.connect = MagicMock()
        s.send = MagicMock()
        s.recv = MagicMock()
        s.close = MagicMock()

        return s

    def setup_method(self, method):
        self.socket_mock = self.mock_socket()

    @patch('socket.socket')
    def test_connect_ipv4_correct_call(self, socket_function_mock):
        socket_function_mock.return_value = self.socket_mock

        sock = Network.connect_ipv4("127.0.0.1", 80)

        socket_function_mock.assert_called_once_with(socket.AF_INET, socket.SOCK_STREAM)
        assert socket_function_mock.return_value == sock
        sock.connect.assert_called_once_with(("127.0.0.1", 80))

    @patch('socket.socket')
    def test_connect_ipv4_when_ip_is_wrong(self, socket_function_mock):
        socket_function_mock.return_value = self.socket_mock

        with pytest.raises(ValueError):
            sock = Network.connect_ipv4("127.0.01", 80)

        assert socket_function_mock.called == False

    @patch('socket.socket')
    def test_connect_ipv4_when_ip_is_wrong_and_contains_wrong_characters(self, socket_function_mock):
        socket_function_mock.return_value = self.socket_mock

        with pytest.raises(ValueError):
            sock = Network.connect_ipv4("127.0.0.1a", 80)

        assert socket_function_mock.called == False

    @patch('network.Network.send_message')
    def test_send_text_correct_call(self, send_message_function_mock):
        message = 'a'
        for i in range(253):
            message += 'm'

        Network.send_text(self.socket_mock, message)

        send_message_function_mock.assert_has_calls([call(self.socket_mock, 'L' + message),
                                                     ])

    @patch('network.Network.send_message')
    def test_send_text_correct_call_long_message(self, send_message_function_mock):
        text = 'a'
        for i in range(253):
            text += 'b'
        text += 'c'

        Network.send_text(self.socket_mock, text)

        send_message_function_mock.assert_has_calls([call(self.socket_mock, 'M' + text[:-1]),
                                                     call(self.socket_mock, 'Lc'),
                                                     ])

    @patch('network.Network.recv_message')
    def test_send_text_correct_call(self, recv_message_function_mock):
        message = 'La'
        for i in range(253):
            message += 'b'
        recv_message_function_mock.return_value = message

        text = Network.receive_text(self.socket_mock)

        recv_message_function_mock.assert_called_once_with(self.socket_mock)
        assert text == message[1:]
        assert len(text) == 254

    @patch('network.Network.recv_message')
    def test_send_text_correct_call_long_message(self, recv_message_function_mock):
        message = 'Ma'
        for i in range(253):
            message += 'b'
        message2 = 'Lc'
        recv_message_function_mock.side_effect = [message, message2]

        text = Network.receive_text(self.socket_mock)

        assert recv_message_function_mock.call_count == 2
        assert text == message[1:] + message2[1:]
        assert len(text) == 255

    @patch('select.select')
    def test_recv_message_correct_call(self, select_function_mock):
        message = 'test message'
        select_function_mock.side_effect = [[True], [True], [True], [True]]
        self.socket_mock.recv.side_effect = [struct.pack('B', len(message)),
                                             'test',
                                             ' ',
                                             'message']

        received_message = Network.recv_message(self.socket_mock)

        self.socket_mock.recv.assert_has_calls([call(1),
                                                call(12),
                                                call(8),
                                                call(7),
                                                ])
        assert message == received_message

    @patch('select.select')
    def test_recv_message_when_first_select_fail(self, select_function_mock):
        message = 'test message'
        select_function_mock.side_effect = [[False]]

        with pytest.raises(Network.TimeoutException):
            received_message = Network.recv_message(self.socket_mock)

    @patch('select.select')
    def test_recv_message_when_first_recv_fail(self, select_function_mock):
        message = 'test message'
        select_function_mock.side_effect = [[True]]
        self.socket_mock.recv.side_effect = ['']

        with pytest.raises(IOError):
            received_message = Network.recv_message(self.socket_mock)

        self.socket_mock.recv.assert_has_calls([call(1),
                                                ])

    @patch('select.select')
    def test_recv_message_when_select_in_loop_fail(self, select_function_mock):
        message = 'test message'
        select_function_mock.side_effect = [[True], [False]]
        self.socket_mock.recv.side_effect = [struct.pack('B', len(message)),
                                             ]

        with pytest.raises(Network.TimeoutException):
            received_message = Network.recv_message(self.socket_mock)

        self.socket_mock.recv.assert_has_calls([call(1),
                                                ])

    @patch('select.select')
    def test_recv_message_when_second_select_in_loop_fail(self, select_function_mock):
        message = 'test message'
        select_function_mock.side_effect = [[True], [True], [False]]
        self.socket_mock.recv.side_effect = [struct.pack('B', len(message)),
                                             'test',
                                             ]

        with pytest.raises(Network.TimeoutException):
            received_message = Network.recv_message(self.socket_mock)

        self.socket_mock.recv.assert_has_calls([call(1),
                                                call(12),
                                                ])

    @patch('select.select')
    def test_wait_for_data_when_new_data_is_available(self, select_function_mock):
        select_function_mock.return_value = [True]
        wait_time = 3

        status = Network.wait_for_data(self.socket_mock, wait_time)

        select_function_mock.assert_called_once_with([self.socket_mock], [], [], wait_time)
        assert status == Network.WaitStatus.NEW_DATA

    @patch('select.select')
    def test_wait_for_data_when_timeout_occurred(self, select_function_mock):
        select_function_mock.return_value = [False]
        wait_time = 3

        status = Network.wait_for_data(self.socket_mock, wait_time)

        select_function_mock.assert_called_once_with([self.socket_mock], [], [], wait_time)
        assert status == Network.WaitStatus.TIMEOUT

    def test_send_message_correct_call(self):
        message_content = ""
        for i in range(255):
            message_content += 'm'
        assert len(message_content) == 255

        self.socket_mock.send.side_effect = [0, 1, 0, 100, 0, 155]

        Network.send_message(self.socket_mock, message_content)

        self.socket_mock.send.assert_has_calls([call(struct.pack("B", 255)),
                                                call(struct.pack("B", 255)),
                                                call(message_content),
                                                call(message_content),
                                                call(message_content[100:]),
                                                call(message_content[100:]),
                                                ])

    def test_send_message_correct_call_when_message_is_too_long(self):
        message_content = ""
        for i in range(256):
            message_content += 'm'
        assert len(message_content) == 256

        with pytest.raises(ValueError):
            Network.send_message(self.socket_mock, message_content)

    def test_close(self):
        Network.close(self.socket_mock)

        assert self.socket_mock.close.called == True


