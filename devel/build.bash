#!/usr/bin/env bash

opwd=$(pwd)
srdDir=$(pwd)
if [ ! -f ${srdDir}/.trackingtk ]; then
    echo >&2 "error: cannot locate the current dir as the TrackingTK source dir"
    exit 1   
fi

buildDir=/tmp/${USER}/TrackingTK/_build.d
installDir=/tmp/${USER}/TrackingTK/_install.d

if [ ! -d ${buildDir} ]; then
    mkdir -p ${buildDir}
    if [ $? -ne 0 ]; then
	echo >&2 "error: cannot create build dir"
	exit 1
    fi
fi

if [ ! -d ${installDir} ]; then
    mkdir -p ${installDir}
    if [ $? -ne 0 ]; then
	echo >&2 "error: cannot create install dir"
	exit 1
    fi
fi

tree /tmp/${USER}/TrackingTK

cd ${buildDir}
cmake \
    -DCMAKE_INSTAll_PREFIX=${installDir} \
    ${srdDir}
if [ $? -ne 0 ]; then
    echo >&2 "error: cannot configure TrackingTK"
    exit 1
fi

make 
if [ $? -ne 0 ]; then
    echo >&2 "error: cannot build TrackingTK"
    exit 1
fi
tree ${buildDir}

cd ${opwd}
exit 0

