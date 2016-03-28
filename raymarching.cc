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
#include <assert.h>
#include <algorithm>
#include <stdio.h>

#include "color.h"
#include "renderer.h"

extern double DE(const vec3 &p);
void normal (const vec3 & p, vec3 & normal);

void rayMarch(const RenderParams &render_params, const vec3 &from, const vec3  &direction, double eps,
	      pixelData& pix_data)
{

  double dist = 0.0;
  double totalDist = 0.0;

  // We will adjust the minimum distance based on the current zoom

  double epsModified = 0.0;

  int steps=0;
  vec3 p, tempDir;
  do
    {
			MULT_DOUBLE(tempDir, direction, totalDist);
			//printf("tempDir x = %f, y = %f, z = %f\n", tempDir.x, tempDir.y, tempDir.z);
			ADD_POINT(p, from, tempDir);
			//printf("p x = %f, y = %f, z = %f\n", p.x, p.y, p.z);

			//p = from + direction * totalDist;
      dist = DE(p);

      totalDist += .95*dist;

      epsModified = totalDist;
      epsModified*=eps;
      steps++;
    }
  while (dist > epsModified && totalDist <= render_params.maxDistance && steps < render_params.maxRaySteps);

  vec3 hitNormal;
  if (dist < epsModified)
    {
      //we didnt escape
      pix_data.escaped = false;

      // We hit something, or reached MaxRaySteps
      pix_data.hit = p;

      //figure out the normal of the surface at this point
			MULT_DOUBLE(hitNormal, direction, epsModified);
			SUBTRACT_POINT(hitNormal, p, hitNormal);
			//const vec3 normPos = p - direction * epsModified;
      normal(hitNormal, pix_data.normal);
    }
  else
    //we have the background colour
    pix_data.escaped = true;
}


void normal(const vec3 & p, vec3 & normal)
{
  // compute the normal at p
  const double sqrt_mach_eps = 1.4901e-08;
	double mag = 0.0;
	MAGNITUDE(mag, p);
	double eps = MAX(mag, 1.0) * sqrt_mach_eps;
	//double eps = std::max( p.Magnitude(), 1.0 )*sqrt_mach_eps;

  vec3 e1 = {eps, 0,   0};
  vec3 e2= { 0 , eps, 0};
  vec3 e3 = {0  , 0, eps};
	vec3 x1, x2, x3, y1, y2, y3;
	ADD_POINT(x1, p, e1);
	ADD_POINT(x2, p, e2);
	ADD_POINT(x3, p, e3);

	SUBTRACT_POINT(y1, p, e1);
	SUBTRACT_POINT(y2, p, e2);
	SUBTRACT_POINT(y3, p, e3);
	//double x, y, x;
	//x = ;
	//y = ;
	//z = ;
	normal.x = DE(x1)-DE(y1);
	normal.y = DE(x2)-DE(y2);
	normal.z = DE(x3)-DE(y3);
  /*normal = {
		DE(x1)-DE(y1), //x
		DE(x2)-DE(y2), //y
		DE(x3)-DE(y3) //z
	};*/

	NORMALIZE(normal);
  //normal.Normalize();
}
