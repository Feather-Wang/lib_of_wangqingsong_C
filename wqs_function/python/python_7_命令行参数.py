#coding=utf-8

'''
需要模块：sys
参数个数：len(sys, argv)
脚本名：sys.argv[0]
第一个参数：sys.argv[1]
第二个参数：sys.argv[2]
....

'''
import sys
for i in range(1, len(sys.argv)):
	print "参数 " , i, sys.argv[i]
