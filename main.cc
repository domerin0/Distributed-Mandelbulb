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
#include <sys/stat.h>
#include <unistd.h>

#define MAX_FRAMES 7200

#define MAX_FRAMES 7200

void getParameters(char *filename, CameraParams *camera_params, RenderParams *renderer_params,
			 MandelBoxParams *mandelBox_paramsP);
void getPath      (char *filename, CameraParams *camera_path, int *len);
void init3D       (CameraParams *camera_params, const RenderParams *renderer_params);
void renderFractal(const CameraParams &camera_params, const RenderParams &renderer_params, unsigned char* image);
void saveBMP      (const char* filename, const unsigned char* image, int width, int height);


MandelBoxParams mandelBox_params;

int main(int argc, char** argv)
{
	// adapted from:
	//http://stackoverflow.com/questions/9314586/c-faster-way-to-check-if-a-directory-exists
	struct stat s;
	int err = stat("/path/to/frames", &s);
	if(-1 == err) {
  	mkdir("frames", 0700);
		} else {
    	if(S_ISDIR(s.st_mode)) {
        	/* it's a dir */
    	} else {
        	/* exists but is no dir */
    	}
		}
	assert(argc >= 2);
	char* path;
	if (argc == 3){
		path = (char*)malloc(sizeof(char) * (strlen(argv[2]) + 1));
		memcpy(path, argv[2], strlen(argv[2]) + 1);
		printf("%s\n", path);
	}else{
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


	int image_size = renderer_params.width * renderer_params.height;
	unsigned char *image = (unsigned char*)malloc(3*image_size*sizeof(unsigned char));

	for (int i = 0; i < nframes; ++i){

		init3D(&camera_path[i], &renderer_params);
		renderFractal(camera_path[i], renderer_params, image);

		//TODO create render directory from input
		//TODO create starting name from number from input (in case of previously generated frames)
		snprintf(frame_name, sizeof(char) * 256, "./frames/frame_%04d.bmp", i);
		saveBMP(frame_name, image, renderer_params.width, renderer_params.height);

	}

	free(image);

	return 0;
}
