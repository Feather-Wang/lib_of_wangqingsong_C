class FirstClass(object):
    private_value = 1

    # the init function of class is __init__(), must be this name
    def __init__(self):
        print "FirstClass __init_"
        self.private_value = 3
    
    #function has no argments, this is wrong, function must have one more argument
    def toString_f(self, arg1, arg2):
        print "FirstClass toString_f"
        return "private_value = %s, arg1 = %s, arg2 = %s" % (self.private_value,arg1,arg2)
    def first_self(self):
        print "This is FirstClass"

first_class = FirstClass()
print first_class.private_value
print first_class.toString_f("arg1", "arg2")
print
print

class ChildClass(FirstClass):
    child_value = 2
    def __init__(self, arg1):
        print "ChildClass --> arg1 = %s" % arg1
        self.child_value_2 = 100
    def toString_f(self, arg1):
        print "ChildClass toString_f"
        return "child_value = %s, arg1 = %s, FirstClass.private_value = %s" % (self.child_value, arg1, self.private_value)
    def child_self(self):
        print "This is ChildClass"

child_class = ChildClass(100000)
print child_class.toString_f("world")
child_class.first_self()
child_class.child_self()

print
print
###################################################################### notice
second_class = FirstClass()
print ".......1"
print first_class.private_value
print second_class.private_value
# now ?
FirstClass.private_value = 20
print ".......2"
print first_class.private_value
print second_class.private_value
# now ?
first_class.private_value = 30
print ".......3"
print first_class.private_value
print second_class.private_value
# now ?
FirstClass.private_value = 40
print ".......4"
print first_class.private_value
print second_class.private_value

############################################################ import other file
################################# method 1
# this method will import all of moudles, has run part
# import FirstClass
# you can import one more moudles, like this, import moudle_1, moudle_2....
print
print
################################# method 2
from python_4_class import FirstClass

################################# make another name for moudle 
# you can use 'as'
# import FirstClass as FirstClass_small_name

#################################
# you can use '*' import all moudles, like this: import * or from
# import *

