#coding=utf-8
import os
import subprocess

'''
执行系统命令有如下几种常用的方法
'''

'''
第一种：
os.system()：这种方式只能执行系统命令，执行结果直接输出在屏幕上
'''
print "################################################ The first method"
os.system('ls')


'''
第二种：
os.popen()：这种方式不但能执行系统命令，还能获取执行结果
注意：
1、这种方式，如果返回结果含有中文，将被转换成转义字符的形式
2、返回结果是放在一个list里
'''
print "################################################ The second method"
result = os.popen('ls').readlines()
print result

'''
第三种：
subprocess.call()
注意：
1、与os.system()类似，无法获取返回结果
'''
print "################################################ The third method"
subprocess.call(["ls -l"], shell = True)

'''
第四种：
subprocess.Popen()
'''
print "################################################ The fourth method"
result = subprocess.Popen('ls -l', shell = True, stdout = subprocess.PIPE, stderr = subprocess.STDOUT)
for line in result.stdout.readlines():
	print line.strip()

'''
第五种：
commands
'''
print "################################################ The fiveth method"
import commands
result = commands.getoutput('ls -l')
print result
result = commands.getstatusoutput('ls')
print result
