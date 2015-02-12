/*
 * Tyler Harley & Margaux LeBlanc
 * model.c
 * Contains hierarchical modeling functions
 * 10/27/14
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

int main(int argc, char *argv[]) {
  const int rows = 600;
  const int cols = 600;
  DrawState *ds;
  Image *src;
  View3D view;
  Matrix vtm;
  Matrix gtm;
  Module *scene;
  Color blue;
 
  src = image_create(rows, cols);
  color_set(&blue, 65/255.0, 18/255.0, 1);

  //make gtm
  matrix_identity(&gtm);

  // setup vtm
  point_set3D( &(view.vrp), 1, 1, 1 );
  vector_set( &(view.vpn), -1, -1, -1 );
  vector_set( &(view.vup), 0, 1, 0 );
  view.d = 3;
  view.du = 2;
  view.dv = view.du * (float)rows/cols;
  view.f = 0;
  view.b = 4;
  view.screenx = cols;
  view.screeny = rows;

  matrix_setView3D( &vtm, &view );

  // create the scene
  scene = module_create();
  module_color(scene, &blue);
  module_cube(scene, 0);

  // draw scene
  ds = drawstate_create();
  ds->shade = ShadeFrame;
  module_draw(scene, &vtm, &gtm, ds, NULL, src);

  image_write( src, "../images/bloocubeLines.ppm" );

  module_delete(scene);

  free(ds);

  image_free(src);

  return(0);
}
