# -*- coding: utf-8 -*-

from django.db import models


class Settings(models.Model):
    address = models.TextField(verbose_name='Server address')
    port = models.IntegerField(verbose_name='Server port')
