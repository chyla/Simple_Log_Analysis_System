# -*- coding: utf-8 -*-

from django.shortcuts import render_to_response

def configure_actions(request):
    return render_to_response('apache/configure_actions.html')

def status(request):
    return render_to_response("apache/status.html")

def configure_anomaly_detection(request):
    return render_to_response('apache/configure_anomaly_detection.html')

def configure_anomaly_detection_step2(request):
    return render_to_response('apache/configure_anomaly_detection_step2.html')
