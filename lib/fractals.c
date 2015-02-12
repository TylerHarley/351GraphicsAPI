/*
  Tyler Harley
  9/17/2014
  
  This program defines the functions to create Mandlebrot and Julia set fractals
*/

#include <stdio.h>
#include <color.h>
#include <list.h>
#include <image.h>
#include <math.h>
#include "fractals.h"

// fills an Image with a Mandelbrot set
void mandelbrot(Image *dst, float x0, float y0, float dx){
	
	float rows = (float)dst->rows;
	float cols = (float)dst->cols;
	float dy = (rows/cols)*dx;
	float i, j, cx, cy; 
	float zx, zy, tempzx, tempzy;
	float z;
	int n, iterations;
	float color;
	
	for(i=0; i<rows; i++){
		cy = -((dy)/rows)*i+(y0+dy); // calculate y given i
		for(j=0; j<cols; j++){
			cx = ((dx)/cols)*j+x0; // calculate x given j
			
			zx = 0; //reset zx and zy
			zy = 0;
			tempzx = 0;
			tempzy = 0;
			color = 0.0;
			
			// iterate over Mandelbrot equation
			for(n=0; n<500; n++){
				zx = (tempzx * tempzx) - (tempzy * tempzy) - cx; // store zx & zy in temp vars
				zy = 2 * tempzx * tempzy - cy;
				tempzx = zx;
				tempzy = zy;
				
				z = sqrt((zx * zx) + (zy * zy));
				if(z > 2.0){
					iterations = n;
					color = 1.0;
					break;
				}
			}
			
			if(color == 0.0){
				image_setc(dst, i, j, 0, 0.0);
				image_setc(dst, i, j, 1, 0.0);
				image_setc(dst, i, j, 2, 0.0);
			}
			else{
				image_setc(dst, i, j, 0, 0.8);
				image_setc(dst, i, j, 1, (iterations % 50)/50.0); // color with linear gradient
				image_setc(dst, i, j, 2, 0.0);	
			}
			
			// alternate coloring
			/*if(color == 0.0){
				image_setc(dst, i, j, 0, 0.0);
				image_setc(dst, i, j, 1, 0.0);
				image_setc(dst, i, j, 2, 0.0);
			}
			else{
				image_setc(dst, i, j, 0, 1.0);
				image_setc(dst, i, j, 1, 1.0);
				image_setc(dst, i, j, 2, 1.0);	
			}*/
		}
	} 
}

// fills an Image with a Julia set
void julia(Image *dst, float x0, float y0, float dx){
	float rows = (float)dst->rows;
	float cols = (float)dst->cols;
	float dy = (rows/cols)*dx;
	float i, j;
	float cx = 0.415;
	float cy = 0.683;
	float zx, zy, tempzx, tempzy;
	float z;
	int n, iterations;
	float color;
	
	for(i=0; i<rows; i++){
		for(j=0; j<cols; j++){
			zy = -((dy)/rows)*i+(y0+dy); // calculate y given i
			zx = ((dx)/cols)*j+x0; // calculate x given j
			tempzy = zy;
			tempzx = zx;
			color = 0.0;
			
			// iterate over Mandelbrot equation
			for(n=0; n<2000; n++){	
				zx = (tempzx * tempzx) - (tempzy * tempzy) - cx;
				zy = 2 * tempzx * tempzy - cy;
				tempzx = zx;
				tempzy = zy;
				
				z = sqrt((zx * zx) + (zy * zy));
				if(z > 2.0){
					iterations = n;
					color = 1.0;
					break;
				}
			}
			
			if(color == 0.0){
				image_setc(dst, i, j, 0, 0.0);
				image_setc(dst, i, j, 1, 0.0);
				image_setc(dst, i, j, 2, 0.0);
			}
			else{ 
				image_setc(dst, i, j, 0, 1.0 - ((iterations % 50)/50.0));
				image_setc(dst, i, j, 1, 0.5 + ((iterations % 50)/50.0));
				image_setc(dst, i, j, 2, (iterations % 50)/50.0);	
			}
		}
	}
}
