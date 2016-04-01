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
#include "renderer.h"
#include "vector3d.h"
#include <math.h>
#include <algorithm>

using namespace std;

//---lightning and colouring---------
#define CAM_LIGHT {.x=1.0,.y=1.0,.z=1.0}
#define CAM_LIGHT_W 1.8 // 1.27536;
#define CAM_LIGHT_MIN 0.3 // 0.48193;
//-----------------------------------
#define BASE_COLOR {.x=1.0, .y=1.0, .z=1.0}
#define BACK_COLOR {.x=0.4,.y=0.4,.z=0.4}
//-----------------------------------

inline void lighting(const vec3 &n, const vec3 &color, const vec3 &pos, const vec3 &direction,  vec3 &outV)
{
  vec3 CamLight = CAM_LIGHT;
  double CamLightW = CAM_LIGHT_W;
  double CamLightMin = CAM_LIGHT_MIN;

  vec3 nn;

  SUBTRACT_POINT_DOUBLE(nn, n, 1.0);
  //vec3 nn = n -1.0;
  double d = 0.0;
  DOT(d, direction, nn);
  double ambient = MAX(CamLightMin, d) * CamLightW;
  //double ambient = max( CamLightMin, nn.Dot(direction) )*CamLightW;
  MULT_DOUBLE(outV, CamLight, ambient);
  MULT_VEC(outV, outV, color);
  //outV = CamLight*ambient*color;
}

void getColour(const pixelData &pixData, const RenderParams &render_params,
	       const vec3 &from, const vec3  &direction, vec3  &hitColor)
{
  vec3 baseColor = BASE_COLOR;
  vec3 backColor = BACK_COLOR;

  //colouring and lightning
  hitColor = baseColor;

  if (pixData.escaped == false)
    {
      //apply lighting
      lighting(pixData.normal, hitColor, pixData.hit, direction, hitColor);

      //add normal based colouring
      if(render_params.colourType == 0 || render_params.colourType == 1)
	{
    MULT_VEC(hitColor, hitColor, pixData.normal);
    ADD_DOUBLE(hitColor, hitColor, 1.0);
    MULT_DOUBLE(hitColor, hitColor, 0.5);
	  //hitColor = hitColor * pixData.normal;
	  //hitColor = (hitColor + 1.0)/2.0;
    MULT_DOUBLE(hitColor, hitColor, render_params.brightness);
	  //hitColor = hitColor*render_params.brightness;

	  //gamma correction
	  CLAMP(hitColor, 0.0, 1.0);
    //clamp(hitColor, 0.0, 1.0);
    MULT_VEC(hitColor, hitColor, hitColor);
	  //hitColor = hitColor*hitColor;
	}
      if(render_params.colourType == 1)
	{
	  //"swap" colors
	  double t = hitColor.x;
	  hitColor.x = hitColor.z;
	  hitColor.z = t;
	}
    }
  else
    //we have the background colour
    hitColor = backColor;

  //return hitColor;
}
