/*
 * Tyler Harley
 * raindrops.c
 * A raindrop scene implemented with Bezier Curves
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
	Point p[4];
	BezierCurve top, bot;
	DrawState ds;
	Module *drop, *scene;
	View2D view;
	Matrix VTM, GTM;
	int divisions = 4;
	int rows = 500, cols = 500;
	Image *src = image_create(rows, cols);

	color_set(&blue, 10/255.0, 10/255.0, 180/255.0);

	// grab the command line argument, if one exists
	if(argc > 1) {
		int tmp = atoi(argv[1]);
		if( tmp >= 0 && tmp < 10 )
			divisions = tmp;
	}

	// create bezier curves that form drop
	drop = module_create();
	point_set2D(&p[0], 0.0, 0.0);
	point_set2D(&p[1], 0.0, -0.2);
	point_set2D(&p[2], 0.0, -0.3);
	point_set2D(&p[3], -0.1, -0.4);
	bezierCurve_set(&top, p);

	module_bezierCurve(drop, &top, divisions);
	
	point_set2D(&p[0], -0.1, -0.4);
	point_set2D(&p[1], -0.3, -0.6);
	point_set2D(&p[2], -0.3, -0.9);
	point_set2D(&p[3], 0.0, -0.93);
	bezierCurve_set(&bot, p);
	
	module_bezierCurve(drop, &bot, divisions);

	module_scale2D(drop, -1, 1);
	module_bezierCurve(drop, &top, divisions);
	module_bezierCurve(drop, &bot, divisions);

	// create scene with randomly placed raindrops
	scene = module_create();

	for(i=0; i<500; i++){
		if(drand48() <= 0.5)
			module_translate2D(scene, -(drand48()*4 - 1), -(drand48()*12 - 5));
		else
			module_translate2D(scene, drand48()*4 - 1, drand48()*12 - 5);
		module_module(scene, drop);
	}

	// setup camera
	point_set2D(&(view.vrp), 10.0, 0.0);
	vector_set(&(view.x), 1, 0, 0);
	view.du = 6;
	view.C = cols;
	view.R = cols;

	matrix_setView2D( &VTM, &view );
	matrix_identity(&GTM);

	// draw scene
	drawstate_setColor(&ds, blue);
	
	printf("Writing images...\n");
	for(frame=0; frame<120; frame++){
		image_fillrgb(src, 0.85, 0.85, 0.85);
		char buffer[256];
		matrix_translate2D(&GTM, 0.0, -0.20);
		module_draw(scene, &VTM, &GTM, &ds, NULL, src);
		sprintf(buffer, "../images/rain/rain-frame%03d.ppm", frame);
		image_write(src, buffer);
	}
	
	// cleanup	
	image_free( src );

	module_delete( drop );
	module_delete( scene );
	return(0);
}
