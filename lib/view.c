/*
 * Tyler Harley & Margaux LeBlanc
 * view.c
 * Contains functions for 2D and 3D view structures
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
#include "light.h"
#include "polygon.h"
#include "matrix.h"
#include "view.h"

// initialize a given View2D with the given parameters
void view2D_set(View2D *view, Point *vrp, double du, Vector *x, int cols, int rows){
	view->vrp = *vrp;
	view->du = du;
	view->x = *x;
	view->C = cols;
	view->R = rows;
}

//sets up a 3d view and its fields
void view3D_set(View3D *view, Point *vrp, Vector *vpn, Vector *vup, double d, double du, double dv, double f, double b, int screenx, int screeny){
	view->vrp = *vrp;
	view->vpn = *vpn;
	view->vup = *vup;
	view->d = d;
	view->du = du;
	view->dv = dv;
	view->f = f;
	view->b = b;
	view->screenx = screenx;
	view->screeny = screeny;
}

void matrix_setView2D(Matrix *vtm, View2D *view){
	float dv = (view->du * view->R) / view->C;
	
	// 0. start with identity matrix
	matrix_identity(vtm);
	//scale by two to make bigger for anti aliasing
	//matrix_scale2D(vtm, .5*view->vrp.val[0], .5*view->vrp.val[1]);
	// 1. translate VRP to origin
	matrix_translate2D(vtm, -1*view->vrp.val[0], -1*view->vrp.val[1]);
	matrix_print(vtm, stdout);
	// 2. rotate about Z-axis
	matrix_rotateZ(vtm, view->x.val[0], -1*view->x.val[1]);
	matrix_print(vtm, stdout);
	// 3. scale by C/du and -R/dv
	matrix_scale2D(vtm, view->C/view->du, -1*view->R/dv);
	matrix_print(vtm, stdout);
	// 4. shift the range of Y-values to [0, R]
	matrix_translate2D(vtm, view->C/2.0, view->R/2.0);
	//scale down to anti alias
	//matrix_scale2D(vtm, 2*view->vrp.val[0], 2*view->vrp.val[1]);
	matrix_print(vtm, stdout);
}

void matrix_setView3D(Matrix *vtm, View3D *view){
	Vector vpnNormal = view->vpn; 
	Vector u, vup1;
	double b1, d1;
	
	//printf("Starting set view 3D\n");
	// 0. start with identity matrix
	matrix_identity(vtm);
	// 1. translate VRP to origin	
	matrix_translate(vtm, -view->vrp.val[0], -view->vrp.val[1], -view->vrp.val[2]);
	//printf("translated vrp to origin\n");
	//printf("after VRP translation: \n"); 	
	//matrix_print(vtm, stdout);
	// 2. establish orthonormal vectors
	//printf("cross product of %f, %f, %f, and %f, %f, %f\n", view->vup.val[0], view->vup.val[1], view->vup.val[2], view->vpn.val[0], view->vpn.val[1], view->vpn.val[2]);
	vector_cross(&view->vup, &view->vpn, &u);
	vector_cross(&view->vpn, &u, &vup1);
	//printf("finished cross product\n"); //error is in the cross product
	vector_normalize(&vpnNormal);
	vector_normalize(&vup1);
	vector_normalize(&u);
	//printf("view reference axes\n");
	//vector_print(&u, stdout);
	//vector_print(&vup1, stdout);
	//vector_print(&vpnNormal, stdout);
	// 3. orient the axes			
	matrix_rotateXYZ(vtm, &u, &vup1, &vpnNormal);
	//printf("after Rxyz\n");
	//matrix_print(vtm, stdout);
	// 4. translate COP to origin	
	matrix_translate(vtm, 0, 0, view->d);
	//printf("after translating cop to origin\n");
	//matrix_print(vtm, stdout);
	// 5. adjust back plane
	b1 = view->d + view->b;
	// 6. scale view volume			
	matrix_scale(vtm, (2*view->d)/(b1*view->du), (2*view->d)/(b1*view->dv), 1.0/b1);
	//printf("scaling to CVV\n");
	//matrix_print(vtm, stdout);
	// 7. adjust distance from COP
	d1 = view->d/b1;
	// 8. project scene onto the view plane
	matrix_perspective(vtm, d1);
	//printf("after perspective\n");
	//matrix_print(vtm, stdout);
	// 9. convert to screen coordinates
	matrix_scale2D(vtm, (-1*view->screenx)/(2*d1), (-1*view->screeny)/(2*d1));
	//printf("after scale to image coords:\n");
	//matrix_print(vtm, stdout);
	matrix_translate2D(vtm, view->screenx/2.0, view->screeny/2.0);
	//printf("after final translation to image coords\n");
	//matrix_print(vtm, stdout);
	//printf("finished setting 3D view\n");
}
