#!/usr/bin/env python
# -*- coding: utf-8 -*-


class CommandException(Exception):

    def __init__(self, message):
        super(CommandException, self).__init__(message)
