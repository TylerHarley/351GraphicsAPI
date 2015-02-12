#ifndef GRAPHICS_H
#define GRAPHICS_H

//Margaux LeBlanc & Tyler Harley
//Graphics
//9/20/14

typedef struct{
	double val[4];
} Point;

typedef Point Vector;

typedef struct{
	int zBuffer;
	Point a, b;
} Line;

typedef struct{
	int zBuffer;
	int numVertex;
	Point *vertex;
} Polyline;

typedef struct{
	double r;
	Point c;
} Circle;

typedef struct{
	double ra;
	double rb;
	Point c;
} Ellipse;

void vector_set(Vector *v, double x, double y, double z);
void vector_print(Vector *v, FILE *fp);
void vector_copy(Vector *dest, Vector *src);
void vector_average(Vector *v, Vector *vList, int n);
// point functions
void point_set2D(Point *p, double x, double y);
void point_set3D(Point *p, double x, double y, double z);
void point_set(Point *p, double x, double y, double z, double h);
void point_average(Point *p, Point *pList, int n);
void point_copy(Point *to, Point *from);
void point_normalize(Point *p);
void point_draw(Point *p, Image *src, Color c);
void point_drawf(Point *p, Image *src, FPixel c);
// line functions
void line_set2D(Line *l, double x0, double y0, double x1, double y1);
void line_set(Line *l, Point ta, Point tb);
void line_zBuffer(Line *l, int flag);
void line_copy(Line *to, Line *from);
void line_normalize(Line *l);
void line_draw(Line *l, Image *src, Color c, int zBufferFlag);
void line_draw_dash(Line *l, Image *src, Color c);
// polyline functions
Polyline *polyline_create(void);
Polyline *polyline_createp(int numV, Point *vlist);
void polyline_free(Polyline *p);
void polyline_init(Polyline *p);
void polyline_set(Polyline *p, int numV, Point *vlist);
void polyline_clear(Polyline *p);
void polyline_zBuffer(Polyline *p, int flag);
void polyline_copy(Polyline *to, Polyline *from);
void polyline_print(Polyline *p, FILE *fp);
void polyline_normalize(Polyline *p);
void polyline_draw(Polyline *p, Image *src, Color c, int zBufferFlag);
// circle functions
void circle_set(Circle *c, Point tc, double tr);
void circle_draw(Circle *c, Image *src, Color p);
void circle_drawFill(Circle *c, Image *src, Color p);
void arc_set(Circle *c, Point tc, double tr);
void arc_draw(Circle *c, Image *src, Color p);
// ellipse functions
void ellipse_set(Ellipse *e, Point tc, double ta, double tb);
void ellipse_draw(Ellipse *e, Image *src, Color p);
void ellipse_drawFill(Ellipse *e, Image *src, Color p);
void half_ellipse_draw(Ellipse *e, Image *src, Color p);

#endif
