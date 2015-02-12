/*
 * Tyler Harley & Margaux LeBlanc
 * view.c
 * Contains functions for 2D and 3D view structures
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <color.h>
#include <image.h>
#include <list.h>
#include <math.h>
#include "graphics.h"
#include "polygon.h"
#include "matrix.h"
#include "view.h"

int main(int argc, char *argv[]) {
	Vector *c;
	Vector yeah = {{0,1,0,0}};
	Vector man = {{-3,-2,2,0}};
	vector_cross(&yeah, &man, &c);
	vector_cross(&yeah, &c, &man);
	return 0;
}
