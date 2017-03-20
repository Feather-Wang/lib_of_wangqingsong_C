#coding=utf-8
########################  int or double

number = 3
print number
number+=1
print number

number = 3.1415926123456789
print number
number+=1
print number


########################  string
strs = "wang qing song"
print strs
strs += " 1"
print strs
strs += str(12345)
print strs
# the string is one more lines
print "Hello\
world"
print """THis is
a multiline
string."""
print "number one is %s" % 1
# 去掉换行符
strs = 'wangqingsong\n'
strs = strs.strip('\n')
print strs
# 分割字符串
strs = 'wang qing song'
strs = strs.split(' ')
print strs
# 分割字符串-多个间隔符
import re
strs = '1,2;3/4*5'
strs = re.split(',|;|/|\*', strs)
print strs
# 查找字符
strs = 'wangqingsong'
pos = strs.index('i')
print pos
# 字符串长度
str_len = len(strs)
print str_len
# 大小写转换
str_big = strs.upper()
str_small = strs.lower()
# 查找字符串
strs = 'wangqingsong'
str_child = 'qing'
find = strs.find(str_child)
print find
print
print


########################  list,列表
lists = ["fist list", 2, 3.1]
print lists
# print the first element 
print lists[0]
# print the second element 
print lists[1]
# print the last element 
print lists[-1]
# print before the last element 
print lists[-2]
# add new element
lists.append(["wang qing song"])
print lists
# add four element, every charactor is one element
lists += "haha"
print lists
# print value from second to the last
print lists[1:]
# print value from second to fourth
print lists[1:4]
# print value from begin to fourth
print lists[:4]
# print value with one both two
print lists[::2]

print
print

#########################  tuple,元组
tuples = (1, 2, "wang", "qing", "song")
print tuples
# print the first element 
print tuples[0]
# print the second element 
print tuples[1]
# print the last element 
print tuples[-1]
# print before the last element 
print tuples[-2]
# add new element
print tuples
# print value from second to the last
print tuples[1:]
# print value from second to fourth
print tuples[1:4]
# print value from begin to fourth
print tuples[:4]
# print value with one both two
print tuples[::2]
# use tuple to string
print "First: %s\n\
Second: %s\n\
Third: %s\n\
Fourth: %s\n\
Fiveth: %s" % tuples

print
print

#########################  dictionaries,字典
dic = {"first element" : "first", 2 : 2.1, 3 : "hello world"}
print dic
# add new element
dic["new element"] = "good morning"
print dic
# get one of the dictionaries
print dic["first element"]
# change someone's value
dic[2] = "second"
print dic
# use the member to string:this way is only for string ,not 2 or 3
print "the first element is %(first element)s" % dic
