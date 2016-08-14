# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
    ]

    operations = [
        migrations.CreateModel(
            name='Settings',
            fields=[
                ('id', models.AutoField(verbose_name='ID', serialize=False, auto_created=True, primary_key=True)),
                ('address', models.TextField(verbose_name=b'Server address')),
                ('port', models.IntegerField(verbose_name=b'Server port')),
            ],
            options={
            },
            bases=(models.Model,),
        ),
    ]
