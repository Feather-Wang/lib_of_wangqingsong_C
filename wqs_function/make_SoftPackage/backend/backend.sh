#!/bin/bash
#BUILDNO=`date +%Y%m%d%H%M%S`
BUILDNO=`date +%Y%m%d`
PROGRAM=BackEnd_$BUILDNO.sh
rm -rf BackEnd_*.sh*

#创建临时目录
TMP_DIR=.backend_tmp
rm -fr $TMP_DIR
mkdir -p $TMP_DIR

#frontend package
cp -dpRf bin $TMP_DIR/
cp -dpRf conf $TMP_DIR/

find $TMP_DIR -type d -name .svn|xargs rm -rf

mkdir -p _install
shar $TMP_DIR > $PROGRAM
sed -i '$d' $PROGRAM

cat >> $PROGRAM << EOF
TMP_DIR=.backend_tmp

clear
mkdir -p /wangqingsong/topapp/topback/
cp -dpRf \$TMP_DIR/bin/* /wangqingsong/topapp/topback/
mkdir -p /wangqingsong/topconf/topback/
cp -dpRf \$TMP_DIR/conf/* /wangqingsong/topconf/topback/

ldconfig
sync

rm -rf \$TMP_DIR
exit 0
EOF
rm -fr $TMP_DIR
md5sum $PROGRAM > $PROGRAM.md5
chmod 755 $PROGRAM
