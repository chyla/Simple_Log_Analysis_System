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
    redirect_to_correct_session_marks = request.POST.get('redirect_to_correct_session_marks', '')
    configuration_id = None

    try:
        util.bash_save_anomaly_detection_configuration(agent_name_id, begin_date, end_date)
        configuration_id = util.bash_get_configuration_id_for_agent(agent_name_id)
    except Exception as e:
        exception = str(e)

    if exception:
        return render(request,
                      'bash/status.html',
                      {'exception' : exception,
                       });

    if redirect_to_correct_session_marks == 'on':
        url = reverse('bash:configure_anomaly_detection_select_commands')
        return redirect((url + "?configuration_id={}").format(configuration_id),
                        permanent=False)
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

def configure_anomaly_detection_select_commands(request):
    exception = None
    configuration_id = request.GET.get('configuration_id', None)
    all_commands_statistics = None
    selected_commands = None

    try:
        all_commands_statistics = util.bash_get_commands_statistics_by_configuration_id(configuration_id)
        selected_commands = util.bash_get_selected_commands_ids(configuration_id)
    except Exception as e:
        exception = str(e)

    return render(request,
                  'bash/configure_anomaly_detection/select_commands.html',
                  {'exception' : exception,
                   'configuration_id' : configuration_id,
                   'all_commands_statistics' : all_commands_statistics,
                   'selected_commands' : selected_commands,
                   })

def configure_anomaly_detection_select_commands_save_selected(request):
    exception = None
    configuration_id = request.POST.get('configuration_id', None)
    select_default_commands = request.POST.get('select_default_commands', 'false')

    try:
        if select_default_commands == 'true':
            util.bash_select_default_commands(configuration_id)
        else:
            checks = request.POST.getlist('checks')
            selected_commands_ids = [int(i) for i in checks]
            util.bash_save_selected_commands(configuration_id, selected_commands_ids)
    except Exception as e:
        exception = str(e)

    return redirect('bash:status', permanent=False)
