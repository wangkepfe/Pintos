all: setitimer-helper squish-pty squish-unix

CC = gcc
CFLAGS = -Wall -W -D_XOPEN_SOURCE=500 -D__EXTENSIONS__
#LDFLAGS = -lm -lxnet
LDLIBS = -lm
setitimer-helper: setitimer-helper.o
squish-pty: squish-pty.o
squish-unix: squish-unix.o

clean: 
	rm -f *.o setitimer-helper squish-pty squish-unix
