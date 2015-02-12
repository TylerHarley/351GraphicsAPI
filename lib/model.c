/*
 * Tyler Harley & Margaux LeBlanc
 * model.c
 * Contains hierarchical modeling functions
 * 10/24/14
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
#include "model.h"

/* Linked list functions */

// Allocate and return an initialized but empty Element
Element *element_create(){
	Element *e = malloc(sizeof(Element));
	if(!e){
		fprintf(stderr, "Element unable to be allocated\n");
		return(NULL);
	}
	e->type = ObjNone;
	e->next = NULL;

	return(e);
}

// Allocate an Element and store a duplicate of the data pointed to by obj in the Element
Element *element_init(ObjectType type, void *obj){
	Element *e = malloc(sizeof(Element));
	if(!e){
		fprintf(stderr, "Element unable to be allocated\n");
		return(NULL);
	}
	if(!type){
		free(e);
		fprintf(stderr, "Element type not found\n");
		return(NULL);
	}

	e->type = type;
	//duplicate object data being passed in
	switch (e->type){
		case ObjNone:
			break;
		case ObjLine:
			line_copy(&(e->obj.line), obj); 
			break;
		case ObjPoint:
			point_copy(&(e->obj.point), obj);
			break;
		case ObjPolyline:
			//init polyline
			polyline_init(&(e->obj.polyline));
			polyline_copy(&(e->obj.polyline), obj);
			break;
		case ObjPolygon:
			//init polygon
			polygon_init(&(e->obj.polygon));
			polygon_copy(&(e->obj.polygon), obj);
			break;
		case ObjIdentity:
			break;
		case ObjMatrix:
			matrix_copy(&(e->obj.matrix), obj);
			break;
		case ObjColor:
			color_copy(&(e->obj.color), obj);
			break;
		case ObjBodyColor:
			color_copy(&(e->obj.color), obj);
			break;
		case ObjSurfaceColor:
			color_copy(&(e->obj.color), obj);
			break;
		case ObjSurfaceCoeff: 
			e->obj.coeff = *( (float*)obj);
			break;
		case ObjLight:
			break;
		case ObjModule:
			e->obj.module = obj;
			break;
	}
	e->next = NULL;
	return(e);
}

//Free the element and the objects it contains, as appropriate
void element_delete(Element *e){
	if(e){
		if(e->type == ObjPolyline){
			polyline_clear(&(e->obj.polyline));
		}
		else if(e->type == ObjPolygon){
			polygon_clear(&(e->obj.polygon));
		}
		free(e);
	}
}

//Allocates an empty module
Module *module_create(){
	Module *m = malloc(sizeof(Module));
	if(!m){
		fprintf(stderr, "Module unable to be allocated\n");
		return(NULL);
	}
	m->head = NULL;
	m->tail = NULL;
	return(m);
}

//Clears the module's list of elements, freeing memory as appropriate
void module_clear(Module *md){
	while(md->head != NULL){
		//create tmp variable to hold head while we redefine it's meaning
		Element *tmp;
		tmp = md->head;
		md->head = md->head->next;
		element_delete(tmp);
	}
	md->head = NULL;
	md->tail = NULL;
}

//Free all of the memory associated with a module, include memory pointed to by
//md
void module_delete(Module *md){
	//free all the elements
	while(md->head != NULL){
		//create tmp variable to hold head while we redefine it's meaning
		Element *tmp;
		tmp = md->head;
		md->head = md->head->next;
		element_delete(tmp);
	}
	//free module
	free(md);
}

//Generic insert of an element into the module at the tail of the list
void module_insert(Module *md, Element *e){
	if(!md){
		fprintf(stderr, "No module passed in\n");
		exit(1);
	}	
	if(md->head == NULL){
		md->head = e;
		md->tail = e;	
	}
	else{
		md->tail->next = e;
		md->tail = e;
	}
}

// Module functions
//Adds a pointer to the Module sub to the tail of the module's list
void module_module(Module *md, Module *sub){
	Element *e = element_init(ObjModule, sub);	
	module_insert(md, e);
}

//adds p to the tail of the module's list
void module_point(Module *md, Point *p){
	Element *e = element_init(ObjPoint, p);	
	module_insert(md, e);
}

//Adds p to the tail of the module's list
void module_line(Module *md, Line *p){
	Element *e = element_init(ObjLine, p);	
	module_insert(md, e);
}

//Adds p to the tail of the module's list
void module_polyline(Module *md, Polyline *p){
	Element *e = element_init(ObjPolyline, p);	
	module_insert(md, e);
}

//Adds p to the tail of the module's list
void module_polygon(Module *md, Polygon *p){	
	Element *e = element_init(ObjPolygon, p);	
	module_insert(md, e);
}

//Object that sets the current transform to the identity, placed at the tail
//of the module's list
void module_identity(Module *md){
	Element *e = element_init(ObjIdentity, NULL);	//but this does nothing.
	module_insert(md, e);
}

// MStack functions

// create and initialize a new MStack
MStack *mstack_create(void){
	MStack *m = malloc(sizeof(MStack));
	if(!m){
		fprintf(stderr, "MStack unable to be allocated\n");
		return(NULL);
	}
	m->sp = m->mStack;
	return(m);
}

// free an MStack
void mstack_free(MStack *m){
	if(m){
		free(m);
	}
}

// push a Matrix onto the stack
void module_pushMatrix(Matrix mtx, MStack *m){
	*((m->sp)++) = mtx;
}

// pop a Matrix off of the stack
Matrix *module_popMatrix(MStack *m){
	return --(m->sp);
}

// peek at the top Matrix of the stack
Matrix *module_peekMatrix(MStack *m){
	return m->sp;
}

// 2D functions
//Matrix operand to add a translation matrix to the tail of the module's list
void module_translate2D(Module *md, double tx, double ty){
	Matrix m;
	//need to initialize m to identity?? 
	matrix_identity(&m);
	//matrix_translate2D(&m, tx, ty);
	m.m[0][3] = tx;
	m.m[1][3] = ty;
	Element *e = element_init(ObjMatrix, &m);
	module_insert(md, e);
}

//Matrix operand to add a scale matrix to the tail of the module's list
void module_scale2D(Module *md, double sx, double sy){
	Matrix m;
	//need to initialize m to identity?? 
	matrix_identity(&m);
	m.m[0][0] = sx;
	m.m[1][1] = sy;

	Element *e = element_init(ObjMatrix, &m);
	module_insert(md, e);	
}

//Matrix operand adds a rotate about the Zaxis to the tail of the module's list
void module_rotateZ(Module *md, double cth, double sth){
	Matrix m;
	//need to initialize m to identity?? 
	matrix_identity(&m);
	//matrix_rotateZ(&m, cth, sth);
	m.m[0][0] = cth;
	m.m[0][1] = -sth;
	m.m[1][0] = sth;
	m.m[1][1] = cth;
	Element *e = element_init(ObjMatrix, &m);
	module_insert(md, e);
}

//Matrix operand adds a 2D shear matrix to tail of the module's list
void module_shear2D(Module *md, double shx, double shy){
	Matrix m;	
	//need to initialize m to identity?? 
	matrix_identity(&m);
	//matrix_shear2D(&m, shx, shy);
	m.m[0][1] = shx;
	m.m[1][0] = shy;
	Element *e = element_init(ObjMatrix, &m);
	module_insert(md, e);	
}

//Draw the module in to the image using the given view transformation matrix
//[VTM], Lighting and DrawState by traversing the list of Elements.
void module_draw(Module *md, Matrix *VTM, Matrix *GTM, DrawState *ds, Lighting *lighting, Image *src){
	Matrix LTM;
	Element *e;
	Line l;
	Polyline pl;
	Polygon p;
	Point pt, tmpt;
	Matrix m;
	//Color c;
	DrawState d;
	int i;

	matrix_identity(&LTM);
    	e = md->head;

	//initialize polygons and polygons
	polygon_init(&p);
	polyline_init(&pl);
	//traverse elements in module
	//free all the elements
	while(e != NULL){
		switch (e->type){
			case ObjNone:
				break;
			case ObjLine:
				//This procedure relates to Points, Polylines, and Polygons
				//copy 
				line_copy(&l, &(e->obj.line));
				//transform by LTM, GTM, and VTM
				matrix_xformLine(&LTM, &l); //homogenous coordinate turns to 0
				matrix_xformLine(GTM, &l);
				matrix_xformLine(VTM, &l);
				//normalize
				line_normalize(&l);
				//draw
				line_draw(&l, src, ds->color, ds->zBufferFlag);
				break;
			case ObjPoint:
				point_copy(&pt, &(e->obj.point));
				matrix_xformPoint(&LTM, &pt, &tmpt);
				matrix_xformPoint(GTM, &tmpt, &pt);
				matrix_xformPoint(VTM, &pt, &tmpt);
				point_normalize(&tmpt);
				point_draw(&tmpt, src, ds->color);
				break;
			case ObjPolyline:
				polyline_copy(&pl, &(e->obj.polyline));
				matrix_xformPolyline(&LTM, &pl);
				matrix_xformPolyline(GTM, &pl);
				matrix_xformPolyline(VTM, &pl);
				polyline_normalize(&pl);
				polyline_draw(&pl, src, ds->color, ds->zBufferFlag);
				polyline_clear(&pl);
				break;
			case ObjPolygon:
				polygon_copy(&p, &(e->obj.polygon));
				matrix_xformPolygon(&LTM, &p);
				matrix_xformPolygon(GTM, &p);
				// do shading calculations for Gouraud shading
				if((ds->shade == ShadeGouraud || ds->shade == ShadeFlat) && lighting){
					polygon_shade(&p, lighting, ds);
				}
				matrix_xformPolygon(VTM, &p);
				polygon_normalize(&p);
				polygon_drawShade(&p, src, ds, lighting);
				polygon_clear(&p);
				break;
			case ObjIdentity:
				//set LTM to identity matrix
				matrix_identity(&LTM);
				break;
			case ObjMatrix:
				//add matrix to heirarchy
				matrix_multiply(&(e->obj.matrix), &LTM, &LTM);
				break;
			case ObjColor:
				color_copy(&(ds->color), &(e->obj.color));
				break;
			case ObjBodyColor:
				color_copy(&(ds->body), &(e->obj.color));
				color_copy(&(ds->color), &(e->obj.color));
				break;
			case ObjSurfaceColor:
				color_copy(&(ds->surface), &(e->obj.color));
				break;
			case ObjSurfaceCoeff: 
				ds->surfaceCoeff = e->obj.coeff ;
				break;
			case ObjLight:
				//Don't have a light field
				break;
			case ObjModule:
				//never copy a module
 				drawstate_copy(&d, ds);			
				matrix_multiply(GTM, &LTM, &m);
				module_draw(e->obj.module, VTM, &m, &d,lighting, src);
				break;
		}
		e = e->next;
	}
}


//Draw the module in to the image using the given view transformation matrix
//[VTM], Lighting and DrawState by traversing the list of Elements.
//Polygons can be filled with a pattern image or texture.
void module_drawPattern(Module *md, Matrix *VTM, Matrix *GTM, DrawState *ds, Lighting *lighting, Image *src, Image *pattern){
	Matrix LTM;
	Element *e;
	Line l;
	Polyline pl;
	Polygon p;
	Point pt, tmpt;
	Matrix m;
	Color c;
	float coeff;
	DrawState d;

	matrix_identity(&LTM);
    e = md->head;

	//initialize polygons and polygons
	polygon_init(&p);
	polyline_init(&pl);
	//traverse elements in module
	//free all the elements
	while(e != NULL){
		switch (e->type){
			case ObjNone:
				break;
			case ObjLine:
				//This procedure relates to Points, Polylines, and Polygons
				//copy 
				line_copy(&l, &(e->obj.line));
				//transform by LTM, GTM, and VTM
				matrix_xformLine(&LTM, &l); //homogenous coordinate turns to 0
				matrix_xformLine(GTM, &l);
				matrix_xformLine(VTM, &l);
				//normalize
				line_normalize(&l);
				//draw
				line_draw(&l, src, ds->color, ds->zBufferFlag);
				break;
			case ObjPoint:
				point_copy(&pt, &(e->obj.point));
				matrix_xformPoint(&LTM, &pt, &tmpt);
				matrix_xformPoint(GTM, &tmpt, &pt);
				matrix_xformPoint(VTM, &pt, &tmpt);
				point_normalize(&tmpt);
				point_draw(&tmpt, src, ds->color);
				break;
			case ObjPolyline:
				polyline_copy(&pl, &(e->obj.polyline));
				matrix_xformPolyline(&LTM, &pl);
				matrix_xformPolyline(GTM, &pl);
				matrix_xformPolyline(VTM, &pl);
				polyline_normalize(&pl);
				polyline_draw(&pl, src, ds->color, ds->zBufferFlag);
				break;
			case ObjPolygon:
				polygon_copy(&p, &(e->obj.polygon));
				matrix_xformPolygon(&LTM, &p);
				matrix_xformPolygon(GTM, &p);
				matrix_xformPolygon(VTM, &p);
				polygon_normalize(&p);
				if(ds->shade == ShadeFrame){
					//only draw frame
					polygon_draw(&p, src, ds->color, ds->zBufferFlag);
				}
				else{
					//fill frame
 					polygon_drawFillPattern(&p, src, ds->color, pattern, ds);
				}
				break;
			case ObjIdentity:
				//set LTM to identity matrix
				matrix_identity(&LTM);
				break;
			case ObjMatrix:
				//add matrix to heirarchy
				matrix_multiply(&(e->obj.matrix), &LTM, &LTM);
				break;
			case ObjColor:
				color_copy(&(ds->color), &(e->obj.color));
				break;
			case ObjBodyColor:
				color_copy(&(ds->body), &(e->obj.color));
				break;
			case ObjSurfaceColor:
				color_copy(&(ds->surface), &(e->obj.color));
				break;
			case ObjSurfaceCoeff: 
				ds->surfaceCoeff = e->obj.coeff ;
				break;
			case ObjLight:
				//Don't have a light field
				break;
			case ObjModule:
				//never copy a module	
 				drawstate_copy(&d, ds);			
				matrix_multiply(GTM, &LTM, &m);
				module_draw(e->obj.module, VTM, &m, &d,lighting, src);
				break;
		}
		e = e->next;
	}
}


// 3D functions

//Matrix operand to add a 3D translation to the Module
void module_translate(Module *md, double tx, double ty, double tz){
	Matrix m;
	matrix_identity(&m);
	m.m[0][3] = tx;
	m.m[1][3] = ty;
	m.m[2][3] = tz;
	Element *e = element_init(ObjMatrix, &m);
	module_insert(md, e);	
}

//Matrix operand to add a 3D scale to the module
void module_scale(Module *md, double sx, double sy, double sz){
	Matrix m;
	matrix_identity(&m);
	m.m[0][0] = sx;
	m.m[1][1] = sy;
	m.m[2][2] = sz;
	Element *e = element_init(ObjMatrix, &m);
	module_insert(md, e);
}

//Matrix operand to add a rotation about the X-Axis to the module
void module_rotateX(Module *md, double cth, double sth){
	Matrix m;
	matrix_identity(&m);
	m.m[1][1] = cth;
	m.m[1][2] = -sth;
	m.m[2][1] = sth;
	m.m[2][2] = cth;
	Element *e = element_init(ObjMatrix, &m);
	module_insert(md, e);
}

//Matrix operand to add a rotation about the Y-Axis to the module
void module_rotateY(Module *md, double cth, double sth){
	Matrix m;
	matrix_identity(&m);
	m.m[0][0] = cth;
	m.m[0][2] = -sth;
	m.m[2][0] = sth;
	m.m[2][2] = cth;
	Element *e = element_init(ObjMatrix, &m);
	module_insert(md, e);
}

//Matrix operand to add a rotation that orients to the orthonormal axes u,v,w.
void module_rotateXYZ(Module *md, Vector *u, Vector *v, Vector *w){
	Matrix m;
	int i;
	matrix_identity(&m);
	for(i=0; i<3; i++){
		m.m[0][i] = u->val[i];
		m.m[1][i] = v->val[i];
		m.m[2][i] = w->val[i];
	}
	Element *e = element_init(ObjMatrix, &m);
	module_insert(md, e);
}

//Adds a unit cube, axis aligned and centered on zero to the module. If
//solid is 0, add only lines. If solid is !=0, use polygons. Make sure each 
//polygon has surface normals defined for it.
void module_cube(Module *md, int solid){
	Module *m = module_create();
	int i;
	Line l[12];
	Element *e[12];
	Polygon side[6];
	Polygon tpoly;
	Point tv[4];
	Point v[8];
	Point pt[8];
	Vector nList[4];

	//set points for the cube
	point_set3D(&pt[0], .5,.5,.5);
	point_set3D(&pt[1], .5, .5, -.5);
	point_set3D(&pt[2], -.5, .5, -.5);
	point_set3D(&pt[3], -.5, .5, .5);
	point_set3D(&pt[4], -.5, -.5, .5);
	point_set3D(&pt[5], .5, -.5, .5);
	point_set3D(&pt[6], .5, -.5, -.5);
	point_set3D(&pt[7], -.5, -.5, -.5);

	if(solid == 0){
		//we work with lines!
		//create cube out of lines (12 total)
		//top square
		line_set(&(l[0]), pt[0], pt[1]);
		line_set(&(l[1]), pt[1], pt[2]);
		line_set(&(l[2]), pt[2], pt[3]);
		line_set(&(l[3]), pt[3], pt[0]);
		//bottom square
		line_set(&(l[4]), pt[4], pt[5]);
		line_set(&(l[5]), pt[5], pt[6]);
		line_set(&(l[6]), pt[6], pt[7]);
		line_set(&(l[7]), pt[7], pt[4]);
		//connecting top to bottom
		line_set(&(l[8]), pt[3], pt[4]);
		line_set(&(l[9]), pt[0], pt[5]);
		line_set(&(l[10]), pt[1], pt[6]);
		line_set(&(l[11]), pt[2], pt[7]);
		for(i = 0; i<12; i++){
			e[i] = element_init(ObjLine, &(l[i]));
			module_insert(m, e[i]);	
		}
		module_module(md, m);	
	}
	else{
		//We work with polygons!! BLAHHHHAH
		// initialize polygons
		  for(i=0;i<6;i++) {
			polygon_init( &side[i] );
		  }
		  //set points for polygons
		  point_set3D(&v[0], -.5, -.5, -.5);
		  point_set3D(&v[1], .5, -.5, -.5);
		  point_set3D(&v[2], .5, .5, -.5);
		  point_set3D(&v[3], -.5, .5, -.5);
		  point_set3D(&v[4], -.5, -.5, .5);
		  point_set3D(&v[5], .5, -.5, .5);
		  point_set3D(&v[6], .5,.5,.5);
		  point_set3D(&v[7], -.5, .5, .5);
		  
		  // front side
		  for(i=0; i<4; i++){
		  	vector_set(&nList[i], 0, 0, -1);
		  }
		  polygon_set( &side[0], 4, &v[0] );
		  polygon_setNormals( &side[0], 4, nList );
		  
		  // back side
		  for(i=0; i<4; i++){
		  	vector_set(&nList[i], 0, 0, 1);
		  }
		  polygon_set( &side[1], 4, &v[4] );
		  polygon_setNormals( &side[1], 4, nList );
		  
		  // top side
		  point_copy( &tv[0], &v[2] );
		  point_copy( &tv[1], &v[3] );
		  point_copy( &tv[2], &v[7] );
		  point_copy( &tv[3], &v[6] );
		  
		  for(i=0; i<4; i++){
		  	vector_set(&nList[i], 0, 1, 0);
		  }
		  polygon_set( &side[2], 4, tv );
		  polygon_setNormals( &side[2], 4, nList );

		  // bottom side
		  point_copy( &tv[0], &v[0] );
		  point_copy( &tv[1], &v[1] );
		  point_copy( &tv[2], &v[5] );
		  point_copy( &tv[3], &v[4] );
			
		  for(i=0; i<4; i++){
		  	vector_set(&nList[i], 0, -1, 0);
		  }
		  polygon_set( &side[3], 4, tv );
		  polygon_setNormals( &side[3], 4, nList );

		  // left side
		  point_copy( &tv[0], &v[0] );
		  point_copy( &tv[1], &v[3] );
		  point_copy( &tv[2], &v[7] );
		  point_copy( &tv[3], &v[4] );
		  
		  for(i=0; i<4; i++){
		  	vector_set(&nList[i], -1, 0, 0);
		  }
		  polygon_set( &side[4], 4, tv );
		  polygon_setNormals( &side[4], 4, nList );	
		  
		  // right side
		  point_copy( &tv[0], &v[1] );
		  point_copy( &tv[1], &v[2] );
		  point_copy( &tv[2], &v[6] );
		  point_copy( &tv[3], &v[5] );
		  
		  for(i=0; i<4; i++){
		  	vector_set(&nList[i], 1, 0, 0);
		  }
		  polygon_set( &side[5], 4, tv );
		  polygon_setNormals( &side[5], 4, nList );
		  
		  for(i=0;i<6;i++) {
			e[i] = element_init(ObjPolygon, &side[i]);
			module_insert( m, e[i] );
		  }
		
		  module_module(md, m);	

	}	
}

// adds a 3D cylinder (made by Bruce) to the module list. sides tells us
// how many times to divide the cylinder (how many edges along the sides it has)
void module_cylinder( Module *mod, int sides ) {
  Polygon p;
  Point xtop, xbot;
  double x1, x2, z1, z2;
  int i;

  polygon_init( &p );
  point_set3D( &xtop, 0, 1.0, 0.0 );
  point_set3D( &xbot, 0, 0.0, 0.0 );

  // make a fan for the top and bottom sides
  // and quadrilaterals for the sides
  for(i=0;i<sides;i++) {
    Point pt[4];
    Vector n[4];
    int j;

    x1 = cos( i * M_PI * 2.0 / sides );
    z1 = sin( i * M_PI * 2.0 / sides );
    x2 = cos( ( (i+1)%sides ) * M_PI * 2.0 / sides );
    z2 = sin( ( (i+1)%sides ) * M_PI * 2.0 / sides );

    point_copy( &pt[0], &xtop );
    point_set3D( &pt[1], x1, 1.0, z1 );
    point_set3D( &pt[2], x2, 1.0, z2 );

    polygon_set( &p, 3, pt );
    for(j=0;j<3;j++)
	    vector_set( &(n[j]), 0, 1, 0 );
    polygon_setNormals( &p, 3, n );
    module_polygon( mod, &p );

    point_copy( &pt[0], &xbot );
    point_set3D( &pt[1], x1, 0.0, z1 );
    point_set3D( &pt[2], x2, 0.0, z2 );

    polygon_set( &p, 3, pt );
    for(j=0;j<3;j++)
	    vector_set( &(n[j]), 0, -1, 0 );
    polygon_setNormals( &p, 3, n );
    module_polygon( mod, &p );

    point_set3D( &pt[0], x1, 0.0, z1 );
    point_set3D( &pt[1], x2, 0.0, z2 );
    point_set3D( &pt[2], x2, 1.0, z2 );
    point_set3D( &pt[3], x1, 1.0, z1 );

    vector_set( &n[0], x1, 0.0, z1 );
    vector_set( &n[1], x2, 0.0, z2 );
    vector_set( &n[2], x2, 0.0, z2 );
    vector_set( &n[3], x1, 0.0, z1 );
    
    polygon_set( &p, 4, pt );
    polygon_setNormals( &p, 4, n );
    module_polygon( mod, &p );
  }

  polygon_clear( &p );
}

// adds a 3D pyramid with a square base
void module_pyramid(Module *mod){
	Point pt[5];
	Point tpt[3];
	Polygon side[5];
	Vector vList[4];	
	int i;
	
	//initialize polygon
	for(i=0;i<5;i++) {
		polygon_init( &side[i] );
		//polygon_setSided(&side[i], 0);
	}
	// points of a pyramid
	point_set3D( &pt[0], 1, 0, 0 );
	point_set3D( &pt[1],  0, 0, 1 );
	point_set3D( &pt[2],  -1,  0, 0 );
	point_set3D( &pt[3], 0,  0, -1 );
	point_set3D( &pt[4], 0, 1,  0 );
	// bottom
	polygon_set( &side[0], 4, &(pt[0]) );
	for(i = 0; i<4; i++){
		vector_set(&vList[i], 0, -1, 0);
	}
	polygon_setNormals(&side[0], 4, &vList);
	module_polygon( mod, &side[0] );
	
	// front side
	point_copy( &tpt[0], &pt[2] );
	point_copy( &tpt[1], &pt[3] );
	point_copy( &tpt[2], &pt[4] );

	polygon_set( &side[1], 3, tpt );
	for(i = 0; i<3; i++){
		vector_set(&vList[i], 1, 1, -1);
	}
	polygon_setNormals(&side[1], 3, &vList);
	module_polygon( mod, &side[1] );

	//left side
	point_copy( &tpt[0], &pt[1] );
	point_copy( &tpt[1], &pt[2] );
	point_copy( &tpt[2], &pt[4] );

	polygon_set( &side[2], 3, tpt );
	for(i = 0; i<3; i++){
		vector_set(&vList[i], 1, 1, 1);
	}
	polygon_setNormals(&side[2], 3, &vList);
	module_polygon( mod, &side[2] );

	// right side
	point_copy( &tpt[0], &pt[3] );
	point_copy( &tpt[1], &pt[0] );
	point_copy( &tpt[2], &pt[4] );

	polygon_set( &side[3], 3, tpt );
	for(i = 0; i<3; i++){
		vector_set(&vList[i], -1, 1, -1);
	}
	polygon_setNormals(&side[3], 3, &vList);
	module_polygon( mod, &side[3] );

	// back side
	point_copy( &tpt[0], &pt[0] );
	point_copy( &tpt[1], &pt[1] );
	point_copy( &tpt[2], &pt[4] );

	polygon_set( &side[4], 3, tpt );
	for(i = 0; i<3; i++){
		vector_set(&vList[i], -1, 1, 1);
	}
	polygon_setNormals(&side[4], 3, &vList);
	module_polygon( mod, &side[4] );
	printf("set polygon pyramid\n");

	for(i=0;i<5;i++) {
		polygon_clear( &side[i] );
	}
}

// Shading functions
void module_color(Module *md, Color *c){
	Element *e = element_init(ObjColor, c);	
	module_insert(md, e);
}

void module_bodyColor(Module *md, Color *c){
	Element *e = element_init(ObjBodyColor, c);
	module_insert(md, e);
}
void module_surfaceColor(Module *md, Color *c){
	Element *e = element_init(ObjSurfaceColor, c);
	module_insert(md, e);
}

void module_surfaceCoeff(Module *md, float coeff){
	Element *e = element_init(ObjSurfaceCoeff, &coeff);
	module_insert(md, e);
}
