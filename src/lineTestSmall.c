#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>
#include <math.h>

/* put your include file here */
#include "color.h"
#include "image.h"
#include "point.h"
#include "line.h"
#include "ppmIO.h"


#define NUMLINES 200
#define TIMING 5
#define ROWS 200
#define COLS 200

int main(int argc, char *argv[]) {
  Image *src;
  Line l1, l2, l3, l4, l5, l6, l7, l8, l9;
  Point a, b, c, d, e, f, g, h, k, m, n;
  long i,j;
  Color white, c1, c2, c3, c4, c5, c6, c7, c8, c9;
  color_set( &white, 1.0, 1.0, 1.0);
  color_set( &c1, .713, .027, 0.176); //red
  color_set( &c2, .027, .094, .713); //blue
  color_set( &c3, .027, 0.713, .039); //green
  color_set( &c4, 0.713, .027, 0.658); //pink
  color_set( &c5, .949, .929, 0.211); //yellow
  color_set( &c6, .027, .866, 0.874);  //cyan
  color_set( &c7, .337, .027, 0.874); //purle
  color_set( &c8, .874, .454, 0.027); //orange
  //color_set( &c9 , .086, .078, .074); //black 

  // allocate an image ROWS by COLS
  src = image_create(200, 200);
  if(!src) {
    fprintf(stderr, "unable to allocate memory\n");
    exit(0);
  }

  // Initialize the image to all white
  for(i=0;i<src->rows;i++) {
    for(j=0;j<src->cols;j++) {
      image_setColor(src, i, j, white );
    }
  }
  point_set2D(&(a), 100, 100);
  //seventh octant
  point_set2D(&(b), 130, 195);
  //first octant
  point_set2D(&(c), 195, 40);
  //second octant
  point_set2D(&(d), 130, 5);
  //third octant
  point_set2D(&(e), 70, 5);
  //fourth octant
  point_set2D(&(f), 5, 70);
  //Fifth octant
  point_set2D(&(g), 5, 160);
  //sixth octant
  point_set2D(&(h), 70, 195);
  //eigth octant
  point_set2D(&(k), 195, 130);
  //test point
  point_set2D(&(m), 36, 61);
  //point_set2D(&(n), 4, 1);

  //set the lines
  line_set(&l1, a, c);
  line_set(&l2, a, d);
  line_set(&l3, a, e);
  line_set(&l4, a, f);
  line_set(&l5, a, g);
  line_set(&l6, a, h);
  line_set(&l7, a, b);
  line_set(&l8, a, k);
  //line_set(&l9, m, n);
  //draw the lines
  line_draw(&l1, src, c1);
  line_draw(&l2, src, c2);
  line_draw(&l3, src, c3);
  line_draw(&l4, src, c4);
  line_draw(&l5, src, c5);
  line_draw(&l6, src, c6);
  line_draw(&l7, src, c7);
  line_draw(&l8, src, c8);
  //line_draw(&l9, src, c9);

  // write out the image
  image_write(src, "linesMarg.ppm");

  // free the image data
  printf("FREEBIRD THE IMAGE\n");
  image_free(src);

  return(0);
}

