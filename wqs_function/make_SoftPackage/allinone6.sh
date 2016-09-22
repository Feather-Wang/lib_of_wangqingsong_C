#!/bin/bash
#BUILDNO=`date +%Y%m%d%H%M%S`
BUILDNO=`date +%Y%m%d`
PROGRAM=_install/WQS_$BUILDNO.sh
rm -rf _install/WQS_*.sh*

#创建临时目录
TMP_DIR=.topwalk_mtp
rm -fr $TMP_DIR
mkdir -p $TMP_DIR

#后台程序
cp -dpRf backend $TMP_DIR/

#frontend package
cp -dpRf frontend $TMP_DIR/

find $TMP_DIR -type d -name .svn|xargs rm -rf

mkdir -p _install
shar $TMP_DIR > $PROGRAM
sed -i '$d' $PROGRAM

cat >> $PROGRAM << EOF
TMP_DIR=.topwalk_mtp
clear
echo "Start install ..."

#install backend
/bin/sh \$TMP_DIR/backend/*.sh

#install frontend
/bin/sh \$TMP_DIR/frontend/*.sh

rm -rf \$TMP_DIR
exit 0
EOF
rm -fr $TMP_DIR
md5sum $PROGRAM > $PROGRAM.md5
chmod 755 $PROGRAM
