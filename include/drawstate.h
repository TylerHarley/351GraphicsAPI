/*
 * Tyler Harley & Margaux LeBlanc
 * drawstate.h
 * Contains drawstate struct and prototypes
 * 10/25/14
 */

#ifndef DRAWSTATE_H
#define DRAWSTATE_H

 
typedef enum{
	ShadeFrame,
	ShadeConstant,
	ShadeDepth,
	ShadeFlat,
	ShadeGouraud,
	ShadePhong
} ShadeMethod;
 
typedef struct{
	Color color;
	Color flatColor;
	Color body;
	Color surface;
	float surfaceCoeff;
	ShadeMethod shade;
	int zBufferFlag;
	double frontClipPlane;
	Point viewer;
} DrawState;

DrawState *drawstate_create(void);
void drawstate_setColor(DrawState *s, Color c);
void drawstate_setBody(DrawState *s, Color c);
void drawstate_setSurface(DrawState *s, Color c);
void drawstate_setSurfaceCoeff(DrawState *s, float f);
void drawstate_setFrontClipPlane(DrawState *s, double d);
void drawstate_copy(DrawState *to, DrawState *from);

#endif
