#!/usr/bin/env bash

set -e

SUBVERSION="subversion"
CMAKE_BUILD_SYSTEM="cmake"
GNU_CPP_COMPILER="g++"
BLUEZ_PROTOCOL_STACK="libbluetooth3 libbluetooth-dev"
LOG4CXX_LIBRARY="liblog4cxx10 liblog4cxx10-dev"
CHROMIUM_BROWSER="chromium-browser"
PULSEAUDIO_DEV="libpulse-dev"
UPDATE_SOURCES=false
OPENGL_DEV="libgl1-mesa-dev"
AVAHI_CLIENT_LIBRARY="libavahi-client-dev"
AVAHI_COMMON="libavahi-common-dev"
DOXYGEN="doxygen"
GRAPHVIZ="graphviz"
MSCGEN="mscgen"


DISTRIB_CODENAME=$(grep -oP 'CODENAME=(.+)' -m 1 /etc/lsb-release | awk -F= '{ print $NF }')

GSTREAMER_REPO_LINK="deb http://ppa.launchpad.net/gstreamer-developers/ppa/ubuntu"
GSTREAMER_SRC_REPO_LINK="deb-src http://ppa.launchpad.net/gstreamer-developers/ppa/ubuntu"

FULL_GSTREAMER_REPO_LINK="$GSTREAMER_REPO_LINK $DISTRIB_CODENAME main"
FULL_GSTREAMER_SRC_REPO_LINK="$GSTREAMER_SRC_REPO_LINK $DISTRIB_CODENAME main"

function apt-install() {
    if [ -z "$1" ]
    then
        echo "warning: apt-install() function called without parameters"
        return 1;
    fi
    set -x #Show install command to user"
    sudo apt-get install --yes --force-yes ${APT_INSTALL_FLAGS} $*
    set +x
}

echo "Installng CMake build system"
apt-install ${CMAKE_BUILD_SYSTEM}
echo $OK

echo "Installng GNU C++ compiler"
apt-install ${GNU_CPP_COMPILER}
echo $OK

echo "Installng BlueZ Bluetooth protocol stack implementation"
apt-install ${BLUEZ_PROTOCOL_STACK}
echo $OK

echo "Installing log4cxx library"
apt-install ${LOG4CXX_LIBRARY}
echo $OK

echo "Installing Chromium browser"
apt-install ${CHROMIUM_BROWSER}
echo $OK

echo "Installing pulseaudio development files"
apt-install ${PULSEAUDIO_DEV}
echo $OK

echo "Installing Avahi-common-dev library"
apt-install ${AVAHI_COMMON}
echo $OK

echo "Installing Avahi-client-dev library"
apt-install ${AVAHI_CLIENT_LIBRARY}
echo $OK

echo "Installing Doxygen"
apt-install ${DOXYGEN}
echo $OK

echo "Installing Graphviz for doxygen"
apt-install ${GRAPHVIZ}
echo $OK

echo "Installing Mscgen"
apt-install ${MSCGEN}
echo $OK

echo "Installing OpenGL development files"
apt-install ${OPENGL_DEV}
echo $OK

echo "Installing Subversion"
apt-install ${SUBVERSION}
echo $OK

echo "Checking out Qt5 installation runfile, please be patient"
svn checkout ... /tmp
echo $OK

echo "Installing Qt5 libraries"
QT5_RUNFILE="/tmp/qt-linux-opensource-5.1.0-x86-offline.run"
chmod +x ${QT5_RUNFILE}
sudo ${QT5_RUNFILE}
rm ${QT5_RUNFILE}
echo $OK

echo "Setting up Qt5 cmake environment:"
for module in Core DBus Qml Quick
do
  echo "module "$module"..."
  find_command_prefix="find /usr /opt / -name Qt5"
  find_command_suffix="Config.cmake -print -quit"
  find_command=$find_command_prefix$module$find_command_suffix
  find_result=`$find_command`
  file_name_prefix="cmake/Modules/FindQt5"
  file_name_suffix=".cmake"
  file_name=$file_name_prefix$module$file_name_suffix
  echo "include("$find_result")" > $file_name
done
echo $OK

sudo cp /etc/apt/sources.list /etc/apt/sources.list.backup

if ! grep --quiet "$FULL_GSTREAMER_REPO_LINK" /etc/apt/sources.list; then
	echo "Adding gstreamer to /etc/apt/sources.list"
	sudo sed -i "\$i$FULL_GSTREAMER_REPO_LINK" /etc/apt/sources.list
	UPDATE_SOURCES=true
fi

if ! grep --quiet "$FULL_GSTREAMER_SRC_REPO_LINK" /etc/apt/sources.list; then
	echo "Adding gstreamer sources to /etc/apt/sources.list"
	sudo sed -i "\$i$FULL_GSTREAMER_SRC_REPO_LINK" /etc/apt/sources.list
	UPDATE_SOURCES=true
fi

if $UPDATE_SOURCES; then
	echo "Updating repository..."
	sudo apt-get update
fi

echo "Installing gstreamer..."
apt-install gstreamer1.0*
 


