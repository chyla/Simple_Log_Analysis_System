# -*- coding: utf-8 -*-

from django.views.generic.base import RedirectView
from django.conf.urls import url
from . import views

urlpatterns = [
    url(r'^$', views.status, name='status'),

    url(r'^configure_anomaly_detection$', RedirectView.as_view(url='configure_anomaly_detection/select_agent', permanent=True), name='configure_anomaly_detection'),
    url(r'^configure_anomaly_detection/select_agent$', views.configure_anomaly_detection_select_agent, name='configure_anomaly_detection_select_agent'),
    url(r'^configure_anomaly_detection/remove_configuration', views.configure_anomaly_detection_remove_configuration, name='configure_anomaly_detection_remove_configuration'),
    url(r'^configure_anomaly_detection/save_configuration', views.configure_anomaly_detection_save_configuration, name='configure_anomaly_detection_save_configuration'),
    url(r'^configure_anomaly_detection/select_date_range', views.configure_anomaly_detection_select_date_range, name='configure_anomaly_detection_select_date_range'),
    url(r'^configure_anomaly_detection/select_commands$', views.configure_anomaly_detection_select_commands, name='configure_anomaly_detection_select_commands'),
    url(r'^configure_anomaly_detection/select_commands_save_selected', views.configure_anomaly_detection_select_commands_save_selected, name='configure_anomaly_detection_select_commands_save_selected'),
    url(r'^configure_anomaly_detection/correct_sessions_marks$', views.configure_anomaly_detection_correct_sessions_marks, name='configure_anomaly_detection_correct_sessions_marks'),
    url(r'^configure_anomaly_detection/correct_sessions_marks_save', views.configure_anomaly_detection_correct_sessions_marks_save, name='configure_anomaly_detection_correct_sessions_marks_save'),

]
