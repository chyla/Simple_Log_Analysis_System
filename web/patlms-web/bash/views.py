# -*- coding: utf-8 -*-

from django.http import HttpResponseRedirect
from django.shortcuts import render_to_response, render, redirect
from django.core.urlresolvers import reverse
import util


def status(request):
    exception = None
    configurations = None

    try:
        configurations = util.bash_get_anomaly_detection_configurations()
    except Exception as e:
        exception = str(e)

    return render(request,
                  'bash/status.html',
                  {'exception' : exception,
                   'configurations' : configurations,
                   })

def configure_anomaly_detection_select_agent(request):
    exception = None
    unconfigured_agents = None
    configurations = None
    selected_agent_id = request.GET.get('agent_name_id', '')

    try:
        unconfigured_agents = util.bash_get_unconfigured_agents();
        configurations = util.bash_get_anomaly_detection_configurations()
    except Exception as e:
        exception = str(e)

    return render(request,
                  'bash/configure_anomaly_detection/select_agent.html',
                  {'exception' : exception,
                   'agent_name_id' : selected_agent_id,
                   'configurations' : configurations,
                   'unconfigured_agents' : unconfigured_agents,
                   })

def configure_anomaly_detection_remove_configuration(request):
    exception = None
    configuration_id = request.POST.get('configuration_id', '')

    try:
        util.bash_remove_anomaly_detection_configurations(configuration_id)
    except Exception as e:
        exception = str(e)

    if exception:
        return render(request,
                      'bash/status.html',
                      {'exception' : exception,
                       });
    else:
        return redirect('bash:status', permanent=False)

def configure_anomaly_detection_save_configuration(request):
    exception = None
    agent_name_id = request.POST.get('agent_name_id', '')
    begin_date = request.POST.get('begin_date', '')
    end_date = request.POST.get('end_date', '')

    try:
        util.bash_save_anomaly_detection_configuration(agent_name_id, begin_date, end_date)
    except Exception as e:
        exception = str(e)

    if exception:
        return render(request,
                      'bash/status.html',
                      {'exception' : exception,
                       });
    else:
        return redirect('bash:status', permanent=False)

def configure_anomaly_detection_select_date_range(request):
    exception = None
    configurations = None
    selected_agent_id = request.GET.get('agent_name_id', '')
    begin_date = request.GET.get('begin_date', util.get_default_begin_date())
    end_date = request.GET.get('end_date', util.get_default_end_date())
    commands_statistics = None

    try:
        configurations = util.bash_get_anomaly_detection_configurations()
        commands_statistics = util.bash_get_commands_statistics(selected_agent_id, begin_date, end_date)
        print commands_statistics
    except Exception as e:
        exception = str(e)

    return render(request,
                  'bash/configure_anomaly_detection/select_date_range.html',
                  {'exception' : exception,
                   'agent_name_id' : selected_agent_id,
                   'configurations' : configurations,
                   'begin_date' : begin_date,
                   'end_date' : end_date,
                   'commands_statistics' : commands_statistics,
                   })
