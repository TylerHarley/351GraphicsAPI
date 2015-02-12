/*
  Tyler Harley
  9/11/14
  
  This is an example of reading and writing images using the ppmIO.c
  routines and alpha blending them together with an alpha mask.
*/

#include <stdio.h>
#include <stdlib.h>
#include "ppmIO.h"

#define USECPP 0

int main(int argc, char *argv[]) {
  Pixel *imageA, *imageB, *mask;
  int rows, cols, colors, alpha, dx, dy;
  long i, j, k, newI, newJ, newK;

  if(argc < 5) {
    printf("Usage: lab1alpha <input fileA> <input fileB> <mask> <output file> [x-offset] [y-offset]\n");
    exit(-1);
  }

  /* read in the images and the mask */
  imageA = readPPM(&rows, &cols, &colors, argv[1]);
  imageB = readPPM(&rows, &cols, &colors, argv[2]);
  mask = readPPM(&rows, &cols, &colors, argv[3]);
  
  /* read in x and y offsets */
  if(argc >= 6){
  	dx = atoi(argv[5]);
  }
  if(argc >=7){
  	dy = atoi(argv[6]);
  }
  
  if(!imageA) {
    fprintf(stderr, "Unable to read %s\n", argv[1]);
    exit(-1);
  }
  
  if(!imageB) {
    fprintf(stderr, "Unable to read %s\n", argv[2]);
    exit(-1);
  }
  
  if(!mask) {
    fprintf(stderr, "Unable to read %s\n", argv[3]);
    exit(-1);
  }

  /* mess with the image here  */
  for(i=0;i<rows;i++) {
	for(j=0;j<cols;j++) {
		// check to make sure offsets don't go out of bounds
		if(i+dy >= 0 && i+dy < rows){
			newI = i+dy;
		}
		else{
			continue;
		}
		
		if(j-dx >= 0 && j-dx < cols){
			newJ = j-dx;
		}
		else{
			continue;
		}
		
		k = i * cols + j; // coords for the unmasked image
		newK = newI * cols + newJ; // coords for mask/masked image
		printf("%ld,  %ld,  %ld\n", newI, newJ, k);
		
		// this calculates the alpha-blended value for each pixel
		alpha = mask[newK].r / 255.0;
		imageA[k].r = alpha * imageA[k].r + (1.0 - alpha) * imageB[newK].r;
		alpha = mask[newK].g / 255.0;
		imageA[k].g = alpha * imageA[k].g + (1.0 - alpha) * imageB[newK].g;
		alpha = mask[newK].b / 255.0;
		imageA[k].b = alpha * imageA[k].b + (1.0 - alpha) * imageB[newK].b;
	}
  }

  /* write out the resulting image */
  writePPM(imageA, rows, cols, colors /* s/b 255 */, argv[4]);

  /* free the image memory */
#if USECPP
  delete[] imageA;
  delete[] imageB;
  delete[] mask;
#else
  free(imageA);
  free(imageB);
  free(mask);
#endif

  return(0);
}
