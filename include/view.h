/*
 * Tyler Harley & Margaux LeBlanc
 * view.h
 * Contains typedefs and prototypes for 2D and 3D view structures
 */

#ifndef VIEW_H
#define VIEW_H

typedef struct{
	Point vrp;
	double du;
	Vector x;
	int C;
	int R;
} View2D;

typedef struct{
	Point vrp;
	Vector vpn;
	Vector vup;
	double d;
	double du;
	double dv;
	double f;
	double b;
	int screenx;
	int screeny;
} View3D;

void view2D_set(View2D *view, Point *vrp, double du, Vector *x, int cols, int rows);
void view3D_set(View3D *view, Point *vrp, Vector *vpn, Vector *vup, double d, double du, double dv, double f, double b, int screenx, int screeny);
void matrix_setView2D(Matrix *vtm, View2D *view);
void matrix_setView3D(Matrix *vtm, View3D *view);

#endif
