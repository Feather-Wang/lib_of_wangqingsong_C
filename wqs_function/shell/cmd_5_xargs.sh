# -i: 指定替换符，如果不指定得话，则使用{}作为默认替换符
# -I: 指定替换符，与-i不同得是，必须明确指定替换符，才能使用
echo "1................................."
echo "1,2,3,4,5" | xargs -i echo {}

echo "2................................."
echo "1,2,3,4,5" | xargs -I '/' echo /

# -d: 指定分隔符，如果不通过-d指定分隔符，则默认使用'\0'作为分隔符
# -0(数字0): 相当于-d '\0'
echo "3................................."
echo "1,2,3,4,5" | xargs -d ',' -i echo {}

echo "4................................."
echo "1,2,3,4,5" | xargs -0 -i echo {}

# -n: 指定每次打印n个参数，默认是打印所有的参数
echo "5................................."
echo "1,2,3,4,5" | xargs -d ',' -n 2

# -p: 询问是否执行xargs后面的操作，xargs默认执行echo
echo "6................................."
echo "1,2,3,4,5" | xargs -d ',' -p
