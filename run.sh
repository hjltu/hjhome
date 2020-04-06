#!/bin/bash

# stop if error
#set -e

NAME="hjhome"
HJMQTT="hjmqtt"
HJSITE="site"
HJCONNECT="hjconnect"
USER="pi"
PASSWD="raspberry10"

VENV="venv"
DATE=`date +%d-%b-%y_%H%M`
CURR_DIR=${PWD##*/}     # current dir name

HJ_DIR=(hjmqtt hjconnect site)
BACKUP_DIR="backup"

APT_PKG=(mosquitto mosquitto-clients libsqlite3-dev sqlite3
    curl supervisor python-pip vim tmux ncdu rsync
    python3-pip python3-venv libavahi-compat-libdnssd-dev)

APT_PKG2=(mc htop tree bc gcc make cmake
    ethtool iperf iproute socat tcpdump rsync
    arp-scan iftop iptraf nmap tshark picocom minicom
    w3m links2 elinks screen pv mininet git)

APP_PKG3=(mplayer ffmpeg fswebcam motion nudoku cowsay cmatrix)
#mpv gnuplot

PIP_PKG=(psutil paho-mqtt flask pyserial esptool adafruit-ampy)

NODE_RED_PKG=(node-red-dashboard node-red-contrib-sonos
    node-red-contrib-knx node-red-node-serialport)

NETWORK_SETS='
interface eth0
static ip_address=192.168.0.10/24
static routers=192.168.0.1
static domain_name_servers=8.8.8.8, 8.8.4.4
interface wlan0
static ip_address=192.168.0.10/24
static routers=192.168.0.1
static domain_name_servers=8.8.8.8, 8.8.4.4'
NETWORK_MATCH="interface eth0"

function my_help()
{
    echo "usage:"
    echo "    ./run.sh [-n, -i, -u, -f, -t, -d, -c, -r, -s, -b, -h]"
    echo "    -n    network settings"                   # tested
    echo "    -i    hjhome install"                     # tested
    echo "    -u    hjhome upgrade"
    echo "    -f    full install"                       # tested
    echo "    -t    copy files, start hjhome services"  # tested
    echo "    -d    docker install"                     # tested
    echo "    -c    set crontab"                        # tested
    echo "    -r    remove swap and syslog"             # tested
    echo "    -s    save hjhome"
    echo "    -b    hjhome full backup in tgz"
    echo "    -h    print this text"
    echo "full install example:"
    echo "    follow the sequence of arguments: ./run.sh -n -i -t -c"
}

if [ $# -eq 0 ];then
    my_help; fi


function main()
{
    #echo $#     # len
    #echo $@     # names
    for arg in $@; do
        if [[ $arg == "-h" ]]; then
            my_help; fi
        if [[ $arg == "-i" ]]; then
            hjhome_install; fi
        if [[ $arg == "-u" ]]; then
            hjhome_upgrade; fi
        if [[ $arg == "-f" ]]; then
            full_install; fi
        if [[ $arg == "-d" ]]; then
            docker_install; fi
        if [[ $arg == "-n" ]]; then
            network_setup; fi
        if [[ $arg == "-c" ]]; then
            crontab_setup; fi
        if [[ $arg == "-r" ]]; then
            remove_syslog; fi
        if [[ $arg == "-s" ]]; then
            save_hjhome; fi
        if [[ $arg == "-b" ]]; then
            full_backup; fi
        if [[ $arg == "-t" ]]; then
            start_services; fi
    done
}


function hjhome_install()
{
    cd $HOME
    info blue "*** APT INSTALL ***"
    sudo apt update
    apt_install ${APT_PKG[*]}
    sync
    return
    info blue "*** PIP INSTALL ***"
    sudo pip3 install ${PIP_PKG[@]}
    sync
    info green "All pip packages is installed"

    info blue "*** NODE INSTALL ***"
    if which node > /dev/null; then
        info green "node is already installed"
    else
        curl -sL https://deb.nodesource.com/setup_10.x | sudo -E bash -
        #sudo apt install -y nodejs node-gyp npm
        sudo apt install -y nodejs
        sync
    fi
    info blue "Check node version:"
    if which node > /dev/null; then
        echo -n "node version: "; node -v
        echo -n "nodejs version: "; nodejs -v
        echo -n "npm version: "; npm -v
        info green "node is installed"
    else
        info red "NODE IS NOT INSTALLED EXIT"; exit 1
    fi

    cd $HOME
    info blue "*** HOMEKIT2MQTT INSTALL ***"
    if which homekit2mqtt > /dev/null; then
        info green "homekit2mqtt is already installed"
    else
        #sudo npm -g install homebridge --unsafe-perm
        sudo npm install -g homekit2mqtt --unsafe-perm
        sync
        bash homekit2mqtt &
        sleep 9
        killall homekit2mqtt
    fi

    cd $HOME
    info blue "*** NODE-RED INSTALL ***"
    if which node-red > /dev/null; then
        info green "node-red is already installed"
    else
        sudo npm install -g --unsafe-perm node-red
        mkdir -p .node-red
        cd .node-red
        npm i ${NODE_RED_PKG[@]}
        cd $HOME
        bash node-red &
        sleep 9
        killall node-red
    fi
}


function start_services()
{
    info blue "*** COPY FILES ***"
    cd $HOME/$CURR_DIR
    for arc in *.tgz; do
        info blue "extracting $arc..."
        tar xzf $arc -C $HOME
    done
    sudo cp files/hjhome.service /etc/systemd/system
    sudo cp files/node-red.service /etc/systemd/system
    sudo cp files/site.conf /etc/supervisor/conf.d
    sudo cp files/hjmqtt.conf /etc/supervisor/conf.d
    sudo cp files/hjconnect.conf /etc/supervisor/conf.d
    cp files/settings.js $HOME/.node-red
    cp files/flows_raspberrypi.json $HOME/.node-red
    cp files/vimrc $HOME/.vimrc
    cp files/ns.sh $HOME

    info blue "*** START SERVICES ***"
    #echo "sudo ALL=(ALL:ALL) ALL" | sudo EDITOR='tee -a' visudo
    bash $HOME/ns.sh
    info green "okies"
}

function hjhome_upgrade()
{
    info blue "*** BACKUP DB FILES ***"
    cd $HOME
    mkdir -p $BACKUP_DIR
    cp -p hjmqtt/stat.db $BACKUP_DIR
    cp -p site/instance/test.db $BACKUP_DIR

    info blue "*** DELETE FILES ***"
    cd $HOME
    for dir in ${HJ_DIR[@]}; do
        echo "rm $dir"
        rm -rf $dir
    done

    info blue "*** COPY NEW FILES ***"
    cd $HOME/$CURR_DIR
    for arc in *.tgz; do
        info blue "extracting $arc..."
        tar xzf $arc -C $HOME
    done

    info blue "*** COPY DB FILES ***"
    cd $HOME
    cp -p $BACKUP_DIR/stat.db $HJMQTT
    cp -p $BACKUP_DIR/test.db $HJSITE/instance
}

function apt_install()
{
    cd $HOME
    info blue "*** APT INSTALL ***"
    sudo apt update
    echo $@
    for pkg in $@; do
        apt list --installed $pkg 2>/dev/null | grep -o $pkg
        if [ $? -ne "0" ]; then
            info blue "installing $pkg..."
            sudo apt install -y $pkg
        fi
    done
}

function full_install()
{
    info blue "*** APT INSTALL BASIC TOOL ***"
    apt_install ${APT_PKG[*]}
    info blue "*** PIP INSTALL ***"
    #sudo pip3 install ${PIP_PKG[@]}
    info blue "*** APT INSTALL NETWORKS TOOLS ***"
    apt_install ${APT_PKG2[*]}
    info blue "*** APT INSTALL MEDIA & EXTRA ***"
    apt_install ${APT_PKG3[*]}
    info green "okies"
}

function docker_install()
{
    info blue "*** DOCKER INSTALL ***"
    curl -sSL https://get.docker.com | sh
    info green "docker is installed"
    sudo usermod -aG docker $USER
    info green "$USER added to group docker"
    info green "okies"
}

function full_backup()
{
    info blue "*** BACKUP ***"
    cd $HOME
    mkdir -p $BACKUP_DIR
    return
    # TODO copy db
    # TODO copy & replace files
    for dir in *; do
        if [[ -d $dir ]]; then
            echo $dir
        fi
    done
    info green "okies"
}

function network_setup()
{
    info blue "*** NETWORK SETUP ***"
    info blue "change password"
    #sudo su # start root
    #read -p "sudo password:" -s PASS
    #echo $PASS

    #exit
    echo "$USER:$PASSWD" | sudo chpasswd
    #exit # end root
    info blue "enable ssh server"
    sudo touch /boot/ssh
    info blue "set ip address 192.168.0.10"
    #grep -qxF "$NETWORK_SETS" $PWD/test.conf || echo "$NETWORK_SETS" >> $PWD/test.conf
    grep -qxF "$NETWORK_MATCH" /etc/dhcpcd.conf || sudo echo "$NETWORK_SETS" >> /etc/dhcpcd.conf
    info green "okies"

}

function crontab_setup()
{
    info blue "*** CRONTAB SETUP ***"
    crontab files/mycron.txt
    info green "okies"
}

function remove_syslog()
{
    info blue "*** REMOVE SYSLOG ***"
    sudo apt update
    sudo apt purge -y dphys-swapfile
    sudo apt autoremove --purge -y
    sudo apt install -y busybox-syslogd
    sudo dpkg --purge rsyslog
    info green "okies"
}

function my()
{
# install env
if [[ $1 == "-env" ]]; then
    echo "*** INSTALL ENV ***"
    python3 -m venv $VENV
    echo "okies"
    exit 0
fi

# install app
if [[ $1 == "-i" ]]; then
    echo "*** PIP INSTALL ***"
    $VENV/bin/pip install $PIP_PKGS
    echo "okies"
    exit 0
fi
}

function save_hjhome()
{
    info blue "*** SAVE ***"
    tar cvzf ../${CURR_DIR}-${DATE}.tgz \
    --exclude="$VENV" \
    --exclude="__pycache__" \
    --exclude="*/__pycache__" \
    --exclude="*/*/__pycache__" \
    --exclude="*.pyc" \
    --exclude="*/*.pyc" \
    --exclude="*.swp" \
    ../$CURR_DIR
    ls -lh ..
    info green "okies"
}

function info()
{
    local GREEN='\033[1;32m'
    local BLUE='\033[1;34m'
    local RED='\033[1;31m'
    local NC='\033[0m' # No Color
    local MSG=$2
    local COLOR=$1
    if [ $COLOR = "green" ]; then
        echo -e "${GREEN}${MSG}${NC}"
    elif [ $COLOR = "blue" ]; then
        echo -e "${BLUE}${MSG}${NC}"
    elif [ $COLOR = "red" ]; then
        echo -e "${RED}${MSG}${NC}"
    fi
}

main $@
info green "*** THE END ***"
