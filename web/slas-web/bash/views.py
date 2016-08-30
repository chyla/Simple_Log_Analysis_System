# -*- coding: utf-8 -*-

from django.http import HttpResponseRedirect
from django.shortcuts import render_to_response, render, redirect
from django.contrib.auth.decorators import login_required
from django.core.urlresolvers import reverse
from util.type import Classification
import util

@login_required
def status(request):
    exception = None
    configurations = None
    bash_get_agents_with_existing_sessions_not_in_learning_set = None

    try:
        configurations = util.bash_get_anomaly_detection_configurations()
        bash_get_agents_with_existing_sessions_not_in_learning_set = util.bash_get_agents_with_existing_sessions_not_in_learning_set()
    except Exception as e:
        exception = str(e)

    return render(request,
                  'bash/status.html',
                  {'exception' : exception,
                   'configurations' : configurations,
                   'bash_get_agents_with_existing_sessions_not_in_learning_set' : bash_get_agents_with_existing_sessions_not_in_learning_set,
                   })

@login_required
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

@login_required
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

@login_required
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

@login_required
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

@login_required
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

@login_required
def configure_anomaly_detection_select_commands_save_selected(request):
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

        return render(request,
                      'bash/status.html',
                      {'exception' : exception,
                       });

    return redirect('bash:status', permanent=False)

@login_required
def configure_anomaly_detection_correct_sessions_marks(request):
    exception = None
    configuration_id = request.GET.get('configuration_id', None)
    sessions = None

    try:
        sessions = util.bash_get_daily_user_statistics_from_configuration(configuration_id)
    except Exception as e:
        exception = str(e)

    return render(request,
                  'bash/configure_anomaly_detection/correct_sessions_marks.html',
                  {'exception' : exception,
                   'sessions' : sessions,
                   'configuration_id' : configuration_id,
                   'CLASSIFICATION_ANOMALY' : Classification.ANOMALY,
                   'CLASSIFICATION_NORMAL' : Classification.NORMAL,
                   'CLASSIFICATION_UNKNOWN' : Classification.UNKNOWN,
                   })

@login_required
def configure_anomaly_detection_correct_sessions_marks_save(request):
    exception = None
    configuration_id = request.POST.get('configuration_id', None)
    all_rows_ids = request.POST.getlist('rows_ids')
    anomalies_ids = request.POST.getlist('checks')

    try:
        util.bash_set_sessions_as_anomalies(configuration_id,
                                            [int(i) for i in all_rows_ids],
                                            [int(i) for i in anomalies_ids])
    except Exception as e:
        exception = str(e)

    if exception:
        return render(request,
                      'bash/status.html',
                      {'exception' : exception,
                       });

    return redirect('bash:status', permanent=False)
@login_required
def review_detection_results_show_results(request):
    exception = None
    sessions = None
    agent_name_id = request.GET.get('agent_name_id', None)
    begin_date = request.GET.get('begin_date', util.get_default_begin_date())
    end_date = request.GET.get('end_date', util.get_default_end_date())

    try:
        sessions = util.bash_get_daily_user_statistics_without_learning_set_for_agent(agent_name_id, begin_date, end_date)
    except Exception as e:
        exception = str(e)

    return render(request,
                  'bash/review_detection_results/show_results.html',
                  {'exception' : exception,
                   'sessions' : sessions,
                   'agent_name_id' : agent_name_id,
                   'begin_date' : begin_date,
                   'end_date' : end_date,
                   'CLASSIFICATION_ANOMALY' : Classification.ANOMALY,
                   'CLASSIFICATION_NORMAL' : Classification.NORMAL,
                   'CLASSIFICATION_UNKNOWN' : Classification.UNKNOWN,
                   })

@login_required
def show_daily_user_statistic(request):
    exception = None
    statistic = None
    commands_statistics = None
    statistic_id = request.GET.get('statistic_id', None)

    try:
        statistic = util.bash_get_daily_user_statistic_by_id(statistic_id)
        commands_statistics = util.bash_get_daily_user_named_commands_statistics(statistic_id)
    except Exception as e:
        exception = str(e)

    return render(request,
                  'bash/review_detection_results/show_daily_user_statistic.html',
                  {'exception' : exception,
                   'statistic' : statistic,
                   'commands_statistics' : commands_statistics,
                   'CLASSIFICATION_ANOMALY' : Classification.ANOMALY,
                   'CLASSIFICATION_NORMAL' : Classification.NORMAL,
                   'CLASSIFICATION_UNKNOWN' : Classification.UNKNOWN,
                   })
