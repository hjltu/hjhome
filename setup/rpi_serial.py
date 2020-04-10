#!/usr/bin/env python3
"""
rpi_serial.py
"""

import os, sys, socket

DEFAULT_SN = 'default'
SERIAL_PATH = "../instance/serial.rpi"
MAC_PATH = "../instance/homekit.mac"
PIN_PATH = "../instance/homekit.pin"

def get_serial():
    # Extract rpi serial from cpuinfo fi
    try:
        with open('/proc/cpuinfo', 'r') as f:
            for line in f:
                if 'Serial' in line:
                    sn = str(line[10:26])
                    print('serial number found:',sn)
                    os.environ['HJ_SERIAL'] = sn
                    os.system("echo " + sn + " > " + SERIAL_PATH)
                    return sn
            print('serial number not found in /proc/cpuinfo file')
            return DEFAULT_SN
    except Exception as e:
        return "ger serial ERR:" + str(e)

serial = get_serial()

def get_mac():
    sn = get_serial()
    mac = "AA:BB:"
    for i in range(8, 16, 2):
        mac += str(sn[i])+str(sn[i+1])
        if i<13:
            mac += ':'
    print('mac',mac)
    os.environ['HJ_MAC_ADDR'] = mac
    os.system("echo " + mac + " > " + MAC_PATH)
    return mac

mac = get_mac()

def get_pin():
    sn = get_serial()
    pin = ''
    for i in range(8, 16):
        pin += str(sn[i])
        if i == 10 or i == 12:
            pin += '-'
    print('pin',pin)
    os.environ['HJ_PIN_NUMBER'] = pin
    os.system("echo " + pin + " > " + PIN_PATH)
    return pin

pin = get_pin()

def get_ip():
    ip = "192.168.0.10"
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        #s.connect(('10.255.255.255', 1))
        s.connect(('1.1.1.1', 1))
        ip = s.getsockname()[0]
        print("IP Address:", ip)
    except:
        print("IP address not found")
    return ip

ip = get_ip()

def get_hostname():
    hostname = socket.gethostname()
    print("Host name:", hostname)
    return hostname

hostname = get_hostname()

#if __name__ == "__main__":
#    get_serial()
#    get_mac()
