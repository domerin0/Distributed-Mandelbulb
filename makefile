CC=pgc++
CXX=pgc++
#FLAGS    = -O2 -Wall
#CFLAGS   = $(FLAGS)
#CXXFLAGS = $(FLAGS)
#LDFLAGS  = -lm

PROGRAM_NAME=mandelbulb

OBJS=main.o print.o timing.o savebmp.o getparams.o getpath.o getnextframe.o 3d.o getcolor.o distance_est.o \
	mandelboxde.o raymarching.o renderer.o init3D.o mandelbulb_dist_est.o

$(PROGRAM_NAME): $(OBJS)
	$(CC) -o $@ $? $(CFLAGS) $(LDFLAGS)


acc: CFLAGS=-fast -acc -Minfo=accel -ta=tesla:cc50
acc: CXXFLAGS=-fast -acc -Minfo=accel -ta=tesla:cc50
acc: LDFLAGS=-acc -ta=tesla:cc50
acc: $(OBJS)
	$(CC) $(LDFLAGS) -o$(PROGRAM_NAME) $? -lm

omp: CFLAGS=-O2
omp: CXXFLAGS=-O2
omp: LDFLAGS=
omp:$(OBJS)
	$(CC) $(LDFLAGS) -o $(PROGRAM_NAME) $? -lgomp -lm

clean:
	rm *.o $(PROGRAM_NAME)


video:
	#change to frames folder and run script
	@/bin/bash -c "  \
	pushd ./frames;  \
	./make_video.sh; \
	popd;"
