#!/usr/bin/env python
# -*- coding: utf-8 -*-

import socket
import logging
import re
import struct
import select


class Network(object):

    TIMEOUT_SECONDS = 3
    MAX_MESSAGE_LENGTH = 255

    class TimeoutException(IOError):
        def __init__(self):
            super(Network.TimeoutException, self).__init__("Timeout")

    class WaitStatus(object):
        TIMEOUT = 0
        NEW_DATA = 1

    @staticmethod
    def connect_ipv4(address, port):
        logging.debug("network::Network::connect_ipv4: Method call")

        if re.match(r'^\d{1,4}\.\d{1,4}\.\d{1,4}\.\d{1,4}$', address) == None:
            raise ValueError

        logging.debug("network::Network::connect_ipv4: socket call")
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

        logging.debug("network::Network::connect_ipv4: connect call")
        s.connect((address, port))

        return s

    @staticmethod
    def close(socket):
        logging.debug("network::Network::close: Method call")
        socket.close()

    @staticmethod
    def send_text(socket, text):
        logging.debug("network::Network::send_text: Function call")

        to_copy = 0
        for i in range(0, len(text), Network.MAX_MESSAGE_LENGTH - 1):
            if i + Network.MAX_MESSAGE_LENGTH <= len(text):
                to_copy = Network.MAX_MESSAGE_LENGTH - 1
                msg_type = 'M'
            else:
                to_copy = len(text) - i
                msg_type = 'L'

            message = msg_type + text[i:(i + to_copy)]

            logging.debug("network::Network::send_text: Sending message: " + message)
            Network.send_message(socket, message)

    @staticmethod
    def receive_text(socket):
        logging.debug("network::Network::receive_text: Function call")
        text = ''

        while True:
            message = Network.recv_message(socket)
            text += message[1:]

            if message[0] == 'L':
                break;

        logging.debug("network::Network::receive_text: Received: " + text)
        return text

    @staticmethod
    def recv_message(socket):
        logging.debug("network::Network::recv_message: Function call")

        wait_status = Network.wait_for_data(socket, Network.TIMEOUT_SECONDS)
        if wait_status != Network.WaitStatus.NEW_DATA:
            logging.error("network::Network::recv_message: Timeout")
            raise Network.TimeoutException

        logging.debug("network::Network::recv_message: Receiving first part")
        received = socket.recv(1)
        if len(received) != 1:
            logging.error("network::Network::recv_message: Received " + str(len(received)) + " bytes, expected 1")
            raise IOError("Received " + str(len(received)) + " bytes, expected 1")

        message_length = struct.unpack('B', received)[0]

        logging.debug("network::Network::recv_message: Receiving message")
        message = ''
        while len(message) < message_length:
            left_to_receive = message_length - len(message)

            wait_status = Network.wait_for_data(socket, Network.TIMEOUT_SECONDS)
            if wait_status != Network.WaitStatus.NEW_DATA:
                logging.debug("network::Network::recv_message: Timeout")
                raise Network.TimeoutException

            message += socket.recv(left_to_receive)

        logging.debug("network::Network::recv_message: Received")
        return message

    @staticmethod
    def send_message(socket, message):
        logging.debug("network::Network::send_message: Function call with (message=" + message + ')')

        if len(message) > Network.MAX_MESSAGE_LENGTH:
            logging.debug("network::Network::send_message: Message too long (" + str(len(message)) + ")")
            raise ValueError

        logging.debug("network::Network::send_message: Sending first part")
        message_length = len(message)
        sent = 0;
        while True:
            sent = socket.send(struct.pack("B", message_length))
            if sent == 1:
                break

        logging.debug("network::Network::send_message: Sending message")
        sent = 0
        left = message_length - sent
        while left > 0:
            sent += socket.send(message[sent:])
            left = message_length - sent

        logging.debug("network::Network::send_message: Done")

    @staticmethod
    def wait_for_data(socket, seconds):
        logging.debug("network::Network::wait_for_data: Function call with (seconds=" + str(seconds) + ')')
        status = select.select([socket], [], [], seconds)
        if status[0]:
            logging.debug("network::Network::wait_for_data: New data available")
            return Network.WaitStatus.NEW_DATA
        else:
            logging.debug("network::Network::wait_for_data: Timeout occurred")
            return Network.WaitStatus.TIMEOUT
