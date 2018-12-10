# awk是一个强大的文本分析工具，相对于grep的查找，sed的编辑，awk在其对数据分析并生成报告时，显得尤为强大。简单来说awk就是把文件逐行的读入，以空格为默认分隔符将每行切片，切开的部分再进行各种分析处理。
# awk有3个不同版本: awk、nawk和gawk，未作特别说明，一般指gawk，gawk 是 AWK 的 GNU版本。
# awk其名称得自于它的创始人 Alfred Aho 、Peter Weinberger 和 Brian Kernighan姓氏的首个字母。实际上 AWK 的确拥有自己的语言： AWK 程序设计语言 ，三位创建者已将它正式定义为“样式扫描和处理语言”。它允许您创建简短的程序，这些程序读取输入文件、为数据排序、处理数据、对输入执行计算以及生成报表，还有无数其他的功能。

# 相关名词（只限于本文章，每个人用的名词可能有差异）
# 记录：因为默认的记录分隔符是换行符，所以一个记录一般指一行
# 域：因为默认的域分隔符是空格，所以一个域值用空格分开的字符串。例如wang qing song，这一条记录有3个域，第一个域为wang，第二个域为qing，第三个域为song
# 语句块: 一个{}称为一个语句块，语句块内可以有一个或多个语句，多个语句由分号隔开。多个语句块之间不需要分隔符，语句块内可以嵌入语句块

# awk格式
# awk [options] 'script' var=value file(s) 
# awk [options] -f scriptfile var=value file(s)
# awk组成
# awk脚本是由模式和操作组成的。
# 模式：模式可以是以下任意一个：
# 1、/正则表达式/：使用通配符的扩展集。
# 2、关系表达式：使用运算符进行操作，可以是字符串或数字的比较测试。
# 3、模式匹配表达式：用运算符~（匹配）和~!（不匹配）。
# 4、BEGIN语句块、pattern语句块、END语句块：参见awk的工作原理 
# 正则表达式模式，例子中，匹配了字符ｅ，只有包含字符ｅ的记录才会被处理
echo " 0........................................"
echo "wang qing song
Hello World
you are welcome" | awk '/e/{print $0}'
# 关系表达式模式
echo " 1........................................"
echo "" | awk -v var=1 'var==1{print "var == 1"} var!=1{print "var != 1"}'
# 模式匹配表达式模式
# 这个模式还没接触过

# BEGIN、END模式
echo " 2........................................"
echo "this
is
BEGIN" | awk 'BEGIN{print "Init..."}
{print $0}
END{print "BEGIN ENd"}'

# 操作：操作由一个或多个命令、函数、表达式组成，之间由换行符或分号隔开，并位于大括号内，主要部分是：
# 1、变量或数组赋值 
# 2、输出命令 
# 3、内置函数 
# 4、控制流语句 

# awk脚本基本结构
# awk 'BEGIN{ print "start" } pattern{ commands } END{ print "end" }' file
# 一个awk脚本通常由：BEGIN语句块、能够使用模式匹配的通用语句块、END语句块3部分组成，这三个部分是可选的。任意一个部分都可以不出现在脚本中，脚本通常是被单引号或双引号中，例如：
# awk 'BEGIN{ i=0 } { i++ } END{ print i }' filename 
# awk "BEGIN{ i=0 } { i++ } END{ print i }" filename 

# awk的工作原理 
# awk 'BEGIN{ commands } pattern{ commands } END{ commands }' 
# 第一步：执行BEGIN{ commands }语句块中的语句；
# 第二步：从文件或标准输入(stdin)读取一行，然后执行pattern{ commands }语句块，它逐行扫描文件，从第一行到最后一行重复这个过程，直到文件全部被读取完毕。
# 第三步：当读至输入流末尾时，执行END{ commands }语句块。
# BEGIN语句块在awk开始从输入流中读取行之前被执行，这是一个可选的语句块，比如变量初始化、打印输出表格的表头等语句通常可以写在BEGIN语句块中。
# END语句块在awk从输入流中读取完所有的行之后即被执行，比如打印所有行的分析结果这类信息汇总都是在END语句块中完成，它也是一个可选语句块。
# pattern语句块中的通用命令是最重要的部分，它也是可选的。如果没有提供pattern语句块，则默认执行{ print }，即打印每一个读取到的行，awk读取的每一行都会执行该语句块。 
# 示例 echo -e "A line 1nA line 2" | awk 'BEGIN{ print "Start" } { print } END{ print "End" }'
# Start 
# A line 1 
# A line 2 
# End
# 当使用不带参数的print时，它就打印当前行，当print的参数是以逗号进行分隔时，打印时则以空格作为定界符。在awk的print语句块中双引号是被当作拼接符使用，例如：
# echo | awk '{ var1="v1"; var2="v2"; var3="v3"; print var1,var2,var3; }' v1 v2 v3
# 双引号拼接使用： echo | awk '{ var1="v1"; var2="v2"; var3="v3"; print var1"="var2"="var3; }' v1=v2=v3 
# {}类似一个循环体，会对文件中的每一行进行迭代，通常变量初始化语句（如：i=0）以及打印文件头部的语句放入BEGIN语句块中，将打印的结果等语句放在END语句块中。

# 选项：
# -F：指定分隔符，例如：-F:，表示用分号作为分隔符，分隔符和-F之间可以有空格，也可以没有，如果不指定的话，awk默认是空格和/t符作为分隔符，分隔符也可以使用双引号括起来，如果分隔符是换行符的话，就需要使用双引号括起来了
# -f：格式：-f progfile。允许awk调用并执行progfile程序文件，当然，progfile必须是一个符合awk语法的程序文件
# -v：变量赋值，例如：shell脚本有变量value=100，awk -v var=$value '{print var}'，100将会被打印出来，通过这种方式，在shell和awk之间传递字符串时需要注意，字符串中不能含有空白符和空格，而且，一个-v只能对应一个变量赋值，多个变量需要赋值时，每个变量前都要有一个-v
# -m[fr] val：对val值设置内在限制，-mf选项限制分配给val的最大块数；-mr选项限制记录的最大数目。这两个功能是Bell实验室版awk的扩展功能，在标准awk中不适用

# 示例
# -F 指定:为分隔符
echo " 3........................................"
echo "wang:qing:song" | awk -F: '{print $1,22$3}' # -F:也可以写成-F :或者-F ":"
# -v 赋值
echo " 4........................................"
wang="HelloWorld"
echo "" | awk -v var=$wang '{print var}' # 除了-v传递外可以写这样 echo "" | awk '{print "'$wang'"}'
echo " 5........................................"
qing=10
echo "" | awk '{print '$qing'}'
echo " 6........................................"
echo "" | awk '{print "Hello"}{print "World";print "wangqingsong";{print "Hello World"}}'

# 内置变量
# 说明：[A][N][P][G]表示第一个支持变量的工具，[A]=awk、[N]=nawk、[P]=POSIXawk、[G]=gawk
# [N] ARGC	　			命令行参数个数，包括awk本身，当没有参数时，ARGC=1，就是只有awk本身
# [N] ARGV				命令行参数排列，ARGV是一个一维数组，与Ｃ语言基本一样，ARGV[0]为awk本身，ARGV[1]为第一个参数，以此类推
# [G] ARGIND			当前被处理文件的ARGV标志符，并不是指ARGV的下标，ARGIND比ARGV当前的下标小１，也就是ARGIND不包含awk本身，而且用print不能打印出当前的ARGIND的指
# [P] ENVIRON			支持队列中系统环境变量的使用，获取系统环境变量的值，格式：ENVIRON["环境变量名"]
# [A] FILENAME          awk当前处理的文件的名字
# [P] FNR               浏览文件的记录数，当前文件浏览的第几次，从１开始计数
# [A] NR				已读的记录数，当前浏览的第几次，从１开始计数，与FNR不同的是，FNR是文件的记录数，浏览新的文件后，FNR会被清零，NR不会清零
# [A] NF                浏览记录的域的个数，当前这条记录的域的个数
# [A] FS                设置输入域分隔符，等价于命令行 -F选项
# [A] OFS               输出域分隔符
# [A] ORS               输出记录分隔符
# [A] RS                输入记录分隔符
# [N] RSTART 			由match函数所匹配的字符串的第一个位置。
# [N] RLENGTH 			由match函数所匹配的字符串的长度。
# [N] SUBSEP 			数组下标分隔符（默认值是34）。
# [G] CONVFMT 			数字转换格式（默认值为%.6g）。
# [N] ERRNO 			最后一个系统错误的描述。
# [G] FIELDWIDTHS 		字段宽度列表（用空格键分隔）。
# [G] IGNORECASE 		如果为真，则进行忽略大小写的匹配。
# [A] OFMT 				数字的输出格式（默认值是%.6g）。
# $0-$n					$0表示所有的域，$1表示第一个域，以此类推
# 命令行参数，是指跟在awk后面的参数，不包括选项和执行语句，例如awk -F ":" '{print $0}' file1 file2，在这个awk语句中，只有awk,file1,file2是参数，其中ARGV[0]是awk本身，这跟Ｃ语言基本一样
echo " 7........................................"
echo "wang:qing:song" | awk -F: '{print NR}'
echo " 8........................................"
echo "wang:qing:song" | awk -F: '{print "ARGC="ARGC}{print "ARGV="ARGV[0]}'
# ARGIND的值为当前处理(ARGIND不知道为什么不管作用），FILENAME显示当前处理的文件的名字
echo " 9........................................"
awk 'ARGIND==0{print "first file"FILENAME}ARGIND==1{print "second file"FILENAME}' /etc/rc.local /etc/profile
# ENVIRON用来获取环境变量的值，格式：ENVIRON["环境变量名"]
echo "10........................................"
echo "" | awk '{print ENVIRON["PATH"]}'
# FNR、NR、NF
echo "11........................................"
awk '{printf "FNR = "FNR; printf ", NR = "NR; print ", NF = "NF}' /etc/rc.local
# RS,ORS：RS可以使用正则表达式，例如：RS="[a-z]:"
echo "12........................................"
echo "wang:qing:song" | awk 'BEGIN{RS=":";ORS="-"}{print "this record is "$1}END{print "this awk End"}'
# FS,OFS：FS可以使用正则表达式
echo "13........................................"
echo "wang:qing:song" | awk 'BEGIN{FS=":";OFS="-"}{print $1,$2,$3}END{}'
# OFMT
echo "14........................................"
echo "" | awk 'BEGIN{OFMT="%.2f"} {i=sin(1); print i}'

# 自定义变量
# 在awk中变量没有类型，直接使用变量名就行，读取变量的值时，直接使用变量名就可以
echo "15........................................"
echo "" | awk '{value="Hello World";print value}'
# 需要说明的是，在shell中使用awk，如果shell中的变量需要在awk中使用的三种方式
# 1、可以通过-v选项设置awk的变量，然后把shell的变量赋值给awk的变量，
str1="Hello"
value1=100
echo "16........................................"
echo "" | awk -v str=$str1 -v var=$value1 '{print "str="str", var="var}'
# 2、也可以在awk中直接引用变量，不过需要有点变化，如果引用的是字符串，需要在$value外层加上"''"一层双引号和一层单引号，如果是数值，只需要一层单引号就可以了
echo "17........................................"
echo "" | awk '{print "str='$str1', var='$value1'"}'
# 3、在命令行参数中定义awk变量，然后把shell的变量赋值给awk变量
echo "18........................................"
echo "" | awk '{print "str is "str", var is "var}' str=$str1 var=$value1

# 数组
# 数组是awk的灵魂，处理文本中最不能少的就是它的数组处理。因为数组索引（下标）可以是数字和字符串在awk中数组叫做关联数组(associative arrays)。awk中的数组不必提前声明，也不必声明大小。数组元素用0或空字符串来初始化，这根据上下文而定。
# 数字做数组索引（下标）：
echo "19........................................"
echo "" | awk '{ i=1;Array[1]="Array 1"; print "Array[1]="Array[i]}'
# 字符串做数组索引（下标）：
echo "20........................................"
echo "" | awk '{ i="first";Array["first"]="Array first"; print "Array[\"first\"]="Array[i]}'
# 读取数组的值
# { for(item in array) {print array[item]}; }       #这种方式一般用于访问数组，这种方式是一种无序访问，而且访问时，是从下标1开始访问，没有下标0
# { for(i=1;i<=len;i++) {print array[i]}; }         #Len是数组的长度
# 判断键值是否存在
echo "21........................................"
echo "" | awk '{
array["first"]="Array first";
array["second"]="Array second";
if( array["first"] == "1" ){
	print "== not found";
}
else{
	print "== found";
}
}'
echo "22........................................"
echo "" | awk '{
array["first"]="Array first";
array["second"]="Array second";
if( "third" in array ){
	print "in found";
}
else{
	print "in not found";
}
}'
# 删除键值对应的数组元素
echo "23........................................"
echo "" | awk '{
array["first"]="Array first";
array["second"]="Array second";
if( "first" in array ){
	print "delete found";
}
else{
	print "delete not found";
}
delete array["first"];
if( "first" in array ){
	print "delete found";
}
else{
	print "delete not found";
}
}'
# 多维数组
# awk的多维数组在本质上是一维数组，更确切一点，awk在存储上并不支持多维数组。awk提供了逻辑上模拟二维数组的访问方式。例如，array[2,4]=1这样的访问是允许的。awk使用一个特殊的字符串SUBSEP(�34)作为分割字段，在上面的例子中，关联数组array存储的键值实际上是2�344。 类似一维数组的成员测试，多维数组可以使用if ( (i,j) in array)这样的语法，但是下标必须放置在圆括号中。类似一维数组的循环访问，多维数组使用for ( item in array )这样的语法遍历数组。与一维数组不同的是，多维数组必须使用split()函数来访问单独的下标分量。
# 类似一维数组的成员测试，多维数组可以使用if ( (i,j) in array)这样的语法，但是下标必须放置在圆括号中。类似一维数组的循环访问，多维数组使用for ( item in array )这样的语法遍历数组。与一维数组不同的是，多维数组必须使用split()函数来访问单独的下标分量。

# 控制语句
# if语句：与C语言基本一样，例如：if($1 == 5){print "Hello World"}
# 还有while、do{}while、for、break、continue都与C语言基本一样

# if{}else if{}else{}
echo "24........................................"
echo "" | awk '{
value=75;
if(value>=0&&value<60)
	{
		print "this value is C"
	}
else if(value>=60&&value<75)
	{
		print "this value is B"
	}
else
	{
		print "this value is A"
	}
}'
# while{}
echo "25........................................"
echo "" | awk '{
i=0;
while(i<5)
	{
		print "while test : i = "i;
		++i;
	}
	print "while End : i = "i;
}'
# do{}while();
echo "26........................................"
echo "" | awk '{
i=0;
	do{
		print "do{}while test : i = "i;
		++i;
	}while(i<5);
	print "do{}while End : i = "i;
}'
# for : method one
echo "27........................................"
echo "" | awk '{
for(value=0;value<5;value++)
	{
		printf("for method one : value=%d\n", value);
	}
	printf("for method one End : value=%d\n", value);
}'
# for : method two
echo "28........................................"
echo "" | awk '{
arr[0]="wang";
arr[1]="qing";
arr[2]="song";
for( i in arr )
	{
		print "for method two : i = "i", arr["i"] = "arr[i];
	}
}'

# 运算与判断
# 算术运算符，变量在进行算术运算时，非数值型都变为0
# + -		加、减
# * / &		乘、除、求余
# + - !		一元加、减和逻辑非
# ^ ***		求幂
# ++ --		自加、自减。前缀或者后缀都可以
# 赋值运算符
# = += -= *= /= %= ^= **=
# 逻辑运算符
# || 		逻辑或 
# &&		逻辑与 
# 正则运算符
# ~ ~! 		匹配正则表达式和不匹配正则表达式 
echo "29........................................"
echo "" | awk 'BEGIN{a="100testa";if(a ~ /^100*/){print "ok";}}'
# 关系运算符
# 注意：> < 可以作为字符串比较，也可以用作数值比较，关键看操作数如果是字符串就会转换为字符串比较。两个都为数字才转为数值比较。字符串比较：按照ASCII码顺序比较。
# < <= > >= != ==
# 其他运算符
# $ 		字段引用 
# 空格 		字符串连接符 
# ?: 		C条件表达式 
# in 		数组中是否存在某键值 
echo "30........................................"
echo "" | awk 'BEGIN{a="b";print a=="b"?"ok":"err";}'
echo "31........................................"
echo "" | awk 'BEGIN{a="b";arr[0]="b";arr[1]="c";print "is there \"b\" in arr ? "(a in arr);}'
echo "32........................................"
echo "" | awk 'BEGIN{a="b";arr[0]="b";arr["b"]="c";print "is there \"b\" in arr ? "(a in arr);}'
# 重定向符号
# > >>		输出重定向
echo "33........................................"
echo "" | awk '{print "Hello World" > "test_file"}'
echo "34........................................"
echo "" | awk '{print "wang qing song" >> "test_file"}'

# awk内置语句
# next		过滤掉当前记录，读取下一条记录
# break		跳出循环
# continue	跳过本次循环
# exit		进入END阶段，如果没有END语句块，则直接终止awk的执行

# 过滤掉第二天记录
echo "35........................................"
echo "wang
qing
song" | awk '{if(NR==2)next; print $0}'

# awk内置函数

# 输出函数
# print：类似于C语言中的puts，自带换行
# printf：类似于C语言中的printf，如果不指定换行的话，输出后不换行
# 输出内容的格式：$1"wang"$2"qing"，输出内容可以是变量、字符串或者数值，如果输出内容是变量或者数值的话，直接使用变量名或者数值，如果是字符串，使用双引号将字符串括起来，
# 字符串和变量或者数值需要连接的话，直接依次排开写就行，没有连接符。如果是数值和变量连接的话，数值在变量前面的话直接顺序排开就行，如果数值在变量的后面，需要在变量和数值之间加上一个空字符串，
# 也可以使用逗号，例如$1,22，只不过这样的话，变量$1和22之间会以域分隔符隔开
# printf支持与C语言相同的语法，例如：awk '{printf("Hello World --> %s", $2)}'

# printf可以使用的输出格式符
# %d	十进制有符号整数 
# %u	十进制无符号整数 
# %f	浮点数 
# %s	字符串 
# %c	单个字符 
# %p	指针的值 
# %e	指数形式的浮点数 
# %x	无符号以十六进制表示的整数，字母为小写
# %X 	无符号以十六进制表示的整数，字母为大写
# %o	无符号以八进制表示的整数 
# %g	自动选择合适的表示法

echo "36........................................"
echo "wang qing song" | awk '{print $1}'
echo "37........................................"
echo "wang:qing:song" | awk -F: '/wang/{printf("Hello World --> %s\n", $2)}'

# 算术函数
# atan2( y, x )		返回 y/x 的反正切。 
# cos( x )			返回 x 的余弦；x 是弧度。 
# sin( x )			返回 x 的正弦；x 是弧度。 
# exp( x )			返回 x 幂函数。 
# log( x )			返回 x 的自然对数。 
# sqrt( x )			返回 x 平方根。 
# int( x )			返回 x 的截断至整数的值。
# rand( )			返回任意数字 n，其中 0 <= n < 1。 
# srand( [expr] )	将 rand函数的种子值设置为 Expr 参数的值，或如果省略 Expr参数则使用某天的时间。返回先前的种子值。
echo "38........................................"
echo "" | awk '{i=3.1415926; print "int("i")="int(i)}'

# 字符串函数，在下列参数中，Ere都可以使用正则表达式 
# 格式	描述 
# gsub( Ere, Repl, [ In ] )		除了正则表达式所有匹配到的都被替代这点（ sub 只替换匹配到的第一个，然后就会结束替换），它和 sub 函数完全一样地执行。 
# sub( Ere, Repl, [ In ] )		用 Repl 参数指定的字符串替换 In 参数指定的字符串中的由 Ere 参数指定的扩展正则表达式的第一个具体值。sub 函数返回替换的数量。出现在 Repl 参数指定的字符串中的 &（和符号）由 In 参数指定的与 Ere 参数的指定的扩展正则表达式匹配的字符串替换。如果未指定 In 参数，缺省值是整个记录（$0 记录变量）。 
# index( String1, String2 ) 	查找在字符串 String1 中是否包含字符串 String2，返回值为包含字符串 String2 的个数，不包含字符串 String2，则返回0
# length[(String)]				返回 String 参数指定的字符串的长度（字符形式）。如果未给出 String 参数，则返回整个记录的长度（$0 记录变量）。 
# blength [(String)]			返回 String 参数指定的字符串的长度（以字节为单位）。如果未给出 String 参数，则返回整个记录的长度（$0 记录变量）。 
# substr( String, M, [ N ] )	从字符串 String 中截取子串，M 指定子串在 String 的开始位置（String第一个字符的位置为1），N 指定子串的长度，如果 N 未指定，则子串的长度为位置 M 到 String 的结尾，返回值：截取到的子串，如果 M 值超过 String 的长度，则返回空串
# match( String, Ere )			在字符串 String 中匹配 Ere。返回匹配的位置（位置以1开始）,没有匹配到，则返回0，与match相关的两个内置变量RSTART和RLENGTH，RSTART是匹配到的子串，在String的起始位置，序号从1开始，RLENGTH是匹配到的字符长度
# split( String, Array, [Ere] )	以 Ere 为分隔符，对字符串 String 进行分割，分割后的子串存入 Array 数组中。返回值：成功，返回切割后的子串的个数
# tolower( String )				返回 String 参数指定的字符串，字符串中每个大写字符将更改为小写。大写和小写的映射由当前语言环境的 LC_CTYPE 范畴定义。 
# toupper( String )				返回 String 参数指定的字符串，字符串中每个小写字符将更改为大写。大写和小写的映射由当前语言环境的 LC_CTYPE 范畴定义。 
# sprintf(Format, Expr, Expr, . . . )	根据 Format 参数指定的 printf 子例程格式字符串来格式化 Expr 参数指定的表达式并返回最后生成的字符串。

# gsub
echo "39........................................"
echo "" | awk '{info="wang123qing456song"; gsub( /[0-9]+/, "-", info ); print info}'
echo "40........................................"
echo "" | awk '{info="wang123qing456song"; sub( /[0-9]+/, "-", info ); print info}'
# index
echo "41........................................"
echo "" | awk '{info="wang123qing456song"; result = index( info, "n" ); print "result = "result}'
# length
echo "42........................................"
echo "" | awk '{info="wangqingsong"; result = length( info ); print "the length ( charactor ) of info = "result}'
# blength，下列命令，返回结果为 blength not defined，原因未知
# echo "" | awk '{info="wangqingsong"; result = blength( info ); print "the length ( bit ) of info = "result}'
# substr
echo "43........................................"
echo "" | awk '{info="wang qing song"; child_str=substr( info, 20, 8); print "child string is "child_str}'
# match
echo "44........................................"
echo "" | awk 'BEGIN{info="Hello 2010 World! 2012 Welcome !"; i=match(info,/[0-9]+/); print "index = "i, "RSTART = "RSTART, "RLENGTH = "RLENGTH}'
echo "45........................................"
echo "" | awk 'BEGIN{info="Hello World! Welcome !"; i=match(info,/[0-9]+/); print i, RSTART, RLENGTH}'
# split 切割字符串
echo "46........................................"
echo "" | awk '{info="wang:qing:song"; num = split( info, arr, ":" ); for( i in arr ) print "index = "i", arr["i"] = "arr[i]}'
# tolower
echo "47........................................"
echo "" | awk '{info="ABC"; small = tolower( info ); print "small = "small}'
# toupper
echo "48........................................"
echo "" | awk '{info="Abc"; small = toupper( info ); print "big = "small}'

# 一般函数 
# 格式	描述 
# close( Expression )				用同一个带字符串值的 Expression 参数来关闭由 print 或 printf 语句打开的或调用 getline 函数打开的文件或管道。如果文件或管道成功关闭，则返回 0；其它情况下返回非零值。如果打算写一个文件，并稍后在同一个程序中读取文件，则 close 语句是必需的。 
# system(command )					执行 Command 参数指定的命令，并返回退出状态 ( 成功0，失败非0 )。等同于 system 子例程。 
# Expression | getline [ Variable ]	从来自 Expression 参数指定的命令的输出中通过管道传送的流中读取一个输入记录，并将该记录的值指定给 Variable 参数指定的变量。如果当前未打开将 Expression 参数的值作为其命令名称的流，则创建流。创建的流等同于调用 popen 子例程，此时 Command 参数取 Expression 参数的值且 Mode 参数设置为一个是 r 的值。只要流保留打开且 Expression 参数求得同一个字符串，则对 getline 函数的每次后续调用读取另一个记录。如果未指定 Variable 参数，则 $0 记录变量和 NF 特殊变量设置为从流读取的记录。 
# getline [ Variable ] < Expression	从 Expression 参数指定的文件读取输入的下一个记录，并将 Variable 参数指定的变量设置为该记录的值。只要流保留打开且 Expression 参数对同一个字符串求值，则对 getline 函数的每次后续调用读取另一个记录。如果未指定 Variable 参数，则 $0 记录变量和 NF 特殊变量设置为从流读取的记录。 
# getline [ Variable ]				将 Variable 参数指定的变量设置为从当前输入文件读取的下一个输入记录。如果未指定 Variable 参数，则 $0 记录变量设置为该记录的值，还将设置 NF、NR 和 FNR 特殊变量。

# close ( ? 关闭文件总是返回-1 )
echo "49........................................"
echo "" | awk '
{
	num=0;
	while("cat /etc/passwd"|getline){
		num+=1;
		print $0;
		if(num == 2){
			result = close("/etc/passwd");
			print "file is closed ? "(result==0?"Yes":"No."result);
			break;
		}
	}
}'

# 运行外部程序的两种方式
# 方式一，"ls -l" | getline d只能获取命令执行后打印的第一行信息，所以需要不断循环读取
echo "50........................................"
echo "" | awk '{ while( "ls -l" | getline d )print d}'
# 方式二，system() 这种方式，返回值只能获取到执行状态
echo "51........................................"
echo "" | awk '{ d=system( "ls -l" ); print "the system result = "d}'

# 注：
# 当getline通过|或<读取管道或文件的记录时，每读取一条记录，都会有记录(至于怎么记录的，不知道)，如果流没有关闭，下次读取同一个管道或文件时，getline会根据记录，读取下一条记录，而不会从新开始读，当读取到末尾后，在读取，就会没有数据
# 当getline左右两侧没有|或<时，读取下一条记录给Variable或$0(当没有指定Variable时)，因为在执行getline之前，awk已经读取了一条记录，所以getline读取的是当前记录的下一条记录
# getline分为三大类：没有|与<( 这时候getline是作用于当前处理的文件 )，有|或<( 这时候getline是作用于管道和重定向的文件 )
# 每类又分为两种情况：设置(没有设置)了用于接收记录的变量
# 格式						影响
# getline					sets $0, NF, FNR, NR
# getline var				sets var, FNR, NR
# getline < file			sets $0, NF
# getline var < file		sets var
# command | getline			sets $0, NF
# command | getline var		sets var
# command |& getline		sets $0, NF
# command |& getline var	sets var
echo "52........................................"
awk '{print FNR; getline ; print FNR,$0}' file
echo "53........................................"
awk '{print FNR; getline d; print d,FNR,$0}' file
echo "54........................................"
awk '{print FNR; getline < "file"; print FNR,$0}' file
echo "55........................................"
awk '{print FNR; getline d < "file"; print d,FNR,$0}' file
echo "56........................................"
awk '{print FNR; "cat file" | getline ; print FNR,$0}' file
echo "57........................................"
awk '{print FNR; "cat file" | getline d; print d,FNR,$0}' file
echo "58........................................"
awk '{print FNR; "cat file" |& getline ; print FNR,$0}' file
echo "59........................................"
awk '{print FNR; "cat file" |& getline d; print d,FNR,$0}' file

# 时间函数 
# 格式	描述
# mktime( YYYY MM dd HH MM ss[ DST]) 	根据输入的字符串，生成时间戳，得到的结果是自1970年1月1日0时0分0秒到字符串所示时间的吗秒数，因为时区的关系，在不同地域得到的结果会有差别
# strftime([format [, timestamp]])		格式化时间输出，将时间戳转为时间字符串，具体格式，见下表. 
# systime()								得到系统时间戳,返回从1970年1月1日开始到当前时间(不计闰年)的整秒数
echo "60........................................"
echo "" | awk '{ts_mktime=mktime("1970 01 01 00 00 00"); print "mktime is --> "strftime("%c", ts_mktime)}'
echo "61........................................"
echo "" | awk '{ts_system=systime();print "system is --> "strftime("%c", ts_system)}' 

# strftime日期和时间格式说明符 
# 格式	描述 
# %a	星期几的缩写(Sun) 
# %A	星期几的完整写法(Sunday) 
# %b	月名的缩写(Oct) 
# %B	月名的完整写法(October) 
# %c	本地日期和时间 
# %d	十进制日期 
# %D	日期 08/20/99 
# %e	日期，如果只有一位会补上一个空格 
# %H	用十进制表示24小时格式的小时 
# %I	用十进制表示12小时格式的小时 
# %j	从1月1日起一年中的第几天 
# %m	十进制表示的月份 
# %M	十进制表示的分钟 
# %p	12小时表示法(AM/PM) 
# %S	十进制表示的秒 
# %U	十进制表示的一年中的第几个星期(星期天作为一个星期的开始) 
# %w	十进制表示的星期几(星期天是0) 
# %W	十进制表示的一年中的第几个星期(星期一作为一个星期的开始) 
# %x	重新设置本地日期(08/20/99) 
# %X	重新设置本地时间(12：00：00) 
# %y	两位数字表示的年(99) 
# %Y	当前月份 
# %Z	时区(PDT) 
# %%	百分号(%)

# 判断第四列是否为数字和字母'u'的组合，并打印出整行
lsof -p 11667 | awk '{if($4 ~ /^[0-9u]+$/) print $0;}'
