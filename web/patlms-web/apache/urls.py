# -*- coding: utf-8 -*-

from django.conf.urls import url
from . import views

urlpatterns = [
    url(r'^$', views.status, name='status'),
    url(r'^configure_actions', views.configure_actions, name='configure_actions'),
    url(r'^configure_anomaly_detection$', views.configure_anomaly_detection, name='configure_anomaly_detection'),
    url(r'^configure_anomaly_detection_step2$', views.configure_anomaly_detection_step2, name='configure_anomaly_detection_step2'),
]
