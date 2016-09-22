########################################################## method 1
func_1 = lambda x : x + 1
print func_1(1)
print


########################################################## method 2
def func_2( arg1, arg2, arg3 ):
    print "arg1 is %s" % arg1
    print "arg2 is %s" % arg2
    print "arg3 is %s" % arg3
    return "func_2 run over"

print func_2(1, 2, 3)
print

# if second = 2 is set, third must be set too
def func_3( arg1, arg2 = 2, arg3 = 3):
    print "arg1 is %s" % arg1
    print "arg2 is %s" % arg2
    print "arg3 is %s" % arg3
    return "func_3 run over"
print func_3(1, 3, 4)
