# -*- coding: utf-8 -*-

from django.views.generic.base import RedirectView
from django.conf.urls import url
from . import views

urlpatterns = [
    url(r'^$', views.status, name='status'),
    url(r'^configure_actions', views.configure_actions, name='configure_actions'),

    url(r'^configure_anomaly_detection$', RedirectView.as_view(url='configure_anomaly_detection_select_agent_name', permanent=True), name='configure_anomaly_detection'),
    url(r'^configure_anomaly_detection_select_agent_name$', views.configure_anomaly_detection_select_agent_name, name='configure_anomaly_detection_select_agent_name'),
    url(r'^configure_anomaly_detection_select_virtualhost$', views.configure_anomaly_detection_select_virtualhost, name='configure_anomaly_detection_select_virtualhost'),
    url(r'^configure_anomaly_detection_select_data_range$', views.configure_anomaly_detection_select_data_range, name='configure_anomaly_detection_select_data_range'),
    url(r'^configure_anomaly_detection_correct_sessions_marks$', views.configure_anomaly_detection_correct_sessions_marks, name='configure_anomaly_detection_correct_sessions_marks'),
    url(r'^configure_anomaly_detection_save_settings$', views.configure_anomaly_detection_save_settings, name='configure_anomaly_detection_save_settings'),

    url(r'^review_detection_results$', RedirectView.as_view(url='review_detection_results/select_agent_and_virtualhost', permanent=True), name='review_detection_results'),
    url(r'^review_detection_results/select_agent_and_virtualhost', views.review_detection_results_select_agent_and_virtualhost, name='review_detection_results_select_agent_and_virtualhost'),
    url(r'^review_detection_results/show_results', views.review_detection_results_show_results, name='configure_anomaly_detection_show_results'),
]
