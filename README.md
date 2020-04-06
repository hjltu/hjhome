# hjhome
### Install:
`git clone --single-branch --branch testing https://github.com/hjltu/hjhome.git`
<br>`cd hjhome`
<br>`git clone --single-branch --branch testing https://github.com/hjltu/hjmqtt.git`
<br>`git clone --single-branch --branch master https://github.com/hjltu/hjsite.git`
<br>`git clone --single-branch --branch testing https://github.com/hjltu/hjconnect.git`
### Config:
**Accessory file:**
<br>config/{SERIAL}.csv
<br>SERIAL: Rpi serial number (/proc/cpuinfo)
<br>default csv file: setup/default.csv
<br>documentation: https://github.com/OSA83/hjhome-doc
## hjmqtt
**Configuration file:**
<br>config/hjmqtt.py
<br>MQTT_SERVER: mqtt server IP address
<br>VERSION: version string
### Build:
>`docker build -f Docker.hjmqtt -t hjmqtt .`
### Run:
>`docker run -d --rm --name=hjmqtt -v "$(pwd)"/my/config:/root/config hjmqtt`
<br>or
<br>`docker run -d --restart=always --name=hjmqtt -v "$(pwd)"/my/config:/root/config hjmqtt`
## hjsite
**Configuration file:**
<br>config/hjsite.py
<br>MQTT_SERVER: mqtt server IP address
<br>VERSION: version string
### Build:
>`docker build -f Docker.hjsite -t hjsite .`
### Run:
>`docker run -d --rm --name=hjsite -v "$(pwd)"/my/config:/root/config hjsite`
<br>or
<br>`docker run -d --restart=always --name=hjsite -v "$(pwd)"/my/config:/root/config hjsite`
## homekit
