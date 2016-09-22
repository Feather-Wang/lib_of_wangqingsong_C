#!/bin/sh
# This is a shell archive (produced by GNU sharutils 4.7).
# To extract the files from this archive, save it to some FILE, remove
# everything before the `#!/bin/sh' line above, then type `sh FILE'.
#
lock_dir=_sh05658
# Made on 2015-04-13 16:08 CST by <root@mtp-develop>.
# Source directory was `/root/test/make_SoftPackage'.
#
# Existing files will *not* be overwritten, unless `-c' is specified.
#
# This shar contains:
# length mode       name
# ------ ---------- ------------------------------------------
#     55 -rw-r--r-- .topwalk_mtp/frontend/FrontEnd_20150413.sh.md5
#    729 -rwxr-xr-x .topwalk_mtp/frontend/frontend.sh
#  18427 -rwxr-xr-x .topwalk_mtp/frontend/FrontEnd_20150413.sh
#   3418 -rw-r--r-- .topwalk_mtp/frontend/conf/frontend.conf
#   6487 -rwxr-xr-x .topwalk_mtp/frontend/bin/frontend
#    721 -rwxr-xr-x .topwalk_mtp/backend/backend.sh
#  18357 -rwxr-xr-x .topwalk_mtp/backend/BackEnd_20150413.sh
#     54 -rw-r--r-- .topwalk_mtp/backend/BackEnd_20150413.sh.md5
#   3418 -rw-r--r-- .topwalk_mtp/backend/conf/backend.conf
#   6487 -rwxr-xr-x .topwalk_mtp/backend/bin/backend
#
MD5SUM=${MD5SUM-md5sum}
f=`${MD5SUM} --version | egrep '^md5sum .*(core|text)utils'`
test -n "${f}" && md5check=true || md5check=false
${md5check} || \
  echo 'Note: not verifying md5sums.  Consider installing GNU coreutils.'
save_IFS="${IFS}"
IFS="${IFS}:"
gettext_dir=FAILED
locale_dir=FAILED
first_param="$1"
for dir in $PATH
do
  if test "$gettext_dir" = FAILED && test -f $dir/gettext \
     && ($dir/gettext --version >/dev/null 2>&1)
  then
    case `$dir/gettext --version 2>&1 | sed 1q` in
      *GNU*) gettext_dir=$dir ;;
    esac
  fi
  if test "$locale_dir" = FAILED && test -f $dir/shar \
     && ($dir/shar --print-text-domain-dir >/dev/null 2>&1)
  then
    locale_dir=`$dir/shar --print-text-domain-dir`
  fi
done
IFS="$save_IFS"
if test "$locale_dir" = FAILED || test "$gettext_dir" = FAILED
then
  echo=echo
else
  TEXTDOMAINDIR=$locale_dir
  export TEXTDOMAINDIR
  TEXTDOMAIN=sharutils
  export TEXTDOMAIN
  echo="$gettext_dir/gettext -s"
fi
if (echo "testing\c"; echo 1,2,3) | grep c >/dev/null
then if (echo -n test; echo 1,2,3) | grep n >/dev/null
     then shar_n= shar_c='
'
     else shar_n=-n shar_c= ; fi
else shar_n= shar_c='\c' ; fi
f=shar-touch.$$
st1=200112312359.59
st2=123123592001.59
st2tr=123123592001.5 # old SysV 14-char limit
st3=1231235901

if touch -am -t ${st1} ${f} >/dev/null 2>&1 && \
   test ! -f ${st1} && test -f ${f}; then
  shar_touch='touch -am -t $1$2$3$4$5$6.$7 "$8"'

elif touch -am ${st2} ${f} >/dev/null 2>&1 && \
   test ! -f ${st2} && test ! -f ${st2tr} && test -f ${f}; then
  shar_touch='touch -am $3$4$5$6$1$2.$7 "$8"'

elif touch -am ${st3} ${f} >/dev/null 2>&1 && \
   test ! -f ${st3} && test -f ${f}; then
  shar_touch='touch -am $3$4$5$6$2 "$8"'

else
  shar_touch=:
  echo
  ${echo} 'WARNING: not restoring timestamps.  Consider getting and'
  ${echo} 'installing GNU `touch'\'', distributed in GNU coreutils...'
  echo
fi
rm -f ${st1} ${st2} ${st2tr} ${st3} ${f}
#
if test ! -d ${lock_dir}
then : ; else ${echo} 'lock directory '${lock_dir}' exists'
  exit 1
fi
if mkdir ${lock_dir}
then ${echo} 'x - created lock directory `'${lock_dir}\''.'
else ${echo} 'x - failed to create lock directory `'${lock_dir}\''.'
  exit 1
fi
# ============= .topwalk_mtp/frontend/FrontEnd_20150413.sh.md5 ==============
if test ! -d '.topwalk_mtp'; then
  mkdir '.topwalk_mtp'
if test $? -eq 0
then ${echo} 'x - created directory `.topwalk_mtp'\''.'
else ${echo} 'x - failed to create directory `.topwalk_mtp'\''.'
  exit 1
fi
fi
if test ! -d '.topwalk_mtp/frontend'; then
  mkdir '.topwalk_mtp/frontend'
if test $? -eq 0
then ${echo} 'x - created directory `.topwalk_mtp/frontend'\''.'
else ${echo} 'x - failed to create directory `.topwalk_mtp/frontend'\''.'
  exit 1
fi
fi
if test -f '.topwalk_mtp/frontend/FrontEnd_20150413.sh.md5' && test "$first_param" != -c; then
  ${echo} 'x -SKIPPING .topwalk_mtp/frontend/FrontEnd_20150413.sh.md5 (file already exists)'
else
${echo} 'x - extracting .topwalk_mtp/frontend/FrontEnd_20150413.sh.md5 (text)'
  sed 's/^X//' << 'SHAR_EOF' > '.topwalk_mtp/frontend/FrontEnd_20150413.sh.md5' &&
dcfcab7a35887a1b3ba83ddf60ffc47e  FrontEnd_20150413.sh
SHAR_EOF
  (set 20 15 04 13 15 59 25 '.topwalk_mtp/frontend/FrontEnd_20150413.sh.md5'; eval "$shar_touch") &&
  chmod 0644 '.topwalk_mtp/frontend/FrontEnd_20150413.sh.md5'
if test $? -ne 0
then ${echo} 'restore of .topwalk_mtp/frontend/FrontEnd_20150413.sh.md5 failed'
fi
  if ${md5check}
  then (
       ${MD5SUM} -c >/dev/null 2>&1 || ${echo} '.topwalk_mtp/frontend/FrontEnd_20150413.sh.md5: MD5 check failed'
       ) << \SHAR_EOF
c066341329cba69e4bcf355ce0f6b501  .topwalk_mtp/frontend/FrontEnd_20150413.sh.md5
SHAR_EOF
  else
test `LC_ALL=C wc -c < '.topwalk_mtp/frontend/FrontEnd_20150413.sh.md5'` -ne 55 && \
  ${echo} 'restoration warning:  size of .topwalk_mtp/frontend/FrontEnd_20150413.sh.md5 is not 55'
  fi
fi
# ============= .topwalk_mtp/frontend/frontend.sh ==============
if test ! -d '.topwalk_mtp/frontend'; then
  mkdir '.topwalk_mtp/frontend'
if test $? -eq 0
then ${echo} 'x - created directory `.topwalk_mtp/frontend'\''.'
else ${echo} 'x - failed to create directory `.topwalk_mtp/frontend'\''.'
  exit 1
fi
fi
if test -f '.topwalk_mtp/frontend/frontend.sh' && test "$first_param" != -c; then
  ${echo} 'x -SKIPPING .topwalk_mtp/frontend/frontend.sh (file already exists)'
else
${echo} 'x - extracting .topwalk_mtp/frontend/frontend.sh (binary)'
  sed 's/^X//' << 'SHAR_EOF' | uudecode &&
begin 600 .topwalk_mtp/frontend/frontend.sh
M(R$O8FEN+V)A<V@*(T)524Q$3D\]8&1A=&4@*R59)6TE9"5()4TE4V`*0E5)
M3$1.3SU@9&%T92`K)5DE;25D8`I04D]'4D%-/49R;VYT16YD7R1"54E,1$Y/
M+G-H"G)M("UR9B!&<F]N=$5N9%\J+G-H*@H*(^6(F^6[NN2XM.:7MN>;KN6]
ME0I435!?1$E2/2YF<F]N=&5N9%]T;7`*<FT@+69R("1435!?1$E2"FUK9&ER
M("UP("1435!?1$E2"@HC9G)O;G1E;F0@<&%C:V%G90IC<"`M9'!29B!B:6X@
M)%1-4%]$25(O"F-P("UD<%)F(&-O;F8@)%1-4%]$25(O"@IF:6YD("1435!?
M1$E2("UT>7!E(&0@+6YA;64@+G-V;GQX87)G<R!R;2`M<F8*"FUK9&ER("UP
M(%]I;G-T86QL"G-H87(@)%1-4%]$25(@/B`D4%)/1U)!30IS960@+6D@)R1D
M)R`D4%)/1U)!30H*8V%T(#X^("104D]'4D%-(#P\($5/1@I435!?1$E2/2YF
M<F]N=&5N9%]T;7`*"F-L96%R"FUK9&ER("UP("]W86YG<6EN9W-O;F<O=&]P
M87!P+W1O<&9R;VYT+PIC<"`M9'!29B!<)%1-4%]$25(O8FEN+RH@+W=A;F=Q
M:6YG<V]N9R]T;W!A<'`O=&]P9G)O;G0O"FUK9&ER("UP("]W86YG<6EN9W-O
M;F<O=&]P8V]N9B]T;W!F<F]N="\*8W`@+61P4F8@7"1435!?1$E2+V-O;F8O
M*B`O=V%N9W%I;F=S;VYG+W1O<&-O;F8O=&]P9G)O;G0O"@IL9&-O;F9I9PIS
M>6YC"@IR;2`M<F8@7"1435!?1$E2"F5X:70@,`I%3T8*<FT@+69R("1435!?
M1$E2"FUD-7-U;2`D4%)/1U)!32`^("104D]'4D%-+FUD-0IC:&UO9"`W-34@
))%!23T=204T*
`
end
SHAR_EOF
  (set 20 15 04 13 15 58 39 '.topwalk_mtp/frontend/frontend.sh'; eval "$shar_touch") &&
  chmod 0755 '.topwalk_mtp/frontend/frontend.sh'
if test $? -ne 0
then ${echo} 'restore of .topwalk_mtp/frontend/frontend.sh failed'
fi
  if ${md5check}
  then (
       ${MD5SUM} -c >/dev/null 2>&1 || ${echo} '.topwalk_mtp/frontend/frontend.sh: MD5 check failed'
       ) << \SHAR_EOF
c64cf8bcd341eb6b067f5482ad2c521b  .topwalk_mtp/frontend/frontend.sh
SHAR_EOF
  else
test `LC_ALL=C wc -c < '.topwalk_mtp/frontend/frontend.sh'` -ne 729 && \
  ${echo} 'restoration warning:  size of .topwalk_mtp/frontend/frontend.sh is not 729'
  fi
fi
# ============= .topwalk_mtp/frontend/FrontEnd_20150413.sh ==============
if test -f '.topwalk_mtp/frontend/FrontEnd_20150413.sh' && test "$first_param" != -c; then
  ${echo} 'x -SKIPPING .topwalk_mtp/frontend/FrontEnd_20150413.sh (file already exists)'
else
${echo} 'x - extracting .topwalk_mtp/frontend/FrontEnd_20150413.sh (text)'
  sed 's/^X//' << 'SHAR_EOF' > '.topwalk_mtp/frontend/FrontEnd_20150413.sh' &&
#!/bin/sh
# This is a shell archive (produced by GNU sharutils 4.7).
# To extract the files from this archive, save it to some FILE, remove
# everything before the `#!/bin/sh' line above, then type `sh FILE'.
#
lock_dir=_sh05546
# Made on 2015-04-13 15:59 CST by <root@mtp-develop>.
# Source directory was `/root/test/make_SoftPackage/frontend'.
#
# Existing files will *not* be overwritten, unless `-c' is specified.
#
# This shar contains:
# length mode       name
# ------ ---------- ------------------------------------------
#   3418 -rw-r--r-- .frontend_tmp/conf/frontend.conf
#   6487 -rwxr-xr-x .frontend_tmp/bin/frontend
#
MD5SUM=${MD5SUM-md5sum}
f=`${MD5SUM} --version | egrep '^md5sum .*(core|text)utils'`
test -n "${f}" && md5check=true || md5check=false
${md5check} || \
X  echo 'Note: not verifying md5sums.  Consider installing GNU coreutils.'
save_IFS="${IFS}"
IFS="${IFS}:"
gettext_dir=FAILED
locale_dir=FAILED
first_param="$1"
for dir in $PATH
do
X  if test "$gettext_dir" = FAILED && test -f $dir/gettext \
X     && ($dir/gettext --version >/dev/null 2>&1)
X  then
X    case `$dir/gettext --version 2>&1 | sed 1q` in
X      *GNU*) gettext_dir=$dir ;;
X    esac
X  fi
X  if test "$locale_dir" = FAILED && test -f $dir/shar \
X     && ($dir/shar --print-text-domain-dir >/dev/null 2>&1)
X  then
X    locale_dir=`$dir/shar --print-text-domain-dir`
X  fi
done
IFS="$save_IFS"
if test "$locale_dir" = FAILED || test "$gettext_dir" = FAILED
then
X  echo=echo
else
X  TEXTDOMAINDIR=$locale_dir
X  export TEXTDOMAINDIR
X  TEXTDOMAIN=sharutils
X  export TEXTDOMAIN
X  echo="$gettext_dir/gettext -s"
fi
if (echo "testing\c"; echo 1,2,3) | grep c >/dev/null
then if (echo -n test; echo 1,2,3) | grep n >/dev/null
X     then shar_n= shar_c='
'
X     else shar_n=-n shar_c= ; fi
else shar_n= shar_c='\c' ; fi
f=shar-touch.$$
st1=200112312359.59
st2=123123592001.59
st2tr=123123592001.5 # old SysV 14-char limit
st3=1231235901
X
if touch -am -t ${st1} ${f} >/dev/null 2>&1 && \
X   test ! -f ${st1} && test -f ${f}; then
X  shar_touch='touch -am -t $1$2$3$4$5$6.$7 "$8"'
X
elif touch -am ${st2} ${f} >/dev/null 2>&1 && \
X   test ! -f ${st2} && test ! -f ${st2tr} && test -f ${f}; then
X  shar_touch='touch -am $3$4$5$6$1$2.$7 "$8"'
X
elif touch -am ${st3} ${f} >/dev/null 2>&1 && \
X   test ! -f ${st3} && test -f ${f}; then
X  shar_touch='touch -am $3$4$5$6$2 "$8"'
X
else
X  shar_touch=:
X  echo
X  ${echo} 'WARNING: not restoring timestamps.  Consider getting and'
X  ${echo} 'installing GNU `touch'\'', distributed in GNU coreutils...'
X  echo
fi
rm -f ${st1} ${st2} ${st2tr} ${st3} ${f}
#
if test ! -d ${lock_dir}
then : ; else ${echo} 'lock directory '${lock_dir}' exists'
X  exit 1
fi
if mkdir ${lock_dir}
then ${echo} 'x - created lock directory `'${lock_dir}\''.'
else ${echo} 'x - failed to create lock directory `'${lock_dir}\''.'
X  exit 1
fi
# ============= .frontend_tmp/conf/frontend.conf ==============
if test ! -d '.frontend_tmp'; then
X  mkdir '.frontend_tmp'
if test $? -eq 0
then ${echo} 'x - created directory `.frontend_tmp'\''.'
else ${echo} 'x - failed to create directory `.frontend_tmp'\''.'
X  exit 1
fi
fi
if test ! -d '.frontend_tmp/conf'; then
X  mkdir '.frontend_tmp/conf'
if test $? -eq 0
then ${echo} 'x - created directory `.frontend_tmp/conf'\''.'
else ${echo} 'x - failed to create directory `.frontend_tmp/conf'\''.'
X  exit 1
fi
fi
if test -f '.frontend_tmp/conf/frontend.conf' && test "$first_param" != -c; then
X  ${echo} 'x -SKIPPING .frontend_tmp/conf/frontend.conf (file already exists)'
else
${echo} 'x - extracting .frontend_tmp/conf/frontend.conf (text)'
X  sed 's/^X//' << 'SHAR_EOF' > '.frontend_tmp/conf/frontend.conf' &&
ip_list : 
{
XX  ip_list_1 : 
XX  {
XX    tmsip = "10.125.122.67";
XX    videoip = "172.27.16.1";
XX  };
XX  ip_list_2 : 
XX  {
XX    tmsip = "10.125.122.68";
XX    videoip = "172.23.32.1";
XX  };
XX  ip_list_3 : 
XX  {
XX    tmsip = "10.125.122.69";
XX    videoip = "172.21.32.1";
XX  };
XX  ip_list_4 : 
XX  {
XX    tmsip = "10.125.122.70";
XX    videoip = "172.22.72.1";
XX  };
XX  ip_list_5 : 
XX  {
XX    tmsip = "10.125.122.71";
XX    videoip = "172.24.48.1";
XX  };
XX  ip_list_6 : 
XX  {
XX    tmsip = "10.125.122.72";
XX    videoip = "172.24.16.1";
XX  };
XX  ip_list_7 : 
XX  {
XX    tmsip = "10.125.122.73";
XX    videoip = "172.21.48.1";
XX  };
XX  ip_list_8 : 
XX  {
XX    tmsip = "10.125.122.74";
XX    videoip = "172.23.56.1";
XX  };
XX  ip_list_9 : 
XX  {
XX    tmsip = "10.125.122.75";
XX    videoip = "172.21.72.1";
XX  };
XX  ip_list_10 : 
XX  {
XX    tmsip = "10.125.122.76";
XX    videoip = "172.22.64.1";
XX  };
XX  ip_list_11 : 
XX  {
XX    tmsip = "10.125.122.77";
XX    videoip = "172.24.24.1";
XX  };
XX  ip_list_12 : 
XX  {
XX    tmsip = "10.125.122.78";
XX    videoip = "172.23.48.1";
XX  };
XX  ip_list_13 : 
XX  {
XX    tmsip = "10.125.122.79";
XX    videoip = "172.27.24.1";
XX  };
XX  ip_list_14 : 
XX  {
XX    tmsip = "10.125.122.80";
XX    videoip = "172.21.16.1";
XX  };
XX  ip_list_15 : 
XX  {
XX    tmsip = "10.125.122.81";
XX    videoip = "172.25.32.1";
XX  };
XX  ip_list_16 : 
XX  {
XX    tmsip = "10.125.122.82";
XX    videoip = "172.21.56.1";
XX  };
XX  ip_list_17 : 
XX  {
XX    tmsip = "10.125.122.83";
XX    videoip = "172.24.56.1";
XX  };
XX  ip_list_18 : 
XX  {
XX    tmsip = "10.125.122.84";
XX    videoip = "172.22.32.1";
XX  };
XX  ip_list_19 : 
XX  {
XX    tmsip = "10.125.122.85";
XX    videoip = "172.24.32.1";
XX  };
XX  ip_list_20 : 
XX  {
XX    tmsip = "10.125.122.86";
XX    videoip = "172.26.24.1";
XX  };
XX  ip_list_21 : 
XX  {
XX    tmsip = "10.125.122.87";
XX    videoip = "172.21.40.1";
XX  };
XX  ip_list_22 : 
XX  {
XX    tmsip = "10.125.122.88";
XX    videoip = "172.22.16.1";
XX  };
XX  ip_list_23 : 
XX  {
XX    tmsip = "10.125.122.89";
XX    videoip = "172.23.72.1";
XX  };
XX  ip_list_24 : 
XX  {
XX    tmsip = "10.125.122.90";
XX    videoip = "172.22.40.1";
XX  };
XX  ip_list_25 : 
XX  {
XX    tmsip = "10.125.122.91";
XX    videoip = "172.22.24.1";
XX  };
XX  ip_list_26 : 
XX  {
XX    tmsip = "10.125.122.92";
XX    videoip = "172.21.88.1";
XX  };
XX  ip_list_27 : 
XX  {
XX    tmsip = "10.125.122.93";
XX    videoip = "172.23.64.1";
XX  };
XX  ip_list_28 : 
XX  {
XX    tmsip = "10.125.122.94";
XX    videoip = "172.21.64.1";
XX  };
XX  ip_list_29 : 
XX  {
XX    tmsip = "10.125.122.95";
XX    videoip = "172.23.24.1";
XX  };
XX  ip_list_30 : 
XX  {
XX    tmsip = "10.125.122.96";
XX    videoip = "172.21.80.1";
XX  };
XX  ip_list_31 : 
XX  {
XX    tmsip = "10.125.122.97";
XX    videoip = "172.24.64.1";
XX  };
XX  ip_list_32 : 
XX  {
XX    tmsip = "10.125.122.98";
XX    videoip = "172.26.16.1";
XX  };
XX  ip_list_33 : 
XX  {
XX    tmsip = "10.125.122.99";
XX    videoip = "172.22.56.1";
XX  };
XX  ip_list_34 : 
XX  {
XX    tmsip = "10.125.122.100";
XX    videoip = "172.21.24.1";
XX  };
XX  ip_list_35 : 
XX  {
XX    tmsip = "10.125.122.101";
XX    videoip = "172.22.48.1";
XX  };
XX  ip_list_36 : 
XX  {
XX    tmsip = "10.125.122.102";
XX    videoip = "172.24.40.1";
XX  };
XX  ip_list_37 : 
XX  {
XX    tmsip = "10.125.122.103";
XX    videoip = "172.23.16.1";
XX  };
XX  ip_list_38 : 
XX  {
XX    tmsip = "10.125.122.104";
XX    videoip = "172.27.32.1";
XX  };
XX  ip_list_39 : 
XX  {
XX    tmsip = "10.125.122.105";
XX    videoip = "172.23.40.1";
XX  };
XX  ip_list_40 : 
XX  {
XX    tmsip = "10.125.122.106";
XX    videoip = "172.25.16.1";
XX  };
XX  ip_list_41 : 
XX  {
XX    tmsip = "10.125.122.107";
XX    videoip = "172.25.24.1";
XX  };
};
XSHAR_EOF
X  (set 20 15 04 13 15 46 48 '.frontend_tmp/conf/frontend.conf'; eval "$shar_touch") &&
X  chmod 0644 '.frontend_tmp/conf/frontend.conf'
if test $? -ne 0
then ${echo} 'restore of .frontend_tmp/conf/frontend.conf failed'
fi
X  if ${md5check}
X  then (
X       ${MD5SUM} -c >/dev/null 2>&1 || ${echo} '.frontend_tmp/conf/frontend.conf: MD5 check failed'
X       ) << \SHAR_EOF
c2a06b447f14124949ec66b6ed78692a  .frontend_tmp/conf/frontend.conf
XSHAR_EOF
X  else
test `LC_ALL=C wc -c < '.frontend_tmp/conf/frontend.conf'` -ne 3418 && \
X  ${echo} 'restoration warning:  size of .frontend_tmp/conf/frontend.conf is not 3418'
X  fi
fi
# ============= .frontend_tmp/bin/frontend ==============
if test ! -d '.frontend_tmp/bin'; then
X  mkdir '.frontend_tmp/bin'
if test $? -eq 0
then ${echo} 'x - created directory `.frontend_tmp/bin'\''.'
else ${echo} 'x - failed to create directory `.frontend_tmp/bin'\''.'
X  exit 1
fi
fi
if test -f '.frontend_tmp/bin/frontend' && test "$first_param" != -c; then
X  ${echo} 'x -SKIPPING .frontend_tmp/bin/frontend (file already exists)'
else
${echo} 'x - extracting .frontend_tmp/bin/frontend (binary)'
X  sed 's/^X//' << 'SHAR_EOF' | uudecode &&
begin 600 .frontend_tmp/bin/frontend
M?T5,1@(!`0````````````(`/@`!````X`-```````!``````````.@)````
M`````````$``.``(`$``'@`;``8````%````0`````````!``$```````$``
M0```````P`$```````#``0````````@``````````P````0``````@``````
M```"0`````````)````````<`````````!P``````````0`````````!````
M!0````````````````!``````````$```````,P&````````S`8`````````
M`"````````$````&````T`8```````#0!F```````-`&8```````[`$`````
M`````@``````````(````````@````8```#X!@```````/@&8```````^`9@
M``````"0`0```````)`!````````"``````````$````!````!P"````````
M'`)````````<`D```````$0`````````1``````````$`````````%#E=&0$
M````+`8````````L!D```````"P&0```````)``````````D``````````0`
M````````4>5T9`8`````````````````````````````````````````````
M````````````"``````````O;&EB-C0O;&0M;&EN=7@M>#@V+38T+G-O+C(`
M!````!`````!````1TY5```````"````!@```!(````$````%`````,```!'
M3E4`:G+&A0Q`CAK.3[%!MOZP1QRA&,0!`````0````$`````````````````
M`````````````````````````````````````````````!H````2````````
M``````````````````$````@`````````````````````````"$````2````
M``````````````````````!?7V=M;VY?<W1A<G1?7P!L:6)C+G-O+C8`<')I
M;G1F`%]?;&EB8U]S=&%R=%]M86EN`$=,24)#7S(N,BXU``````(````"``$`
M`0`0````$`````````!U&FD)```"`#,`````````B`A@```````&`````@``
M````````````J`A@```````'`````0``````````````L`A@```````'````
M`P``````````````2(/L".AS````Z`(!``#H'0(``$B#Q`C#_S7J!"``_R7L
M!"``#Q]``/\EZ@0@`&@`````Z>#_____)>($(`!H`0```.G0____````````
M```Q[4F)T5Y(B>)(@^3P4%1)Q\`@!4``2,?!,`5``$C'Q\0$0`#HO_____20
MD$B#[`A(BP5Q!"``2(7`=`+_T$B#Q`C#D)"0D)"0D)"0D)"0D%5(B>532(/L
M"(`]@`0@``!U2[OH!F``2(L%>@0@`$B!Z^`&8`!(P?L#2(/K`4@YV',D9@\?
M1```2(/``4B)!54$(`#_%,7@!F``2(L%1P0@`$@YV'+BQ@4S!"```4B#Q`A;
MR<-F9F8N#Q^$``````!(@SU(`B```%5(B>5T$K@`````2(7`=`B_\`9@`,G_
MX,G#D)!52(GE2(/L((E][$B)=>!(C47P2(E%^+@8!D``2(U5\$B#P@)(B=9(
MB<>X`````.C`_O__2(M%^$B-4`*X(@9``$B)UDB)Q[@`````Z*/^__^X````
M`,G#D)"0D//#9F9F9F8N#Q^$``````!(B6PDV$R)9"3@2(TMBP$@`$R-)80!
M(`!,B6PDZ$R)="3P3(E\)/A(B5PDT$B#[#A,*>5!B?U)B?9(P?T#28G7Z!O^
M__](A>UT'#';#Q]``$R)^DR)]D2)[T'_%-Q(@\,!2#GK<NI(BUPD"$B+;"00
M3(MD)!A,BVPD($R+="0H3(M\)#!(@\0XPY"0D)"0D)!52(GE4TB#[`A(BP4`
M`2``2(/X_W09N]`&8``/'T0``$B#ZPC_T$B+`TB#^/]U\4B#Q`A;R<.0D$B#
M[`CH+_[__TB#Q`C#```!``(`````````````````<W1R(#T@)60*`'`@/2`E
M9`H````!&P,[(`````,```"8_O__/````/3^__]<````!/___W0````4````
M``````%Z4@`!>!`!&PP'")`!```<````'````%3^__]8`````$$.$(8"0PT&
M`E,,!P@``!0````\````D/[__P(``````````````"0```!4````B/[__XD`
M````48P%A@9?#D"#!X\"C@.-!`)8#@@``````````````/__________````
M``````#__________P`````````````````````!`````````!``````````
M#`````````"0`T````````T`````````^`5```````#U_O]O`````&`"0```
M````!0````````#@`D````````8`````````@`)````````*`````````#\`
M````````"P`````````8`````````!4````````````````````#````````
M`)`(8````````@`````````P`````````!0`````````!P`````````7````
M`````&`#0```````!P````````!(`T````````@`````````&``````````)
M`````````!@`````````_O__;P`````H`T```````/___V\``````0``````
M``#P__]O`````"`#0```````````````````````````````````````````
M````````````````````````````````````````````````````````````
M``````````````````````````````````````````#X!F``````````````
M``````````````"^`T```````,X#0````````````$=#0SH@*$=.52D@-"XT
M+C<@,C`Q,C`S,3,@*%)E9"!(870@-"XT+C<M-"D``"YS>6UT86(`+G-T<G1A
M8@`N<VAS=')T86(`+FEN=&5R<``N;F]T92Y!0DDM=&%G`"YN;W1E+F=N=2YB
M=6EL9"UI9``N9VYU+FAA<V@`+F1Y;G-Y;0`N9'EN<W1R`"YG;G4N=F5R<VEO
M;@`N9VYU+G9E<G-I;VY?<@`N<F5L82YD>6X`+G)E;&$N<&QT`"YI;FET`"YT
M97AT`"YF:6YI`"YR;V1A=&$`+F5H7V9R86UE7VAD<@`N96A?9G)A;64`+F-T
M;W)S`"YD=&]R<P`N:F-R`"YD>6YA;6EC`"YG;W0`+F=O="YP;'0`+F1A=&$`
M+F)S<P`N8V]M;65N=```````````````````````````````````````````
M```````````````````````````````````````````````;`````0````(`
M``````````)``````````@```````!P````````````````````!````````
M````````````(P````<````"`````````!P"0```````'`(````````@````
M````````````````!````````````````````#$````'`````@`````````\
M`D```````#P"````````)`````````````````````0`````````````````
M``!$````]O__;P(`````````8`)```````!@`@```````!P`````````!0``
M```````(````````````````````3@````L````"`````````(`"0```````
M@`(```````!@``````````8````!````"``````````8`````````%8````#
M`````@````````#@`D```````.`"````````/P````````````````````$`
M``````````````````!>````____;P(`````````(`-````````@`P``````
M``@`````````!0`````````"``````````(`````````:P```/[__V\"````
M`````"@#0```````*`,````````@``````````8````!````"```````````
M`````````'H````$`````@````````!(`T```````$@#````````&```````
M```%``````````@`````````&`````````"$````!`````(`````````8`-`
M``````!@`P```````#``````````!0````P````(`````````!@`````````
MC@````$````&`````````)`#0```````D`,````````8````````````````
M````!````````````````````(D````!````!@````````"H`T```````*@#
M````````,`````````````````````0`````````$`````````"4`````0``
M``8`````````X`-```````#@`P```````!@"```````````````````0````
M````````````````F@````$````&`````````/@%0```````^`4````````.
M````````````````````!````````````````````*`````!`````@``````
M```(!D````````@&````````(@````````````````````@`````````````
M``````"H`````0````(`````````+`9````````L!@```````"0`````````
M```````````$````````````````````M@````$````"`````````%`&0```
M````4`8```````!\````````````````````"````````````````````,``
M```!`````P````````#0!F```````-`&````````$```````````````````
M``@```````````````````#'`````0````,`````````X`9@``````#@!@``
M`````!`````````````````````(````````````````````S@````$````#
M`````````/`&8```````\`8````````(````````````````````"```````
M`````````````-,````&`````P````````#X!F```````/@&````````D`$`
M```````&``````````@`````````$`````````#<`````0````,`````````
MB`A@``````"("`````````@````````````````````(``````````@`````
M````X0````$````#`````````)`(8```````D`@````````H````````````
M````````"``````````(`````````.H````!`````P````````"X"&``````
M`+@(````````!`````````````````````0```````````````````#P````
M"`````,`````````P`A@``````"\"````````!`````````````````````(
M````````````````````]0````$````P````````````````````O`@`````
M```L`````````````````````0`````````!`````````!$````#````````
M`````````````````.@(````````_@````````````````````$`````````
M```````````!`````@````````````````````````!H$0`````````&````
M````'0```"X````(`````````!@`````````"0````,`````````````````
M````````:!<```````#O`0```````````````````0``````````````````
M```````````````````````````````````````#``$```)`````````````
M```````````#``(`'`)````````````````````````#``,`/`)`````````
M```````````````#``0`8`)````````````````````````#``4`@`)`````
M```````````````````#``8`X`)````````````````````````#``<`(`-`
M```````````````````````#``@`*`-````````````````````````#``D`
M2`-````````````````````````#``H`8`-````````````````````````#
M``L`D`-````````````````````````#``P`J`-`````````````````````
M```#``T`X`-````````````````````````#``X`^`5`````````````````
M```````#``\`"`9````````````````````````#`!``+`9`````````````
M```````````#`!$`4`9````````````````````````#`!(`T`9@````````
M```````````````#`!,`X`9@```````````````````````#`!0`\`9@````
M```````````````````#`!4`^`9@```````````````````````#`!8`B`A@
M```````````````````````#`!<`D`A@```````````````````````#`!@`
MN`A@```````````````````````#`!D`P`A@```````````````````````#
M`!H```````````````````````$````"``T`#`1``````````````````!$`
M```$`/'_`````````````````````!P````!`!(`T`9@````````````````
M`"H````!`!,`X`9@`````````````````#@````!`!0`\`9@````````````
M`````$4````"``T`,`1``````````````````%L````!`!D`P`A@```````!
M`````````&H````!`!D`R`A@```````(`````````'@````"``T`H`1`````
M`````````````!$````$`/'_`````````````````````(0````!`!(`V`9@
M`````````````````)$````!`!$`R`9``````````````````)\````!`!0`
M\`9@`````````````````*L````"``T`P`5``````````````````,$````$
M`/'_`````````````````````,4````!`!<`D`A@`````````````````-L`
M`````!(`S`9@`````````````````.P``````!(`S`9@````````````````
M`/\````!`!4`^`9@``````````````````@!```@`!@`N`A@````````````
M`````!,!```2`````````````````````````"<!```2``T`(`5````````"
M`````````#<!```2``T`X`-``````````````````#X!```@````````````
M`````````````$T!```@`````````````````````````&$!```2``X`^`5`
M`````````````````&<!```2`````````````````````````(8!```1``\`
M"`9````````$`````````)4!```0`!@`N`A@`````````````````*(!```1
M`@\`$`9``````````````````*\!```1`A,`Z`9@`````````````````+P!
M```2``T`,`5```````")`````````,P!```0`/'_O`A@````````````````
M`-@!```0`/'_T`A@`````````````````-T!```0`/'_O`A@````````````
M`````.0!```2``T`Q`1```````!8`````````.D!```2``L`D`-`````````
M``````````!C86QL7V=M;VY?<W1A<G0`8W)T<W1U9F8N8P!?7T-43U)?3$E3
M5%]?`%]?1%1/4E],25-47U\`7U]*0U)?3$E35%]?`%]?9&]?9VQO8F%L7V1T
M;W)S7V%U>`!C;VUP;&5T960N-C,T.0!D=&]R7VED>"XV,S4Q`&9R86UE7V1U
M;6UY`%]?0U1/4E]%3D1?7P!?7T9204U%7T5.1%]?`%]?2D-27T5.1%]?`%]?
M9&]?9VQO8F%L7V-T;W)S7V%U>`!A+F,`7T=,3T)!3%]/1D93151?5$%"3$5?
M`%]?:6YI=%]A<G)A>5]E;F0`7U]I;FET7V%R<F%Y7W-T87)T`%]$64Y!34E#
M`&1A=&%?<W1A<G0`<')I;G1F0$!'3$E"0U\R+C(N-0!?7VQI8F-?8W-U7V9I
M;FD`7W-T87)T`%]?9VUO;E]S=&%R=%]?`%]*=E]296=I<W1E<D-L87-S97,`
M7V9I;FD`7U]L:6)C7W-T87)T7VUA:6Y`0$=,24)#7S(N,BXU`%])3U]S=&1I
M;E]U<V5D`%]?9&%T85]S=&%R=`!?7V1S;U]H86YD;&4`7U]$5$]27T5.1%]?
M`%]?;&EB8U]C<W5?:6YI=`!?7V)S<U]S=&%R=`!?96YD`%]E9&%T80!M86EN
'`%]I;FET`%]C
`
end
XSHAR_EOF
X  (set 20 15 04 13 15 45 49 '.frontend_tmp/bin/frontend'; eval "$shar_touch") &&
X  chmod 0755 '.frontend_tmp/bin/frontend'
if test $? -ne 0
then ${echo} 'restore of .frontend_tmp/bin/frontend failed'
fi
X  if ${md5check}
X  then (
X       ${MD5SUM} -c >/dev/null 2>&1 || ${echo} '.frontend_tmp/bin/frontend: MD5 check failed'
X       ) << \SHAR_EOF
162c7197a0a1e7cdcaf93af94e6a670d  .frontend_tmp/bin/frontend
XSHAR_EOF
X  else
test `LC_ALL=C wc -c < '.frontend_tmp/bin/frontend'` -ne 6487 && \
X  ${echo} 'restoration warning:  size of .frontend_tmp/bin/frontend is not 6487'
X  fi
fi
if rm -fr ${lock_dir}
then ${echo} 'x - removed lock directory `'${lock_dir}\''.'
else ${echo} 'x - failed to remove lock directory `'${lock_dir}\''.'
X  exit 1
fi
TMP_DIR=.frontend_tmp
X
clear
mkdir -p /wangqingsong/topapp/topfront/
cp -dpRf $TMP_DIR/bin/* /wangqingsong/topapp/topfront/
mkdir -p /wangqingsong/topconf/topfront/
cp -dpRf $TMP_DIR/conf/* /wangqingsong/topconf/topfront/
X
ldconfig
sync
X
rm -rf $TMP_DIR
Xexit 0
SHAR_EOF
  (set 20 15 04 13 15 59 25 '.topwalk_mtp/frontend/FrontEnd_20150413.sh'; eval "$shar_touch") &&
  chmod 0755 '.topwalk_mtp/frontend/FrontEnd_20150413.sh'
if test $? -ne 0
then ${echo} 'restore of .topwalk_mtp/frontend/FrontEnd_20150413.sh failed'
fi
  if ${md5check}
  then (
       ${MD5SUM} -c >/dev/null 2>&1 || ${echo} '.topwalk_mtp/frontend/FrontEnd_20150413.sh: MD5 check failed'
       ) << \SHAR_EOF
dcfcab7a35887a1b3ba83ddf60ffc47e  .topwalk_mtp/frontend/FrontEnd_20150413.sh
SHAR_EOF
  else
test `LC_ALL=C wc -c < '.topwalk_mtp/frontend/FrontEnd_20150413.sh'` -ne 18427 && \
  ${echo} 'restoration warning:  size of .topwalk_mtp/frontend/FrontEnd_20150413.sh is not 18427'
  fi
fi
# ============= .topwalk_mtp/frontend/conf/frontend.conf ==============
if test ! -d '.topwalk_mtp/frontend/conf'; then
  mkdir '.topwalk_mtp/frontend/conf'
if test $? -eq 0
then ${echo} 'x - created directory `.topwalk_mtp/frontend/conf'\''.'
else ${echo} 'x - failed to create directory `.topwalk_mtp/frontend/conf'\''.'
  exit 1
fi
fi
if test -f '.topwalk_mtp/frontend/conf/frontend.conf' && test "$first_param" != -c; then
  ${echo} 'x -SKIPPING .topwalk_mtp/frontend/conf/frontend.conf (file already exists)'
else
${echo} 'x - extracting .topwalk_mtp/frontend/conf/frontend.conf (text)'
  sed 's/^X//' << 'SHAR_EOF' > '.topwalk_mtp/frontend/conf/frontend.conf' &&
ip_list : 
{
X  ip_list_1 : 
X  {
X    tmsip = "10.125.122.67";
X    videoip = "172.27.16.1";
X  };
X  ip_list_2 : 
X  {
X    tmsip = "10.125.122.68";
X    videoip = "172.23.32.1";
X  };
X  ip_list_3 : 
X  {
X    tmsip = "10.125.122.69";
X    videoip = "172.21.32.1";
X  };
X  ip_list_4 : 
X  {
X    tmsip = "10.125.122.70";
X    videoip = "172.22.72.1";
X  };
X  ip_list_5 : 
X  {
X    tmsip = "10.125.122.71";
X    videoip = "172.24.48.1";
X  };
X  ip_list_6 : 
X  {
X    tmsip = "10.125.122.72";
X    videoip = "172.24.16.1";
X  };
X  ip_list_7 : 
X  {
X    tmsip = "10.125.122.73";
X    videoip = "172.21.48.1";
X  };
X  ip_list_8 : 
X  {
X    tmsip = "10.125.122.74";
X    videoip = "172.23.56.1";
X  };
X  ip_list_9 : 
X  {
X    tmsip = "10.125.122.75";
X    videoip = "172.21.72.1";
X  };
X  ip_list_10 : 
X  {
X    tmsip = "10.125.122.76";
X    videoip = "172.22.64.1";
X  };
X  ip_list_11 : 
X  {
X    tmsip = "10.125.122.77";
X    videoip = "172.24.24.1";
X  };
X  ip_list_12 : 
X  {
X    tmsip = "10.125.122.78";
X    videoip = "172.23.48.1";
X  };
X  ip_list_13 : 
X  {
X    tmsip = "10.125.122.79";
X    videoip = "172.27.24.1";
X  };
X  ip_list_14 : 
X  {
X    tmsip = "10.125.122.80";
X    videoip = "172.21.16.1";
X  };
X  ip_list_15 : 
X  {
X    tmsip = "10.125.122.81";
X    videoip = "172.25.32.1";
X  };
X  ip_list_16 : 
X  {
X    tmsip = "10.125.122.82";
X    videoip = "172.21.56.1";
X  };
X  ip_list_17 : 
X  {
X    tmsip = "10.125.122.83";
X    videoip = "172.24.56.1";
X  };
X  ip_list_18 : 
X  {
X    tmsip = "10.125.122.84";
X    videoip = "172.22.32.1";
X  };
X  ip_list_19 : 
X  {
X    tmsip = "10.125.122.85";
X    videoip = "172.24.32.1";
X  };
X  ip_list_20 : 
X  {
X    tmsip = "10.125.122.86";
X    videoip = "172.26.24.1";
X  };
X  ip_list_21 : 
X  {
X    tmsip = "10.125.122.87";
X    videoip = "172.21.40.1";
X  };
X  ip_list_22 : 
X  {
X    tmsip = "10.125.122.88";
X    videoip = "172.22.16.1";
X  };
X  ip_list_23 : 
X  {
X    tmsip = "10.125.122.89";
X    videoip = "172.23.72.1";
X  };
X  ip_list_24 : 
X  {
X    tmsip = "10.125.122.90";
X    videoip = "172.22.40.1";
X  };
X  ip_list_25 : 
X  {
X    tmsip = "10.125.122.91";
X    videoip = "172.22.24.1";
X  };
X  ip_list_26 : 
X  {
X    tmsip = "10.125.122.92";
X    videoip = "172.21.88.1";
X  };
X  ip_list_27 : 
X  {
X    tmsip = "10.125.122.93";
X    videoip = "172.23.64.1";
X  };
X  ip_list_28 : 
X  {
X    tmsip = "10.125.122.94";
X    videoip = "172.21.64.1";
X  };
X  ip_list_29 : 
X  {
X    tmsip = "10.125.122.95";
X    videoip = "172.23.24.1";
X  };
X  ip_list_30 : 
X  {
X    tmsip = "10.125.122.96";
X    videoip = "172.21.80.1";
X  };
X  ip_list_31 : 
X  {
X    tmsip = "10.125.122.97";
X    videoip = "172.24.64.1";
X  };
X  ip_list_32 : 
X  {
X    tmsip = "10.125.122.98";
X    videoip = "172.26.16.1";
X  };
X  ip_list_33 : 
X  {
X    tmsip = "10.125.122.99";
X    videoip = "172.22.56.1";
X  };
X  ip_list_34 : 
X  {
X    tmsip = "10.125.122.100";
X    videoip = "172.21.24.1";
X  };
X  ip_list_35 : 
X  {
X    tmsip = "10.125.122.101";
X    videoip = "172.22.48.1";
X  };
X  ip_list_36 : 
X  {
X    tmsip = "10.125.122.102";
X    videoip = "172.24.40.1";
X  };
X  ip_list_37 : 
X  {
X    tmsip = "10.125.122.103";
X    videoip = "172.23.16.1";
X  };
X  ip_list_38 : 
X  {
X    tmsip = "10.125.122.104";
X    videoip = "172.27.32.1";
X  };
X  ip_list_39 : 
X  {
X    tmsip = "10.125.122.105";
X    videoip = "172.23.40.1";
X  };
X  ip_list_40 : 
X  {
X    tmsip = "10.125.122.106";
X    videoip = "172.25.16.1";
X  };
X  ip_list_41 : 
X  {
X    tmsip = "10.125.122.107";
X    videoip = "172.25.24.1";
X  };
};
SHAR_EOF
  (set 20 15 04 13 15 46 48 '.topwalk_mtp/frontend/conf/frontend.conf'; eval "$shar_touch") &&
  chmod 0644 '.topwalk_mtp/frontend/conf/frontend.conf'
if test $? -ne 0
then ${echo} 'restore of .topwalk_mtp/frontend/conf/frontend.conf failed'
fi
  if ${md5check}
  then (
       ${MD5SUM} -c >/dev/null 2>&1 || ${echo} '.topwalk_mtp/frontend/conf/frontend.conf: MD5 check failed'
       ) << \SHAR_EOF
c2a06b447f14124949ec66b6ed78692a  .topwalk_mtp/frontend/conf/frontend.conf
SHAR_EOF
  else
test `LC_ALL=C wc -c < '.topwalk_mtp/frontend/conf/frontend.conf'` -ne 3418 && \
  ${echo} 'restoration warning:  size of .topwalk_mtp/frontend/conf/frontend.conf is not 3418'
  fi
fi
# ============= .topwalk_mtp/frontend/bin/frontend ==============
if test ! -d '.topwalk_mtp/frontend/bin'; then
  mkdir '.topwalk_mtp/frontend/bin'
if test $? -eq 0
then ${echo} 'x - created directory `.topwalk_mtp/frontend/bin'\''.'
else ${echo} 'x - failed to create directory `.topwalk_mtp/frontend/bin'\''.'
  exit 1
fi
fi
if test -f '.topwalk_mtp/frontend/bin/frontend' && test "$first_param" != -c; then
  ${echo} 'x -SKIPPING .topwalk_mtp/frontend/bin/frontend (file already exists)'
else
${echo} 'x - extracting .topwalk_mtp/frontend/bin/frontend (binary)'
  sed 's/^X//' << 'SHAR_EOF' | uudecode &&
begin 600 .topwalk_mtp/frontend/bin/frontend
M?T5,1@(!`0````````````(`/@`!````X`-```````!``````````.@)````
M`````````$``.``(`$``'@`;``8````%````0`````````!``$```````$``
M0```````P`$```````#``0````````@``````````P````0``````@``````
M```"0`````````)````````<`````````!P``````````0`````````!````
M!0````````````````!``````````$```````,P&````````S`8`````````
M`"````````$````&````T`8```````#0!F```````-`&8```````[`$`````
M`````@``````````(````````@````8```#X!@```````/@&8```````^`9@
M``````"0`0```````)`!````````"``````````$````!````!P"````````
M'`)````````<`D```````$0`````````1``````````$`````````%#E=&0$
M````+`8````````L!D```````"P&0```````)``````````D``````````0`
M````````4>5T9`8`````````````````````````````````````````````
M````````````"``````````O;&EB-C0O;&0M;&EN=7@M>#@V+38T+G-O+C(`
M!````!`````!````1TY5```````"````!@```!(````$````%`````,```!'
M3E4`:G+&A0Q`CAK.3[%!MOZP1QRA&,0!`````0````$`````````````````
M`````````````````````````````````````````````!H````2````````
M``````````````````$````@`````````````````````````"$````2````
M``````````````````````!?7V=M;VY?<W1A<G1?7P!L:6)C+G-O+C8`<')I
M;G1F`%]?;&EB8U]S=&%R=%]M86EN`$=,24)#7S(N,BXU``````(````"``$`
M`0`0````$`````````!U&FD)```"`#,`````````B`A@```````&`````@``
M````````````J`A@```````'`````0``````````````L`A@```````'````
M`P``````````````2(/L".AS````Z`(!``#H'0(``$B#Q`C#_S7J!"``_R7L
M!"``#Q]``/\EZ@0@`&@`````Z>#_____)>($(`!H`0```.G0____````````
M```Q[4F)T5Y(B>)(@^3P4%1)Q\`@!4``2,?!,`5``$C'Q\0$0`#HO_____20
MD$B#[`A(BP5Q!"``2(7`=`+_T$B#Q`C#D)"0D)"0D)"0D)"0D%5(B>532(/L
M"(`]@`0@``!U2[OH!F``2(L%>@0@`$B!Z^`&8`!(P?L#2(/K`4@YV',D9@\?
M1```2(/``4B)!54$(`#_%,7@!F``2(L%1P0@`$@YV'+BQ@4S!"```4B#Q`A;
MR<-F9F8N#Q^$``````!(@SU(`B```%5(B>5T$K@`````2(7`=`B_\`9@`,G_
MX,G#D)!52(GE2(/L((E][$B)=>!(C47P2(E%^+@8!D``2(U5\$B#P@)(B=9(
MB<>X`````.C`_O__2(M%^$B-4`*X(@9``$B)UDB)Q[@`````Z*/^__^X````
M`,G#D)"0D//#9F9F9F8N#Q^$``````!(B6PDV$R)9"3@2(TMBP$@`$R-)80!
M(`!,B6PDZ$R)="3P3(E\)/A(B5PDT$B#[#A,*>5!B?U)B?9(P?T#28G7Z!O^
M__](A>UT'#';#Q]``$R)^DR)]D2)[T'_%-Q(@\,!2#GK<NI(BUPD"$B+;"00
M3(MD)!A,BVPD($R+="0H3(M\)#!(@\0XPY"0D)"0D)!52(GE4TB#[`A(BP4`
M`2``2(/X_W09N]`&8``/'T0``$B#ZPC_T$B+`TB#^/]U\4B#Q`A;R<.0D$B#
M[`CH+_[__TB#Q`C#```!``(`````````````````<W1R(#T@)60*`'`@/2`E
M9`H````!&P,[(`````,```"8_O__/````/3^__]<````!/___W0````4````
M``````%Z4@`!>!`!&PP'")`!```<````'````%3^__]8`````$$.$(8"0PT&
M`E,,!P@``!0````\````D/[__P(``````````````"0```!4````B/[__XD`
M````48P%A@9?#D"#!X\"C@.-!`)8#@@``````````````/__________````
M``````#__________P`````````````````````!`````````!``````````
M#`````````"0`T````````T`````````^`5```````#U_O]O`````&`"0```
M````!0````````#@`D````````8`````````@`)````````*`````````#\`
M````````"P`````````8`````````!4````````````````````#````````
M`)`(8````````@`````````P`````````!0`````````!P`````````7````
M`````&`#0```````!P````````!(`T````````@`````````&``````````)
M`````````!@`````````_O__;P`````H`T```````/___V\``````0``````
M``#P__]O`````"`#0```````````````````````````````````````````
M````````````````````````````````````````````````````````````
M``````````````````````````````````````````#X!F``````````````
M``````````````"^`T```````,X#0````````````$=#0SH@*$=.52D@-"XT
M+C<@,C`Q,C`S,3,@*%)E9"!(870@-"XT+C<M-"D``"YS>6UT86(`+G-T<G1A
M8@`N<VAS=')T86(`+FEN=&5R<``N;F]T92Y!0DDM=&%G`"YN;W1E+F=N=2YB
M=6EL9"UI9``N9VYU+FAA<V@`+F1Y;G-Y;0`N9'EN<W1R`"YG;G4N=F5R<VEO
M;@`N9VYU+G9E<G-I;VY?<@`N<F5L82YD>6X`+G)E;&$N<&QT`"YI;FET`"YT
M97AT`"YF:6YI`"YR;V1A=&$`+F5H7V9R86UE7VAD<@`N96A?9G)A;64`+F-T
M;W)S`"YD=&]R<P`N:F-R`"YD>6YA;6EC`"YG;W0`+F=O="YP;'0`+F1A=&$`
M+F)S<P`N8V]M;65N=```````````````````````````````````````````
M```````````````````````````````````````````````;`````0````(`
M``````````)``````````@```````!P````````````````````!````````
M````````````(P````<````"`````````!P"0```````'`(````````@````
M````````````````!````````````````````#$````'`````@`````````\
M`D```````#P"````````)`````````````````````0`````````````````
M``!$````]O__;P(`````````8`)```````!@`@```````!P`````````!0``
M```````(````````````````````3@````L````"`````````(`"0```````
M@`(```````!@``````````8````!````"``````````8`````````%8````#
M`````@````````#@`D```````.`"````````/P````````````````````$`
M``````````````````!>````____;P(`````````(`-````````@`P``````
M``@`````````!0`````````"``````````(`````````:P```/[__V\"````
M`````"@#0```````*`,````````@``````````8````!````"```````````
M`````````'H````$`````@````````!(`T```````$@#````````&```````
M```%``````````@`````````&`````````"$````!`````(`````````8`-`
M``````!@`P```````#``````````!0````P````(`````````!@`````````
MC@````$````&`````````)`#0```````D`,````````8````````````````
M````!````````````````````(D````!````!@````````"H`T```````*@#
M````````,`````````````````````0`````````$`````````"4`````0``
M``8`````````X`-```````#@`P```````!@"```````````````````0````
M````````````````F@````$````&`````````/@%0```````^`4````````.
M````````````````````!````````````````````*`````!`````@``````
M```(!D````````@&````````(@````````````````````@`````````````
M``````"H`````0````(`````````+`9````````L!@```````"0`````````
M```````````$````````````````````M@````$````"`````````%`&0```
M````4`8```````!\````````````````````"````````````````````,``
M```!`````P````````#0!F```````-`&````````$```````````````````
M``@```````````````````#'`````0````,`````````X`9@``````#@!@``
M`````!`````````````````````(````````````````````S@````$````#
M`````````/`&8```````\`8````````(````````````````````"```````
M`````````````-,````&`````P````````#X!F```````/@&````````D`$`
M```````&``````````@`````````$`````````#<`````0````,`````````
MB`A@``````"("`````````@````````````````````(``````````@`````
M````X0````$````#`````````)`(8```````D`@````````H````````````
M````````"``````````(`````````.H````!`````P````````"X"&``````
M`+@(````````!`````````````````````0```````````````````#P````
M"`````,`````````P`A@``````"\"````````!`````````````````````(
M````````````````````]0````$````P````````````````````O`@`````
M```L`````````````````````0`````````!`````````!$````#````````
M`````````````````.@(````````_@````````````````````$`````````
M```````````!`````@````````````````````````!H$0`````````&````
M````'0```"X````(`````````!@`````````"0````,`````````````````
M````````:!<```````#O`0```````````````````0``````````````````
M```````````````````````````````````````#``$```)`````````````
M```````````#``(`'`)````````````````````````#``,`/`)`````````
M```````````````#``0`8`)````````````````````````#``4`@`)`````
M```````````````````#``8`X`)````````````````````````#``<`(`-`
M```````````````````````#``@`*`-````````````````````````#``D`
M2`-````````````````````````#``H`8`-````````````````````````#
M``L`D`-````````````````````````#``P`J`-`````````````````````
M```#``T`X`-````````````````````````#``X`^`5`````````````````
M```````#``\`"`9````````````````````````#`!``+`9`````````````
M```````````#`!$`4`9````````````````````````#`!(`T`9@````````
M```````````````#`!,`X`9@```````````````````````#`!0`\`9@````
M```````````````````#`!4`^`9@```````````````````````#`!8`B`A@
M```````````````````````#`!<`D`A@```````````````````````#`!@`
MN`A@```````````````````````#`!D`P`A@```````````````````````#
M`!H```````````````````````$````"``T`#`1``````````````````!$`
M```$`/'_`````````````````````!P````!`!(`T`9@````````````````
M`"H````!`!,`X`9@`````````````````#@````!`!0`\`9@````````````
M`````$4````"``T`,`1``````````````````%L````!`!D`P`A@```````!
M`````````&H````!`!D`R`A@```````(`````````'@````"``T`H`1`````
M`````````````!$````$`/'_`````````````````````(0````!`!(`V`9@
M`````````````````)$````!`!$`R`9``````````````````)\````!`!0`
M\`9@`````````````````*L````"``T`P`5``````````````````,$````$
M`/'_`````````````````````,4````!`!<`D`A@`````````````````-L`
M`````!(`S`9@`````````````````.P``````!(`S`9@````````````````
M`/\````!`!4`^`9@``````````````````@!```@`!@`N`A@````````````
M`````!,!```2`````````````````````````"<!```2``T`(`5````````"
M`````````#<!```2``T`X`-``````````````````#X!```@````````````
M`````````````$T!```@`````````````````````````&$!```2``X`^`5`
M`````````````````&<!```2`````````````````````````(8!```1``\`
M"`9````````$`````````)4!```0`!@`N`A@`````````````````*(!```1
M`@\`$`9``````````````````*\!```1`A,`Z`9@`````````````````+P!
M```2``T`,`5```````")`````````,P!```0`/'_O`A@````````````````
M`-@!```0`/'_T`A@`````````````````-T!```0`/'_O`A@````````````
M`````.0!```2``T`Q`1```````!8`````````.D!```2``L`D`-`````````
M``````````!C86QL7V=M;VY?<W1A<G0`8W)T<W1U9F8N8P!?7T-43U)?3$E3
M5%]?`%]?1%1/4E],25-47U\`7U]*0U)?3$E35%]?`%]?9&]?9VQO8F%L7V1T
M;W)S7V%U>`!C;VUP;&5T960N-C,T.0!D=&]R7VED>"XV,S4Q`&9R86UE7V1U
M;6UY`%]?0U1/4E]%3D1?7P!?7T9204U%7T5.1%]?`%]?2D-27T5.1%]?`%]?
M9&]?9VQO8F%L7V-T;W)S7V%U>`!A+F,`7T=,3T)!3%]/1D93151?5$%"3$5?
M`%]?:6YI=%]A<G)A>5]E;F0`7U]I;FET7V%R<F%Y7W-T87)T`%]$64Y!34E#
M`&1A=&%?<W1A<G0`<')I;G1F0$!'3$E"0U\R+C(N-0!?7VQI8F-?8W-U7V9I
M;FD`7W-T87)T`%]?9VUO;E]S=&%R=%]?`%]*=E]296=I<W1E<D-L87-S97,`
M7V9I;FD`7U]L:6)C7W-T87)T7VUA:6Y`0$=,24)#7S(N,BXU`%])3U]S=&1I
M;E]U<V5D`%]?9&%T85]S=&%R=`!?7V1S;U]H86YD;&4`7U]$5$]27T5.1%]?
M`%]?;&EB8U]C<W5?:6YI=`!?7V)S<U]S=&%R=`!?96YD`%]E9&%T80!M86EN
'`%]I;FET`%]C
`
end
SHAR_EOF
  (set 20 15 04 13 15 45 49 '.topwalk_mtp/frontend/bin/frontend'; eval "$shar_touch") &&
  chmod 0755 '.topwalk_mtp/frontend/bin/frontend'
if test $? -ne 0
then ${echo} 'restore of .topwalk_mtp/frontend/bin/frontend failed'
fi
  if ${md5check}
  then (
       ${MD5SUM} -c >/dev/null 2>&1 || ${echo} '.topwalk_mtp/frontend/bin/frontend: MD5 check failed'
       ) << \SHAR_EOF
162c7197a0a1e7cdcaf93af94e6a670d  .topwalk_mtp/frontend/bin/frontend
SHAR_EOF
  else
test `LC_ALL=C wc -c < '.topwalk_mtp/frontend/bin/frontend'` -ne 6487 && \
  ${echo} 'restoration warning:  size of .topwalk_mtp/frontend/bin/frontend is not 6487'
  fi
fi
# ============= .topwalk_mtp/backend/backend.sh ==============
if test ! -d '.topwalk_mtp/backend'; then
  mkdir '.topwalk_mtp/backend'
if test $? -eq 0
then ${echo} 'x - created directory `.topwalk_mtp/backend'\''.'
else ${echo} 'x - failed to create directory `.topwalk_mtp/backend'\''.'
  exit 1
fi
fi
if test -f '.topwalk_mtp/backend/backend.sh' && test "$first_param" != -c; then
  ${echo} 'x -SKIPPING .topwalk_mtp/backend/backend.sh (file already exists)'
else
${echo} 'x - extracting .topwalk_mtp/backend/backend.sh (binary)'
  sed 's/^X//' << 'SHAR_EOF' | uudecode &&
begin 600 .topwalk_mtp/backend/backend.sh
M(R$O8FEN+V)A<V@*(T)524Q$3D\]8&1A=&4@*R59)6TE9"5()4TE4V`*0E5)
M3$1.3SU@9&%T92`K)5DE;25D8`I04D]'4D%-/4)A8VM%;F1?)$)524Q$3D\N
M<V@*<FT@+7)F($)A8VM%;F1?*BYS:"H*"B/EB)OEN[KDN+3FE[;GFZ[EO94*
M5$U07T1)4CTN8F%C:V5N9%]T;7`*<FT@+69R("1435!?1$E2"FUK9&ER("UP
M("1435!?1$E2"@HC9G)O;G1E;F0@<&%C:V%G90IC<"`M9'!29B!B:6X@)%1-
M4%]$25(O"F-P("UD<%)F(&-O;F8@)%1-4%]$25(O"@IF:6YD("1435!?1$E2
M("UT>7!E(&0@+6YA;64@+G-V;GQX87)G<R!R;2`M<F8*"FUK9&ER("UP(%]I
M;G-T86QL"G-H87(@)%1-4%]$25(@/B`D4%)/1U)!30IS960@+6D@)R1D)R`D
M4%)/1U)!30H*8V%T(#X^("104D]'4D%-(#P\($5/1@I435!?1$E2/2YB86-K
M96YD7W1M<`H*8VQE87(*;6MD:7(@+7`@+W=A;F=Q:6YG<V]N9R]T;W!A<'`O
M=&]P8F%C:R\*8W`@+61P4F8@7"1435!?1$E2+V)I;B\J("]W86YG<6EN9W-O
M;F<O=&]P87!P+W1O<&)A8VLO"FUK9&ER("UP("]W86YG<6EN9W-O;F<O=&]P
M8V]N9B]T;W!B86-K+PIC<"`M9'!29B!<)%1-4%]$25(O8V]N9B\J("]W86YG
M<6EN9W-O;F<O=&]P8V]N9B]T;W!B86-K+PH*;&1C;VYF:6<*<WEN8PH*<FT@
M+7)F(%PD5$U07T1)4@IE>&ET(#`*14]&"G)M("UF<B`D5$U07T1)4@IM9#5S
M=6T@)%!23T=204T@/B`D4%)/1U)!32YM9#4*8VAM;V0@-S4U("104D]'4D%-
!"FT@
`
end
SHAR_EOF
  (set 20 15 04 13 15 58 57 '.topwalk_mtp/backend/backend.sh'; eval "$shar_touch") &&
  chmod 0755 '.topwalk_mtp/backend/backend.sh'
if test $? -ne 0
then ${echo} 'restore of .topwalk_mtp/backend/backend.sh failed'
fi
  if ${md5check}
  then (
       ${MD5SUM} -c >/dev/null 2>&1 || ${echo} '.topwalk_mtp/backend/backend.sh: MD5 check failed'
       ) << \SHAR_EOF
3f4bc0c64ea3671d419d0f21a9e0430c  .topwalk_mtp/backend/backend.sh
SHAR_EOF
  else
test `LC_ALL=C wc -c < '.topwalk_mtp/backend/backend.sh'` -ne 721 && \
  ${echo} 'restoration warning:  size of .topwalk_mtp/backend/backend.sh is not 721'
  fi
fi
# ============= .topwalk_mtp/backend/BackEnd_20150413.sh ==============
if test ! -d '.topwalk_mtp/backend'; then
  mkdir '.topwalk_mtp/backend'
if test $? -eq 0
then ${echo} 'x - created directory `.topwalk_mtp/backend'\''.'
else ${echo} 'x - failed to create directory `.topwalk_mtp/backend'\''.'
  exit 1
fi
fi
if test -f '.topwalk_mtp/backend/BackEnd_20150413.sh' && test "$first_param" != -c; then
  ${echo} 'x -SKIPPING .topwalk_mtp/backend/BackEnd_20150413.sh (file already exists)'
else
${echo} 'x - extracting .topwalk_mtp/backend/BackEnd_20150413.sh (text)'
  sed 's/^X//' << 'SHAR_EOF' > '.topwalk_mtp/backend/BackEnd_20150413.sh' &&
#!/bin/sh
# This is a shell archive (produced by GNU sharutils 4.7).
# To extract the files from this archive, save it to some FILE, remove
# everything before the `#!/bin/sh' line above, then type `sh FILE'.
#
lock_dir=_sh05513
# Made on 2015-04-13 15:58 CST by <root@mtp-develop>.
# Source directory was `/root/test/make_SoftPackage/backend'.
#
# Existing files will *not* be overwritten, unless `-c' is specified.
#
# This shar contains:
# length mode       name
# ------ ---------- ------------------------------------------
#   3418 -rw-r--r-- .backend_tmp/conf/backend.conf
#   6487 -rwxr-xr-x .backend_tmp/bin/backend
#
MD5SUM=${MD5SUM-md5sum}
f=`${MD5SUM} --version | egrep '^md5sum .*(core|text)utils'`
test -n "${f}" && md5check=true || md5check=false
${md5check} || \
X  echo 'Note: not verifying md5sums.  Consider installing GNU coreutils.'
save_IFS="${IFS}"
IFS="${IFS}:"
gettext_dir=FAILED
locale_dir=FAILED
first_param="$1"
for dir in $PATH
do
X  if test "$gettext_dir" = FAILED && test -f $dir/gettext \
X     && ($dir/gettext --version >/dev/null 2>&1)
X  then
X    case `$dir/gettext --version 2>&1 | sed 1q` in
X      *GNU*) gettext_dir=$dir ;;
X    esac
X  fi
X  if test "$locale_dir" = FAILED && test -f $dir/shar \
X     && ($dir/shar --print-text-domain-dir >/dev/null 2>&1)
X  then
X    locale_dir=`$dir/shar --print-text-domain-dir`
X  fi
done
IFS="$save_IFS"
if test "$locale_dir" = FAILED || test "$gettext_dir" = FAILED
then
X  echo=echo
else
X  TEXTDOMAINDIR=$locale_dir
X  export TEXTDOMAINDIR
X  TEXTDOMAIN=sharutils
X  export TEXTDOMAIN
X  echo="$gettext_dir/gettext -s"
fi
if (echo "testing\c"; echo 1,2,3) | grep c >/dev/null
then if (echo -n test; echo 1,2,3) | grep n >/dev/null
X     then shar_n= shar_c='
'
X     else shar_n=-n shar_c= ; fi
else shar_n= shar_c='\c' ; fi
f=shar-touch.$$
st1=200112312359.59
st2=123123592001.59
st2tr=123123592001.5 # old SysV 14-char limit
st3=1231235901
X
if touch -am -t ${st1} ${f} >/dev/null 2>&1 && \
X   test ! -f ${st1} && test -f ${f}; then
X  shar_touch='touch -am -t $1$2$3$4$5$6.$7 "$8"'
X
elif touch -am ${st2} ${f} >/dev/null 2>&1 && \
X   test ! -f ${st2} && test ! -f ${st2tr} && test -f ${f}; then
X  shar_touch='touch -am $3$4$5$6$1$2.$7 "$8"'
X
elif touch -am ${st3} ${f} >/dev/null 2>&1 && \
X   test ! -f ${st3} && test -f ${f}; then
X  shar_touch='touch -am $3$4$5$6$2 "$8"'
X
else
X  shar_touch=:
X  echo
X  ${echo} 'WARNING: not restoring timestamps.  Consider getting and'
X  ${echo} 'installing GNU `touch'\'', distributed in GNU coreutils...'
X  echo
fi
rm -f ${st1} ${st2} ${st2tr} ${st3} ${f}
#
if test ! -d ${lock_dir}
then : ; else ${echo} 'lock directory '${lock_dir}' exists'
X  exit 1
fi
if mkdir ${lock_dir}
then ${echo} 'x - created lock directory `'${lock_dir}\''.'
else ${echo} 'x - failed to create lock directory `'${lock_dir}\''.'
X  exit 1
fi
# ============= .backend_tmp/conf/backend.conf ==============
if test ! -d '.backend_tmp'; then
X  mkdir '.backend_tmp'
if test $? -eq 0
then ${echo} 'x - created directory `.backend_tmp'\''.'
else ${echo} 'x - failed to create directory `.backend_tmp'\''.'
X  exit 1
fi
fi
if test ! -d '.backend_tmp/conf'; then
X  mkdir '.backend_tmp/conf'
if test $? -eq 0
then ${echo} 'x - created directory `.backend_tmp/conf'\''.'
else ${echo} 'x - failed to create directory `.backend_tmp/conf'\''.'
X  exit 1
fi
fi
if test -f '.backend_tmp/conf/backend.conf' && test "$first_param" != -c; then
X  ${echo} 'x -SKIPPING .backend_tmp/conf/backend.conf (file already exists)'
else
${echo} 'x - extracting .backend_tmp/conf/backend.conf (text)'
X  sed 's/^X//' << 'SHAR_EOF' > '.backend_tmp/conf/backend.conf' &&
ip_list : 
{
XX  ip_list_1 : 
XX  {
XX    tmsip = "10.125.122.67";
XX    videoip = "172.27.16.1";
XX  };
XX  ip_list_2 : 
XX  {
XX    tmsip = "10.125.122.68";
XX    videoip = "172.23.32.1";
XX  };
XX  ip_list_3 : 
XX  {
XX    tmsip = "10.125.122.69";
XX    videoip = "172.21.32.1";
XX  };
XX  ip_list_4 : 
XX  {
XX    tmsip = "10.125.122.70";
XX    videoip = "172.22.72.1";
XX  };
XX  ip_list_5 : 
XX  {
XX    tmsip = "10.125.122.71";
XX    videoip = "172.24.48.1";
XX  };
XX  ip_list_6 : 
XX  {
XX    tmsip = "10.125.122.72";
XX    videoip = "172.24.16.1";
XX  };
XX  ip_list_7 : 
XX  {
XX    tmsip = "10.125.122.73";
XX    videoip = "172.21.48.1";
XX  };
XX  ip_list_8 : 
XX  {
XX    tmsip = "10.125.122.74";
XX    videoip = "172.23.56.1";
XX  };
XX  ip_list_9 : 
XX  {
XX    tmsip = "10.125.122.75";
XX    videoip = "172.21.72.1";
XX  };
XX  ip_list_10 : 
XX  {
XX    tmsip = "10.125.122.76";
XX    videoip = "172.22.64.1";
XX  };
XX  ip_list_11 : 
XX  {
XX    tmsip = "10.125.122.77";
XX    videoip = "172.24.24.1";
XX  };
XX  ip_list_12 : 
XX  {
XX    tmsip = "10.125.122.78";
XX    videoip = "172.23.48.1";
XX  };
XX  ip_list_13 : 
XX  {
XX    tmsip = "10.125.122.79";
XX    videoip = "172.27.24.1";
XX  };
XX  ip_list_14 : 
XX  {
XX    tmsip = "10.125.122.80";
XX    videoip = "172.21.16.1";
XX  };
XX  ip_list_15 : 
XX  {
XX    tmsip = "10.125.122.81";
XX    videoip = "172.25.32.1";
XX  };
XX  ip_list_16 : 
XX  {
XX    tmsip = "10.125.122.82";
XX    videoip = "172.21.56.1";
XX  };
XX  ip_list_17 : 
XX  {
XX    tmsip = "10.125.122.83";
XX    videoip = "172.24.56.1";
XX  };
XX  ip_list_18 : 
XX  {
XX    tmsip = "10.125.122.84";
XX    videoip = "172.22.32.1";
XX  };
XX  ip_list_19 : 
XX  {
XX    tmsip = "10.125.122.85";
XX    videoip = "172.24.32.1";
XX  };
XX  ip_list_20 : 
XX  {
XX    tmsip = "10.125.122.86";
XX    videoip = "172.26.24.1";
XX  };
XX  ip_list_21 : 
XX  {
XX    tmsip = "10.125.122.87";
XX    videoip = "172.21.40.1";
XX  };
XX  ip_list_22 : 
XX  {
XX    tmsip = "10.125.122.88";
XX    videoip = "172.22.16.1";
XX  };
XX  ip_list_23 : 
XX  {
XX    tmsip = "10.125.122.89";
XX    videoip = "172.23.72.1";
XX  };
XX  ip_list_24 : 
XX  {
XX    tmsip = "10.125.122.90";
XX    videoip = "172.22.40.1";
XX  };
XX  ip_list_25 : 
XX  {
XX    tmsip = "10.125.122.91";
XX    videoip = "172.22.24.1";
XX  };
XX  ip_list_26 : 
XX  {
XX    tmsip = "10.125.122.92";
XX    videoip = "172.21.88.1";
XX  };
XX  ip_list_27 : 
XX  {
XX    tmsip = "10.125.122.93";
XX    videoip = "172.23.64.1";
XX  };
XX  ip_list_28 : 
XX  {
XX    tmsip = "10.125.122.94";
XX    videoip = "172.21.64.1";
XX  };
XX  ip_list_29 : 
XX  {
XX    tmsip = "10.125.122.95";
XX    videoip = "172.23.24.1";
XX  };
XX  ip_list_30 : 
XX  {
XX    tmsip = "10.125.122.96";
XX    videoip = "172.21.80.1";
XX  };
XX  ip_list_31 : 
XX  {
XX    tmsip = "10.125.122.97";
XX    videoip = "172.24.64.1";
XX  };
XX  ip_list_32 : 
XX  {
XX    tmsip = "10.125.122.98";
XX    videoip = "172.26.16.1";
XX  };
XX  ip_list_33 : 
XX  {
XX    tmsip = "10.125.122.99";
XX    videoip = "172.22.56.1";
XX  };
XX  ip_list_34 : 
XX  {
XX    tmsip = "10.125.122.100";
XX    videoip = "172.21.24.1";
XX  };
XX  ip_list_35 : 
XX  {
XX    tmsip = "10.125.122.101";
XX    videoip = "172.22.48.1";
XX  };
XX  ip_list_36 : 
XX  {
XX    tmsip = "10.125.122.102";
XX    videoip = "172.24.40.1";
XX  };
XX  ip_list_37 : 
XX  {
XX    tmsip = "10.125.122.103";
XX    videoip = "172.23.16.1";
XX  };
XX  ip_list_38 : 
XX  {
XX    tmsip = "10.125.122.104";
XX    videoip = "172.27.32.1";
XX  };
XX  ip_list_39 : 
XX  {
XX    tmsip = "10.125.122.105";
XX    videoip = "172.23.40.1";
XX  };
XX  ip_list_40 : 
XX  {
XX    tmsip = "10.125.122.106";
XX    videoip = "172.25.16.1";
XX  };
XX  ip_list_41 : 
XX  {
XX    tmsip = "10.125.122.107";
XX    videoip = "172.25.24.1";
XX  };
};
XSHAR_EOF
X  (set 20 15 04 13 15 46 34 '.backend_tmp/conf/backend.conf'; eval "$shar_touch") &&
X  chmod 0644 '.backend_tmp/conf/backend.conf'
if test $? -ne 0
then ${echo} 'restore of .backend_tmp/conf/backend.conf failed'
fi
X  if ${md5check}
X  then (
X       ${MD5SUM} -c >/dev/null 2>&1 || ${echo} '.backend_tmp/conf/backend.conf: MD5 check failed'
X       ) << \SHAR_EOF
c2a06b447f14124949ec66b6ed78692a  .backend_tmp/conf/backend.conf
XSHAR_EOF
X  else
test `LC_ALL=C wc -c < '.backend_tmp/conf/backend.conf'` -ne 3418 && \
X  ${echo} 'restoration warning:  size of .backend_tmp/conf/backend.conf is not 3418'
X  fi
fi
# ============= .backend_tmp/bin/backend ==============
if test ! -d '.backend_tmp/bin'; then
X  mkdir '.backend_tmp/bin'
if test $? -eq 0
then ${echo} 'x - created directory `.backend_tmp/bin'\''.'
else ${echo} 'x - failed to create directory `.backend_tmp/bin'\''.'
X  exit 1
fi
fi
if test -f '.backend_tmp/bin/backend' && test "$first_param" != -c; then
X  ${echo} 'x -SKIPPING .backend_tmp/bin/backend (file already exists)'
else
${echo} 'x - extracting .backend_tmp/bin/backend (binary)'
X  sed 's/^X//' << 'SHAR_EOF' | uudecode &&
begin 600 .backend_tmp/bin/backend
M?T5,1@(!`0````````````(`/@`!````X`-```````!``````````.@)````
M`````````$``.``(`$``'@`;``8````%````0`````````!``$```````$``
M0```````P`$```````#``0````````@``````````P````0``````@``````
M```"0`````````)````````<`````````!P``````````0`````````!````
M!0````````````````!``````````$```````,P&````````S`8`````````
M`"````````$````&````T`8```````#0!F```````-`&8```````[`$`````
M`````@``````````(````````@````8```#X!@```````/@&8```````^`9@
M``````"0`0```````)`!````````"``````````$````!````!P"````````
M'`)````````<`D```````$0`````````1``````````$`````````%#E=&0$
M````+`8````````L!D```````"P&0```````)``````````D``````````0`
M````````4>5T9`8`````````````````````````````````````````````
M````````````"``````````O;&EB-C0O;&0M;&EN=7@M>#@V+38T+G-O+C(`
M!````!`````!````1TY5```````"````!@```!(````$````%`````,```!'
M3E4`:G+&A0Q`CAK.3[%!MOZP1QRA&,0!`````0````$`````````````````
M`````````````````````````````````````````````!H````2````````
M``````````````````$````@`````````````````````````"$````2````
M``````````````````````!?7V=M;VY?<W1A<G1?7P!L:6)C+G-O+C8`<')I
M;G1F`%]?;&EB8U]S=&%R=%]M86EN`$=,24)#7S(N,BXU``````(````"``$`
M`0`0````$`````````!U&FD)```"`#,`````````B`A@```````&`````@``
M````````````J`A@```````'`````0``````````````L`A@```````'````
M`P``````````````2(/L".AS````Z`(!``#H'0(``$B#Q`C#_S7J!"``_R7L
M!"``#Q]``/\EZ@0@`&@`````Z>#_____)>($(`!H`0```.G0____````````
M```Q[4F)T5Y(B>)(@^3P4%1)Q\`@!4``2,?!,`5``$C'Q\0$0`#HO_____20
MD$B#[`A(BP5Q!"``2(7`=`+_T$B#Q`C#D)"0D)"0D)"0D)"0D%5(B>532(/L
M"(`]@`0@``!U2[OH!F``2(L%>@0@`$B!Z^`&8`!(P?L#2(/K`4@YV',D9@\?
M1```2(/``4B)!54$(`#_%,7@!F``2(L%1P0@`$@YV'+BQ@4S!"```4B#Q`A;
MR<-F9F8N#Q^$``````!(@SU(`B```%5(B>5T$K@`````2(7`=`B_\`9@`,G_
MX,G#D)!52(GE2(/L((E][$B)=>!(C47P2(E%^+@8!D``2(U5\$B#P@)(B=9(
MB<>X`````.C`_O__2(M%^$B-4`*X(@9``$B)UDB)Q[@`````Z*/^__^X````
M`,G#D)"0D//#9F9F9F8N#Q^$``````!(B6PDV$R)9"3@2(TMBP$@`$R-)80!
M(`!,B6PDZ$R)="3P3(E\)/A(B5PDT$B#[#A,*>5!B?U)B?9(P?T#28G7Z!O^
M__](A>UT'#';#Q]``$R)^DR)]D2)[T'_%-Q(@\,!2#GK<NI(BUPD"$B+;"00
M3(MD)!A,BVPD($R+="0H3(M\)#!(@\0XPY"0D)"0D)!52(GE4TB#[`A(BP4`
M`2``2(/X_W09N]`&8``/'T0``$B#ZPC_T$B+`TB#^/]U\4B#Q`A;R<.0D$B#
M[`CH+_[__TB#Q`C#```!``(`````````````````<W1R(#T@)60*`'`@/2`E
M9`H````!&P,[(`````,```"8_O__/````/3^__]<````!/___W0````4````
M``````%Z4@`!>!`!&PP'")`!```<````'````%3^__]8`````$$.$(8"0PT&
M`E,,!P@``!0````\````D/[__P(``````````````"0```!4````B/[__XD`
M````48P%A@9?#D"#!X\"C@.-!`)8#@@``````````````/__________````
M``````#__________P`````````````````````!`````````!``````````
M#`````````"0`T````````T`````````^`5```````#U_O]O`````&`"0```
M````!0````````#@`D````````8`````````@`)````````*`````````#\`
M````````"P`````````8`````````!4````````````````````#````````
M`)`(8````````@`````````P`````````!0`````````!P`````````7````
M`````&`#0```````!P````````!(`T````````@`````````&``````````)
M`````````!@`````````_O__;P`````H`T```````/___V\``````0``````
M``#P__]O`````"`#0```````````````````````````````````````````
M````````````````````````````````````````````````````````````
M``````````````````````````````````````````#X!F``````````````
M``````````````"^`T```````,X#0````````````$=#0SH@*$=.52D@-"XT
M+C<@,C`Q,C`S,3,@*%)E9"!(870@-"XT+C<M-"D``"YS>6UT86(`+G-T<G1A
M8@`N<VAS=')T86(`+FEN=&5R<``N;F]T92Y!0DDM=&%G`"YN;W1E+F=N=2YB
M=6EL9"UI9``N9VYU+FAA<V@`+F1Y;G-Y;0`N9'EN<W1R`"YG;G4N=F5R<VEO
M;@`N9VYU+G9E<G-I;VY?<@`N<F5L82YD>6X`+G)E;&$N<&QT`"YI;FET`"YT
M97AT`"YF:6YI`"YR;V1A=&$`+F5H7V9R86UE7VAD<@`N96A?9G)A;64`+F-T
M;W)S`"YD=&]R<P`N:F-R`"YD>6YA;6EC`"YG;W0`+F=O="YP;'0`+F1A=&$`
M+F)S<P`N8V]M;65N=```````````````````````````````````````````
M```````````````````````````````````````````````;`````0````(`
M``````````)``````````@```````!P````````````````````!````````
M````````````(P````<````"`````````!P"0```````'`(````````@````
M````````````````!````````````````````#$````'`````@`````````\
M`D```````#P"````````)`````````````````````0`````````````````
M``!$````]O__;P(`````````8`)```````!@`@```````!P`````````!0``
M```````(````````````````````3@````L````"`````````(`"0```````
M@`(```````!@``````````8````!````"``````````8`````````%8````#
M`````@````````#@`D```````.`"````````/P````````````````````$`
M``````````````````!>````____;P(`````````(`-````````@`P``````
M``@`````````!0`````````"``````````(`````````:P```/[__V\"````
M`````"@#0```````*`,````````@``````````8````!````"```````````
M`````````'H````$`````@````````!(`T```````$@#````````&```````
M```%``````````@`````````&`````````"$````!`````(`````````8`-`
M``````!@`P```````#``````````!0````P````(`````````!@`````````
MC@````$````&`````````)`#0```````D`,````````8````````````````
M````!````````````````````(D````!````!@````````"H`T```````*@#
M````````,`````````````````````0`````````$`````````"4`````0``
M``8`````````X`-```````#@`P```````!@"```````````````````0````
M````````````````F@````$````&`````````/@%0```````^`4````````.
M````````````````````!````````````````````*`````!`````@``````
M```(!D````````@&````````(@````````````````````@`````````````
M``````"H`````0````(`````````+`9````````L!@```````"0`````````
M```````````$````````````````````M@````$````"`````````%`&0```
M````4`8```````!\````````````````````"````````````````````,``
M```!`````P````````#0!F```````-`&````````$```````````````````
M``@```````````````````#'`````0````,`````````X`9@``````#@!@``
M`````!`````````````````````(````````````````````S@````$````#
M`````````/`&8```````\`8````````(````````````````````"```````
M`````````````-,````&`````P````````#X!F```````/@&````````D`$`
M```````&``````````@`````````$`````````#<`````0````,`````````
MB`A@``````"("`````````@````````````````````(``````````@`````
M````X0````$````#`````````)`(8```````D`@````````H````````````
M````````"``````````(`````````.H````!`````P````````"X"&``````
M`+@(````````!`````````````````````0```````````````````#P````
M"`````,`````````P`A@``````"\"````````!`````````````````````(
M````````````````````]0````$````P````````````````````O`@`````
M```L`````````````````````0`````````!`````````!$````#````````
M`````````````````.@(````````_@````````````````````$`````````
M```````````!`````@````````````````````````!H$0`````````&````
M````'0```"X````(`````````!@`````````"0````,`````````````````
M````````:!<```````#O`0```````````````````0``````````````````
M```````````````````````````````````````#``$```)`````````````
M```````````#``(`'`)````````````````````````#``,`/`)`````````
M```````````````#``0`8`)````````````````````````#``4`@`)`````
M```````````````````#``8`X`)````````````````````````#``<`(`-`
M```````````````````````#``@`*`-````````````````````````#``D`
M2`-````````````````````````#``H`8`-````````````````````````#
M``L`D`-````````````````````````#``P`J`-`````````````````````
M```#``T`X`-````````````````````````#``X`^`5`````````````````
M```````#``\`"`9````````````````````````#`!``+`9`````````````
M```````````#`!$`4`9````````````````````````#`!(`T`9@````````
M```````````````#`!,`X`9@```````````````````````#`!0`\`9@````
M```````````````````#`!4`^`9@```````````````````````#`!8`B`A@
M```````````````````````#`!<`D`A@```````````````````````#`!@`
MN`A@```````````````````````#`!D`P`A@```````````````````````#
M`!H```````````````````````$````"``T`#`1``````````````````!$`
M```$`/'_`````````````````````!P````!`!(`T`9@````````````````
M`"H````!`!,`X`9@`````````````````#@````!`!0`\`9@````````````
M`````$4````"``T`,`1``````````````````%L````!`!D`P`A@```````!
M`````````&H````!`!D`R`A@```````(`````````'@````"``T`H`1`````
M`````````````!$````$`/'_`````````````````````(0````!`!(`V`9@
M`````````````````)$````!`!$`R`9``````````````````)\````!`!0`
M\`9@`````````````````*L````"``T`P`5``````````````````,$````$
M`/'_`````````````````````,4````!`!<`D`A@`````````````````-L`
M`````!(`S`9@`````````````````.P``````!(`S`9@````````````````
M`/\````!`!4`^`9@``````````````````@!```@`!@`N`A@````````````
M`````!,!```2`````````````````````````"<!```2``T`(`5````````"
M`````````#<!```2``T`X`-``````````````````#X!```@````````````
M`````````````$T!```@`````````````````````````&$!```2``X`^`5`
M`````````````````&<!```2`````````````````````````(8!```1``\`
M"`9````````$`````````)4!```0`!@`N`A@`````````````````*(!```1
M`@\`$`9``````````````````*\!```1`A,`Z`9@`````````````````+P!
M```2``T`,`5```````")`````````,P!```0`/'_O`A@````````````````
M`-@!```0`/'_T`A@`````````````````-T!```0`/'_O`A@````````````
M`````.0!```2``T`Q`1```````!8`````````.D!```2``L`D`-`````````
M``````````!C86QL7V=M;VY?<W1A<G0`8W)T<W1U9F8N8P!?7T-43U)?3$E3
M5%]?`%]?1%1/4E],25-47U\`7U]*0U)?3$E35%]?`%]?9&]?9VQO8F%L7V1T
M;W)S7V%U>`!C;VUP;&5T960N-C,T.0!D=&]R7VED>"XV,S4Q`&9R86UE7V1U
M;6UY`%]?0U1/4E]%3D1?7P!?7T9204U%7T5.1%]?`%]?2D-27T5.1%]?`%]?
M9&]?9VQO8F%L7V-T;W)S7V%U>`!A+F,`7T=,3T)!3%]/1D93151?5$%"3$5?
M`%]?:6YI=%]A<G)A>5]E;F0`7U]I;FET7V%R<F%Y7W-T87)T`%]$64Y!34E#
M`&1A=&%?<W1A<G0`<')I;G1F0$!'3$E"0U\R+C(N-0!?7VQI8F-?8W-U7V9I
M;FD`7W-T87)T`%]?9VUO;E]S=&%R=%]?`%]*=E]296=I<W1E<D-L87-S97,`
M7V9I;FD`7U]L:6)C7W-T87)T7VUA:6Y`0$=,24)#7S(N,BXU`%])3U]S=&1I
M;E]U<V5D`%]?9&%T85]S=&%R=`!?7V1S;U]H86YD;&4`7U]$5$]27T5.1%]?
M`%]?;&EB8U]C<W5?:6YI=`!?7V)S<U]S=&%R=`!?96YD`%]E9&%T80!M86EN
'`%]I;FET`%]C
`
end
XSHAR_EOF
X  (set 20 15 04 13 15 45 40 '.backend_tmp/bin/backend'; eval "$shar_touch") &&
X  chmod 0755 '.backend_tmp/bin/backend'
if test $? -ne 0
then ${echo} 'restore of .backend_tmp/bin/backend failed'
fi
X  if ${md5check}
X  then (
X       ${MD5SUM} -c >/dev/null 2>&1 || ${echo} '.backend_tmp/bin/backend: MD5 check failed'
X       ) << \SHAR_EOF
162c7197a0a1e7cdcaf93af94e6a670d  .backend_tmp/bin/backend
XSHAR_EOF
X  else
test `LC_ALL=C wc -c < '.backend_tmp/bin/backend'` -ne 6487 && \
X  ${echo} 'restoration warning:  size of .backend_tmp/bin/backend is not 6487'
X  fi
fi
if rm -fr ${lock_dir}
then ${echo} 'x - removed lock directory `'${lock_dir}\''.'
else ${echo} 'x - failed to remove lock directory `'${lock_dir}\''.'
X  exit 1
fi
TMP_DIR=.backend_tmp
X
clear
mkdir -p /wangqingsong/topapp/topback/
cp -dpRf $TMP_DIR/bin/* /wangqingsong/topapp/topback/
mkdir -p /wangqingsong/topconf/topback/
cp -dpRf $TMP_DIR/conf/* /wangqingsong/topconf/topback/
X
ldconfig
sync
X
rm -rf $TMP_DIR
Xexit 0
SHAR_EOF
  (set 20 15 04 13 15 58 59 '.topwalk_mtp/backend/BackEnd_20150413.sh'; eval "$shar_touch") &&
  chmod 0755 '.topwalk_mtp/backend/BackEnd_20150413.sh'
if test $? -ne 0
then ${echo} 'restore of .topwalk_mtp/backend/BackEnd_20150413.sh failed'
fi
  if ${md5check}
  then (
       ${MD5SUM} -c >/dev/null 2>&1 || ${echo} '.topwalk_mtp/backend/BackEnd_20150413.sh: MD5 check failed'
       ) << \SHAR_EOF
cbef05875942e6761915280ca75d8540  .topwalk_mtp/backend/BackEnd_20150413.sh
SHAR_EOF
  else
test `LC_ALL=C wc -c < '.topwalk_mtp/backend/BackEnd_20150413.sh'` -ne 18357 && \
  ${echo} 'restoration warning:  size of .topwalk_mtp/backend/BackEnd_20150413.sh is not 18357'
  fi
fi
# ============= .topwalk_mtp/backend/BackEnd_20150413.sh.md5 ==============
if test -f '.topwalk_mtp/backend/BackEnd_20150413.sh.md5' && test "$first_param" != -c; then
  ${echo} 'x -SKIPPING .topwalk_mtp/backend/BackEnd_20150413.sh.md5 (file already exists)'
else
${echo} 'x - extracting .topwalk_mtp/backend/BackEnd_20150413.sh.md5 (text)'
  sed 's/^X//' << 'SHAR_EOF' > '.topwalk_mtp/backend/BackEnd_20150413.sh.md5' &&
cbef05875942e6761915280ca75d8540  BackEnd_20150413.sh
SHAR_EOF
  (set 20 15 04 13 15 58 59 '.topwalk_mtp/backend/BackEnd_20150413.sh.md5'; eval "$shar_touch") &&
  chmod 0644 '.topwalk_mtp/backend/BackEnd_20150413.sh.md5'
if test $? -ne 0
then ${echo} 'restore of .topwalk_mtp/backend/BackEnd_20150413.sh.md5 failed'
fi
  if ${md5check}
  then (
       ${MD5SUM} -c >/dev/null 2>&1 || ${echo} '.topwalk_mtp/backend/BackEnd_20150413.sh.md5: MD5 check failed'
       ) << \SHAR_EOF
62311c1b1a8214ec41397cc787cc8c96  .topwalk_mtp/backend/BackEnd_20150413.sh.md5
SHAR_EOF
  else
test `LC_ALL=C wc -c < '.topwalk_mtp/backend/BackEnd_20150413.sh.md5'` -ne 54 && \
  ${echo} 'restoration warning:  size of .topwalk_mtp/backend/BackEnd_20150413.sh.md5 is not 54'
  fi
fi
# ============= .topwalk_mtp/backend/conf/backend.conf ==============
if test ! -d '.topwalk_mtp/backend/conf'; then
  mkdir '.topwalk_mtp/backend/conf'
if test $? -eq 0
then ${echo} 'x - created directory `.topwalk_mtp/backend/conf'\''.'
else ${echo} 'x - failed to create directory `.topwalk_mtp/backend/conf'\''.'
  exit 1
fi
fi
if test -f '.topwalk_mtp/backend/conf/backend.conf' && test "$first_param" != -c; then
  ${echo} 'x -SKIPPING .topwalk_mtp/backend/conf/backend.conf (file already exists)'
else
${echo} 'x - extracting .topwalk_mtp/backend/conf/backend.conf (text)'
  sed 's/^X//' << 'SHAR_EOF' > '.topwalk_mtp/backend/conf/backend.conf' &&
ip_list : 
{
X  ip_list_1 : 
X  {
X    tmsip = "10.125.122.67";
X    videoip = "172.27.16.1";
X  };
X  ip_list_2 : 
X  {
X    tmsip = "10.125.122.68";
X    videoip = "172.23.32.1";
X  };
X  ip_list_3 : 
X  {
X    tmsip = "10.125.122.69";
X    videoip = "172.21.32.1";
X  };
X  ip_list_4 : 
X  {
X    tmsip = "10.125.122.70";
X    videoip = "172.22.72.1";
X  };
X  ip_list_5 : 
X  {
X    tmsip = "10.125.122.71";
X    videoip = "172.24.48.1";
X  };
X  ip_list_6 : 
X  {
X    tmsip = "10.125.122.72";
X    videoip = "172.24.16.1";
X  };
X  ip_list_7 : 
X  {
X    tmsip = "10.125.122.73";
X    videoip = "172.21.48.1";
X  };
X  ip_list_8 : 
X  {
X    tmsip = "10.125.122.74";
X    videoip = "172.23.56.1";
X  };
X  ip_list_9 : 
X  {
X    tmsip = "10.125.122.75";
X    videoip = "172.21.72.1";
X  };
X  ip_list_10 : 
X  {
X    tmsip = "10.125.122.76";
X    videoip = "172.22.64.1";
X  };
X  ip_list_11 : 
X  {
X    tmsip = "10.125.122.77";
X    videoip = "172.24.24.1";
X  };
X  ip_list_12 : 
X  {
X    tmsip = "10.125.122.78";
X    videoip = "172.23.48.1";
X  };
X  ip_list_13 : 
X  {
X    tmsip = "10.125.122.79";
X    videoip = "172.27.24.1";
X  };
X  ip_list_14 : 
X  {
X    tmsip = "10.125.122.80";
X    videoip = "172.21.16.1";
X  };
X  ip_list_15 : 
X  {
X    tmsip = "10.125.122.81";
X    videoip = "172.25.32.1";
X  };
X  ip_list_16 : 
X  {
X    tmsip = "10.125.122.82";
X    videoip = "172.21.56.1";
X  };
X  ip_list_17 : 
X  {
X    tmsip = "10.125.122.83";
X    videoip = "172.24.56.1";
X  };
X  ip_list_18 : 
X  {
X    tmsip = "10.125.122.84";
X    videoip = "172.22.32.1";
X  };
X  ip_list_19 : 
X  {
X    tmsip = "10.125.122.85";
X    videoip = "172.24.32.1";
X  };
X  ip_list_20 : 
X  {
X    tmsip = "10.125.122.86";
X    videoip = "172.26.24.1";
X  };
X  ip_list_21 : 
X  {
X    tmsip = "10.125.122.87";
X    videoip = "172.21.40.1";
X  };
X  ip_list_22 : 
X  {
X    tmsip = "10.125.122.88";
X    videoip = "172.22.16.1";
X  };
X  ip_list_23 : 
X  {
X    tmsip = "10.125.122.89";
X    videoip = "172.23.72.1";
X  };
X  ip_list_24 : 
X  {
X    tmsip = "10.125.122.90";
X    videoip = "172.22.40.1";
X  };
X  ip_list_25 : 
X  {
X    tmsip = "10.125.122.91";
X    videoip = "172.22.24.1";
X  };
X  ip_list_26 : 
X  {
X    tmsip = "10.125.122.92";
X    videoip = "172.21.88.1";
X  };
X  ip_list_27 : 
X  {
X    tmsip = "10.125.122.93";
X    videoip = "172.23.64.1";
X  };
X  ip_list_28 : 
X  {
X    tmsip = "10.125.122.94";
X    videoip = "172.21.64.1";
X  };
X  ip_list_29 : 
X  {
X    tmsip = "10.125.122.95";
X    videoip = "172.23.24.1";
X  };
X  ip_list_30 : 
X  {
X    tmsip = "10.125.122.96";
X    videoip = "172.21.80.1";
X  };
X  ip_list_31 : 
X  {
X    tmsip = "10.125.122.97";
X    videoip = "172.24.64.1";
X  };
X  ip_list_32 : 
X  {
X    tmsip = "10.125.122.98";
X    videoip = "172.26.16.1";
X  };
X  ip_list_33 : 
X  {
X    tmsip = "10.125.122.99";
X    videoip = "172.22.56.1";
X  };
X  ip_list_34 : 
X  {
X    tmsip = "10.125.122.100";
X    videoip = "172.21.24.1";
X  };
X  ip_list_35 : 
X  {
X    tmsip = "10.125.122.101";
X    videoip = "172.22.48.1";
X  };
X  ip_list_36 : 
X  {
X    tmsip = "10.125.122.102";
X    videoip = "172.24.40.1";
X  };
X  ip_list_37 : 
X  {
X    tmsip = "10.125.122.103";
X    videoip = "172.23.16.1";
X  };
X  ip_list_38 : 
X  {
X    tmsip = "10.125.122.104";
X    videoip = "172.27.32.1";
X  };
X  ip_list_39 : 
X  {
X    tmsip = "10.125.122.105";
X    videoip = "172.23.40.1";
X  };
X  ip_list_40 : 
X  {
X    tmsip = "10.125.122.106";
X    videoip = "172.25.16.1";
X  };
X  ip_list_41 : 
X  {
X    tmsip = "10.125.122.107";
X    videoip = "172.25.24.1";
X  };
};
SHAR_EOF
  (set 20 15 04 13 15 46 34 '.topwalk_mtp/backend/conf/backend.conf'; eval "$shar_touch") &&
  chmod 0644 '.topwalk_mtp/backend/conf/backend.conf'
if test $? -ne 0
then ${echo} 'restore of .topwalk_mtp/backend/conf/backend.conf failed'
fi
  if ${md5check}
  then (
       ${MD5SUM} -c >/dev/null 2>&1 || ${echo} '.topwalk_mtp/backend/conf/backend.conf: MD5 check failed'
       ) << \SHAR_EOF
c2a06b447f14124949ec66b6ed78692a  .topwalk_mtp/backend/conf/backend.conf
SHAR_EOF
  else
test `LC_ALL=C wc -c < '.topwalk_mtp/backend/conf/backend.conf'` -ne 3418 && \
  ${echo} 'restoration warning:  size of .topwalk_mtp/backend/conf/backend.conf is not 3418'
  fi
fi
# ============= .topwalk_mtp/backend/bin/backend ==============
if test ! -d '.topwalk_mtp/backend/bin'; then
  mkdir '.topwalk_mtp/backend/bin'
if test $? -eq 0
then ${echo} 'x - created directory `.topwalk_mtp/backend/bin'\''.'
else ${echo} 'x - failed to create directory `.topwalk_mtp/backend/bin'\''.'
  exit 1
fi
fi
if test -f '.topwalk_mtp/backend/bin/backend' && test "$first_param" != -c; then
  ${echo} 'x -SKIPPING .topwalk_mtp/backend/bin/backend (file already exists)'
else
${echo} 'x - extracting .topwalk_mtp/backend/bin/backend (binary)'
  sed 's/^X//' << 'SHAR_EOF' | uudecode &&
begin 600 .topwalk_mtp/backend/bin/backend
M?T5,1@(!`0````````````(`/@`!````X`-```````!``````````.@)````
M`````````$``.``(`$``'@`;``8````%````0`````````!``$```````$``
M0```````P`$```````#``0````````@``````````P````0``````@``````
M```"0`````````)````````<`````````!P``````````0`````````!````
M!0````````````````!``````````$```````,P&````````S`8`````````
M`"````````$````&````T`8```````#0!F```````-`&8```````[`$`````
M`````@``````````(````````@````8```#X!@```````/@&8```````^`9@
M``````"0`0```````)`!````````"``````````$````!````!P"````````
M'`)````````<`D```````$0`````````1``````````$`````````%#E=&0$
M````+`8````````L!D```````"P&0```````)``````````D``````````0`
M````````4>5T9`8`````````````````````````````````````````````
M````````````"``````````O;&EB-C0O;&0M;&EN=7@M>#@V+38T+G-O+C(`
M!````!`````!````1TY5```````"````!@```!(````$````%`````,```!'
M3E4`:G+&A0Q`CAK.3[%!MOZP1QRA&,0!`````0````$`````````````````
M`````````````````````````````````````````````!H````2````````
M``````````````````$````@`````````````````````````"$````2````
M``````````````````````!?7V=M;VY?<W1A<G1?7P!L:6)C+G-O+C8`<')I
M;G1F`%]?;&EB8U]S=&%R=%]M86EN`$=,24)#7S(N,BXU``````(````"``$`
M`0`0````$`````````!U&FD)```"`#,`````````B`A@```````&`````@``
M````````````J`A@```````'`````0``````````````L`A@```````'````
M`P``````````````2(/L".AS````Z`(!``#H'0(``$B#Q`C#_S7J!"``_R7L
M!"``#Q]``/\EZ@0@`&@`````Z>#_____)>($(`!H`0```.G0____````````
M```Q[4F)T5Y(B>)(@^3P4%1)Q\`@!4``2,?!,`5``$C'Q\0$0`#HO_____20
MD$B#[`A(BP5Q!"``2(7`=`+_T$B#Q`C#D)"0D)"0D)"0D)"0D%5(B>532(/L
M"(`]@`0@``!U2[OH!F``2(L%>@0@`$B!Z^`&8`!(P?L#2(/K`4@YV',D9@\?
M1```2(/``4B)!54$(`#_%,7@!F``2(L%1P0@`$@YV'+BQ@4S!"```4B#Q`A;
MR<-F9F8N#Q^$``````!(@SU(`B```%5(B>5T$K@`````2(7`=`B_\`9@`,G_
MX,G#D)!52(GE2(/L((E][$B)=>!(C47P2(E%^+@8!D``2(U5\$B#P@)(B=9(
MB<>X`````.C`_O__2(M%^$B-4`*X(@9``$B)UDB)Q[@`````Z*/^__^X````
M`,G#D)"0D//#9F9F9F8N#Q^$``````!(B6PDV$R)9"3@2(TMBP$@`$R-)80!
M(`!,B6PDZ$R)="3P3(E\)/A(B5PDT$B#[#A,*>5!B?U)B?9(P?T#28G7Z!O^
M__](A>UT'#';#Q]``$R)^DR)]D2)[T'_%-Q(@\,!2#GK<NI(BUPD"$B+;"00
M3(MD)!A,BVPD($R+="0H3(M\)#!(@\0XPY"0D)"0D)!52(GE4TB#[`A(BP4`
M`2``2(/X_W09N]`&8``/'T0``$B#ZPC_T$B+`TB#^/]U\4B#Q`A;R<.0D$B#
M[`CH+_[__TB#Q`C#```!``(`````````````````<W1R(#T@)60*`'`@/2`E
M9`H````!&P,[(`````,```"8_O__/````/3^__]<````!/___W0````4````
M``````%Z4@`!>!`!&PP'")`!```<````'````%3^__]8`````$$.$(8"0PT&
M`E,,!P@``!0````\````D/[__P(``````````````"0```!4````B/[__XD`
M````48P%A@9?#D"#!X\"C@.-!`)8#@@``````````````/__________````
M``````#__________P`````````````````````!`````````!``````````
M#`````````"0`T````````T`````````^`5```````#U_O]O`````&`"0```
M````!0````````#@`D````````8`````````@`)````````*`````````#\`
M````````"P`````````8`````````!4````````````````````#````````
M`)`(8````````@`````````P`````````!0`````````!P`````````7````
M`````&`#0```````!P````````!(`T````````@`````````&``````````)
M`````````!@`````````_O__;P`````H`T```````/___V\``````0``````
M``#P__]O`````"`#0```````````````````````````````````````````
M````````````````````````````````````````````````````````````
M``````````````````````````````````````````#X!F``````````````
M``````````````"^`T```````,X#0````````````$=#0SH@*$=.52D@-"XT
M+C<@,C`Q,C`S,3,@*%)E9"!(870@-"XT+C<M-"D``"YS>6UT86(`+G-T<G1A
M8@`N<VAS=')T86(`+FEN=&5R<``N;F]T92Y!0DDM=&%G`"YN;W1E+F=N=2YB
M=6EL9"UI9``N9VYU+FAA<V@`+F1Y;G-Y;0`N9'EN<W1R`"YG;G4N=F5R<VEO
M;@`N9VYU+G9E<G-I;VY?<@`N<F5L82YD>6X`+G)E;&$N<&QT`"YI;FET`"YT
M97AT`"YF:6YI`"YR;V1A=&$`+F5H7V9R86UE7VAD<@`N96A?9G)A;64`+F-T
M;W)S`"YD=&]R<P`N:F-R`"YD>6YA;6EC`"YG;W0`+F=O="YP;'0`+F1A=&$`
M+F)S<P`N8V]M;65N=```````````````````````````````````````````
M```````````````````````````````````````````````;`````0````(`
M``````````)``````````@```````!P````````````````````!````````
M````````````(P````<````"`````````!P"0```````'`(````````@````
M````````````````!````````````````````#$````'`````@`````````\
M`D```````#P"````````)`````````````````````0`````````````````
M``!$````]O__;P(`````````8`)```````!@`@```````!P`````````!0``
M```````(````````````````````3@````L````"`````````(`"0```````
M@`(```````!@``````````8````!````"``````````8`````````%8````#
M`````@````````#@`D```````.`"````````/P````````````````````$`
M``````````````````!>````____;P(`````````(`-````````@`P``````
M``@`````````!0`````````"``````````(`````````:P```/[__V\"````
M`````"@#0```````*`,````````@``````````8````!````"```````````
M`````````'H````$`````@````````!(`T```````$@#````````&```````
M```%``````````@`````````&`````````"$````!`````(`````````8`-`
M``````!@`P```````#``````````!0````P````(`````````!@`````````
MC@````$````&`````````)`#0```````D`,````````8````````````````
M````!````````````````````(D````!````!@````````"H`T```````*@#
M````````,`````````````````````0`````````$`````````"4`````0``
M``8`````````X`-```````#@`P```````!@"```````````````````0````
M````````````````F@````$````&`````````/@%0```````^`4````````.
M````````````````````!````````````````````*`````!`````@``````
M```(!D````````@&````````(@````````````````````@`````````````
M``````"H`````0````(`````````+`9````````L!@```````"0`````````
M```````````$````````````````````M@````$````"`````````%`&0```
M````4`8```````!\````````````````````"````````````````````,``
M```!`````P````````#0!F```````-`&````````$```````````````````
M``@```````````````````#'`````0````,`````````X`9@``````#@!@``
M`````!`````````````````````(````````````````````S@````$````#
M`````````/`&8```````\`8````````(````````````````````"```````
M`````````````-,````&`````P````````#X!F```````/@&````````D`$`
M```````&``````````@`````````$`````````#<`````0````,`````````
MB`A@``````"("`````````@````````````````````(``````````@`````
M````X0````$````#`````````)`(8```````D`@````````H````````````
M````````"``````````(`````````.H````!`````P````````"X"&``````
M`+@(````````!`````````````````````0```````````````````#P````
M"`````,`````````P`A@``````"\"````````!`````````````````````(
M````````````````````]0````$````P````````````````````O`@`````
M```L`````````````````````0`````````!`````````!$````#````````
M`````````````````.@(````````_@````````````````````$`````````
M```````````!`````@````````````````````````!H$0`````````&````
M````'0```"X````(`````````!@`````````"0````,`````````````````
M````````:!<```````#O`0```````````````````0``````````````````
M```````````````````````````````````````#``$```)`````````````
M```````````#``(`'`)````````````````````````#``,`/`)`````````
M```````````````#``0`8`)````````````````````````#``4`@`)`````
M```````````````````#``8`X`)````````````````````````#``<`(`-`
M```````````````````````#``@`*`-````````````````````````#``D`
M2`-````````````````````````#``H`8`-````````````````````````#
M``L`D`-````````````````````````#``P`J`-`````````````````````
M```#``T`X`-````````````````````````#``X`^`5`````````````````
M```````#``\`"`9````````````````````````#`!``+`9`````````````
M```````````#`!$`4`9````````````````````````#`!(`T`9@````````
M```````````````#`!,`X`9@```````````````````````#`!0`\`9@````
M```````````````````#`!4`^`9@```````````````````````#`!8`B`A@
M```````````````````````#`!<`D`A@```````````````````````#`!@`
MN`A@```````````````````````#`!D`P`A@```````````````````````#
M`!H```````````````````````$````"``T`#`1``````````````````!$`
M```$`/'_`````````````````````!P````!`!(`T`9@````````````````
M`"H````!`!,`X`9@`````````````````#@````!`!0`\`9@````````````
M`````$4````"``T`,`1``````````````````%L````!`!D`P`A@```````!
M`````````&H````!`!D`R`A@```````(`````````'@````"``T`H`1`````
M`````````````!$````$`/'_`````````````````````(0````!`!(`V`9@
M`````````````````)$````!`!$`R`9``````````````````)\````!`!0`
M\`9@`````````````````*L````"``T`P`5``````````````````,$````$
M`/'_`````````````````````,4````!`!<`D`A@`````````````````-L`
M`````!(`S`9@`````````````````.P``````!(`S`9@````````````````
M`/\````!`!4`^`9@``````````````````@!```@`!@`N`A@````````````
M`````!,!```2`````````````````````````"<!```2``T`(`5````````"
M`````````#<!```2``T`X`-``````````````````#X!```@````````````
M`````````````$T!```@`````````````````````````&$!```2``X`^`5`
M`````````````````&<!```2`````````````````````````(8!```1``\`
M"`9````````$`````````)4!```0`!@`N`A@`````````````````*(!```1
M`@\`$`9``````````````````*\!```1`A,`Z`9@`````````````````+P!
M```2``T`,`5```````")`````````,P!```0`/'_O`A@````````````````
M`-@!```0`/'_T`A@`````````````````-T!```0`/'_O`A@````````````
M`````.0!```2``T`Q`1```````!8`````````.D!```2``L`D`-`````````
M``````````!C86QL7V=M;VY?<W1A<G0`8W)T<W1U9F8N8P!?7T-43U)?3$E3
M5%]?`%]?1%1/4E],25-47U\`7U]*0U)?3$E35%]?`%]?9&]?9VQO8F%L7V1T
M;W)S7V%U>`!C;VUP;&5T960N-C,T.0!D=&]R7VED>"XV,S4Q`&9R86UE7V1U
M;6UY`%]?0U1/4E]%3D1?7P!?7T9204U%7T5.1%]?`%]?2D-27T5.1%]?`%]?
M9&]?9VQO8F%L7V-T;W)S7V%U>`!A+F,`7T=,3T)!3%]/1D93151?5$%"3$5?
M`%]?:6YI=%]A<G)A>5]E;F0`7U]I;FET7V%R<F%Y7W-T87)T`%]$64Y!34E#
M`&1A=&%?<W1A<G0`<')I;G1F0$!'3$E"0U\R+C(N-0!?7VQI8F-?8W-U7V9I
M;FD`7W-T87)T`%]?9VUO;E]S=&%R=%]?`%]*=E]296=I<W1E<D-L87-S97,`
M7V9I;FD`7U]L:6)C7W-T87)T7VUA:6Y`0$=,24)#7S(N,BXU`%])3U]S=&1I
M;E]U<V5D`%]?9&%T85]S=&%R=`!?7V1S;U]H86YD;&4`7U]$5$]27T5.1%]?
M`%]?;&EB8U]C<W5?:6YI=`!?7V)S<U]S=&%R=`!?96YD`%]E9&%T80!M86EN
'`%]I;FET`%]C
`
end
SHAR_EOF
  (set 20 15 04 13 15 45 40 '.topwalk_mtp/backend/bin/backend'; eval "$shar_touch") &&
  chmod 0755 '.topwalk_mtp/backend/bin/backend'
if test $? -ne 0
then ${echo} 'restore of .topwalk_mtp/backend/bin/backend failed'
fi
  if ${md5check}
  then (
       ${MD5SUM} -c >/dev/null 2>&1 || ${echo} '.topwalk_mtp/backend/bin/backend: MD5 check failed'
       ) << \SHAR_EOF
162c7197a0a1e7cdcaf93af94e6a670d  .topwalk_mtp/backend/bin/backend
SHAR_EOF
  else
test `LC_ALL=C wc -c < '.topwalk_mtp/backend/bin/backend'` -ne 6487 && \
  ${echo} 'restoration warning:  size of .topwalk_mtp/backend/bin/backend is not 6487'
  fi
fi
if rm -fr ${lock_dir}
then ${echo} 'x - removed lock directory `'${lock_dir}\''.'
else ${echo} 'x - failed to remove lock directory `'${lock_dir}\''.'
  exit 1
fi
TMP_DIR=.topwalk_mtp
clear
echo "Start install ..."

#install backend
/bin/sh $TMP_DIR/backend/*.sh

#install frontend
/bin/sh $TMP_DIR/frontend/*.sh

rm -rf $TMP_DIR
exit 0
