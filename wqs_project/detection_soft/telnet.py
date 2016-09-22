#!/usr/bin/env python
#encoding=utf-8

'''
Created on 2015年3月23日

@author: yanwf
'''


import telnetlib
import socket
import sys
import time

def do_log(Host, Port, username, password, state, log_file):
    
    if state == "successed":
        log_file.write(time.strftime('%Y%m%d%H%M%S')+ ":")
        log_file.write("weakpasswd")
        log_file.write(" ip:" + Host)
        log_file.write(" port:" + Port)
        log_file.write(" telnet:" + state)
        log_file.write(" username:" + username)
        log_file.write(" password:" + password + "\n")
    elif state == "failed":
        log_file.write(time.strftime('%Y%m%d%H%M%S') + ":")
        log_file.write("unweakpasswd")
        log_file.write(" ip:" + Host)
        log_file.write(" port:" + Port + "\n")
        #log_file.write(" telnet:" + state)
    else:
        log_file.write(time.strftime('%Y%m%d%H%M%S') + ":")
        log_file.write("network can not connected")
        log_file.write(" ip:" + Host)
        log_file.write(" port:" + Port + "\n")

def do_telnet(ip, port, account, password, log_file):
    enter_key = "\r\n"
    command = "exit"

    t = telnetlib.Telnet()
    #t.set_debuglevel(3)
    t.open(ip, port)

    ret = t.read_until("Login:", 5)

    t.write(enter_key)
    ret = t.read_until("Password:", 5)

    t.write(enter_key)
    ret = t.read_until("Login:", 5)

    t.write(account + enter_key)
    ret = t.read_until("Password:", 5)

    t.write(password + enter_key)
    ret = t.read_very_eager()

    finish = '[#$>]'       # 登陆成功提示
    index = t.expect([finish, '[Ii]ncorrect', 'Unknown host', 'Login Failed'], timeout=1)
    #print '------------------'
    #print index[0]
    #print '------------------'
    if index[0] == 0:
        t.write(command + enter_key)
        print "username: " + account+ " password: " + password + ' login success'
        do_log(ip, port, account, password, "successed", log_file)
        return True
    else:
        #print password + 'Error'
        #print "****** login incorrect!\n"
        return False


def check_telnet_login(ip, port, username, log_file):
    
    weakpasswd = 0
    pw_file = open('./pw.txt','r+') #密码文件
    Index = 0
#    print time.asctime(),":   ****** begin","\n"
    while True:
        password = pw_file.readline().strip('\n')
        Index +=1;
        password = password.strip()
        #print Index,time.asctime(),":   ****** try","",username,":",password,""
        if len(password) == 0:
            break;
        ret = do_telnet(ip, port, username, password, log_file)
        if ret:
            weakpasswd = 1
    pw_file.close()
    
    if weakpasswd == 1:
        return True
    else:
        return False



    
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

def telnet():
    if sys.argv.__len__() < 5:
        print '\nUsage: %s <HOST> <PORT> <USERNAME> <LOGNAME>' % sys.argv[0]
        sys.exit()
    
    #log_file_name = "telnet" + time.strftime('%Y%m%d%H%M%S') + '.log'

    ip   = sys.argv[1]
    port = sys.argv[2]
    username = sys.argv[3]
    log_file_name = sys.argv[4]

    log_file = open(log_file_name, "a")
    if check_port_on(ip, port):
        vuln_exists = check_telnet_login(ip, port, username, log_file)
        if vuln_exists:
            print 'Target %s telnet is weakpasswd.' % ip
        else:
            print 'Target %s telnet may not be weakpasswd.' %ip
            do_log(ip, port, username, "", "failed", log_file)
    else:
        print 'telnet(ip %s port %s) closed' %(ip,port)
        do_log(ip, port, "", "", "", log_file)

    log_file.close() 

if __name__ == '__main__':
    telnet()
   
