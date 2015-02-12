/*
  Tyler Harley
  9/11/14
  
  This is an example of reading and writing an image using the ppmIO.c
  routines.  To manipulate the image, change the pixel color values.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ppmIO.h"

#define USECPP 0

int main(int argc, char *argv[]) {
  Pixel *image;
   
    /* this separate output image array is for rotation
       and mirroring. this way, you can perform a change
       on a pixel you may have already edited, and this stops
       the image from being "half-transformed". */
  Pixel *newImage; 
  int rows, cols, colors;
  long i, j, k;
  int rotate = 0;
  
  // this is my Boolean in C
  // C should have bool types
  typedef enum { false, true } bool;
  bool mirrored = false;

  if(argc < 4) {
    printf("Usage: lab1transform <input file> <output file> 90|180|270|mirror\n");
    exit(-1);
  }
  
	if(strcmp(argv[3], "mirror") == 0){
		mirrored = true;
		printf("mirrored\n");
	}
	else{
		rotate = atoi(argv[3]);
		if(rotate != 90 && rotate != 180 && rotate != 270){
		  printf("Rotate argument must be either 90, 180, or 270\n");
		  exit(-1);
		} 
	}
  
  /* read in the image (and read again for the separate output image */

  image = readPPM(&rows, &cols, &colors, argv[1]);
  newImage = readPPM(&rows, &cols, &colors, argv[1]);

  
  if(!image) {
    fprintf(stderr, "Unable to read %s\n", argv[1]);
    exit(-1);
  }

  /* mess with the image here  */

  // tried swapping rows and columns here, did not work
  if(rotate == 90 || rotate == 270){
	  for(i=0;i<cols;i++){
	  	for(j=0;j<rows;j++){
			k = i*cols+j;
		
			newImage[k].r = image[k].r;
			newImage[k].g = image[k].g;
			newImage[k].b = image[k].b;
	  	}
	  }
  }
  else{
	  for(i=0;i<rows;i++) {
		for(j=0;j<cols;j++) {
			if(rotate == 180){ // basically invert the i*cols+j equation
							   // to make the image upside-down
				k = (rows - i) * cols - (cols - j);
			
				newImage[i*cols+j].r = image[k].r;
				newImage[i*cols+j].g = image[k].g;
				newImage[i*cols+j].b = image[k].b;
			}
			else{
				if(mirrored){ // swap the columns but not the rows
					newImage[i*cols+j].r = image[(1+i)*cols-j].r;
					newImage[i*cols+j].g = image[(1+i)*cols-j].g;
					newImage[i*cols+j].b = image[(1+i)*cols-j].b;
				}
				else{	// just write the regular image to the new image if no rotation
					k = i*cols+j;
			
					newImage[k].r = image[k].r;
					newImage[k].g = image[k].g;
					newImage[k].b = image[k].b;
				}					
			}
		}
	  }
  }

  /* write out the resulting image */
  writePPM(newImage, rows, cols, colors /* s/b 255 */, argv[2]);

  /* free the image memory */
#if USECPP
  delete[] image;
  delete[] newImage;
#else
  free(image);
  free(newImage);
#endif

  return(0);
}
