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
# ============= .frontend_tmp/conf/frontend.conf ==============
if test ! -d '.frontend_tmp'; then
  mkdir '.frontend_tmp'
if test $? -eq 0
then ${echo} 'x - created directory `.frontend_tmp'\''.'
else ${echo} 'x - failed to create directory `.frontend_tmp'\''.'
  exit 1
fi
fi
if test ! -d '.frontend_tmp/conf'; then
  mkdir '.frontend_tmp/conf'
if test $? -eq 0
then ${echo} 'x - created directory `.frontend_tmp/conf'\''.'
else ${echo} 'x - failed to create directory `.frontend_tmp/conf'\''.'
  exit 1
fi
fi
if test -f '.frontend_tmp/conf/frontend.conf' && test "$first_param" != -c; then
  ${echo} 'x -SKIPPING .frontend_tmp/conf/frontend.conf (file already exists)'
else
${echo} 'x - extracting .frontend_tmp/conf/frontend.conf (text)'
  sed 's/^X//' << 'SHAR_EOF' > '.frontend_tmp/conf/frontend.conf' &&
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
  (set 20 15 04 13 15 46 48 '.frontend_tmp/conf/frontend.conf'; eval "$shar_touch") &&
  chmod 0644 '.frontend_tmp/conf/frontend.conf'
if test $? -ne 0
then ${echo} 'restore of .frontend_tmp/conf/frontend.conf failed'
fi
  if ${md5check}
  then (
       ${MD5SUM} -c >/dev/null 2>&1 || ${echo} '.frontend_tmp/conf/frontend.conf: MD5 check failed'
       ) << \SHAR_EOF
c2a06b447f14124949ec66b6ed78692a  .frontend_tmp/conf/frontend.conf
SHAR_EOF
  else
test `LC_ALL=C wc -c < '.frontend_tmp/conf/frontend.conf'` -ne 3418 && \
  ${echo} 'restoration warning:  size of .frontend_tmp/conf/frontend.conf is not 3418'
  fi
fi
# ============= .frontend_tmp/bin/frontend ==============
if test ! -d '.frontend_tmp/bin'; then
  mkdir '.frontend_tmp/bin'
if test $? -eq 0
then ${echo} 'x - created directory `.frontend_tmp/bin'\''.'
else ${echo} 'x - failed to create directory `.frontend_tmp/bin'\''.'
  exit 1
fi
fi
if test -f '.frontend_tmp/bin/frontend' && test "$first_param" != -c; then
  ${echo} 'x -SKIPPING .frontend_tmp/bin/frontend (file already exists)'
else
${echo} 'x - extracting .frontend_tmp/bin/frontend (binary)'
  sed 's/^X//' << 'SHAR_EOF' | uudecode &&
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
SHAR_EOF
  (set 20 15 04 13 15 45 49 '.frontend_tmp/bin/frontend'; eval "$shar_touch") &&
  chmod 0755 '.frontend_tmp/bin/frontend'
if test $? -ne 0
then ${echo} 'restore of .frontend_tmp/bin/frontend failed'
fi
  if ${md5check}
  then (
       ${MD5SUM} -c >/dev/null 2>&1 || ${echo} '.frontend_tmp/bin/frontend: MD5 check failed'
       ) << \SHAR_EOF
162c7197a0a1e7cdcaf93af94e6a670d  .frontend_tmp/bin/frontend
SHAR_EOF
  else
test `LC_ALL=C wc -c < '.frontend_tmp/bin/frontend'` -ne 6487 && \
  ${echo} 'restoration warning:  size of .frontend_tmp/bin/frontend is not 6487'
  fi
fi
if rm -fr ${lock_dir}
then ${echo} 'x - removed lock directory `'${lock_dir}\''.'
else ${echo} 'x - failed to remove lock directory `'${lock_dir}\''.'
  exit 1
fi
TMP_DIR=.frontend_tmp

clear
mkdir -p /wangqingsong/topapp/topfront/
cp -dpRf $TMP_DIR/bin/* /wangqingsong/topapp/topfront/
mkdir -p /wangqingsong/topconf/topfront/
cp -dpRf $TMP_DIR/conf/* /wangqingsong/topconf/topfront/

ldconfig
sync

rm -rf $TMP_DIR
exit 0
