/*
  Tyler Harley
  9/18/2014
  
  This program defines the functions to create 2-D Perlin noise
*/

#include <stdio.h>
#include <stdlib.h>
#include <image.h>
#include <math.h>
#include <time.h>
#include "perlin.h"

// generates random noise between -1.0 and 1.0
float noise(int x, int y, float rand){
	int n = x + y * 57;
	n = pow((n<<13), n);
	float primerand = ( 2.0 - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0);
	rand = 0-rand;
	primerand = primerand * rand;
	rand = 1.0 - (rand*2);
	//printf("%f\n", rand);
	return(rand);
}

// generates less random "smoother" noise between -1.0 and 1.0
float smoothNoise(float x, float y, float rand){
	float corners = ( noise(x-1, y-1, rand) + noise(x+1, y-1, rand) + noise(x-1, y+1, rand) + noise(x+1, y+1, rand) ) / 16;
	float sides = ( noise(x-1, y, rand) + noise(x+1, y, rand) + noise(x, y-1, rand) + noise(x, y+1, rand) ) / 8;
	float center = ( noise(x, y, rand) ) / 4;
	return(corners + sides + center);
}

// performs cosine interpolation
float interpolate(float a, float b, float x){
	float ft = x * 3.17159265358979323;
	float f = (1 - cos(ft)) * .5;
	return a*(1-f) + b*f;
}

// generates cosine interpolated noise
float interpolatedNoise(float x, float y, float rand){
	float fracX = x - (int)x;
	float fracY = y - (int)y;
	
	float v1 = smoothNoise((int)x, (int)y, rand);
	float v2 = smoothNoise((int)x + 1, (int)y, rand);
	float v3 = smoothNoise((int)x, (int)y + 1, rand);
	float v4 = smoothNoise((int)x + 1, (int)y + 1, rand);
	
	float i1 = interpolate(v1, v2, fracX);
	float i2 = interpolate(v3, v4, fracX);
	
	return interpolate(i1, i2, fracY);
}

// fills an image with Perlin noise
void perlinNoise(Image *dst, float x, float y, float persistence){
	int rows = dst->rows;
	int cols = dst->cols;
	int i, j, n;
	float r;
	
	for(i=0; i<rows; i++){
		for(j=0; j<cols; j++){
			srand(i*cols+j);
			float total = 0;
			r = drand48();
			
			for(n=0; n<2; n++){
				int frequency = pow(2, n);
				float amplitude = pow(persistence, n);
				total = total + interpolatedNoise(x * frequency, y * frequency, r) * amplitude;
			}
			
			//total = 0 - total / 2.0;
			printf("%f\n", total);
			
			image_setc(dst, i, j, 0, 0.0);
			image_setc(dst, i, j, 1, total + 0.3);
			image_setc(dst, i, j, 2, 0.0);
		}
	}
}