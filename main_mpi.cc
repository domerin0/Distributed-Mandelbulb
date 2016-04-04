#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <mpi.h>
#include "a2.h"
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

int main(int argc, char** argv)
{

  int my_rank, p;
  MPI_Status status;
  double mytime;
  double *rbuf;

  MPI_Init(&argc, &argv);
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

    RenderParams renderer_params;
    CameraParams *camera_path = (CameraParams*)malloc(MAX_FRAMES*sizeof(CameraParams));
    assert(camera_path);
    int nframes;
    char frame_name[256];

    getParameters(argv[1], &camera_path[0], &renderer_params, &mandelBox_params);

    getPath(path, camera_path, &nframes);


  }else{

  }

  /*
    All nodes do rendering work and save images
  */
  int image_size = renderer_params.width * renderer_params.height;
  unsigned char *image = (unsigned char*)malloc(3*image_size*sizeof(unsigned char));

  for (int i = 0; i < nframes; ++i)
  {
    init3D(&camera_path[i], &renderer_params);
    renderFractal(camera_path[i], renderer_params, image);

    //TODO create render directory from input
    //TODO create starting name from number from input (in case of previously generated frames)
    snprintf(frame_name, sizeof(char) * 256, "./frames/frame_%04d.bmp", i);
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
//MPI_Gather(&mytime, 1, MPI_DOUBLE, rbuf, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

//        MPI_Request myReq;
//        MPI_Irecv(&nextProcess, 1, MPI_INT, MPI_ANY_SOURCE,
//          MPI_ANY_TAG, MPI_COMM_WORLD, &myReq);
//        MPI_Wait(&myReq, &status);
//        MPI_Isend(tempStart, strlen(tempStart) + 1, MPI_BYTE,
//          status.MPI_SOURCE, tag, MPI_COMM_WORLD, &myReq);
//        MPI_Wait(&myReq, MPI_STATUS_IGNORE);
//        MPI_Isend(tempEnd, strlen(tempEnd) + 1, MPI_BYTE,
//          status.MPI_SOURCE, tag, MPI_COMM_WORLD, &myReq);
//        MPI_Wait(&myReq, MPI_STATUS_IGNORE);

//      MPI_Request myReq;
//      MPI_Isend(&my_rank,1,MPI_INT, 0, tag, MPI_COMM_WORLD, &myReq);
//      MPI_Wait(&myReq, &status);
//      MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
//      MPI_Get_count(&status, MPI_BYTE, &bufferSize);
//      MPI_Irecv(startStr,bufferSize, MPI_BYTE, 0, MPI_ANY_TAG,
//        MPI_COMM_WORLD, &myReq);

//      MPI_Wait(&myReq, &status);
//      MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
//      MPI_Get_count(&status, MPI_BYTE, &bufferSize);
//      MPI_Irecv(endStr,bufferSize, MPI_BYTE, 0, MPI_ANY_TAG,
//        MPI_COMM_WORLD, &myReq);
//      MPI_Wait(&myReq, &status);
//  MPI_Gather(&mytime, 1, MPI_DOUBLE, rbuf, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
