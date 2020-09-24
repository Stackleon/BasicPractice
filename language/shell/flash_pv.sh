#!/bin/bash
if [ $# -ne 1 ]
then
echo "please input only one parameter!!!"
exit 1
fi

cd /mnt/hgfs/ulixs114/ChromeDownload/

Suffix=.zip
File=$1
Target=$File$Suffix

echo "Target:${Target}"

if [ ! -e $Targer ];
then
    echo "Target:${Target} not exist"
    exit 1
fi

if [ ! -e $File ];
then
echo "unzip -o ${Target}"
unzip -o ${Target}
fi

cd ${File}
pwd

if [ -e fota.img ];
then
echo "rm fota.img"
rm -rf fota.img fota_yf.img
fi
./build.sh


