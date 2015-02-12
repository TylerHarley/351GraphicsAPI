/*
  Tyler Harley
  9/18/2014
  
  This program tests and draws Perlin noise
*/

#include <stdio.h>
#include <stdlib.h>
#include "image.h"

int main(int argc, char *argv[]) {
  Image *src;

  src = image_create(750, 1000);
  
  image_fillSinBands(src);
  image_write(src, "../images/sinband.ppm");
  
  image_fillCosBands(src);
  image_write(src, "../images/cosband.ppm");
  
  image_fillTan(src);
  image_write(src, "../images/tan.ppm");

  image_free( src );

  return(0);
}