Config_Path="$HOME/.bashrc"
log=`cat $Config_Path`
if [[ $log =~ "lib_of_wangqingsong_C" ]]
then
    echo "envirement is already installed"
else
    echo "if [ -f \$HOME/lib_of_wangqingsong_C/build.sh ]" >> $Config_Path
    echo "then" >> $Config_Path
    echo "      source \$HOME/lib_of_wangqingsong_C/build.sh" >> $Config_Path
    echo "fi" >> $Config_Path
    echo "install success"
fi
ooo
