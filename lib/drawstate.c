/*
 * Tyler Harley & Margaux LeBlanc
 * drawstate.c
 * Contains drawstate functions
 * 10/25/14
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <color.h>
#include <list.h>
#include <image.h>
#include <math.h>
#include "graphics.h"
#include "drawstate.h"

// create a new DrawState structure and initialize the fields
DrawState *drawstate_create(){
	DrawState *d = malloc(sizeof(DrawState));
	if(!d){
		fprintf(stderr, "Draw state unable to be allocated\n");
		exit(1);
	}
	color_set(&(d->color),1.0, 1.0, 1.0); //initialize to white
	color_set(&(d->flatColor),1.0, 1.0, 1.0);
	color_set(&(d->body),1.0, 1.0, 1.0);
	color_set(&(d->surface), .1, .1, .1);
	d->surfaceCoeff = 10.0;  // these 3 seem like default values
	d->shade = ShadeFrame;
	d->zBufferFlag = 1;
	d->frontClipPlane = 2.0;
	point_set3D(&(d->viewer), 0.0, 0.0, 0.0); //initialize to origin
	
	return(d);
}

// set the color field to c
void drawstate_setColor(DrawState *s, Color c){
	s->color = c;
}

// set the body field to c
void drawstate_setBody(DrawState *s, Color c){
	s->body = c;
}

// set the surface field to c
void drawstate_setSurface(DrawState *s, Color c){
	s->surface = c;
}

// set the surfaceCoeff field to f
void drawstate_setSurfaceCoeff(DrawState *s, float f){
	s->surfaceCoeff = f;
}

// set the front clip plane
void drawstate_setFrontClipPlane(DrawState *s, double d){
	s->frontClipPlane = d;
}

// copy DrawState data
void drawstate_copy(DrawState *to, DrawState *from){
	to->color = from->color;
	to->flatColor = from->flatColor;
	to->body = from->body;
	to->surface = from->surface;
	to->surfaceCoeff = from->surfaceCoeff;
	to->shade = from->shade;
	to->zBufferFlag = from->zBufferFlag;
	to->viewer = from->viewer;
}
