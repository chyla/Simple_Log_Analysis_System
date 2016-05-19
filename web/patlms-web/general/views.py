# -*- coding: utf-8 -*-

from django.shortcuts import render_to_response
import util

def status(request):
    exception = None
    process_id = None
    process_pidfile = None

    try:
        process_id = util.get_process_id_from_server()
        process_pidfile = util.get_process_pidfile_path_from_server()
    except IOError as e:
        exception = e.strerror

    return render_to_response("general/status.html", {'exception' : exception,
                                                      'process_id' : process_id,
                                                      'process_pidfile' : process_pidfile})
