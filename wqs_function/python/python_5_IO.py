#coding=utf-8

import pickle

################################################## the data is not string
lists = ["this ", "is", 4, 1112]
# write to file
file_f = open(r"not_string.txt", "w")
pickle.dump(lists, file_f)
file_f.close()
# read from file
file_f = open(r"not_string.txt")
load_data = pickle.load(file_f)
file_f.close()
print load_data


################################################## the data is string
# write to file
file_f = open(r"string.txt", "w")
file_f.write("This is a simple string")
file_f.close()
# read from file
file_f = open(r"string.txt")
load_string = file_f.read()
print load_string

################################################# 
