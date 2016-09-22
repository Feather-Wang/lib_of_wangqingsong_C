#!/usr/bin/env python
#coding=utf-8
#import os
'''
Created on 2015年3月23日

@author: yanwf
'''
import commands
import cgi

form = cgi.FieldStorage()
ip = form.getvalue('ip')
port = form.getvalue('port')

print "Content-type: text/html"
print
print """\
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<meta name="robots" content="index,follow">
<title>北京天行网安信息技术有限责任公司——视频设备安全检测</title>
</head>
<body>
<center><h1>视频设备安全检测工具示例</h1>
<form action='/cgi-bin/form.py' method=post>"""
print "视频设备地址<input type=text name=ip value=",ip," /><br>"
print "视频设备端口<input type=text name=port value=",port,"/><br>"
print """\
<input type=submit value=submit />
</form>
<br/>
<form><textarea rows=20 cols=80 >"""
if ip != None:
    print "地址:", ip
    print "端口:", port
    command = "python ./http.py " + ip + " " + port + " admin log"
    output = commands.getoutput(command)
    print output
    #os.system(command)
    #print command

print "</textarea></form></center></body>"
print "</html>"
