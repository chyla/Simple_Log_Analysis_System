# -*- coding: utf-8 -*-

from django.shortcuts import render_to_response
import util

def status(request):
    exception = None
    process_id = None
    process_pidfile = None
    server_version = None

    try:
        process_id = util.get_process_id_from_server()
        process_pidfile = util.get_process_pidfile_path_from_server()
        server_version = util.get_server_version()
    except Exception as e:
        exception = str(e)

    return render_to_response("general/status.html", {'exception' : exception,
                                                      'process_id' : process_id,
                                                      'process_pidfile' : process_pidfile,
                                                      'server_version' : server_version,
                                                      })
