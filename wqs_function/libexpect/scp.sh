#£¡/bin/bash
SFile=$1
DPath=$2
#echo $SFile
#echo $DPath

#echo "wang is "
#wang=${SFile}${DPath}
#echo $wang

expect -c "
set timeout 1200;
spawn /usr/bin/scp -r $SFile root@192.168.92.106:$DPath
expect {
\"*yes/no*\" {send \"yes\r\"; exp_continue}
\"*password*\" {send \"cmriots\r\";}
}
expect eof;"
