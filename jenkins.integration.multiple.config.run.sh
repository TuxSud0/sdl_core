#!/bin/bash 
DBG_OPTION=""
TOOLCHAIN_OPTION=""
HMI_OPTION=""
if [ $DEBUG ] && [ $DEBUG = "DBG_OFF" ]; then
     DBG_OPTION="-DCMAKE_BUILD_TYPE=Release"
fi
if [ $OS_TYPE ] && [ $OS_TYPE = "QNX" ]; then
     TOOLCHAIN_OPTION="-DCMAKE_TOOLCHAIN_FILE=./qnx_6.5.0_linux_x86.cmake"
fi
if [ $HMI_TYPE ] && [ $HMI_TYPE = "QML2" ]; then
     HMI_OPTION="-DHMI2=ON"
fi

echo cmake ${DBG_OPTION} ${TOOLCHAIN_OPTION} ${HMI_OPTION} .
cmake ${DBG_OPTION} ${TOOLCHAIN_OPTION} ${HMI_OPTION} .
make install