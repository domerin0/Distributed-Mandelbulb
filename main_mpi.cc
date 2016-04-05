#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <mpi.h>
#include <stdio.h>
#include "camera.h"
#include "renderer.h"
#include "mandelbox.h"
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_FRAMES 7200

void getParameters(char *filename, CameraParams *camera_params, RenderParams *renderer_params,
			 MandelBoxParams *mandelBox_paramsP);
void getPath      (char *filename, CameraParams *camera_path, int *len);
void init3D       (CameraParams *camera_params, const RenderParams *renderer_params);
void renderFractal(const CameraParams &camera_params, const RenderParams &renderer_params, unsigned char* image);
void saveBMP      (const char* filename, const unsigned char* image, int width, int height);
void writeTimesToTextFile(double* buffer, int p);

MandelBoxParams mandelBox_params;

int main(int argc, char** argv)
{

  MPI_Init(&argc, &argv);
  int nframes;
  int my_rank, p;
  MPI_Status status;
  double mytime;
  double *rbuf;
	CameraParams *camera_path, *sub_camera_path;
  RenderParams renderer_params;
	char frame_name[256];

  mytime = MPI_Wtime();

  MPI_Comm_size(MPI_COMM_WORLD, &p);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

  /*
   adapted from:
  http://stackoverflow.com/questions/9314586/c-faster-way-to-check-if-a-directory-exists
  All process' check to ensure they have a directory to save the frames
  */
  struct stat s;
  int err = stat("/path/to/frames", &s);
  if (-1 == err)
  {
    mkdir("frames", 0700);
  }

  /*
  Process 0 reads in path, and splits it up amoungst nodes
  */

  if(my_rank == 0){
    assert(argc >= 2);
    char* path;
    if (argc == 3)
    {
      path = (char*)malloc(sizeof(char) * (strlen(argv[2]) + 1));
      memcpy(path, argv[2], strlen(argv[2]) + 1);
      printf("%s\n", path);
    }
    else
    {
      path = (char*)malloc(sizeof(char) * (strlen("path.dat") + 1));
      sprintf(path, "path.dat");
      printf("%s\n", path);
    }

    camera_path = (CameraParams*)malloc(MAX_FRAMES*sizeof(CameraParams));
    assert(camera_path);

    getParameters(argv[1], &camera_path[0], &renderer_params, &mandelBox_params);

    getPath(path, camera_path, &nframes);
		printf("I'm process 0 and there are: %d\n", nframes);

  }else{
		camera_path = (CameraParams*)malloc(MAX_FRAMES*sizeof(CameraParams));
	}

	/*
	The num frames and render_params are broadcast because every process needs them
	*/
		MPI_Bcast(&nframes, 1, MPI_INT, 0, MPI_COMM_WORLD);

		MPI_Bcast(&renderer_params, sizeof(RenderParams), MPI_BYTE, 0, MPI_COMM_WORLD);

		sub_camera_path = (CameraParams*)malloc((nframes/p) * sizeof(CameraParams));
		printf("I'm process %d, and I have %d frames\n", my_rank, nframes/p);
		MPI_Scatter(camera_path, (nframes/p) * sizeof(CameraParams), MPI_BYTE, sub_camera_path,
			(nframes/p) * sizeof(CameraParams), MPI_BYTE, 0, MPI_COMM_WORLD);

	/*
    All nodes do rendering work and save images
  */
	int offset = (nframes / p) * my_rank;
  int image_size = renderer_params.width * renderer_params.height;
  unsigned char *image = (unsigned char*)malloc(3*image_size*sizeof(unsigned char));


		printf("width %d height %d on process %d\n", renderer_params.width,
			renderer_params.height, my_rank);

  for (int i = 0; i < nframes/p; ++i)
  {
		printf("cam pos x: %f y : %f z: %f on p: %d\n",
		sub_camera_path[i].camPos[0],
		sub_camera_path[i].camPos[1],
		sub_camera_path[i].camPos[2],
		my_rank);

    init3D(&sub_camera_path[i], &renderer_params);
    renderFractal(sub_camera_path[i], renderer_params, image);
    //TODO create render directory from input
    //TODO create starting name from number from input (in case of previously generated frames)
    snprintf(frame_name, sizeof(char) * 256, "./frames/frame_%04d.bmp", i + offset);
	//	printf("I am process %d and I am saving%s\n", my_rank, frame_name);
    saveBMP(frame_name, image, renderer_params.width, renderer_params.height);

  }

  free(image);

  if (my_rank == 0) {
      rbuf = (double *)malloc((p)*sizeof(double));
    }else{
      rbuf = NULL;
    }

  mytime = MPI_Wtime() - mytime;
  MPI_Gather(&mytime, 1, MPI_DOUBLE, rbuf, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  if(my_rank == 0){
    writeTimesToTextFile(rbuf, p);
    free(rbuf);
  }

  MPI_Finalize();
  return 0;
}

void writeTimesToTextFile(double* buffer, int p){
  //write to file
  int i;
  FILE *file;

  char* name = (char*)malloc(sizeof(char)*100);
  sprintf(name,"time_%d", p);
  file=fopen(name, "w");
  if(file==NULL) {
      perror("Error opening file.");
  }
  else {
      for(i=0; i < p ; i ++){
        fprintf(file,"%d\t%.5e\n",i, buffer[i]);
      }
  }
  fclose(file);
  }
