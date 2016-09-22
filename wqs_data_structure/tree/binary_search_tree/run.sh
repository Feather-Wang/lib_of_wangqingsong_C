cc *.c
i=1
while [ 1 ]
do
    echo "i = $i"
    i=`expr $i + 1`
    ./a.out
done
