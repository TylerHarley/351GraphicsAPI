//Margaux LeBlanc
//Graphics
//10/2/14
//Creates a picture of a car out of polygons

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <list.h>
#include "color.h"
#include "image.h"
#include "graphics.h"
#include "polygon.h"

/*
  Program to draw a car. I'm basically making SolidWorks!!!
*/
int main(int argc, char *argv[]) {
  Image *src;
  const int rows = 600;
  const int cols = 1000;
  Polygon *p;
  Line l1, l2, l3, l4;
  Color Red, Black, lightRed;
  Point pt[11];
  Point window1[6];
  Point window2[5];
  Point bmp[6];
  Ellipse wheel1, wheel2;
  Point c1, c2;

  color_set(&Red, 0.941, 0.211, 0.078 );
  color_set(&Black, 0.0, 0.0, 0.0);
  color_set(&lightRed, .952, .282, .207);

  src = image_create(rows, cols);
  image_fillrgb(src, .062, .074, .298);

  //Points for car
  point_set2D(&pt[1], 100, 350);
  point_set2D(&pt[2], 140, 425);
  point_set2D(&pt[3], 120, 450);
  point_set2D(&pt[4], 240, 550);
  point_set2D(&pt[5], 900, 400);
  point_set2D(&pt[6], 950, 300);
  point_set2D(&pt[7], 600, 220 );
  point_set2D(&pt[8], 400, 200);
  point_set2D(&pt[9], 300, 300);
  point_set2D(&pt[10], 100, 350);
  
  p = polygon_createp(11, pt);
  polygon_drawFill(p, src, Red);

  //front bumper
  point_set2D(&bmp[1], 140, 425);
  point_set2D(&bmp[2], 120, 450);
  point_set2D(&bmp[3], 240, 550);
  point_set2D(&bmp[4], 265, 525);
  point_set2D(&bmp[5], 140, 425);  

  polygon_set(p, 6, bmp);
  polygon_drawFill(p, src, lightRed);

  //Points for the first window
  point_set2D(&window1[1], 400, 225);
  point_set2D(&window1[2], 575, 275);
  point_set2D(&window1[3], 475, 370);
  point_set2D(&window1[4], 325, 300);
  point_set2D(&window1[5], 400, 225);

  polygon_set(p, 6, window1);
  polygon_drawFill(p, src, Black);

  //Points for the second window
  point_set2D(&window2[1], 585, 280);
  point_set2D(&window2[2], 490, 370);
  point_set2D(&window2[3], 850, 300);
  point_set2D(&window2[4], 585, 280);

  polygon_set(p, 5, window2);
  polygon_drawFill(p, src, Black);

  //accents
  line_set2D(&l1, 100, 350, 240, 450);
  line_set2D(&l2, 240, 450, 265, 525);
  line_set2D(&l3, 265, 525, 240, 550);
  line_set2D(&l4, 240, 450, 950, 300);
  line_draw(&l1, src, Black);
  line_draw(&l2, src, Black);
  line_draw(&l3, src, Black);
  line_draw(&l4, src, Black);

  //wheels
  point_set2D(&c1, 400, 490);
  point_set2D(&c2, 830, 400);

  ellipse_set(&wheel1, c1, 55, 65);
  ellipse_set(&wheel2, c2, 55, 65);
  ellipse_drawFill(&wheel1, src, Black);
  ellipse_drawFill(&wheel2, src, Black);   

  printf("writing output\n");
  image_write(src, "../images/car.ppm");
  printf("wrote output\n");

  image_free(src);
  printf("freed source\n");

  return(0);
}

