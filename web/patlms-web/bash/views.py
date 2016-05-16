# -*- coding: utf-8 -*-

from django.shortcuts import render_to_response


def status(request):
    return render_to_response("bash/status.html")
