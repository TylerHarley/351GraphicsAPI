//Margaux LeBlanc and Tyler Harley
//Graphics
//10/8/14

//Creating a Transformation Matrix Library

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

//Vector Functions
//sets the vector to (x,y,z,0.0)
void vector_set(Vector *v, double x, double y, double z){
	v->val[0] = x;
	v->val[1] = y;
	v->val[2] = z;
	v->val[3] = 0.0;
}
//Print out the Vector to stream fp in a pretty form
void vector_print(Vector *v, FILE *fp){
	//printf("file: %p\n", fp);
	printf("[%f , %f, %f]\n", v->val[0], v->val[1], v->val[2]);
}

//Copy the src Vector to the dest Vector
void vector_copy(Vector *dest, Vector *src){
	dest->val[0] = src->val[0];
	dest->val[1] = src->val[1];
	dest->val[2] = src->val[2];
	dest->val[3] = 0;
	//last value is always zero
}

void vector_average(Vector *v, Vector *vList, int n){
	int i;
	float x = 0;
	float y = 0;
	float z = 0;
	for(i=0; i<n; i++){
		x+=vList[i].val[0];
		y+=vList[i].val[1];
		z+=vList[i].val[2];
	}
	vector_set(v, x/n, y/n, z/n);
}

//Returns the Euclidean length of the vector, assuming the 
//homogenous coordinate is 1.0
double vector_length(Vector *v){
	double length;
	length = (v->val[0]*v->val[0]) + (v->val[1]*v->val[1]) + (v->val[2]*v->val[2]);
	return sqrt(length);
}
//Normalize the Vector to unit length. Do not modify the homogeneous coordinate
void vector_normalize(Vector *v){
	double length = vector_length(v);
	v->val[0] = v->val[0]/length;
	v->val[1] = v->val[1]/length;
	v->val[2] = v->val[2]/length;
}
//Returns the scalar product of a and b
double vector_dot(Vector *a, Vector *b){
	double d;
	d = (a->val[0]*b->val[0]) + (a->val[1]*b->val[1]) + (a->val[2]*b->val[2]);
	return d;
}
//calulates the cross product of vectors a and b and puts result in vector c
void vector_cross(Vector *a, Vector *b, Vector *c){
	c->val[0] = (a->val[1]*b->val[2]) - (a->val[2]*b->val[1]);
	c->val[1] = (a->val[2]*b->val[0]) - (a->val[0]*b->val[2]);
	c->val[2] = (a->val[0]*b->val[1]) - (a->val[1]*b->val[0]);
	c->val[3] = 0.0;
}

//2D Matrix Functions
//Print out the matrix in a nice 4x4 arrangement with a blank line below
void matrix_print(Matrix *m, FILE *fp){
	int i, j;
	for(i=0; i<4; i++){
		printf("| ");
		for(j=0; j<4; j++){
			printf("%.3f ", m->m[i][j]);
		}
		printf("|\n");
	}
	printf("\n");
}
//Set the matrix to all zeros
void matrix_clear(Matrix *m){
	int i, j;
	for(i=0; i<4; i++){
		for(j=0; j<4; j++){
			m->m[i][j] = 0.0;
		}
	}
}
//Set the matrix to the identity matrix
void matrix_identity(Matrix *m){
	int i, j;
	for(i=0; i<4; i++){
		for(j=0; j<4; j++){
			if( i == j){
				m->m[i][j] = 1.0;
			}
			else{
				m->m[i][j] = 0.0;
			}
		}
	}
}
//return the element of the matrix at row r, col c
double matrix_get(Matrix *m, int r, int c){
	return m->m[r][c];
}
//set the element of the matrix at row r, col c to v
void matrix_set(Matrix *m, int r, int c, double v){
	m->m[r][c] = v;
}
//Copy the src matrix into the dest matrix
void matrix_copy(Matrix *dest, Matrix *src){
	int i, j;
	for(i=0; i<4; i++){
		for(j=0; j<4; j++){
			dest->m[i][j] = src->m[i][j];
		}
	}	
}
//Transpose the matrix m in place
void matrix_transpose(Matrix *m){
	//flip rows and cols
	m->m[0][4] = m->m[4][0];
	m->m[0][3] = m->m[3][0];
	m->m[4][1] = m->m[1][4];
	m->m[1][0] = m->m[0][1];
	m->m[2][1] = m->m[1][2];
	m->m[3][2] = m->m[2][3];
}
//Multiply left and right and put the result in m
void matrix_multiply(Matrix *left, Matrix *right, Matrix *m){
	int i, j;
	//checking to see if m is the same as other inputs. If so, use tmp matrix O
	if(m == left || m == right){
		Matrix *O = malloc(sizeof(Matrix));
		for(i=0; i<4; i++){
			for(j=0; j<4; j++){
				O->m[i][j] = left->m[i][0] * right->m[0][j] + 
							left->m[i][1] * right->m[1][j] +
							left->m[i][2] * right->m[2][j] +
							left->m[i][3] * right->m[3][j];
			}
		}
		matrix_copy(m, O);
		free(O);
	}
	else{
		for(i=0; i<4; i++){
			for(j=0; j<4; j++){
				m->m[i][j] = left->m[i][0] * right->m[0][j] + 
							left->m[i][1] * right->m[1][j] +
							left->m[i][2] * right->m[2][j] +
							left->m[i][3] * right->m[3][j];
			}
		}
	}
}
//Transform the point p by the matrix m and put the result
//in q. For this function, p and q need to be different
//variables						
void matrix_xformPoint(Matrix *m, Point *p, Point *q){
	int i;
	if(p == q){
		fprintf( stderr, "p and q are the same point\n");
		return;
	}
	else{
		for(i=0; i<4; i++){
			q->val[i] = m->m[i][0]*p->val[0] +
						m->m[i][1]*p->val[1] +
						m->m[i][2]*p->val[2] +
						m->m[i][3]*p->val[3]; //might be where getting set to 0
		}
	}
}
//Transform the vector p by the matrix m and put the result in q. For
//This function, p and q need to be different variables
void matrix_xformVector(Matrix *m, Vector *p, Vector *q){
	int i;
	if(p == q){
		fprintf( stderr, "p and q are the same vector\n");
		return;
	}
	else{
		for(i=0; i<4; i++){
			q->val[i] = m->m[i][0]*p->val[0] +
						m->m[i][1]*p->val[1] +
						m->m[i][2]*p->val[2] +
						m->m[i][3]*p->val[3];
		}
	}
}
//Transform the points and surface normals (if they exist) in the polygon p
//by the matrix m
void matrix_xformPolygon(Matrix *m, Polygon *p){
	int i;
	Point tmp;
	Vector v;
	for(i=0; i<(p->nVertex); i++){
		matrix_xformPoint(m, &p->vertex[i], &tmp);
		matrix_xformVector(m, &p->normal[i], &v);
		p->vertex[i] = tmp;
		p->normal[i] = v;
	}
}
//Transform the points in the Polyline p by the matrix m
void matrix_xformPolyline(Matrix *m, Polyline *p){
	int i;
	Point tmp;
	for(i=0; i<(p->numVertex); i++){
		matrix_xformPoint(m, &p->vertex[i], &tmp);
		p->vertex[i] = tmp;
	}
}
//Transform the points in line by the matrix m
void matrix_xformLine(Matrix *m, Line *line){
	Point tmp;
	//printf("In xformLine\n");
	matrix_xformPoint(m, &line->a, &tmp);
	line->a = tmp;
	matrix_xformPoint(m, &line->b, &tmp);
	line->b = tmp; //do I need a tmp2?
}
//premultiply the matrix by a scale matrix parameterized by sx and sy
void matrix_scale2D(Matrix *m, double sx, double sy){
	//matrix multiplication with s=left, m=right.
	//make scaling matrix
	Matrix S;
	matrix_identity(&S);
	S.m[0][0] = sx;
	S.m[1][1] = sy;

	//matrix multiplication with m and s.
	matrix_multiply(&S, m, m);
}
//premultiply the matrix by a Z-axis rotation matrix parameterized by cosTheta
//and sinTheta, where Theta is the angle of rotation about the Z-axis
void matrix_rotateZ(Matrix *m, double cth, double sth){
	Matrix R;
	matrix_identity(&R);
	R.m[0][0] = cth;
	R.m[0][1] = -sth;
	R.m[1][0] = sth;
	R.m[1][1] = cth;

	//matrix multiplication with m and R
	matrix_multiply(&R, m, m);
}
//Premultiply the matrix by a 2D translation matrix parameterized by tx and ty
void matrix_translate2D(Matrix *m, double tx, double ty){
	Matrix T;
	matrix_identity(&T);
	T.m[0][3] = tx;
	T.m[1][3] = ty;

	//matrix multiplication with m and T
	matrix_multiply(&T, m, m);
}
//Premultiply the matrix by a 2D shear matrix parameterized by shx and shy
void matrix_shear2D(Matrix *m, double shx, double shy){
	Matrix S;
	matrix_identity(&S);
	S.m[0][1] = shx;
	S.m[1][0] = shy;

	//matrix multiplication with m and T
	matrix_multiply(&S, m, m);
}

//3D Matrix Functions
//Premultiply the matrix by a 3D translation matrix parameterized by tx, ty and tz
void matrix_translate(Matrix *m, double tx, double ty, double tz){
	Matrix T;
	matrix_identity(&T);
	T.m[0][3] = tx;
	T.m[1][3] = ty;
	T.m[2][3] = tz;

	//matrix multiplication with m and T
	matrix_multiply(&T, m, m);
}
//premultiply the matrix by a scale matrix parameterized by sx, sy, and sz
void matrix_scale(Matrix *m, double sx, double sy, double sz){
	Matrix S;
	matrix_identity(&S);
	S.m[0][0] = sx;
	S.m[1][1] = sy;
	S.m[2][2] = sz;
	
	//matrix multiplication with m and s.
	matrix_multiply(&S, m, m);
}
//premultiply the matrix by an X-axis rotation matrix parameterized by cosTheta
//and sinTheta, where Theta is the angle of rotation about the X-axis
void matrix_rotateX(Matrix *m, double cth, double sth){
	Matrix R;
	matrix_identity(&R);
	R.m[1][1] = cth;
	R.m[1][2] = -sth;
	R.m[2][1] = sth;
	R.m[2][2] = cth;

	//matrix multiplication with m and R
	matrix_multiply(&R, m, m);
}
//premultiply the matrix by an Y-axis rotation matrix parameterized by cosTheta
//and sinTheta, where Theta is the angle of rotation about the X-axis
void matrix_rotateY(Matrix *m, double cth, double sth){
	Matrix R;
	matrix_identity(&R);
	R.m[0][0] = cth;
	R.m[0][2] = -sth;
	R.m[2][0] = sth;
	R.m[2][2] = cth;

	//matrix multiplication with m and R
	matrix_multiply(&R, m, m);
}
//premultiply the matrix by an XYZ-axis rotation matrix parameterized by the vectors u, v, and w, where the three vectors represent an orthonormal 3D basis
void matrix_rotateXYZ(Matrix *m, Vector *u, Vector *v, Vector *w){
	int i;	
	Matrix R;
	matrix_identity(&R);
	for(i=0; i<3; i++){
		R.m[0][i] = u->val[i];
		R.m[1][i] = v->val[i];
		R.m[2][i] = w->val[i];
	}

	//matrix multiplication with m and R
	matrix_multiply(&R, m, m);
}
//premultiply the matrix by a shear Z matrix parameterized by shx and shy
void matrix_shearZ(Matrix *m, double shx, double shy){
	Matrix S;
	matrix_identity(&S);
	S.m[0][2] = shx;
	S.m[1][2] = shy;

	//matrix multiplication with m and T
	matrix_multiply(&S, m, m);
}
//premultiplt the matrix by a perspective matrix parameterized by d 
void matrix_perspective(Matrix *m, double d){
	Matrix P;
	matrix_identity(&P);
	P.m[3][2] = 1/d;
	P.m[3][3] = 0.0;

	//matrix multiplication with m and T
	matrix_multiply(&P, m, m);
}
