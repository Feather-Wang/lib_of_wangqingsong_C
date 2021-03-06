# sed是一个很好的文件处理工具，本身是一个管道命令，主要是以行为单位进行处理，可以将数据行进行替换、删除、新增、选取等特定工作，下面先了解一下sed的用法
# sed命令行格式为：sed [-nefri] ‘command’ 输入文本        

# 常用选项：
# -n∶使用安静(silent)模式。在一般 sed 的用法中，所有来自STDIN的资料一般都会被列出到萤幕上。但如果加上-n参数后，则只有经过sed 特殊处理的那一行(或者动作)才会被列出来。
# -e∶直接在指令列模式上进行 sed 的动作编辑；
# -f∶直接将 sed 的动作写在一个档案内， -f filename 则可以执行 filename 内的sed 动作；
# -r∶sed的动作支援的是延伸型正规表示法的语法。(预设是基础正规表示法语法) 
# -i∶直接修改读取的档案内容，而不是由萤幕输出。       

# 常用命令：
# a   ∶新增， a的后面可以接字串，而这些字串会在新的一行出现(目前的下一行)～
# c   ∶取代， c的后面可以接字串，这些字串可以取代n1,n2之间的行！
# d   ∶删除，因为是删除啊，所以d后面通常不接任何咚咚;
# i   ∶插入，i的后面可以接字串，而这些字串会在新的一行出现(目前的上一行)；
# p   ∶列印，亦即将某个选择的资料印出。通常p会与参数sed -n一起运作～
# s   ∶取代，可以直接进行取代的工作哩！通常这个s的动作可以搭配正规表示法！例如: 1,20s/old/new/g就是啦！

# 示例

# 选项 -n 的运用，如果不加-n，就会把所有的内容都打印出来
echo "1..........................."
sed -n "1p" file 

# 命令 p 的运用，用于显示指定的行
# 格式 ：sed -n '行号p' filename，行号可以是1，也可以是$($表示最后一行，如果使用该符号表示最后一行，外层的引号必须是单引号)，也可以是1,2这种指定范围的行号
# 在shell中使用$p打印最后一行的时候，如果$p使用双引号括起来的，p就会被认为是一个变量，从而最后一行打印失败，这时候就需要把双引号改为单引号
# 显示第一行
echo "2..........................."
sed -n "1p" file 
# 显示最后一行
echo "3..........................."
sed -n '$p' file 
# 显示第一行到第二行
echo "4..........................."
sed -n "1,2p" file 
# 显示第二行到最后一行
echo "5..........................."
sed -n '2,$p' file

# 命令 a 的运用，用于新增一行
# 格式 ：sed '行号a 字符串' filename，行号可以是1，也可以是$($表示最后一行，如果使用该符号表示最后一行，外层的引号必须是单引号)，也可以是1,2这种指定范围的行号

# 在第一行后面新增一行
echo "6..........................."
sed '1a hello world' file
# 在最后一行后面新增一行
echo "7..........................."
sed '$a hello world' file
# 在第一行到第二行，每行后面都新增一行
echo "8..........................."
sed '1,2a hello world' file
# 在第一行到第二行，每行后面都新增多行，这里需要用到特殊字符，换行符
echo "9..........................."
sed '1,2a hello\nworld' file

# 命令 c 的运用，用指定字符串取代指定的行
# 格式 ：sed '行号c 用于取代的字符串' filename，行号可以是1，也可以是$($表示最后一行，如果使用该符号表示最后一行，外层的引号必须是单引号)，也可以是1,2这种指定范围的行号

# 用hello world取代第一行
echo "10..........................."
sed '1c hello world' file
# 用hello world取代第一行到第二行，这里并不是每行进行一次替换，而是把第一行和第二行作为整体，替换成一行字符串hello world
echo "11..........................."
sed '1,2c hello world' file

# 命令 s 的运用，类似于命令c，也是用于取代，只不过对象是字符串，而不是行
# 格式 ：sed '行号s/old string/new string/g' filename，
# 行号，可以不指定，则扫描所有的行，也可以指定，可以是1，也可以是$($表示最后一行，如果使用该符号表示最后一行，外层的引号必须是单引号)，也可以是1,2这种指定范围的行号
# old string和new string可以是正则表达式，也可以是字符串，最后的g用于扫描整行所有的字符串，如果不加g的话，只取代匹配到的第一个字符串，如果后面还有匹配的，就不会取代了

# 用hello取代wang
echo "12..........................."
sed '2s/wang/hello/' file
# 先匹配wang，然后用hello取代匹配到的行中的wang
echo "13..........................."
sed '/wang/s/wang/hello/' file

# 命令 d 的运用，删除匹配字符串的行，如果匹配字符串是引用的变量，外层的引号必须是双引号，而不能用单引号
# 格式 ：
# sed '/string/d' filename，删除匹配string的行
# sed '行号d' filename，删除指定行号的行
# 行号，可以不指定，则删除所有的行，也可以指定，可以是1，也可以是$($表示最后一行，如果使用该符号表示最后一行，外层的引号必须是单引号)，也可以是1,2这种指定范围的行号

# 删除 含有字符串wang的行
echo "14..........................."
sed '/wang/d' file
# 删除 含有字符串wang的行
echo "15..........................."
sed '2d' file
# 删除，除含有字符串wang以外的行
sed '/wang/!d' file

# 命令 i 的运用。在指定行的前一行插入
# 格式：
# sed '行号i 要插入的字符串' filename
# sed '/匹配的字符串/i 要插入的字符串' filename
# 行号，可以不指定，则在每一行前都插入，也可以指定，可以是1，也可以是$($表示最后一行，如果使用该符号表示最后一行，外层的引号必须是单引号)，也可以是1,2这种指定范围的行号

# 在所有行的前面都插入一行hello world
echo "16..........................."
sed 'i hello world' file
# 在第一行的前面都插入一行hello world
echo "17..........................."
sed '1i hello world' file
# 在最后一行的前面都插入一行hello world
echo "18..........................."
sed '$i hello world' file
# 查找匹配字符串的行，然后在前面都插入一行hello world
echo "19..........................."
sed '/song/i hello world' file
