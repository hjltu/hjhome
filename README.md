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
<br>email seettings:
<br>SMTP_SERVER, SMTP_PORT - email server settings
<br>FROM_EMAIL, FROM_EMAIL_PASSWD email account settings
<br>TO_EMAILS - list of address to receive registration email

## hjmqtt
### Build:
>`docker build -f Dockerfile.hjmqtt -t hjmqtt .`
### Run:
>`docker run --rm --name=hjmqtt -v "$(pwd)"/config:/root/config hjmqtt`
<br>or
<br>`docker run -d --restart=always --name=hjmqtt -v "$(pwd)"/config:/root/config hjmqtt`
<br>**Check:**
<br>`mosquitto_sub -t /# -v`
## hjsite
### Build:
>`docker build -f Dockerfile.hjsite -t hjsite .`
### Run:
>`docker run --rm --name=hjsite -p 5000:5000 -v "$(pwd)"/config:/root/config -v "$(pwd)"/instance:/root/instance -v /var/run/docker.sock:/var/run/docker.sock hjsite`
<br>or
<br>`docker run -d --restart=always --name=hjsite -p 5000:5000 -v "$(pwd)"/config:/root/config -v "$(pwd)"/instance:/root/instance -v /var/run/docker.sock:/var/run/docker.sock hjsite`
<br>**Check:**
<br>http://192.168.0.10:5000    # where 192.168.0.10 - raspberry pi IP address
## hjhomekit
### Build:
>`docker build -f Dockerfile.hjhomekit -t hjhomekit .`
### Run:
>`docker run --rm --name=hjhomekit --network=host -v "$(pwd)"/config:/root/config -v "$(pwd)"/instance:/root/instance -v "$(pwd)"/hass:/root/hass hjhomekit`
<br>or
<br>`docker run -d --restart=always --name=hjhomekit --network=host -v "$(pwd)"/config:/root/config -v "$(pwd)"/instance:/root/instance -v "$(pwd)"/hass:/root/hass hjhomekit`
<br>**Check:**
<br>http://192.168.0.10:51888
## hjnode-red
### Build:
>`docker build -f Dockerfile.hjnode-red -t hjnode-red .`
### Run:
>`docker run --rm --name=hjnode-red -p 1880:1880 -v "$(pwd)"/config:/root/config hjnode-red`
<br>or
<br>`docker run -d --restart=always --name=hjnode-red -p 1880:1880 -v "$(pwd)"/config:/root/config hjnode-red`
<br>**Check:**
<br>http://192.168.0.10:1880
## hjconnect
### Build:
>`docker build -f Dockerfile.hjconnect -t hjconnect .`
### Run:
>`docker run --rm --name=hjconnect -v "$(pwd)"/config:/root/config -v "$(pwd)"/instance:/root/instance hjconnect`
<br>or
<br>`docker run -d --restart=always --name=hjconnect -v "$(pwd)"/config:/root/config -v "$(pwd)"/instance:/root/instance hjconnect`
<br>**Check:**
<br>`mosquitto_sub -t /# -v`
<br>or
<br>`mosquitto_sub -h test.mosquitto.org -t /hjconnect/# -v`
## homeassistant
### Run:
>`docker run --init --rm --name=hass -p 8123:8123 -v "$(pwd)"/hass:/config homeassistant/raspberrypi3-homeassistant:stable`
<br>or
<br>`docker run --init -d --restart=always --name=hass -p 8123:8123 -v "$(pwd)"/hass:/config homeassistant/raspberrypi3-homeassistant:stable`
<br>**Check and setup:**
<br>http://192.168.0.10:8123
