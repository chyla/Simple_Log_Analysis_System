# -*- coding: utf-8 -*-

from django.shortcuts import render_to_response, render, redirect
from django.http import HttpResponseRedirect
from django.contrib import auth
from django.contrib.auth import authenticate, login, logout

def user_login(request):
    return render(request,
                  'user/login.html',
                  {})

def user_auth(request):
    username = request.POST.get('username', '')
    password = request.POST.get('password', '')

    user = authenticate(username=username, password=password)

    if user is not None:
        login(request, user)
        return HttpResponseRedirect("/")
    else:
        return HttpResponseRedirect("/user/invalid_login")

def user_logout(request):
    logout(request)

    return HttpResponseRedirect("/user/login")

def user_invalid_login(request):
    return render(request,
                  'user/invalid_login.html',
                  {})
