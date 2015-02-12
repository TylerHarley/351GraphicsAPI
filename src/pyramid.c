/*
 * Tyler Harley and Margaux LeBlanc
 * pyramid.c
 * 10/17/14
 * file that creates a unit pyramid and draws it.
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

int main(int argc, char *argv[]){
	const int columns = 200;
	const int rows = 200;
	int i,t;
	Image *src;
	Matrix vtm, rotate, translate;
	View3D view;

	Point pt[5];
	Point tpt[3];
	Polygon side[5];
	Polygon tpoly[5];	
	int nFrames = 40;
    char filename[256];
	int angle = 0;
	Vector tmp, tmpTrans;
	Color pink, green, blue, purple, orange;
	
	src = image_create(rows, columns);

	/* construct pyramid */

	//set colors
	color_set(&pink, .941, 0.0, .635);
	color_set(&green, 0.0, .941, .031);
	color_set(&blue, 0.0, .941, .921);
	color_set(&purple, .403, 0.0, .941 );
	color_set(&orange, .941, .509, 0.0);
	
	// initialize polygons
	for(i=0;i<6;i++) {
		polygon_init( &side[i] );
	}

	// points of a pyramid
	point_set3D( &pt[0], 1, 0, 0 );
	point_set3D( &pt[1],  0, 0, 1 );
	point_set3D( &pt[2],  -1,  0, 0 );
	point_set3D( &pt[3], 0,  0, -1 );
	point_set3D( &pt[4], 0, 1,  0 );

	// bottom
	polygon_set( &side[0], 4, &(pt[0]) );

	// front side
	point_copy( &tpt[0], &pt[2] );
	point_copy( &tpt[1], &pt[3] );
	point_copy( &tpt[2], &pt[4] );

	polygon_set( &side[1], 3, tpt );

	//left side
	point_copy( &tpt[0], &pt[1] );
	point_copy( &tpt[1], &pt[2] );
	point_copy( &tpt[2], &pt[4] );

	polygon_set( &side[2], 3, tpt );

	// right side
	point_copy( &tpt[0], &pt[3] );
	point_copy( &tpt[1], &pt[0] );
	point_copy( &tpt[2], &pt[4] );

	polygon_set( &side[3], 3, tpt );

	// back side
	point_copy( &tpt[0], &pt[0] );
	point_copy( &tpt[1], &pt[1] );
	point_copy( &tpt[2], &pt[4] );

	polygon_set( &side[4], 3, tpt );
	printf("set polygon pyramid\n");

	/* create view */
	point_set3D( &(view.vrp), 5, 5, -1);
	vector_set(&(view.vpn), -view.vrp.val[0], -view.vrp.val[1], -view.vrp.val[2] -4);
	vector_set( &(view.vup), 0, 1, 0);
	view.d = 1;
	view.du = .5;
	view.dv = view.du * (float)rows / columns;
	view.f = 0;
	view.b = 4;
	view.screenx = columns;
	view.screeny = rows;

	matrix_setView3D( &vtm, &view );

	//create series of frames for gif
	for(t = 0; t<nFrames; t++){
		//fill in black background.		
		image_fillrgb(src, 0.0, 0.0, 0.0);
		/* copy polygons into temp list first and apply VTM */
		for(i=0; i<5; i++){
			polygon_copy(&tpoly[i], &side[i]);
		} 
		//apply VTM
		for(i=0; i<5; i++){
			matrix_xformPolygon(&vtm, &tpoly[i]);
			polygon_normalize(&tpoly[i]);
		}
		/* draw */
		polygon_drawFill(&tpoly[0], src, orange);
		polygon_drawFill(&tpoly[1], src, pink);
		polygon_drawFill(&tpoly[2], src, green);
		polygon_drawFill(&tpoly[3], src, blue);
		polygon_drawFill(&tpoly[4], src, purple);

		//write out images
    	sprintf(filename, "../images/pyramid/img-%04d.ppm", t );
		image_write(src, filename);
		//printf("wrote images\n");

		//rotate by moving vup. Some odd stuff happens here. Not sure what.
		/*angle = 2*(t+1)/nFrames; //uncomment to rotate scene about z axis
		matrix_identity(&rotate);
		matrix_set(&rotate, 0, 0, cos(angle));
		matrix_set(&rotate, 0, 1, -sin(angle));
		matrix_set(&rotate, 1, 0, sin(angle));
		matrix_set(&rotate, 1, 1, cos(angle));
		matrix_xformVector(&rotate, &view.vup, &tmp);
		vector_copy(&view.vup, &tmp);*/

		//zoom in on scene
		point_set3D( &(view.vrp), 5, 5, -1 + .25*t);
		//translate so looking at side of pyramid by moving vpn
		matrix_identity(&translate);
		matrix_set(&translate, 0, 3, 5+ 5*t);
		matrix_set(&translate, 1, 3, 5 + 5*t);
		matrix_xformVector(&translate, &view.vpn, &tmpTrans);
		vector_copy(&view.vpn, &tmpTrans);
    	matrix_setView3D( &vtm, &view );
    	//matrix_print( &vtm, stdout );
	}
	
	image_free(src);
	
	return(0);
}
