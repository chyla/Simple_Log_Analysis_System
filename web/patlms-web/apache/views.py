# -*- coding: utf-8 -*-

from django.http import HttpResponseRedirect
from django.shortcuts import render_to_response, render, redirect
from django.core.urlresolvers import reverse
import util


def configure_actions(request):
    return render_to_response('apache/configure_actions.html')

def status(request):
    exception = None
    configuration = None
    available_classification_for_agent_host_names = None

    try:
        configuration = util.get_apache_anomaly_detection_configuration()
        available_classification_for_agent_host_names = util.get_agents_and_virtualhosts_names_filtered_by_sessions_classification_exists()
    except Exception as e:
        exception = str(e)

    return render_to_response("apache/status.html",
                              {'exception' : exception,
                               'configuration' : configuration,
                               'available_classification_for_agent_host_names' : available_classification_for_agent_host_names,
                               })

def configure_anomaly_detection_select_agent_and_virtualhost(request):
    agent_name = request.GET.get('agent_name', '')
    virtualhost_name = request.GET.get('virtualhost_name', '')
    agent_names = None
    virtualhosts_names = None
    exception = None

    try:
        agent_names = util.get_apache_agent_names()
        if agent_name != '':
            virtualhosts_names = util.get_apache_virtualhost_names(agent_name)
    except Exception as e:
        exception = str(e)

    return render_to_response('apache/configure_anomaly_detection/select_agent_and_virtualhost.html',
                              {'exception' : exception,
                               'agent_name' : agent_name,
                               'virtualhost_name' : virtualhost_name,
                               'agent_names' : agent_names,
                               'virtualhosts_names' : virtualhosts_names,
                               })

def configure_anomaly_detection_select_data_range(request):
    agent_name = request.GET.get('agent_name', '')
    virtualhost_name = request.GET.get('virtualhost_name', '')
    begin_date = request.GET.get('begin_date', util.get_default_begin_date())
    end_date = request.GET.get('end_date', util.get_default_end_date())
    sessions = []
    exception = None

    try:
        sessions = util.get_apache_sessions(agent_name, virtualhost_name, begin_date, end_date)
    except Exception as e:
        exception = str(e)

    return render(request,
                  'apache/configure_anomaly_detection/select_data_range.html',
                  {'exception' : exception,
                   'agent_name' : agent_name,
                   'virtualhost_name' : virtualhost_name,
                   'begin_date' : begin_date,
                   'end_date' : end_date,
                   'sessions' : sessions,
                   })

def configure_anomaly_detection_select_data_range_save_settings(request):
    agent_name = request.POST.get('agent_name', '')
    virtualhost_name = request.POST.get('virtualhost_name', '')
    begin_date = request.POST.get('begin_date', '')
    end_date = request.POST.get('end_date', '')
    redirect_to_correct_session_marks = request.POST.get('redirect_to_correct_session_marks', '')
    exception = None

    print redirect_to_correct_session_marks

    try:
        sessions = util.set_apache_anomaly_detection_configuration(agent_name, virtualhost_name, begin_date, end_date)
    except Exception as e:
        return render(request,
                      'apache/configure_anomaly_detection/select_data_range.html',
                      {'exception' : str(e),
                       })

    if redirect_to_correct_session_marks == 'on':
        url = reverse('apache:configure_anomaly_detection_correct_sessions_marks')
        return redirect((url + "?agent_name={}&virtualhost_name={}").format(agent_name, virtualhost_name),
                        permanent=False)
    else:
        return redirect('apache:status', permanent=False)

def configure_anomaly_detection_correct_sessions_marks(request, *args, **kwargs):
    exception = None
    sessions = []
    agent_name = request.GET.get('agent_name', '')
    virtualhost_name = request.GET.get('virtualhost_name', '')

    try:
        sessions = util.get_learning_set_sessions(agent_name, virtualhost_name)
    except Exception as e:
        exception = str(e)

    return render(request,
                  'apache/configure_anomaly_detection/correct_sessions_marks.html',
                  {'exception' : exception,
                   'agent_name' : agent_name,
                   'virtualhost_name' : virtualhost_name,
                   'sessions' : sessions,
                   })

def configure_anomaly_detection_correct_sessions_marks_automatically_mark_anomalies(request):
    exception = None
    sessions = []
    agent_name = request.POST.get('agent_name', '')
    virtualhost_name = request.POST.get('virtualhost_name', '')

    try:
        util.mark_learning_set_with_iqr_method(agent_name, virtualhost_name)
    except Exception as e:
        exception = str(e)

    url = reverse('apache:configure_anomaly_detection_correct_sessions_marks')
    return redirect((url + "?agent_name={}&virtualhost_name={}").format(agent_name, virtualhost_name),
                    permanent=False)

def configure_anomaly_detection_save_settings(request):
    exception = None
    agent_name = request.POST.get('agent_name', '')
    virtualhost_name = request.POST.get('virtualhost_name', '')
    all_rows_ids = request.POST.getlist('rows_ids')
    anomalies = request.POST.getlist('checks')

    try:
        util.set_apache_sessions_as_anomaly(agent_name,
                                            virtualhost_name,
                                            [int(i) for i in all_rows_ids],
                                            [int(i) for i in anomalies])
    except Exception as e:
        exception = str(e)

    if exception:
        return redirect('apache:configure_anomaly_detection_correct_sessions_marks',
                        permanent=False,
                        args=(agent_name, virtualhost_name, exception))
    else:
        return redirect('apache:status', permanent=False)

def review_detection_results_show_results(request):
    agent_name = request.GET.get('agent_name', '')
    virtualhost_name = request.GET.get('virtualhost_name', '')
    begin_date = request.GET.get('begin_date', util.get_default_begin_date())
    end_date = request.GET.get('end_date', util.get_default_end_date())
    sessions = []
    exception = None

    try:
        if begin_date != '' and end_date != '':
            sessions = util.get_apache_sessions_without_learning_set(agent_name, virtualhost_name, begin_date, end_date)
    except Exception as e:
        exception = str(e)

    return render(request,
                  'apache/review_detection_results/show_results.html',
                  {'exception' : exception,
                   'agent_name' : agent_name,
                   'virtualhost_name' : virtualhost_name,
                   'begin_date' : begin_date,
                   'end_date' : end_date,
                   'sessions' : sessions,
                   })
