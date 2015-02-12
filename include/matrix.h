//Margaux LeBlanc and Tyler Harley
//Graphics
//10/8/14

//Creating a Transformation Matrix Library include file

#ifndef MATRIX_H
#define MATRIX_H

//define matrix
typedef struct {
	double m[4][4];
} Matrix;

//Vector Functions
double vector_length(Vector *v);
void vector_normalize(Vector *v);
double vector_dot(Vector *a, Vector *b);
void vector_cross(Vector *a, Vector *b, Vector *c);

//2D Matrix Functions
void matrix_print(Matrix *m, FILE *fp);
void matrix_clear(Matrix *m);
void matrix_identity(Matrix *m);
double matrix_get(Matrix *m, int r, int c);
void matrix_set(Matrix *m, int r, int c, double v);
void matrix_copy(Matrix *dest, Matrix *src);
void matrix_transpose(Matrix *m);
void matrix_multiply(Matrix *left, Matrix *right, Matrix *m);
void matrix_xformPoint(Matrix *m, Point *p, Point *q);
void matrix_xformVector(Matrix *m, Vector *p, Vector *q);
void matrix_xformPolygon(Matrix *m, Polygon *p);
void matrix_xformPolyline(Matrix *m, Polyline *p);
void matrix_xformLine(Matrix *m, Line *line);
void matrix_scale2D(Matrix *m, double sx, double sy);
void matrix_rotateZ(Matrix *m, double cth, double sth);
void matrix_translate2D(Matrix *m, double tx, double ty);
void matrix_shear2D(Matrix *m, double shx, double shy);

//3D Matrix Functions
void matrix_translate(Matrix *m, double tx, double ty, double tz);
void matrix_scale(Matrix *m, double sx, double sy, double sz);
void matrix_rotateX(Matrix *m, double cth, double sth);
void matrix_rotateY(Matrix *m, double cth, double sth);
void matrix_rotateXYZ(Matrix *m, Vector *u, Vector *v, Vector *w);
void matrix_shearZ(Matrix *m, double shx, double shy);
void matrix_perspective(Matrix *m, double d);


#endif
