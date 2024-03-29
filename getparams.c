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
#include <stdlib.h>
#include <string.h>
#include "renderer.h"
#include "mandelbox.h"
#include "camera.h"

#define BUF_SIZE 1024

static char buf[BUF_SIZE];

void getParameters(char *filename, RenderParams *renP, MandelBoxParams *boxP)
{
	FILE *fp;
	int ret;

	renP->fractalType = 0;
	renP->maxRaySteps = 8000;
	renP->maxDistance = 1000;

	fp = fopen(filename,"r");

	if( !fp )
		{
			printf(" *** File %s does not exist\n", filename);
			exit(1);
		}

	int count = 0;

	while (1){
		memset(buf, 0, BUF_SIZE);

		ret = fscanf(fp, "%1023[^\n]\n", buf);
		if (ret == EOF) break;

		if(buf[0] == '#') // comment line
		continue;

		switch(count){
			//IMAGE
			//width, height
			case 0:
				sscanf(buf, "%d %d", &renP->width, &renP->height);
				break;
				//detail
			case 1:
				sscanf(buf, "%f", &renP->detail);
				break;

			//FRACTAL
			case 2:
				sscanf(buf, "%f %f %f", &boxP->scale, &boxP->rMin, &boxP->rFixed);
				break;

			case 3:
				sscanf(buf, "%d %f ", &boxP->num_iter, &boxP->escape_time);
				break;

			//COLORING
			case 4:
				sscanf(buf, "%d", &renP->colourType);
				break;
			case 5:
				sscanf(buf, "%f ", &renP->brightness);
				break;
			}
		count++;
	}
	fclose(fp);
}
