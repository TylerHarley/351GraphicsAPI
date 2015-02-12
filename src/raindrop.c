/*
 * Tyler Harley
 * raindrop.c
 * A raindrop implemented with a Bezier Surface
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
	Module *drop;
	View3D view;
	Matrix VTM, GTM;
	int divisions = 3;
	int rows = 500, cols = 300;
	Image *src = image_create(rows, cols);

	// grab the command line argument, if one exists
	if(argc > 1) {
		int tmp = atoi(argv[1]);
		if( tmp >= 0 && tmp < 10 )
			divisions = tmp;
	}

	color_set(&blue, .6, .9, 1);
	
	drop = module_create();
	point_set3D(&p[0], 0.0, 1, 0.0); 
	point_set3D(&p[1], 0.1, .55, 0.0);
	point_set3D(&p[2], 0.3, .3, 0.0);
	point_set3D(&p[3], 0.6, 0.0, 0.0);
	
	point_set3D(&p[4], 0.0, 1, 0.0); 
	point_set3D(&p[5], 0.066, .55, 0.033);
	point_set3D(&p[6], 0.2, .3, 0.1);
	point_set3D(&p[7], 0.4, 0.0, 0.2);
	
	point_set3D(&p[8], 0.0, 1, 0.0);
	point_set3D(&p[9], 0.033, .55, 0.066);
	point_set3D(&p[10], 0.1, .3, 0.2);
	point_set3D(&p[11], 0.2, 0.0, 0.4);
	
	point_set3D(&p[12], 0.0, 1, 0.0); 
	point_set3D(&p[13], 0.0, .55, 0.1);
	point_set3D(&p[14], 0.0, .3, 0.3);
	point_set3D(&p[15], 0.0, 0.0, 0.6);
	bezierSurface_set(&bc, p);

	module_color(drop, &blue);
	module_bezierSurface(drop, &bc, divisions, 0);
	module_rotateY(drop, 0, 1);
	module_translate(drop, 0, 0, 1);
	//module_bezierSurface(drop, &bc, divisions, 0);

	// set up drawstate
	ds.shade = ShadeConstant;

	// view
	point_set3D(&(view.vrp), 1, .5, 1);
	vector_set(&(view.vpn), 1, 0, 1);
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
	for(frame=0;frame<60;frame++) {
		char buffer[256];
		
		matrix_rotateY(&GTM, cos(M_PI/30.0), sin(M_PI/30.0) );
		module_draw( drop, &VTM, &GTM, &ds, NULL, src );

		sprintf(buffer, "../images/drop/drop-frame%03d.ppm", frame);
		image_write(src, buffer);
		image_reset(src);
	}

	// clean up
	image_free( src );

	module_delete( drop );

	return(0);
}
