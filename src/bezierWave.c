/*
 * Tyler Harley
 * bezierWave.c
 * A wave shape implemented with a Bezier Surface
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <color.h>
#include <image.h>
#include <list.h>
#include <math.h>
#include "graphics.h"
#include "polygon.h"
#include "matrix.h"
#include "view.h"
#include "drawstate.h"
#include "model.h"
#include "bezier.h"

int main(int argc, char *argv[]){
	int frame;
	int i;
	Color blue;
	Point p[16];
	BezierSurface bc;
	DrawState ds;
	Module *curve;
	View3D view;
	Matrix VTM, GTM;
	int divisions = 3;
	int rows = 300, cols = 400;
	Image *src = image_create(rows, cols);

	// grab the command line argument, if one exists
	if(argc > 1) {
		int tmp = atoi(argv[1]);
		if( tmp >= 0 && tmp < 10 )
			divisions = tmp;
	}

	color_set(&blue, 25/255.0, 71/255.0, 163/255.0);

	curve = module_create();
	point_set3D(&p[0], 0.0, .25, 0.0); 
	point_set3D(&p[1], 0.25, 0.0, 0.0);
	point_set3D(&p[2], 0.5, -.25, 0.0);
	point_set3D(&p[3], 0.75, 0.0, 0.0);
	point_set3D(&p[4], 0.0, .25, 0.25); 
	point_set3D(&p[5], 0.25, 0.0, 0.25);
	point_set3D(&p[6], 0.5, -.25, 0.25);
	point_set3D(&p[7], 0.75, 0.0, 0.25);
	point_set3D(&p[8], 0.0, .25, 0.75);
	point_set3D(&p[9], 0.25, 0.0, 0.75);
	point_set3D(&p[10], 0.5, -.25, 0.75);
	point_set3D(&p[11], 0.75, 0.0, 0.75);
	point_set3D(&p[12], 0.0, .25, 1.0); 
	point_set3D(&p[13], 0.25, 0.0, 1.0);
	point_set3D(&p[14], 0.5, -.25, 1.0);
	point_set3D(&p[15], 0.75, 0.0, 1.0);
	bezierSurface_set(&bc, p);

	module_color(curve, &blue);
	module_bezierSurface(curve, &bc, divisions, 0);
	for(i=0; i<9; i++){
		module_scale(curve, 1, -1, 1);
		module_translate2D(curve, -.75, .25);
		module_bezierSurface(curve, &bc, divisions, 0);
	}
	
	// set up drawstate
	ds.shade = ShadeConstant;

	// view
	point_set3D(&(view.vrp), -5.3, .1, .6);
	vector_set(&(view.vpn), 1, -0.15, .1);
	vector_set(&(view.vup), 0, 1, 0);
	view.d = 4.5;
	view.du = 3.5;
	view.dv = 3.5*rows/cols;
	view.screeny = rows;
	view.screenx = cols;
	view.f = 0.0;
	view.b = 3.0;

	matrix_setView3D( &VTM, &view );
	matrix_identity( &GTM );
	printf("set up camera\n");
	
	// draw images
	for(frame=0;frame<90;frame++) {
		char buffer[256];
		
		matrix_translate(&GTM, -.05, 0, 0); // pan along the wave
		module_draw( curve, &VTM, &GTM, &ds, NULL, src );

		sprintf(buffer, "../images/wave/wave-frame%03d.ppm", frame);
		image_write(src, buffer);
		image_reset(src);
	}

	// clean up
	image_free( src );

	module_delete( curve );

	return(0);
}
