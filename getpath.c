#include <assert.h>
#include <stdio.h>
#include "camera.h"

void getPath(char *filename, CameraParams *path, int *len)
{
	FILE* fid;

	fid = fopen(filename,"r");
	assert(fid != NULL);

	int line = 0;
	int ret;

	// read as many line as possible
	while (!feof(fid))
	{

		CameraParams *camP;
		camP = &path[line];

		//parse each line for camera position, target, up vector and field of view
		ret = fscanf(fid, "%lf %lf %lf\t%lf %lf %lf\t%lf %lf %lf\t%lf\n",  	&camP->camPos[0],    &camP->camPos[1],    &camP->camPos[2],
																			&camP->camTarget[0], &camP->camTarget[1], &camP->camTarget[2],
																			&camP->camUp[0],     &camP->camUp[1],     &camP->camUp[2],
																			&camP->fov);

		if(ret != 10) printf("Bad parse in pathfile, line:%d \n", line);

		line++;;
	}

	fclose(fid);
	*len = line;
}