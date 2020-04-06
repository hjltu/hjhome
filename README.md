# hjhome
### Install:
`git clone --single-branch --branch testing https://github.com/hjltu/hjhome.git`
<br>`cd hjhome`
<br>`git clone --single-branch --branch testing https://github.com/hjltu/hjmqtt.git`
<br>`git clone --single-branch --branch master https://github.com/hjltu/hjsite.git`
<br>`git clone --single-branch --branch master https://github.com/hjltu/hjhomekit.git`
<br>`git clone --single-branch --branch master https://github.com/hjltu/hjnode-red.git`
<br>`git clone --single-branch --branch testing https://github.com/hjltu/hjconnect.git`
### Config:
**Accessory file:**
<br>config/{SERIAL}.csv
<br>SERIAL: Rpi serial number (/proc/cpuinfo)
<br>default csv file: setup/default.csv
<br>documentation: https://github.com/OSA83/hjhome-doc

**Configuration file:**
<br>config/hjhome.py
<br>MQTT_SERVER: mqtt server IP address
<br>VERSION: version string
## hjmqtt
### Build:
>`docker build -f Docker.hjmqtt -t hjmqtt .`
### Run:
>`docker run -d --rm --name=hjmqtt -v "$(pwd)"/my/config:/root/config hjmqtt`
<br>or
<br>`docker run -d --restart=always --name=hjmqtt -v "$(pwd)"/my/config:/root/config hjmqtt`
## hjsite
### Build:
>`docker build -f Docker.hjsite -t hjsite .`
### Run:
>`docker run -d --rm --name=hjsite -v "$(pwd)"/my/config:/root/config hjsite`
<br>or
<br>`docker run -d --restart=always --name=hjsite -v "$(pwd)"/my/config:/root/config hjsite`
## hjhomekit
### Build:
>`docker build -f Docker.hjhomekit -t hjhomekit .`
### Run:
>`docker run -d --rm --name=hjhomekit -v "$(pwd)"/my/config:/root/config hjhomekit`
<br>or
<br>`docker run -d --restart=always --name=hjhomekit -v "$(pwd)"/my/config:/root/config hjhomekit`
## hjnode-red
### Build:
>`docker build -f Docker.hjnode-red -t hjnode-red .`
### Run:
>`docker run -d --rm --name=hjnode-red -v "$(pwd)"/my/config:/root/config hjnode-red`
<br>or
<br>`docker run -d --restart=always --name=hjnode-red -v "$(pwd)"/my/config:/root/config hjnode-red`
## hjconnect
### Build:
>`docker build -f Docker.hjconnect -t hjconnect .`
### Run:
>`docker run -d --rm --name=hjconnect -v "$(pwd)"/my/config:/root/config hjconnect`
<br>or
<br>`docker run -d --restart=always --name=hjconnect -v "$(pwd)"/my/config:/root/config hjconnect`
