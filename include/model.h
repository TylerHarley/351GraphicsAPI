/*
 * Tyler Harley & Margaux LeBlanc
 * model.h
 * Contains prototypes and typedefs for hierarchical
 * modeling functions
 * 10/24/14
 */

#ifndef MODEL_H
#define MODEL_H


typedef enum { // example of an enumerated type
	ObjNone,
	ObjLine,
	ObjPoint,
	ObjPolyline,
	ObjPolygon,
	ObjIdentity,
	ObjMatrix,
	ObjColor,
	ObjBodyColor,
	ObjSurfaceColor,
	ObjSurfaceCoeff,
	ObjLight,
	ObjModule
} ObjectType;

typedef union {
	Point point;
	Line line;
	Polyline polyline;
	Polygon polygon;
	Matrix matrix;
	Color color;
	float coeff;
	void *module;
} Object;

// Module structure
typedef struct {
	ObjectType type;
	Object obj;
	void *next;
} Element;

typedef struct{
	Element *head;
	Element *tail;
} Module;

typedef struct{
	Matrix mStack[1000];
	Matrix *sp;
} MStack;

// Linked list functions
Element *element_create(void);
Element *element_init(ObjectType type, void *obj);
void element_delete(Element *e);
Module *module_create(void);
void module_clear(Module *md);
void module_delete(Module *md);
void module_insert(Module *md, Element *e);

// Module functions
void module_module(Module *md, Module *sub);
void module_point(Module *md, Point *p);
void module_line(Module *md, Line *p);
void module_polyline(Module *md, Polyline *p);
void module_polygon(Module *md, Polygon *p);
void module_identity(Module *md);

// MStack functions
MStack *mstack_create(void);
void mstack_free(MStack *m);
void module_pushMatrix(Matrix mtx, MStack *m);
Matrix *module_popMatrix(MStack *m);
Matrix *module_peekMatrix(MStack *m);

// 2D functions
void module_translate2D(Module *md, double tx, double ty);
void module_scale2D(Module *md, double sx, double sy);
void module_rotateZ(Module *md, double cth, double sth);
void module_shear2D(Module *md, double shx, double shy);
void module_draw(Module *md, Matrix *VTM, Matrix *GTM, DrawState *ds, Lighting *lighting, Image *src);
void module_drawPattern(Module *md, Matrix *VTM, Matrix *GTM, DrawState *ds, Lighting *lighting, Image *src, Image *pattern);

// 3D functions
void module_translate(Module *md, double tx, double ty, double tz);
void module_scale(Module *md, double sx, double sy, double sz);
void module_rotateX(Module *md, double cth, double sth);
void module_rotateY(Module *md, double cth, double sth);
void module_rotateXYZ(Module *md, Vector *u, Vector *v, Vector *w);
void module_cube(Module *md, int solid);
void module_cylinder(Module *md, int sides);
void module_pyramid(Module *md);

// Shading functions
void module_color(Module *md, Color *c);
void module_bodyColor(Module *md, Color *c);
void module_surfaceColor(Module *md, Color *c);
void module_surfaceCoeff(Module *md, float coeff);

#endif
