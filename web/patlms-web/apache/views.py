# -*- coding: utf-8 -*-

from django.shortcuts import render_to_response
import util


def configure_actions(request):
    return render_to_response('apache/configure_actions.html')

def status(request):
    return render_to_response("apache/status.html")

def configure_anomaly_detection_select_agent_name(request):
    exception = None
    agent_names = None

    try:
        agent_names = util.get_apache_agent_names()
    except IOError as e:
        exception = e.strerror

    return render_to_response('apache/configure_anomaly_detection/select_agent_name.html', {'exception' : exception,
                                                                                            'agent_names' : agent_names,
                                                                                            })

def configure_anomaly_detection_select_data_range(request):
    return render_to_response('apache/configure_anomaly_detection/select_data_range.html')

def configure_anomaly_detection_select_virtualhost(request):
    agent_name = request.GET.get('agent_name', '')
    exception = None
    virtualhosts_names = None

    try:
        virtualhosts_names = util.get_apache_virtualhost_names(agent_name)
    except IOError as e:
        exception = e.strerror

    return render_to_response('apache/configure_anomaly_detection/select_virtualhost.html', {'exception' : exception,
                                                                                             'virtualhosts_names' : virtualhosts_names,
                                                                                             'agent_name' : agent_name,
                                                                                             })

def configure_anomaly_detection_correct_sessions_marks(request):
    return render_to_response('apache/configure_anomaly_detection/correct_sessions_marks.html')
