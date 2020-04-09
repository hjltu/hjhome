#!/usr/bin/env python3

"""
hjsite.py

Copyright (C) 2016  hjltu@ya.ru

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files
(the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge,
publish, distribute, sublicense, and/or sell copies of the Software,
and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
"""

# hjmqtt
MQTT_SERVER = '192.168.5.1'
VERSION = '5-Apr-20'
INSTANCE_DIR = '../instance'
SETUP_DIR = '../setup'
# hjhomekit
HOMEKIT_NAME = "hjhome"
HOMEKIT_ACCESSORY_FILE = "homekit.json"
MAC_FILE = "homekit.mac"
PIN_FILE = "pin.code"
# hjconnect
REMOTE_MQTT_SERVER = 'test.mosquitto.org'
REMOTE_MQTT_PORT = 1883
# hjsite
SMTP_SERVER = 'smtp.example.com'
SMTP_PORT = 465
FROM_EMAIL = 'email@example.com'
FROM_EMAIL_PASSWD = 'password'
TO_EMAILS = ['email@example.org', 'email_2@example.org']
