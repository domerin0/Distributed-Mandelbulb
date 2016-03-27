
CC=gcc
FLAGS    = -O2 -Wall
CFLAGS   = $(FLAGS)
CXXFLAGS = $(FLAGS)
LDFLAGS  = -lm

PROGRAM_NAME=mandelbox

OBJS=main.o print.o timing.o savebmp.o getparams.o getpath.o 3d.o getcolor.o distance_est.o \
	mandelboxde.o raymarching.o renderer.o init3D.o

$(PROGRAM_NAME): $(OBJS)
	$(CC) -o $@ $? $(CFLAGS) $(LDFLAGS)


omp: CFLAGS=-g -Wall -O2 -fopenmp
omp: LDFLAGS=-fopenmp
omp:$(OBJS)
	$(CC) $(LDFLAGS) -o $(PROGRAM_NAME) $? -lgomp -lm

clean:
	rm *.o $(PROGRAM_NAME) $(EXEEXT) *~

video:
	#change to frames folder and run script
	@/bin/bash -c "  \
	pushd ./frames;  \
	./make_video.sh; \
	popd;"