all: tutorial02.c
	cc tutorial02.c -lavformat -lavcodec -lavutil -lswscale -lswresample -lm -lz -lpthread -lbz2 -lrt `sdl-config --cflags --libs` -lSDL_image
#all: tutorial2.c
#	cc tutorial2.c -lavformat -lavcodec -lavutil -lswscale -lswresample -lm -lz -lSDL -lpthread -lbz2 -lrt
#all: test_ffmpeg.c
#	cc test_ffmpeg.c -lavformat -lavcodec -lavutil -lswscale -lswresample -lm -lz -lSDL -lpthread -lbz2 -lrt
clean:
	rm a.out
