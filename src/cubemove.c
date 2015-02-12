/*
 * Tyler Harley
 * cubeshift.c
 * Example of a cube passing through a plane-like
 * solid, showing z-buffering
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <color.h>
#include <list.h>
#include <image.h>
#include <math.h>
#include "graphics.h"
#include "drawstate.h"
#include "polygon.h"
#include "matrix.h"
#include "view.h"
#include "model.h"

int main(int argc, char *argv[]) {
	int rows = 450;
	int cols = 450;
	int frame;
	int j = 0;
	Matrix VTM, GTM;
	View3D view;
	DrawState *ds;
	Module *cube, *plane, *scene;
	Point p[4];
	Polygon poly;
	
	Color green;
	color_set(&green, 0, 1, 1);
	Color yellow;
	color_set(&yellow, 1, 1, 0);

	Image *src = image_create(rows, cols);
	// create the cube
	cube = module_create();
	module_color(cube, &green);
	module_cube(cube, 1);
	
	// create the solid "plane"
	plane = module_create();
	module_color(plane, &yellow);
	polygon_init(&poly);
	point_set3D(&p[0], 1.5, 1.5, 2);
	point_set3D(&p[1], -1.5, 1.5, 2);
	point_set3D(&p[2], -1.5, -1.5, 2);
	point_set3D(&p[3], 1.5, -1.5, 2);
		
	polygon_set(&poly, 4, p);
	module_polygon(plane, &poly);
	module_translate(plane, 0, 0, 0.5); 
	module_polygon(plane, &poly);
	module_translate(plane, 0, 0, -0.5);

	point_set3D(&p[0], 1.5, 1.5, 2);
	point_set3D(&p[1], 1.5, 1.5, 2.5);
	point_set3D(&p[2], -1.5, 1.5, 2.5);
	point_set3D(&p[3], -1.5, 1.5, 2);

	polygon_set(&poly, 4, p);
	module_polygon(plane, &poly);
	module_translate(plane, 0, -3, 0);
	module_polygon(plane, &poly);
	module_translate(plane, 0, 3, 0);

	point_set3D(&p[0], 1.5, 1.5, 2);
	point_set3D(&p[1], 1.5, 1.5, 2.5);
	point_set3D(&p[2], 1.5, -1.5, 2.5);
	point_set3D(&p[3], 1.5, -1.5, 2);

	polygon_set(&poly, 4, p);
	module_polygon(plane, &poly);
	module_translate(plane, -3, 0, 0);
	module_polygon(plane, &poly);
	module_translate(plane, 3, 0, 0);

	// set up the view
	point_set3D(&(view.vrp), -2.0, 1.0, -0.5);
	vector_set(&(view.vpn), 0.5, -0.2, 0.5);
	vector_set(&(view.vup), 0.0, 1.0, 0.0);
	view.d = 7.0;
	view.du = 5.0;
	view.dv = 5.0;
	view.f = 0.5;
	view.b = 10;
	view.screenx = cols;
	view.screeny = rows;
	matrix_setView3D(&VTM, &view);
	
	matrix_identity(&GTM);
	// draw scene
	scene = module_create(); 
	ds = drawstate_create();
 	ds->shade = ShadeDepth;
	drawstate_setFrontClipPlane(ds, 1.0/( (view.d + view.f) / (view.d + view.b) ));
	
	for(frame=0; frame<100; frame++){
		char buffer[256];
		// reset scene		
		module_clear(scene);
		image_fillrgb(src, 1, 1, 1);
		image_fillz(src, 1.0);
		
		// move the cube
		module_module(scene, plane);
		module_translate(scene, 0, 0, (0.04 * frame));
		module_module(scene, cube);
		
		// rotate the camera
		if((frame > 0 && frame < 50) || (frame > 64)){
			matrix_identity(&GTM);
			matrix_translate(&GTM, .0125*j, 0, -.025*j);
			matrix_rotateY(&GTM, cos(j*2*M_PI/400.0), sin(j*2*M_PI/400.0));
			j++;
		}
		module_draw(scene, &VTM, &GTM, ds, NULL, src);
		sprintf(buffer, "../images/cubemove/cubemove-frame%03d.ppm", frame);
		image_write(src, buffer);
	}

	// clean up
	polygon_clear(&poly);
	module_delete(cube);
	module_delete(plane);
	module_delete(scene);
	image_free(src);
	return(0);
}
