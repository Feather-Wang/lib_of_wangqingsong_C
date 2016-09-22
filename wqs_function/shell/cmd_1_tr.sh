# 替换字符，一一对应，使用1替换w，使用2替换q，如果第二个字符串不够，就使用最后一个字符 
echo "1."
echo "wangqingsong" | tr wqs 123

# 删除制定的字符
echo "2."
echo "anaq" | tr -d n

# -s为去重选项，如果第二个字符串存在，去重后就去替换，如果去重后，仍然有连续重复的字符，再去一次重
echo "3."
echo "wannsg" | tr -s n s

# -：连接符，a-z表示从字符a到字符z的所有字符
echo "4."
echo "wanng" | tr -s a-z A-Z

# 字符类：格式为[:类名:]，具体有哪些字符类，请看笔记
echo "5."
echo "wanng" | tr -s [:lower:] [:upper:]

#
echo "6."
echo "wassg" | tr -s [s*3] n 

# 双引号：平时不加双引号也行，但是有些情况需要使用双引号
echo "7."
echo -e "Hello-World" | tr "-" " "

# -c：补码，意外除了第一个字符串中的字符外，其他所有的字符
echo "8."
echo "abcg" | tr -c "g\n" w

# 删除空行，不能用-d，否则就会变成wangqingsong，而且输出后不会换行
echo "9."
echo "wang

qing

song" | tr -s "\r\n"
