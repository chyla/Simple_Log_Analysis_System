# -*- coding: utf-8 -*-

from django.shortcuts import render_to_response, render, redirect
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

    return render_to_response('apache/configure_anomaly_detection/select_agent_name.html',
                              {'exception' : exception,
                               'agent_names' : agent_names,
                               })

def configure_anomaly_detection_select_data_range(request):
    agent_name = request.GET.get('agent_name', '')
    virtualhost_name = request.GET.get('virtualhost_name', '')
    exception = None

    return render_to_response('apache/configure_anomaly_detection/select_data_range.html',
                              {'exception' : exception,
                               'agent_name' : agent_name,
                               'virtualhost_name' : virtualhost_name,
                               })

def configure_anomaly_detection_select_virtualhost(request):
    agent_name = request.GET.get('agent_name', '')
    exception = None
    virtualhosts_names = None

    try:
        virtualhosts_names = util.get_apache_virtualhost_names(agent_name)
    except IOError as e:
        exception = e.strerror

    return render_to_response('apache/configure_anomaly_detection/select_virtualhost.html',
                              {'exception' : exception,
                               'agent_name' : agent_name,
                               'virtualhosts_names' : virtualhosts_names,
                               })

def configure_anomaly_detection_correct_sessions_marks(request):
    exception = None
    sessions = []
    agent_name = request.GET.get('agent_name', '')
    virtualhost_name = request.GET.get('virtualhost_name', '')
    begin_date = request.GET.get('begin-date', '')
    end_date = request.GET.get('end-date', '')

    try:
        sessions = util.get_apache_sessions(agent_name, virtualhost_name, begin_date, end_date)
    except IOError as e:
        exception = e.strerror

    return render(request,
                  'apache/configure_anomaly_detection/correct_sessions_marks.html',
                  {'exception' : exception,
                   'agent_name' : agent_name,
                   'virtualhost_name' : virtualhost_name,
                   'begin_date' : begin_date,
                   'end_date' : end_date,
                   'sessions' : sessions,
                   })

def configure_anomaly_detection_save_settings(request):

    return redirect('apache:status', permanent=False)
