#!/usr/bin/env python3
"""
rpi_serial.py
"""

import os, sys

DEFAULT_SN = 'default'

def get_serial():
    # Extract rpi serial from cpuinfo fi
    try:
        with open('/proc/cpuinfo', 'r') as f:
            for line in f:
                if 'Serial' in line:
                    sn = str(line[10:26])
                    print('serial number found:',sn)
                    os.environ['HJ_SERIAL'] = sn
                    os.system("echo " + sn + " > ../instance/serial.rpi")
                    return sn
            print('serial number not found in /proc/cpuinfo file')
            return DEFAULT_SN
    except Exception as e:
        return "ger serial ERR:" + str(e)

serial = get_serial()

def get_mac():
    sn = get_serial()
    mac = "AA:BB:"
    for i in range(8, 15, 2):
        mac += str(sn[i])+str(sn[i+1])
        if i<13:
            mac += ':'
    print('mac',mac)
    os.environ['HJ_MAC_ADDR'] = mac
    os.system("echo " + mac + " > ../instance/homekit.mac")
    return mac

mac = get_mac()

def get_pin():
    sn = get_serial()
    pin = ''
    for i in range(8, 15):
        pin += str(sn[i])
        if i == 10 or i == 12:
            pin += '-'
    print('pin',pin)
    os.environ['HJ_PIN_NUMBER'] = pin
    os.system("echo " + pin + " > ../instance/homekit.pin")
    return pin

pin = get_pin()

#if __name__ == "__main__":
#    get_serial()
#    get_mac()
