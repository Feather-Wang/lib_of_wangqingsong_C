#!/usr/bin/env python
#coding=utf-8
'''
Created on 2015年3月23日

@author: yanwf
'''
import commands
import cgi

http_port = '80'
rtsp_port = '554'
telnet_port = '23'
username = 'admin'
logname = 'log'

iptoint = lambda x:sum([256**j*int(i) for j,i in enumerate(x.split('.')[::-1])])
inttoip = lambda x: '.'.join([str(x/(256**i)%256) for i in range(3,-1,-1)])  

str_ip1 = ""
str_ip2 = ""

def do_http(str_ip):
     command = "python ./http.py " + str_ip + " " + http_port + " " + username + " " + logname
     output = commands.getoutput(command)
     print output

def do_rtsp(str_ip):
    for rtspbugid in range(1, 4):
        command = "python ./rtsp.py " + str_ip + " " + rtsp_port + " " + str(rtspbugid) + " " + logname
        output = commands.getoutput(command)
        print output
    
def do_telnet(str_ip):
     command = "python ./telnet.py " + str_ip + " " + telnet_port + " " + username + " " + logname
     output = commands.getoutput(command)
     print output


def http_end():
    print '''</textarea></form>
    </div>
    </body>
    </html>'''
def do_weak_or_bug(weak_flag, bug_flag, str_ip):

    if weak_flag == "ON":
        do_http(str_ip)
        do_telnet(str_ip)
    if bug_flag == "ON":
        do_rtsp(str_ip)


def do_weak_and_bug(ip2_true, weak_flag, bug_flag, str_ip1, str_ip2):
    
    if ip2_true == 0:

        int_ip1 = iptoint(str_ip1) 
        int_ip2 = iptoint(str_ip2)

        if int_ip2 < int_ip1:
            temp = int_ip1
            int_ip1 = int_ip2
            int_ip2 = temp

        for ip in range(int_ip1 ,  int_ip2+1):
            str_ip = inttoip(ip)
            do_weak_or_bug(weak_flag, bug_flag, str_ip)

    else:
        do_weak_or_bug(weak_flag, bug_flag, str_ip1)
       
form = cgi.FieldStorage()
ip1 = form.getvalue('ip1')
ip2 = form.getvalue('ip2')
ip3 = form.getvalue('ip3')
ip4 = form.getvalue('ip4')
ip5 = form.getvalue('ip5')
ip6 = form.getvalue('ip6')
ip7 = form.getvalue('ip7')
ip8 = form.getvalue('ip8')

if form.getvalue('bug'):
   bug_flag= "ON"
else:
   bug_flag = "OFF"

if form.getvalue('weak'):
   weak_flag = "ON"
else:
   weak_flag = "OFF"

print "Content-type: text/html"
print
print '''<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<title>北京天行网安信息技术有限责任公司</title>
<style type="text/css">
.bj{ background-image:url(/images/bj1.png); margin:0; width:100%; }
.top {text-align:center; margin:0;font-family:"Arial Black", Gadget, sans-serif; font-family:"微软雅黑";}
img{ border:none;}
.min {font-family: "Arial Black", Gadget, sans-serif; margin:0;}
.wenbenk{ width:36px;}
.box {text-align: center;}
.biaoti{ font-family:"微软雅黑"; color:#E1FBFF; font-size:20px;}
p{ color:#E1FBFF; }
.annv{ background-image:url(/images/annv.png); width:114px; height:48px; margin-left:auto; margin-right:auto; margin-top:10px;}
.tijiao{ margin-top:-34px;}
</style>
</head>
<body class="bj">
<div class="top">
<p class="biaoti">视频设备安全检测工具示例</p>
<p><span class="min"></span><span class="min"></span></p>
<p>&nbsp;</p>
<form id="form1" name="form1" method="post" action="/cgi-bin/newform.py">
<p><span class="yanzi">扫描主机：
<label>'''
print '<input class=wenbenk name=ip1 type=text  value="%s" />' % ip1
print '<input class=wenbenk name=ip2 type=text  value="%s" />' % ip2
print '<input class=wenbenk name=ip3 type=text  value="%s" />' % ip3
print '<input class=wenbenk name=ip4 type=text  value="%s" />' % ip4
print '-'
print '<input class=wenbenk name=ip5 type=text value="%s" />' % ip5
print '<input class=wenbenk name=ip6 type=text value="%s" />' % ip6
print '<input class=wenbenk name=ip7 type=text value="%s" />' % ip7
print '<input class=wenbenk name=ip8 type=text value="%s" />' % ip8
print ''' </label>
</span></p>
<p><span class="box"> '''
if bug_flag == "ON":
    print '<input type=checkbox name=bug id=bug value = on checked = checked/> 漏洞检测'
else:
    print '<input type=checkbox name=bug id=bug value = on /> 漏洞检测'
if weak_flag  == "ON":
    print '<input type=checkbox name=weak id=weak  value = on  checked = checked/> 弱密码检测'
else:
    print '<input type=checkbox name=weak id=weak  value = on /> 弱密码检测'
print '''</span></p>
<hr />
<input type="submit" name="tijiao" id="tijiao" value="提交" />
<p>&nbsp;</p>
</form>
<form>
<label for="wbk"></label>
<textarea name="textarea" id="textarea" cols="80" rows="10">'''
#print '起始ip:%s.%s.%s.%s' %(ip1, ip2, ip3, ip4)
#print '终止ip:%s.%s.%s.%s' %(ip5, ip6, ip7, ip8)
#print 'bug_flag:%s weak_flag:%s' %(bug_flag, weak_flag)
ip_one = [ip1, ip2, ip3, ip4]
ip_two = [ip5, ip6, ip7, ip8]
if ip_one.count('None') == 0:
        ip1_true = 0 
        str_ip1 = '.'.join(ip_one)
else:
        print "起始ip地址输入有误"
        ip1_true = 1 

if ip_two.count('None') < 4 and ip_two.count('None') > 0:
        print "终止ip地址输入有误"
        ip2_true = 1 
elif ip_two.count('None') == 4:
        ip2_true = 2 
else:
        ip2_true = 0 
        str_ip2 = '.'.join(ip_two)

if ip1_true == 1 or ip2_true == 1: 
    http_end()
else:   
    if weak_flag == "ON" or bug_flag == "ON":
        do_weak_and_bug(ip2_true, weak_flag, bug_flag, str_ip1, str_ip2)
    else:
        print '请选择操作类型!'
http_end()
