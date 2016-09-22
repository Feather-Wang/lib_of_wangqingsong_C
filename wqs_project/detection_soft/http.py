#!/usr/bin/env python
# coding: utf-8

'''
Created on 2015年3月23日

@author: yanwf
'''

import httplib,urllib2
import base64
import socket
import sys
import time

#ip= "190.9.114.152"
#port = 80
#"Basic YWRtaW46MTIzNDU="

def do_log(Host, Port, username, password, state, log_file):
    
    if state == "successed":
        log_file.write(time.strftime('%Y%m%d%H%M%S')+ ":")
        log_file.write("weakpasswd")
        log_file.write(" ip:" + Host)
        log_file.write(" port:" + Port)
        log_file.write(" http:" + state)
        log_file.write(" username:" + username)
        log_file.write(" password:" + password + "\n")
    elif state == "failed":
        log_file.write(time.strftime('%Y%m%d%H%M%S') + ":")
        log_file.write("unweakpasswd")
        log_file.write(" ip:" + Host)
        log_file.write(" port:" + Port + "\n")
        #log_file.write(" http:" + state)
    else:
        log_file.write(time.strftime('%Y%m%d%H%M%S') + ":")
        log_file.write("network can not connected")
        log_file.write(" ip:" + Host)
        log_file.write(" port:" + Port + "\n")

def do_http(ip, port, username, password, log_file):

    userpass= username +":"+password
    user=userpass.strip()
    #print "--%s---"%user
    auth = base64.b64encode(user)
    conn = httplib.HTTPConnection(ip, port, False)  

    header = {"Accept": "*/*",
    "Accept-Language": "zh-cn",
    "Referer": "http://%s:%s/doc/page/login.asp"%(ip, port),
    "Authorization": "Basic %s"%auth,
    "x-requested-with": "XMLHttpRequest",
    "If-Modified-Since": "0",
    "Accept-Encoding": "gzip, deflate",
    "User-Agent": "Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 5.2; Trident/4.0; .NET CLR 1.1.4322)",
    "Host": "%s:%s"%(ip, port),
    "Connection": "Keep-Alive"}

    header1 = { "Host":" %s:%s" %(ip, port),
              "Connection":" keep-alive",
              "Cache-Control":" max-age=0",
              "Accept":" */*",
              "X-Requested-With":" XMLHttpRequest",
              "If-Modified-Since":" 0",
              "User-Agent":" Mozilla/5.0 (Windows NT 6.1; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/41.0.2272.76 Safari/537.36",
              "Authorization":" Basic %s"%auth,
              "Referer":" http://%s:%s/doc/page/login.asp" %(ip, port),
              "Accept-Encoding":" gzip, deflate, sdch",
              "Accept-Language":" zh-CN,zh;q=0.8,en;q=0.6"
              }

    try:
        conn.request('GET', '/PSIA/Custom/SelfExt/userCheck', headers = header1)  
    except socket.error:
        do_log(ip, port, "", "", "", log_file)
        conn.close()
        return False

    try:
        res = conn.getresponse()  
    except: 
        #print 'no response'
        conn.close()
        return False         
    #print 'version:', res.version  
    #print 'reason:', res.reason  
    #print 'status:', res.status  
    #print 'msg:', res.msg  
    #print 'headers:', res.getheaders()
    xmlret=res.read().find("<statusValue>200</statusValue>")
    #print xmlret
    conn.close()
    if xmlret == -1:
        return False
    else:
        print "username: " + username + " password: " + password + ' login success'
        do_log(ip, port, username, password, "successed", log_file)
        return True



def check_port_on(ip, port):

    socket.setdefaulttimeout(2)
    soc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    check_target = (ip, int(port))

    port_status = soc.connect_ex(check_target)
    soc.close()
    if port_status == 0:
        return True
    else:
        return False

def check_http_login(ip, port, username, log_file):
    
    weakpasswd = 0
    pw_file = open('./pw.txt','r+')
    Index = 0
    #print time.asctime(),":   ****** begin","\n"
    while not weakpasswd:
        password = pw_file.readline().strip('\n')
        Index +=1;
        #print Index,time.asctime(),":   ****** try","",username,":",password,""
        if len(password) == 0:
            break;
        ret = do_http(ip, port, username, password, log_file)
        if ret:
            weakpasswd = 1
    pw_file.close()
    
    if weakpasswd == 1:
        return True
    else:
        return False
    
    
def http(*args, **kw):
    if sys.argv.__len__() < 5:
        print '\nUsage: %s <HOST> <PORT> <USERNAME> <LOGNAME>' % sys.argv[0]
        sys.exit()

    ip   = sys.argv[1]
    port = sys.argv[2]
    username = sys.argv[3]
    log_file_name = sys.argv[4]

    #log_file_name = "http" + time.strftime('%Y%m%d%H%M%S') + '.log'
    log_file = open(log_file_name, "a")

    if check_port_on(ip, port):
        vuln_exists = check_http_login(ip, port, username, log_file)
        if vuln_exists:
            print 'Target %s is weakpasswd.' %ip
        else:
            print 'Target %s may not be weakpasswd.' %ip
            do_log(ip, port, username, "", "failed", log_file)
    else:
        print 'http(ip %s port %s) closed' %(ip,port)
        do_log(ip, port, "", "", "", log_file)

    log_file.close() 
    

if __name__ == '__main__':
    http()
    
