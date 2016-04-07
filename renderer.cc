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
#include <stdio.h>
#include <string.h>

#include "color.h"
#include "mandelbox.h"
#include "camera.h"
#include "vector3d.h"
#include "3d.h"

extern MandelBoxParams mandelBox_params;

// extern double getTime();
// extern void   printProgress( double perc, double time );

#pragma acc routine seq
extern void UnProject(float winX, float winY, CameraParams camP, float *obj);

#pragma acc routine seq
void MultiplyMatrixByVector1(float *resultvector, const float *matrix, float *pvector)
{
  resultvector[0]=matrix[0]*pvector[0]+matrix[4]*pvector[1]+matrix[8]*pvector[2]+matrix[12]*pvector[3];
  resultvector[1]=matrix[1]*pvector[0]+matrix[5]*pvector[1]+matrix[9]*pvector[2]+matrix[13]*pvector[3];
  resultvector[2]=matrix[2]*pvector[0]+matrix[6]*pvector[1]+matrix[10]*pvector[2]+matrix[14]*pvector[3];
  resultvector[3]=matrix[3]*pvector[0]+matrix[7]*pvector[1]+matrix[11]*pvector[2]+matrix[15]*pvector[3];
}

#pragma acc routine seq
void local_UnProject(float winX, float winY, const int * viewport, const float * matInvProjModel, float *obj)
{
  //Transformation vectors
  float in[4], out[4];
  
  //Transformation of normalized coordinates between -1 and 1
  in[0]=(winX-(float)(viewport[0]))/(float)(viewport[2])*2.0-1.0;
  in[1]=(winY-(float)(viewport[1]))/(float)(viewport[3])*2.0-1.0;
  in[2]=2.0-1.0;
  in[3]=1.0;
  
  //Objects coordinates
  const float *matrix = matInvProjModel;
  MultiplyMatrixByVector1(out, matrix, in);
  
  if(out[3]==0.0)
    return;

  out[3] = 1.0/out[3];
  obj[0] = out[0]*out[3];
  obj[1] = out[1]*out[3];
  obj[2] = out[2]*out[3];
  return;
}



#pragma acc routine seq
extern void rayMarch (const RenderParams &render_params, const vec3 &from, const vec3  &to, \
                      float eps, pixelData &pix_data, MandelBoxParams &box_params, float * tot_dist);
#pragma acc routine seq
extern void getColour(const pixelData &pixData, const RenderParams &render_params,
          const vec3 &from, const vec3  &direction, vec3 &hitcolor);


void renderFractal(const CameraParams &camera_params, const RenderParams &renderer_params,
		               unsigned char* image, float * dist_matrix)
{

  const float eps = powf(10.0, renderer_params.detail);
  vec3 from;

  //from.SetDoublePoint(camera_params.camPos);
  from.x = camera_params.camPos[0];
  from.y = camera_params.camPos[1];
  from.z = camera_params.camPos[2];

  // const double * from_x = &(camera_params.camPos[0]);
  // const double * from_y = &(camera_params.camPos[1]);
  // const double * from_z = &(camera_params.camPos[2]);

  const int height = renderer_params.height;
  const int width  = renderer_params.width;

  // pixelData pix_data;

  // double time1 = getTime();

  int n = height*width;

  int viewport[4];
  float matInvProjModel[16];

  memcpy(viewport, camera_params.viewport, 4*sizeof(int));
  memcpy(matInvProjModel, camera_params.matInvProjModel, 16*sizeof(float));
  int size1 = 4*sizeof(int);
  int size2 = 16*sizeof(float);
  // int size3 = sizeof(vec3);

  RenderParams renderer_params1 = renderer_params;
  
  vec3 color, to;
  pixelData pix_data;
  float farPoint[3];

  #pragma acc data copyout(image[0:3*n], dist_matrix[0:4*n]) copyin(eps, from, renderer_params1, mandelBox_params, viewport[:size1], matInvProjModel[:size2])
  {
  #pragma acc parallel loop independent private(color, to, pix_data, farPoint) //present(image, eps, from, renderer_params1, mandelBox_params, viewport, matInvProjModel)
  for(int j = 0; j < height; j++)
  {
    //for each column pixel in the row
    //#pragma acc for independent private(j) shared (image[0:3*n])
    #pragma acc loop private(color, to, pix_data, farPoint)
    for(int i = 0; i < width; i++)
    {
      // float * dist;

      local_UnProject(i, j, viewport, matInvProjModel, farPoint);

      to.x = farPoint[0] - from.x;
      to.y = farPoint[1] - from.y;
      to.z = farPoint[2] - from.z;

      NORMALIZE(to);

      rayMarch(renderer_params1, from, to, eps, pix_data, mandelBox_params, &dist_matrix[(4 * j * width) + (4 * i)]);

      getColour(pix_data,renderer_params1, from, to, color);

      int k = (j * width + i)*3;
      image[k+2] = (unsigned char)(color.x * 255);
      image[k+1] = (unsigned char)(color.y * 255);
      image[k]   = (unsigned char)(color.z * 255);
    }
  }
  }
  // printf("completed frame\n");
}
