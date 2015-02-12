//Margaux LeBlanc
//10/17/18
//Graphics 2d scene that pans and zooms across the scene

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


int main(int argc, char *argv[]) {
  const int nLines = 30;
  const int nFrames = 30;
  const int rows = 180;
  const int cols = 320;
  View2D view;
  Matrix vtm;
  Matrix ltm;
  Color red, white;
  int i, t;
  char filename[256];
  Image *src;
  Point pt[4];
  Polygon p, temp;

  src = image_create( rows, cols );

  color_set(&red, .65, .047, .047); 
  color_set(&white, 1.0, 1.0, 1.0);
	
  //make a triangle to draw in [0,1] square
  point_set2D( &pt[0], 0, 0);
  point_set2D( &pt[1], 3, 0);
  point_set2D( &pt[2], 3, 4);
  point_set2D( &pt[3],0, 0);
  
  //set the polygon triangle
  polygon_init(&p);
  polygon_set(&p, 4, pt);

  // set up a view centered on (1.8, 1.8) with x pointing right
  point_set2D( &(view.vrp), 1.8, 1.8 );
  view.du = 30.0;
  vector_set( &(view.x), 1.0, 0.0, 0.0 );
  view.C = cols;
  view.R = rows;

  matrix_setView2D( &vtm, &view );
  printf("vtm:\n");
  matrix_print(&vtm, stdout);

  // loop for some number of frames
  for(t=0;t<nFrames;t++) {
    
    image_fillrgb(src, .65, .047, .047);

	//copy polygon then apply vtm and ltm
	polygon_copy(&temp, &p);
	printf("copied polygon\n");

	//transform polygon, normalize it, and draw it.
	matrix_xformPolygon(&vtm, &temp);
	printf("transformed polygon\n");
	polygon_normalize(&temp);
	printf("normalized polygon\n");
	polygon_drawFill(&temp, src, white);
	printf("drew polygon\n");

    printf("writing file\n");
    sprintf(filename, "../images/2DII/img-%04d.ppm", t );
    image_write( src, filename );
	printf("wrote file\n");
    // translate the view across the scene
    point_set2D( &(view.vrp), 1.8 - 2.5*(t+1)/nFrames, 1.8 - 2.5*(t+1)/nFrames );
	//zoom in on scene
	view.du = view.du - .98;
    matrix_setView2D( &vtm, &view );
    matrix_print( &vtm, stdout );
  }

  // cleanup
  image_free( src );

  return(0);
}
