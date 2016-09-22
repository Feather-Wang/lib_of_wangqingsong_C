r:read.c ../shm/wqs_shm.c wqs_sem.c
	cc -g -Wall read.c wqs_sem.c ../shm/wqs_shm.c -o r
w:write.c ../shm/wqs_shm.c wqs_sem.c
	cc -g -Wall write.c wqs_sem.c ../shm/wqs_shm.c -o w
clean:
	rm r w
