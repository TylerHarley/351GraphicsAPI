/*
 * Tyler Harley
 * image.c
 * 9/16/14
 * A collection of functions for manipulating an
 * Image data structure.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <ppmIO.h>
#include <math.h>
#include <color.h>
#include <list.h>
#include "image.h"

// allocates an Image and initializes fields
// returns an Image pointer if successful and NULL if unsuccessful
Image *image_create(int rows, int cols){
	int i;
	Image *src = NULL;
	src = malloc(sizeof(Image));
	if(!src){
		fprintf(stderr, "Image unable to be allocated\n");
		return(NULL);
	}
	
	src->data = malloc(sizeof(FPixel*)*rows); // allocate data pointer	
	if(!src->data){
		fprintf(stderr, "Image data unable to be allocated\n");
		free(src);
		return(NULL);
	}

	src->data[0] = malloc(sizeof(FPixel)*rows*cols); // allocate first row of data
	if(!src->data[0]){
		fprintf(stderr, "Image data unable to be allocated\n");
		free(src->data);
		free(src);
		return(NULL);
	}
	
	for(i=1; i<rows; i++){
		src->data[i] = &(src->data[0][i*cols]); // allocate remaining rows
	}
	
	src->rows = rows;
	src->cols = cols;
	image_fillz(src, 1.0);
	
	return(src);
}

// free the Image
void image_free(Image *src){
	if(src){
		if(src->data){
			if(src->data[0]){
				free(src->data[0]);
			}
			free(src->data);
		}
		free(src);
	}
}

// initialize a given Image to default values
void image_init(Image *src){
	src->data = NULL;
	src->rows = 0;
	src->cols = 0;
}

// allocates and initializes Image data field
// returns 0 if successful, -1 if unsuccessful
int image_alloc(Image *src, int rows, int cols){
	int i;
	
	/* Allocate all FPixels */
	src->data = malloc(sizeof(FPixel*)*rows);
	if(!src->data){
		fprintf(stderr, "Image data unable to be allocated\n");
		src->data = NULL;
		src->rows = 0;
		src->cols = 0;
		return(1);
	}
	
	src->data[0] = malloc(sizeof(FPixel)*rows*cols);
	if(!src->data[0]){
		fprintf(stderr, "Image data unable to be allocated\n");
		free(src->data);
		src->data = NULL;
		src->rows = 0;
		src->cols = 0;
		return(1);
	}

	for(i=1; i<rows; i++){
		src->data[i] = &(src->data[0][i*cols]); // allocate remaining rows
	}	
	
	
	src->rows = rows;
	src->cols = cols;
	/* Initialize all FPixels */
	image_reset(src);
	
	return(0);
}

// free all Image data
void image_dealloc(Image *src){
	if(src){
		if(src->data){
			if(src->data[0]){
				free(src->data[0]);
			}
			free(src->data);
		}
		
		src->data = NULL;
		src->rows = 0;
		src->cols = 0;
	}
}

// read in a PPM file and store all of its pixel values in an Image
// returns the created Image if successful, NULL if unsuccessful
Image *image_read(char *filename){
	int rows, cols, colors;
	int i, j, k;	

	Pixel *image;
	image = readPPM(&rows, &cols, &colors, filename);

	Image *src;
	src = image_create(rows, cols);

	for(i=0; i<rows; i++){
		for(j=0; j<cols; j++){
			k = i*cols+j;
			src->data[i][j].rgb[0] = image[k].r / 255.0;
			src->data[i][j].rgb[1] = image[k].g / 255.0;
			src->data[i][j].rgb[2] = image[k].b / 255.0;
		}
	}
	
	free(image);
	return src;
}

// write values of an Image into a PPM file
int image_write(Image *src, char *filename){
	int rows = src->rows;
	int cols = src->cols;
	int i, j, k;
	Pixel *ppm = malloc(sizeof(Pixel)*rows*cols);
	
	if(!ppm){
		fprintf(stderr, "PPM file unable to be allocated\n");
		return(-1);
	}
	
	// write all the color values of the PPM
	
	for(i=0; i<rows; i++){
		for(j=0; j<cols; j++){
			k = i*cols+j;
			ppm[k].r = src->data[i][j].rgb[0] * 255;
			ppm[k].g = src->data[i][j].rgb[1] * 255;
			ppm[k].b = src->data[i][j].rgb[2] * 255;
		}
	}
	
	writePPM(ppm, rows, cols, 255 /*src->data[0][0].z*/, filename);
	
	free(ppm);

	return(0);
}

// get the FPixel from src at row r and column c
FPixel *image_getf(Image *src, int r, int c){
	if((r>0 && c>0) && (r<src->rows && c<src->cols)){
		return(&src->data[r][c]);
	}
	return(NULL);
}

// return the value of band b at FPixel at (r, c)
float image_getc(Image *src, int r, int c, int b){
	return src->data[r][c].rgb[b];
}

// return the alpha value of FPixel at (r, c)
float image_geta(Image *src, int r, int c){
	return src->data[r][c].a;
}

// return the color depth of FPixel at (r, c)
float image_getz(Image *src, int r, int c){
	return src->data[r][c].z;
}

// return the Color at pixel (r, c)
Color image_getColor(Image *src, int r, int c){
	Color val;
	val.c[0] = src->data[r][c].rgb[0];
	val.c[1] = src->data[r][c].rgb[1];
	val.c[2] = src->data[r][c].rgb[2];
	return val;
}

// sets the FPixel at (r, c) to val
void image_setf(Image *src, int r, int c, FPixel val){
	src->data[r][c] = val;
}

// sets the b band of FPixel at (r, c) to val
void image_setc(Image *src, int r, int c, int b, float val){
	src->data[r][c].rgb[b] = val;
}

// sets the alpha of FPixel at (r, c) to val
void image_seta(Image *src, int r, int c, float val){
	src->data[r][c].a = val;
}

// sets the depth of FPixel at (r, c) to val
void image_setz(Image *src, int r, int c, float val){
	if((r>0 && c>0) && (r<src->rows && c<src->cols)){	
		src->data[r][c].z = val;
	}
}

// copies the Color data to the pixel at (r, c)
void image_setColor(Image *src, int r, int c, Color val){
	//Check if pixel is in the picture. 
	if((r>0 && c>0) && (r<src->rows && c<src->cols)){
		src->data[r][c].rgb[0] = val.c[0];
		src->data[r][c].rgb[1] = val.c[1];
		src->data[r][c].rgb[2] = val.c[2];
	}
}

// reset all pixels to default values
void image_reset(Image *src){
	int i, j;
	int rows = src->rows;
	int cols = src->cols;
	
	// Set each pixel to (0, 0, 0) with 1.0 alpha and depth
	for(i=0; i<rows; i++){
		for(j=0; j<cols; j++){
			FPixel *p = &src->data[i][j];
			p->rgb[0] = 0.0;
			p->rgb[1] = 0.0;
			p->rgb[2] = 0.0;
			p->a = 1.0;
			p->z = 1.0;
		}
	}
}

// sets all FPixels to be val
void image_fill(Image *src, FPixel val){
	int i, j;
	int rows = src->rows;
	int cols = src->cols;
	
	for(i=0; i<rows; i++){
		for(j=0; j<cols; j++){
			src->data[i][j] = val;
		}
		
	}
}

// sets rgb values of all FPixels
void image_fillrgb(Image *src, float r, float g, float b){
	int i, j;
	int rows = src->rows;
	int cols = src->cols;
	
	for(i=0; i<rows; i++){
		for(j=0; j<cols; j++){
			FPixel *p = &src->data[i][j];
			p->rgb[0] = r;
			p->rgb[1] = g;
			p->rgb[2] = b;
		}
	}	
}

// sets alpha values of all FPixels
void image_filla(Image *src, float a){
	int i, j;
	int rows = src->rows;
	int cols = src->cols;
	
	for(i=0; i<rows; i++){
		for(j=0; j<cols; j++){
			src->data[i][j].a = a;
		}
	}
}

// sets depth values of all FPixels
void image_fillz(Image *src, float z){
	int i, j;
	int rows = src->rows;
	int cols = src->cols;
	
	for(i=0; i<rows; i++){
		for(j=0; j<cols; j++){
			src->data[i][j].z = z;
		}
	}
}

// initializes A-Buffer for all pixels
void image_initABuffer(Image *src){
	/*int i, j;
	int rows = src->rows;
	int cols = src->cols;

	for(i=0; i<rows; i++){
		for(j=0; j<cols; j++){
			src->data[i][j].aBuffer = ll_new();
		}
	}*/
}

void image_clearABuffer(Image *src){
/*	int i, j;
	int rows = src->rows;
	int cols = src->cols;

	for(i=0; i<rows; i++){
		for(j=0; j<cols; j++){
			ll_delete(src->data[i][j].aBuffer, (void (*)(const void *))polygon_free);
		}
	}*/
}

// fills the image with sine bands
void image_fillSinBands(Image *src){
	int i, j;
	float c;
	int rows = src->rows;
	int cols = src->cols;
	
	for(i=0; i<rows; i++){
		for(j=0; j<cols; j++){	
			c = sin(j);
			
			if(c>0){
				image_setc(src, i, j, 0, 0.1 + c);
				image_setc(src, i, j, 1, 0.05 + c);
				image_setc(src, i, j, 2, c);
			}
			else{
				image_setc(src, i, j, 0, .9 + c);
				image_setc(src, i, j, 1, .95 + c);
				image_setc(src, i, j, 2, 1.0 + c);
			}
		}
	}
}

// fills the image with cosine bands
void image_fillCosBands(Image *src){
	int i, j;
	float c;
	int rows = src->rows;
	int cols = src->cols;
	
	for(i=0; i<rows; i++){
		for(j=0; j<cols; j++){	
			c = cos(j);
			
			if(c>0){
				image_setc(src, i, j, 0, 0.85);
				image_setc(src, i, j, 1, 0.15);
				image_setc(src, i, j, 2, c);
			}
			else{
				image_setc(src, i, j, 0, 0.15);
				image_setc(src, i, j, 1, 0.85);
				image_setc(src, i, j, 2, 1.0 - c);
			}
		}
	}
}

// fills the image with tangent bands
void image_fillTan(Image *src){
	int i, j;
	float c;
	int rows = src->rows;
	int cols = src->cols;
	
	for(i=0; i<rows; i++){
		for(j=0; j<cols; j++){	
			c = tan(j);
			
			if(c>0){
				image_setc(src, i, j, 0, 0.85);
				image_setc(src, i, j, 1, 0.15);
				image_setc(src, i, j, 2, c);
			}
			else{
				image_setc(src, i, j, 0, 0.15);
				image_setc(src, i, j, 1, 0.85);
				image_setc(src, i, j, 2, 1.0 - c);
			}
		}
	}
}

