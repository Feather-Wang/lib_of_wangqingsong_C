
cc -c *.c
ar crv libwqs.a wqs_*.o

`del wqs_*.o`

PATH_include=$LIB/include/libwqs/
PATH_lib=$LIB/lib

if [ ! -x "$PATH_include" ]
then
	mkdir $PATH_include
fi

cp wqs_*.h $PATH_include
cp libwqs.a $PATH_lib
