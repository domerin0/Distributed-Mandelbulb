
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
#include "color.h"
#include "mandelbox.h"
#ifdef _OPENACC
#include <accelmath.h>
#else
#include <math.h>
#endif



float MandelBulbDistanceEstimator(const vec3 &p0, const MandelBoxParams &params)
{
  vec3 z;
  z = p0;
  
  float dr = 1.0;
  float r = 0.0;

  float Bailout = params.rMin;
  float Power = params.rFixed;

  for (int i=0; i < params.num_iter; i++) 
    {
      MAGNITUDE(r,z);
      if(r > Bailout) break; 

      float theta = acosf(z.z/r);
      float phi   = atan2f(z.y, z.x);
      dr = powf(r, Power - 1.0) * Power * dr + 1.0;

      float zr = powf(r, Power);
      theta     = theta * Power;
      phi       = phi * Power;

      z.x = zr*sinf(theta)*cosf(phi);
      z.y = zr*sinf(phi)*sinf(theta);
      z.z = zr*cosf(theta);

      z.x = z.x + p0.x;
      z.y = z.y + p0.y;
      z.z = z.z + p0.z;
    }

  return 0.5*logf(r)*r/dr;
}





