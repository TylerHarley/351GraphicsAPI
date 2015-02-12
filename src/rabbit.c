/*
 * Tyler Harley
 * rabbit.c
 * Draws a rabbit tangram made of polygons
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <list.h>
#include "color.h"
#include "image.h"
#include "graphics.h"
#include "polygon.h"

int main(int argc, char *argv[]){
	Image *src;
    const int rows = 400;
    const int cols = 400;
	Polygon *p;

	Color Red;
	Color Orange;
	Color Blue;
	Color Cyan;
	Color Pink;
	Color Green;
	Color Black;

	Point pt[4];

	int i;

	color_set(&Red, 0.9, 0.2, 0.1 );
  	color_set(&Orange, 0.95, 0.7, 0.3 );
  	color_set(&Blue, 0.2, 0.1, 0.95 );
	color_set(&Cyan, 10/255.0, 210/255.0, 1.0);
	color_set(&Pink, 243/255.0, 10/255.0, 1.0);
	color_set(&Green, 39/255.0, 1.0, 10/255.0);
	color_set(&Black, 0.0, 0.0, 0.0);

	src = image_create(rows, cols);
	image_fillrgb(src, 1.0, 1.0, 1.0);
	
	/* draw head */
	point_set2D(&(pt[0]), 290, 180);
    point_set2D(&(pt[1]), 250, 220);
    point_set2D(&(pt[2]), 290, 260);
    point_set2D(&(pt[3]), 330, 220);

    p = polygon_createp(4, pt);
    polygon_drawFill(p, src, Pink);

	/* draw front foot */
    point_set2D(&(pt[0]), 250, 220);
	point_set2D(&(pt[1]), 190, 280);
	point_set2D(&(pt[2]), 310, 280);

	polygon_set(p, 3, pt);
    polygon_drawFillB(p, src, Green);

	/* draw body */
	point_set2D(&(pt[0]), 190, 280);
	point_set2D(&(pt[1]), 265, 205);
	point_set2D(&(pt[2]), 115, 205);

	polygon_set(p, 3, pt);
    polygon_drawFillB(p, src, Blue);

	/* draw back foot */
	point_set2D(&(pt[0]), 115, 205);
	point_set2D(&(pt[1]), 190, 280);
	point_set2D(&(pt[2]), 40, 280);

	polygon_set(p, 3, pt);
    polygon_drawFillB(p, src, Red);

	/* draw tail */

	point_set2D(&(pt[0]), 115, 205);
    point_set2D(&(pt[1]), 95, 205);
    point_set2D(&(pt[2]), 95, 185);
    point_set2D(&(pt[3]), 115, 185);

	polygon_set(p, 4, pt);
    polygon_draw(p, src, Black);

	/* draw left ear */
	point_set2D(&(pt[0]), 290, 180);
    point_set2D(&(pt[1]), 230, 160);
    point_set2D(&(pt[2]), 160, 170);
    point_set2D(&(pt[3]), 220, 190);

	polygon_set(p, 4, pt);
    polygon_drawFill(p, src, Cyan);

	/* draw right ear */
	point_set2D(&(pt[0]), 290, 180);
    point_set2D(&(pt[1]), 275, 120);
    point_set2D(&(pt[2]), 285, 60);
    point_set2D(&(pt[3]), 300, 120);

	polygon_set(p, 4, pt);
    polygon_drawFill(p, src, Orange);

	polygon_free(p);
    image_write(src, "../images/rabbit.ppm");

    image_free(src);
	
	return(0);
}
