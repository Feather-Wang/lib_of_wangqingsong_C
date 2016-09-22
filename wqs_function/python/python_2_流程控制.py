############################################################################# if
if_num = 6
if if_num in (1,2,3,4,5):
    print "if_num is in (1,2,3,4,5)"
elif if_num in (6,7,8,9):
    print "if_num is in (6,7,8,9)"
else:
    print "if_num is not in this tuple"

if if_num == 6:
	print "if_num == 6"
elif if_num < 6:
	print "if_num < 6"
elif if_num <= 5:
	print "if_num <= 5"
if if_num == 6 and if_num != 8:
    print "if_num == 6 and if_num != 8"
if if_num == 8 or if_num == 6:
    print "if_num == 8 or if_num == 6"
if not ( if_num == 8 ):
    print "not (if_num == 8)"



############################################################################# for
lists = [1,2,3,4,5,6,7,8]
for for_num in range(10000):
    if for_num in (0,9):
        print "for_num is in (3,5)"
        break
    else:
        print "if...else..."
        continue
    print "continue is useless"
else:
    print "for...else..."

############################################################################# while
while_num = 0
while lists[while_num] != 5:
    print "lists[i] is not 2"
    while_num += 1
