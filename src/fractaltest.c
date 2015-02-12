/*
  Bruce A. Maxwell
  Fall 2013

  Creates a Mandelbrot set at a selected location
*/
#include <stdio.h>
#include "image.h"
#include "fractals.h"

int main(int argc, char *argv[]) {
  Image *src;

  src = image_create(750, 1000);

  mandelbrot( src, 1.755, -0.02, 0.02);

  image_write( src, "../images/fractal.ppm");
  
  image_reset( src );
  
  julia( src, -2, -1.5, 5);
  
  image_write( src, "../images/julia.ppm");

  image_free( src );

  return(0);
}