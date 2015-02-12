//Tyler Harley and Margaux LeBlanc
//11/3/14
//Graphics

#ifndef BEZIER_H
#define BEZIER_H

//Contains functions and construction for Bezier Curves and Surfaces

typedef struct {
	Point controlPt[4];
	int zbuffer;
} BezierCurve;

typedef struct{
	Point controlPt[16];
	int zbuffer;
} BezierSurface;

//Curve and surface functions
void bezierCurve_init(BezierCurve *b);
void bezierSurface_init(BezierSurface *b);
void bezierCurve_set(BezierCurve *b, Point *vlist);
void bezierSurface_set(BezierSurface *b, Point *vlist);
void bezierCurve_zBuffer(BezierCurve *p, int flag);
void bezierSurface_zBuffer(BezierSurface *p, int flag);
void deCasteljau(Point *src, Point *dst, Point *dst2);
void bezierCurve_draw(BezierCurve *b, Image *src, Color c, int zBufferFlag);

//bezier curve and surface module functions
void module_bezierCurve(Module *m, BezierCurve *b, int divisions);
void module_bezierSurface(Module *m, BezierSurface *b, int divisions, int solid);
void module_sphere(Module *md, int divisions, int solid);

#endif
