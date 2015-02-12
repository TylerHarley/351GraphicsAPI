/*
 * Tyler Harley and Margaux LeBlanc
 * 3Dscene.c
 * 10/17/14
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
	const int columns = 300;
	const int rows = 300;
	int i,t;
	int angle = 0;
	Image *src;
	Matrix vtm, rotate, translate;
	View3D view;

	Point pt[6];
	Polygon side[4];	
	Polygon roof[4];
	Polygon steps[6];
	Polygon door;
	Polygon tpoly[15];
	Color brown, blue, red, grey;
	int nFrames = 50;
    char filename[256];
	Vector tmp, tmpTrans;
	
	src = image_create(rows, columns);

	/* construct house */

	//set colors
	color_set(&red, 163/255.0, 29/255.0, 32/255.0);
	color_set(&blue, 0.1, 0.1, 0.85);
	color_set(&brown, 161/255.0, 125/255.0, 58/255.0);
	color_set(&grey, .6, .6, .6);
	
	/* roof */
	for(i=0; i<4; i++){
		polygon_init(&roof[i]);
	}

	point_set3D(&pt[0], 20, 70, 20);
	point_set3D(&pt[1], 50, 90, 20);
	point_set3D(&pt[2], 80, 70, 20);
	
	polygon_set(&roof[0], 3, pt);

	point_set3D(&pt[0], 20, 70, 100);
	point_set3D(&pt[1], 50, 90, 100);
	point_set3D(&pt[2], 80, 70, 100);

	polygon_set(&roof[1], 3, pt);

	point_set3D(&pt[0], 20, 70, 20);
	point_set3D(&pt[1], 50, 90, 20);
	point_set3D(&pt[2], 50, 90, 100);
	point_set3D(&pt[3], 20, 70, 100);

	polygon_set(&roof[2], 4, pt);

	point_set3D(&pt[0], 80, 70, 20);
	point_set3D(&pt[1], 50, 90, 20);
	point_set3D(&pt[2], 50, 90, 100);
	point_set3D(&pt[3], 80, 70, 100);

	polygon_set(&roof[3], 4, pt);

	/* sides */
	for(i=0; i<4; i++){
		polygon_init(&side[i]);
	}
	
	point_set3D(&pt[0], 20, 70, 20);
	point_set3D(&pt[1], 20, 30, 20);
	point_set3D(&pt[2], 80, 30, 20);
	point_set3D(&pt[3], 80, 70, 20);

	polygon_set(&side[0], 4, pt);

	point_set3D(&pt[0], 20, 70, 100);
	point_set3D(&pt[1], 20, 30, 100);
	point_set3D(&pt[2], 80, 30, 100);
	point_set3D(&pt[3], 80, 70, 100);

	polygon_set(&side[1], 4, pt);

	point_set3D(&pt[0], 20, 70, 20);
	point_set3D(&pt[1], 20, 70, 100);
	point_set3D(&pt[2], 20, 30, 100);
	point_set3D(&pt[3], 20, 30, 20);

	polygon_set(&side[2], 4, pt);

	point_set3D(&pt[0], 80, 70, 20);
	point_set3D(&pt[1], 80, 70, 100);
	point_set3D(&pt[2], 80, 30, 100);
	point_set3D(&pt[3], 80, 30, 20);

	polygon_set(&side[3], 4, pt);

	/* steps */
	for(i=0; i<6; i++){
		polygon_init(&steps[i]);
	}

	point_set3D(&pt[0], 45, 30, 0);
	point_set3D(&pt[1], 45, 35, 0);
	point_set3D(&pt[2], 55, 35, 0);
	point_set3D(&pt[3], 55, 30, 0);

	polygon_set(&steps[0], 4, pt);

	point_set3D(&pt[0], 45, 40, 10);
	point_set3D(&pt[1], 45, 35, 10);
	point_set3D(&pt[2], 55, 35, 10);
	point_set3D(&pt[3], 55, 40, 10);

	polygon_set(&steps[1], 4, pt);

	point_set3D(&pt[0], 45, 35, 0);
	point_set3D(&pt[1], 45, 35, 10);
	point_set3D(&pt[2], 55, 35, 10);
	point_set3D(&pt[3], 55, 35, 0);

	polygon_set(&steps[2], 4, pt);

	point_set3D(&pt[0], 45, 40, 10);
	point_set3D(&pt[1], 45, 40, 20);
	point_set3D(&pt[2], 55, 40, 20);
	point_set3D(&pt[3], 55, 40, 10);

	polygon_set(&steps[3], 4, pt);

	point_set3D(&pt[0], 45, 30, 0);
	point_set3D(&pt[1], 45, 35, 0);
	point_set3D(&pt[2], 45, 35, 10);
	point_set3D(&pt[3], 45, 40, 10);
	point_set3D(&pt[4], 45, 40, 20);
	point_set3D(&pt[5], 45, 30, 20);

	polygon_set(&steps[4], 6, pt);

	point_set3D(&pt[0], 55, 30, 0);
	point_set3D(&pt[1], 55, 35, 0);
	point_set3D(&pt[2], 55, 35, 10);
	point_set3D(&pt[3], 55, 40, 10);
	point_set3D(&pt[4], 55, 40, 20);
	point_set3D(&pt[5], 55, 30, 20);

	polygon_set(&steps[5], 6, pt);
	
	/* door */
	polygon_init(&door);
	
	point_set3D(&pt[0], 45, 55, 20);
	point_set3D(&pt[1], 45, 40, 20);
	point_set3D(&pt[2], 55, 40, 20);
	point_set3D(&pt[3], 55, 55, 20);

	polygon_set(&door, 4, pt);

	/* create view */
	point_set3D( &(view.vrp), 50, 50, -10);
	//vector_set( &(view.vpn), 0, 0, 1);
	vector_set(&(view.vpn), -view.vrp.val[0], -view.vrp.val[1], -view.vrp.val[2] );
	vector_set( &(view.vup), 0, 1, 0);
	view.d = 100;
	view.du = 150;
	view.dv = view.du * (float)rows / columns;
	view.f = 0;
	view.b = 50;
	view.screenx = columns;
	view.screeny = rows;

	matrix_setView3D( &vtm, &view );

	//create series of frames for gif
	for(t = 0; t<nFrames; t++){
		//fill in white background.		
		image_fillrgb(src, 1.0, 1.0, 1.0);
		/* copy polygons into temp list first and apply VTM */
		for(i=0; i<6; i++){
			polygon_copy(&tpoly[i], &steps[i]);
		}
		for(i=0; i<4; i++){
			polygon_copy(&tpoly[i+6], &roof[i]);
			polygon_copy(&tpoly[i+10], &side[i]);
		}
		polygon_copy(&tpoly[14], &door);
	
		for(i=0; i<15; i++){
			matrix_xformPolygon(&vtm, &tpoly[i]);
			polygon_normalize(&tpoly[i]);
		}

		/* draw */
		for(i=0; i<6; i++){
			polygon_drawFill(&tpoly[i], src, grey);
		}

		polygon_drawFillB(&tpoly[6], src, red);
		polygon_drawFillB(&tpoly[7], src, red);
		polygon_drawFill(&tpoly[8], src, red);
		polygon_drawFill(&tpoly[9], src, red);

		for(i=10; i<14; i++){
			polygon_drawFill(&tpoly[i], src, brown);
		}

		polygon_drawFill(&tpoly[14], src, blue);

		//write out images
    	sprintf(filename, "../images/house/img-%04d.ppm", t );
		image_write(src, filename);

    	// rotate the scene by moving vup
		/*angle = 2*(t+1)/nFrames; //uncomment to rotate scene about z axis
		matrix_identity(&rotate);
		matrix_set(&rotate, 0, 0, cos(angle));
		matrix_set(&rotate, 0, 1, -sin(angle));
		matrix_set(&rotate, 1, 0, sin(angle));
		matrix_set(&rotate, 1, 1, cos(angle));
		matrix_xformVector(&rotate, &view.vup, &tmp);
		vector_copy(&view.vup, &tmp);*/
		//zoom in on scene
		point_set3D( &(view.vrp), 50, 50, -10 + 5*t);
		//translate so looking at side of house by moving vpn
		matrix_identity(&translate);
		matrix_set(&translate, 0, 3, 50+ 10*t);
		matrix_set(&translate, 1, 3, 50 + 10*t);
		matrix_xformVector(&translate, &view.vpn, &tmpTrans);
		vector_copy(&view.vpn, &tmpTrans);
    	matrix_setView3D( &vtm, &view );
    	//matrix_print( &vtm, stdout );
	}
	
	image_free(src);
	
	return(0);
}
