all:
	cc *.c -lavformat -lavcodec -lavutil -lswscale -lswresample -lm -lz -lpthread -lbz2 -lrt `sdl-config --cflags --libs`
n: wqs_ffmpeg.c
	cc wqs_ffmpeg.c -lavformat -lavcodec -lavutil -lswscale -lswresample -lm -lz -lpthread -lbz2 -lrt `sdl-config --cflags --libs`
sn: net_video.c server.c
	cc server.c -lavformat -lavcodec -lavutil -lswscale -lswresample -lm -lz -lpthread -lbz2 -lrt `sdl-config --cflags --libs`
clean:
	rm a.out
