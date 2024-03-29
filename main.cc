/*
	 This file is part of the Mandelbox program developed for the course
		CS/SE  Distributed Computer Systems taught by N. Nedialkov in the
		Winter of 2015-2016 at McMaster University.

		Copyright (C) 2015-2016 T. Gwosdz and N. Nedialkov

		This program is free software: you can redistribute it and/or modify
		it under the terms of the GNU General Public License as published by
		the Free Software Foundation, either version 3 of the License, or
		(at your option) any later version.

		This program is distributed in the hope that it will be useful,
		but WITHOUT ANY WARRANTY; without even the implied warranty of
		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
		GNU General Public License for more details.

		You should have received a copy of the GNU General Public License
		along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "camera.h"
#include "renderer.h"
#include "mandelbox.h"
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <ctime>
#include <sys/stat.h>
#include <unistd.h>

#define MAX_FRAMES  7200
#define AUTO_FRAMES 10

void getParameters(char *filename, RenderParams *renderer_params, MandelBoxParams *mandelBox_paramsP);
void getPath      (char *filename, CameraParams *camera_path, int *len);
void getNextFrame (CameraParams * next_frame, RenderParams * renderer_params, float * dist_matrix);
void init3D       (CameraParams *camera_params, const RenderParams *renderer_params);
void renderFractal(const CameraParams &camera_params, const RenderParams &renderer_params, unsigned char* image, float * dist_matrix);
void saveBMP      (const char* filename, const unsigned char* image, int width, int height);


MandelBoxParams mandelBox_params;

int main(int argc, char** argv)
{
	//struct timeval timerStart;
	//struct timeval timerEnd;
	double time_spent;

	printf("timer starting\n");
	#ifdef _OPENMP
		double p_time;
		p_time = omp_get_wtime();
	#else
		clock_t begin, end;
		begin = clock();
	#endif

	// adapted from:
	//http://stackoverflow.com/questions/9314586/c-faster-way-to-check-if-a-directory-exists
	struct stat s;
	int err = stat("/path/to/frames", &s);
	if (-1 == err)
	{
		mkdir("frames", 0700);
	}
	else if (S_ISDIR(s.st_mode))
	{
		/* it's a dir */
	}
	else
	{
		/* exists but is no dir */
	}

	assert(argc == 2 | argc == 3);

	char* path;
	CameraParams * camera_path;
	CameraParams * next_frame;
	RenderParams renderer_params;
	int nframes;
	char frame_name[256];

	char auto_path;

	getParameters(argv[1], &renderer_params, &mandelBox_params);

	if (argc == 3)
	{
		auto_path = false;

		camera_path = (CameraParams*)malloc(MAX_FRAMES*sizeof(CameraParams));
		assert(camera_path);

		path = (char*)malloc(sizeof(char) * (strlen(argv[2]) + 1));
		memcpy(path, argv[2], strlen(argv[2]) + 1);
		printf("%s\n", path);

		getPath(path, camera_path, &nframes);
	}
	else
	{
		auto_path = true;

		next_frame = (CameraParams*)malloc(sizeof(CameraParams));

		path = (char*)malloc(sizeof(char) * (strlen("auto_start.dat") + 1));
		sprintf(path, "auto_path.dat");
		printf("%s\n", path);

		getPath(path, next_frame, &nframes);

		nframes = AUTO_FRAMES;
	}

	int image_size = renderer_params.width * renderer_params.height;
	unsigned char *image = (unsigned char*)malloc(3*image_size*sizeof(unsigned char));
	float * dist_matrix = (float *)malloc(4 * image_size * sizeof(float));

	for (int i = 0; i < nframes; ++i)
	{
		if (auto_path == true)
		{
			init3D(next_frame, &renderer_params);
			renderFractal(*next_frame, renderer_params, image, dist_matrix);
			getNextFrame(next_frame, &renderer_params, dist_matrix);
		}
		else
		{
			init3D(&camera_path[i], &renderer_params);
			renderFractal(camera_path[i], renderer_params, image, dist_matrix);
		}

		// printf("frame = %d\tzero dist = %f\t x = %f\ty = %f\tz = %f\n", i, dist_matrix[0], dist_matrix[1], dist_matrix[2], dist_matrix[3]);

		//TODO create render directory from input
		//TODO create starting name from number from input (in case of previously generated frames)
		snprintf(frame_name, sizeof(char) * 256, "./frames/frame_%04d.bmp", i);
		saveBMP(frame_name, image, renderer_params.width, renderer_params.height);

	}

	printf("done\n");

	free(image);
	free(dist_matrix);
	if (auto_path == false)
	{
		free(camera_path);
	}
	free(path);


	#ifdef _OPENMP
		p_time = omp_get_wtime()-p_time;
		printf("time is %f\n", p_time);
	//#elif _OPENACC
	//	gettimeofday(&timerEnd, NULL);
	//	double time_acc = (timerEnd.tv_sec-timerStart.tv_sec) + (timerEnd.tv_usec - timerEnd.tv_usec)*1e-6;
	//	printf("time is %f\n", time_acc);
	#else
		end = clock();
		time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		printf("time is %f\n", time_spent);
	#endif


	return 0;
}
