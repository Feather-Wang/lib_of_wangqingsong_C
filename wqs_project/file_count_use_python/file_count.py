#coding=utf-8

from config import Config
import os
import os.path

config = Config()
config.read_config("commoninfo.ini")

print config.lists
'''
崴泥个琼斯
for parent, dirnames, filenames in os.walk("csv"):
	for dirname in dirnames:
		print "parent is : " + parent
		print "dirname is : " + dirname
		print "-------------------------------------"
	print "------------------------------------------------------"

	for filename in filenames:
		print "parent is : " + parent
		print "filename is : " + filename
		print "the full name of the file is : " + os.path.join(parent, filename) 
		print "-------------------------------------"
	print "###################################################################"
'''

def get_mac(filename):
	fp = open(filename)
	lines = fp.readlines()
	fp.close()

	for mac in config.lists:
		print "mac is " + mac
		for line in lines:
			#print line
			if mac in line:
				print "find " + mac
				return 1
	print "nothing"
	return 0

number = 0
for parent, dirnames, filenames in os.walk("csv"):
	for filename in filenames:
		filepath = os.path.join(parent, filename)
		if 'summary' in filepath:
			print "the full name of the file is : " + filepath 
			number += get_mac(filepath)

print "number = %s" % number
