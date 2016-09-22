#all: simplest_ffmpeg_player.c
#	cc simplest_ffmpeg_player.c -lavformat -lavcodec -lavutil -lswscale -lswresample -lm -lz -lpthread -lbz2 -lrt `sdl-config --cflags --libs`
n: net_video.c
	cc net_video.c -lavformat -lavcodec -lavutil -lswscale -lswresample -lm -lz -lbz2 -lrt
s: server.c net_video.c
	cc server.c net_video.c -lavformat -lavcodec -lavutil -lswscale -lswresample -lm -lz -lbz2 -lrt
b:server.c client.c
	cc client.c -o c
	cc server.c -o s
#all: test_ffmpeg.c
#	cc test_ffmpeg.c -lavformat -lavcodec -lavutil -lswscale -lswresample -lm -lz -lSDL -lpthread -lbz2 -lrt
clean:
	rm a.out #c s
