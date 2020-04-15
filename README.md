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
>`docker build -f Dockerfile.hjmqtt -t hjmqtt .`
### Run:
>`docker run --rm --name=hjmqtt -v "$(pwd)"/config:/root/config hjmqtt`
<br>or
<br>`docker run -d --restart=always --name=hjmqtt -v "$(pwd)"/config:/root/config hjmqtt`
## hjsite
### Build:
>`docker build -f Dockerfile.hjsite -t hjsite .`
### Run:
>`docker run --rm --name=hjsite -p 5000:5000 -v "$(pwd)"/config:/root/config -v "$(pwd)"/instance:/root/instance -v /var/run/docker.sock:/var/run/docker.sock hjsite`
<br>or
<br>`docker run -d --restart=always --name=hjsite -p 5000:5000 -v "$(pwd)"/config:/root/config -v "$(pwd)"/instance:/root/instance -v /var/run/docker.sock:/var/run/docker.sock hjsite`
## hjhomekit
### Build:
>`docker build -f Dockerfile.hjhomekit -t hjhomekit .`
### Run:
>`docker run --rm --name=hjhomekit --network=host -v "$(pwd)"/config:/root/config -v "$(pwd)"/instance:/root/instance hjhomekit`
<br>or
<br>`docker run -d --restart=always --name=hjhomekit --network=host -v "$(pwd)"/config:/root/config -v "$(pwd)"/instance:/root/instance hjhomekit`
## hjnode-red
### Build:
>`docker build -f Dockerfile.hjnode-red -t hjnode-red .`
### Run:
>`docker run --rm --name=hjnode-red -p 1880:1880 -v "$(pwd)"/config:/root/config hjnode-red`
<br>or
<br>`docker run -d --restart=always --name=hjnode-red -p 1880:1880 -v "$(pwd)"/config:/root/config hjnode-red`
## hjconnect
### Build:
>`docker build -f Dockerfile.hjconnect -t hjconnect .`
### Run:
>`docker run --rm --name=hjconnect -v "$(pwd)"/config:/root/config -v "$(pwd)"/instance:/root/instance -v "$(pwd)"/hjhome:/root/hjhome hjconnect`
<br>or
<br>`docker run -d --restart=always --name=hjconnect -v "$(pwd)"/config:/root/config -v "$(pwd)"/instance:/root/instance -v "$(pwd)"/hjhome:/root/hjhome hjconnect`
