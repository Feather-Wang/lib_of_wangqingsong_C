############################################################ exception
# use try-except[exceptionname]
def func_1():
    try:
        10/0
    except ZeroDivisionError:
        print "Catch Error : ZeroDivisionError"
    else:
        print "else..."
    finally:
        print "try....finally...."
func_1()


print
print

