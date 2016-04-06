


#include <stdio.h>
#include <stdlib.h>
// #include <string.h>

#ifdef _OPENACC
#include <accelmath.h>
#else
#include <math.h>
#endif

#include "renderer.h"
#include "camera.h"


void getNextFrame(CameraParams * next_frame, RenderParams * renderer_params, float * dist_matrix)
{
	// printf("next frame found probably\n");
}
