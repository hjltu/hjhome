#!/usr/bin/env python3

"""
hjhome.py

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

VERSION = "5-Apr-20"
INSTANCE_DIR = "../instance"
SETUP_DIR = "../setup"
# rpi
RPI_IP = "192.168.0.10"
# knx
BAOS_IP = "192.168.0.40"
# iport
IPORT_IP = "192.168.0.20"
# hjmqtt
MQTT_SERVER = "172.17.0.1"
# hjhomekit
HOMEKIT_NAME = "hjhome"
HOMEKIT_ACCESSORY_FILE = "homekit.json"
MAC_FILE = "homekit.mac"
PIN_FILE = "pin.code"
# hjconnect
CONN_ENCRYPTION = False
CONN_CA_CRT = "ca.crt"  # /root/config/
CONN_REMOTE_SERVER = "test.mosquitto.org"
CONN_REMOTE_PORT = 1883
CONN_LOCAL_PORT = 1883
CONN_GAP = 399
CONN_RTOPIC = "/hjconnect/"
CONN_LTOPIC = "/hjlocal/"
CONN_TMPDIR = "temp"
CONN_CHUNKSIZE = 9999
# email
SMTP_SERVER = "smtp.yandex.ru"
SMTP_PORT = 465
FROM_EMAIL = "message-intellect@ya.ru"
FROM_EMAIL_PASSWD = "password"
TO_EMAILS = ["message-intellect@ya.ru", "hjltu@ya.ru"]
# homeassistant
HASS_TZ = "Asia/Yekaterinburg"
HASS_WEB_PORT = 8123
