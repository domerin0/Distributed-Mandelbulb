


#include <stdio.h>
#include <stdlib.h>
// #include <string.h>

#ifdef _OPENACC
#include <accelmath.h>
#else
#include <math.h>
#endif
#include "vector3d.h"
#include "renderer.h"
#include "camera.h"

#define STEP_SIZE 0.001

#define STEP_EPSILON 1.1052
#define STEP_THETA 0.001
#define PI 3.141592654

inline void moveVec(CameraParams * next_frame, float max_dist);
inline void rotateVec(CameraParams * next_frame, float xTheta, float yTheta);

void getNextFrame(CameraParams * next_frame, RenderParams * renderer_params, float * dist_matrix)
{
	// printf("direction of largest distance x = %f, y = %f\n",
	// 	next_frame->camTarget[0],
	// 	next_frame->camPos[0]);
	int i,j, x, y;
	//buffer distance to walls
	float max_dist = 0.1;
	for (i = 0 ; i < renderer_params->height ; i++){
		for(j = 0 ; j < renderer_params->width ; j ++){
			if(max_dist < dist_matrix[(j *renderer_params->height) + i])
				max_dist = dist_matrix[(j *renderer_params->height) + i];
				x = j;
				y = i;
		}
	}

	float xTheta = x * ((2 * PI) / renderer_params->width);
	float yTheta = y *((2 * PI) / renderer_params->height);

	// printf("xtheta = %f, ytheta = %f\n", xTheta * PI / 180, yTheta * 180 / PI);

	rotateVec(next_frame, xTheta, yTheta);

	if(max_dist > STEP_SIZE){
		moveVec(next_frame, max_dist);
	}
}

/*
Determine direction of the cam pos

rotation about x:

|1     0           0| |x|   |        x        |   |x'|
|0   cos θ    −sin θ| |y| = |y cos θ − z sin θ| = |y'|
|0   sin θ     cos θ| |z|   |y sin θ + z cos θ|   |z'|

rotation about y:

		| cos θ    0   sin θ| |x|   | x cos θ + z sin θ|   |x'|
    |   0      1       0| |y| = |         y        | = |y'|
    |−sin θ    0   cos θ| |z|   |−x sin θ + z cos θ|   |z'|

*/
inline void rotateVec(CameraParams * next_frame, float xTheta, float yTheta){
	float x = next_frame-> camTarget[0] - next_frame-> camPos[0];
	float y = next_frame-> camTarget[1] - next_frame-> camPos[1];
	float z = next_frame-> camTarget[2] - next_frame-> camPos[2];

	//first rotate in x
	float newY = y * cosf(xTheta) - z * sinf(xTheta);
	float newZ = y * sinf(xTheta) + z * cosf(xTheta);

	//then rotate in y
	float newX = x * cosf(yTheta) - z * sinf(yTheta);
	newZ = z * cosf(yTheta) - x * sinf(yTheta);
	// printf("y = %f, newy = %f\n", y, newY);
	next_frame-> camTarget[0] = newX;
	next_frame-> camTarget[1] = newY;
	next_frame-> camTarget[2] = newZ;
}

/*
move the cam pos one unit in the direction of the cam target
*/
inline void moveVec(CameraParams * next_frame, float max_dist){
	float x = next_frame-> camTarget[0] - next_frame-> camPos[0];
	float y = next_frame-> camTarget[1] - next_frame-> camPos[1];
	float z = next_frame-> camTarget[2] - next_frame-> camPos[2];

	// next_frame-> camPos[0] = next_frame-> camPos[0] + ((x > 0) - (x < 0))*;
	// next_frame-> camPos[1] = next_frame-> camPos[1] + ((y > 0) - (y < 0))*;
	// next_frame-> camPos[2] = next_frame-> camPos[2] + ((z > 0) - (z < 0))*(max_dist);

	float mag = sqrtf((x * x) + (y * y) + (z * z));
	mag *= (1 / STEP_SIZE);

	x = x / mag;
	y = y / mag;
	z = z / mag;

	next_frame-> camTarget[0] += x;
	next_frame-> camTarget[1] += y;
	next_frame-> camTarget[2] += z;

	next_frame-> camPos[0] += x;
	next_frame-> camPos[1] += y;
	next_frame-> camPos[2] += z;
}
