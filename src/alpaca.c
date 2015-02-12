/*
Margaux LeBlanc 
10/2/14
Graphics Project 4
Creates an image of an alpaca in a field with polygons and textures

  C version
*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <list.h>
#include "color.h"
#include "image.h"
#include "graphics.h"
#include "polygon.h"

/*
  Program to draw an alpaca
*/
int main(int argc, char *argv[]) {
  Image *src, *pattern, *grass, *skyPic;
  const int rows = 600;
  const int cols = 600;
  Polygon *p;
  Color Orange;
  Point pt[24];
  Point grnd[6];
  Point sky[6];

  color_set(&Orange, 0.95, 0.7, 0.3 );

  src = image_create(rows, cols);

  //Points for the sky
  point_set2D(&sky[1], 0, 600);
  point_set2D(&sky[2], 600, 600);
  point_set2D(&sky[3], 600, 0);
  point_set2D(&sky[4], 0,0);
  point_set2D(&sky[5], 0, 600);

  p = polygon_createp(6, sky);
  skyPic = image_read("../images/sky.ppm");
  polygon_drawFillPattern(p, src, Orange, skyPic);

  //Points for the ground
  point_set2D(&grnd[1], 0, 600);
  point_set2D(&grnd[2], 600, 600);
  point_set2D(&grnd[3], 600, 450);
  point_set2D(&grnd[4], 0, 450);
  point_set2D(&grnd[5], 0, 600);

  polygon_set(p, 6, grnd);
  grass = image_read("../images/grassbig.ppm");
  polygon_drawFillPattern(p, src, Orange, grass);

  //Points for Alpaca
  point_set2D(&pt[1], 360, 25);
  point_set2D(&pt[2], 375, 60);
  point_set2D(&pt[3], 410, 75);
  point_set2D(&pt[4], 410, 100);
  point_set2D(&pt[5], 450, 100);
  point_set2D(&pt[6], 450, 150);
  point_set2D(&pt[7], 390, 150);
  point_set2D(&pt[8], 390, 350);
  point_set2D(&pt[9], 360, 400);
  point_set2D(&pt[10], 360, 550);
  point_set2D(&pt[11], 300, 550);
  point_set2D(&pt[12], 300, 400);
  point_set2D(&pt[13], 160, 400);
  point_set2D(&pt[14], 125, 450);
  point_set2D(&pt[15], 150, 550);
  point_set2D(&pt[16], 85, 550);
  point_set2D(&pt[17], 60, 450);
  point_set2D(&pt[18], 50, 350);
  point_set2D(&pt[19], 85, 250);
  point_set2D(&pt[20], 325, 250);
  point_set2D(&pt[21], 325, 75);
  point_set2D(&pt[22], 340, 60);
  point_set2D(&pt[23], 360, 25);

  //create the alpaca
  polygon_set(p,24, pt);

  printf("drawing first big polygon\n");
  
  // get a pattern
  pattern = image_read("../images/furBig.ppm");
  polygon_drawFillPattern(p, src, Orange, pattern);

 
  printf("writing output\n");
  image_write(src, "../images/alpaca.ppm");
  printf("wrote output\n");

  image_free(grass);
  image_free(pattern);
  printf("freed pattern\n");
  image_free(src);
  printf("freed source\n");

  return(0);
}
