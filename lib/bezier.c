//Tyler Harley and Margaux LeBlanc
//11/3/14
//Graphics

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
#include "bezier.h"

//Curve and surface functions
//sets the z buffer flag to 1 and the curve points to the X-axis between 0 and 1
void bezierCurve_init(BezierCurve *b){
	int i;
	if(!b){
		fprintf(stderr, "Bezier Curve not found\n");
		return;
	}
	else{
		b->zbuffer = 1;
		for(i=0; i<4; i++){
			b->controlPt[i].val[0] = .5;
			b->controlPt[i].val[1] = 0; //set y and z to 0 so lies on x axis
			b->controlPt[i].val[2] = 0; 
			b->controlPt[i].val[3] = 1;
		}
	}
}

//sets the zbuffer flat to 1 and the surface to the X-Z plane between (0,0) and (1,1)
void bezierSurface_init(BezierSurface *b){
	int i;
	if(!b){
		fprintf(stderr, "Bezier Surface not found\n");
		return;
	}
	else{
		b->zbuffer = 1;
		for(i=0; i<16; i++){
			b->controlPt[i].val[0] = .5; //put it in x-z plane
			b->controlPt[i].val[1] = 0;
			b->controlPt[i].val[2] = .5;
			b->controlPt[i].val[3] = 1;
		}
	}
}

//Sets the control points of the BezierCurve to the four points in the vlist array
void bezierCurve_set(BezierCurve *b, Point *vlist){
	int i;
	//do I have to check if b and vlist are not null?
	//copy vlist to bezier curve
	for(i=0; i<4; i++){ 
		b->controlPt[i] = vlist[i];
	}
}

//Sets the control points of the BezierSurface to the 16 points in the vlist array
void bezierSurface_set(BezierSurface *b, Point *vlist){
	//do I have to check if b and vlist are not null?
	int i;
	for(i=0; i<16; i++){ 
		b->controlPt[i] = vlist[i];
	}
}

//sets the zbuffer flag to the given value
void bezierCurve_zBuffer(BezierCurve *b, int flag){
	b->zbuffer = flag;
}

//sets the zbuffer flag tot he given value
void bezierSurface_zBuffer(BezierSurface *b, int flag){
	b->zbuffer = flag;
}

void deCasteljau(Point *src, Point *dst, Point *dst2){
	dst[0] = src[0];

	dst[1].val[0] = .5*(src[0].val[0] + src[1].val[0]);
	dst[1].val[1] = .5*(src[0].val[1] + src[1].val[1]);
	dst[1].val[2] = .5*(src[0].val[2] + src[1].val[2]);
	dst[1].val[3] = 1.0;

	dst[2].val[0] = .5*(dst[1].val[0]) + .25*(src[1].val[0]+ src[2].val[0]);
	dst[2].val[1] = .5*(dst[1].val[1]) + .25*(src[1].val[1]+ src[2].val[1]);
	dst[2].val[2] = .5*(dst[1].val[2]) + .25*(src[1].val[2]+ src[2].val[2]);
	dst[2].val[3] = 1.0;

	dst2[3] = src[3];

	dst2[2].val[0] = .5*(src[2].val[0] + src[3].val[0]);
	dst2[2].val[1] = .5*(src[2].val[1] + src[3].val[1]);
	dst2[2].val[2] = .5*(src[2].val[2] + src[3].val[2]);
	dst2[2].val[3] = 1.0;

	dst2[1].val[0] = .5*(dst2[2].val[0]) + .25*(src[1].val[0] + src[2].val[0]);
	dst2[1].val[1] = .5*(dst2[2].val[1]) + .25*(src[1].val[1] + src[2].val[1]);
	dst2[1].val[2] = .5*(dst2[2].val[2]) + .25*(src[1].val[2] + src[2].val[2]);
	dst2[1].val[3] = 1.0;

	dst[3].val[0] = .5*(dst[2].val[0] + dst2[1].val[0]);
	dst[3].val[1] = .5*(dst[2].val[1] + dst2[1].val[1]);
	dst[3].val[2] = .5*(dst[2].val[2] + dst2[1].val[2]);
	dst[3].val[3] = 1.0;

	dst2[0] = dst[3];
	//return dst;
}

/*
Draws the bezier curve, given in screen coords, into the image using the 
given color. Uses an appropriate number of line segments to draw the 
curve. Ex) if the bounding box of the control points is less than 10 pixels in the largest dimension, then it is reasonable to draw the lines btw the
control points as an approximation to the curve
*/
void bezierCurve_draw(BezierCurve *b, Image *src, Color c, int zBufferFlag){
	int thresh = 10;
	int i;
	double ymin = b->controlPt[0].val[1];
	double ymax = b->controlPt[0].val[1];
	double xmin = b->controlPt[0].val[0];
	double xmax = b->controlPt[0].val[0]; 

	Polyline pl;
	BezierCurve q, r;
	Point vlistR[4];
	Point vlistQ[4];

	//find bounding box
	for(i=1; i<4; i++){
		if(b->controlPt[i].val[0] < xmin)
			xmin = b->controlPt[i].val[0];
		else if(b->controlPt[i].val[0] > xmax)
			xmax = b->controlPt[i].val[0];
		if(b->controlPt[i].val[1] < ymin)
			ymin = b->controlPt[i].val[1];
		else if(b->controlPt[i].val[1] > ymax)
			ymax = b->controlPt[i].val[1];
	}

	//if bounding box is less than ten pexels, draw
	if( (ymax-ymin <thresh) && (xmax-xmin < thresh) ){
		//draw lines
		/*for(i=0; i<3; i++){
			line_set(&l, b->controlPt[i], b->controlPt[i+1]);
			line_draw(&l, src, c);
			printf("line %d drawing\n", 1 + i);
		}*/
		polyline_init(&pl);
		polyline_set(&pl, 4, b->controlPt);
		polyline_draw(&pl, src, c, zBufferFlag);
		return;
	}
	//all in the recursion
	else{
		deCasteljau(b->controlPt, vlistQ, vlistR);
		bezierCurve_set(&q, vlistQ);
		bezierCurve_set(&r, vlistR);
		bezierCurve_draw(&q, src, c, zBufferFlag);
		bezierCurve_draw(&r, src, c, zBufferFlag);
	}
}
		
//bezier curve and surface module functions
/*
Use the de Casteljau algorithm to subdivide the Bezier surface divisions
times. Then draw either the lines connecting the control points to the module.
For example if divisions is 1, the four original Bezier curve control points will be used to generate eight control points and two new Bezier curves.
Then the algorithm will add six lines to the module, three for each of the 
smaller Bezier curves.
*/
void module_bezierCurve(Module *m, BezierCurve *b, int divisions){
	Line l;
	BezierCurve q, r;
	Point vlistR[4];
	Point vlistQ[4];
	int i;

	if(divisions == 0){
		for(i=0; i<3; i++){
			line_set(&l, b->controlPt[i], b->controlPt[i+1]);
			module_line(m, &l);
		}
	}
	else{
		deCasteljau(b->controlPt, vlistQ, vlistR);
		bezierCurve_set(&q, vlistQ);
		bezierCurve_set(&r, vlistR);
		module_bezierCurve(m, &q, divisions - 1);
		module_bezierCurve(m, &r, divisions - 1);
	}
}

// find the normals of a triangle
void findNormals(Polygon *p){
	int i;
	Vector nList[3];
	Vector v1, v2;
	//printf("within findNormals");
	point_print(&p->vertex[0]);
	//printf("%f\n", x);
	//vector_set(&v1, 1, 0, 0);
	//vector_set(&v1, p->vertex[0].val[0] - p->vertex[1].val[0],
			//		p->vertex[0].val[1] - p->vertex[1].val[1],
		//			p->vertex[0].val[2] - p->vertex[1].val[2]);
	/*vector_set(&v2, p->vertex[2].val[0] - p->vertex[1].val[0],
					p->vertex[2].val[1] - p->vertex[1].val[1],
					p->vertex[2].val[2] - p->vertex[1].val[1]);
	for(i=0; i<3; i++){
		vector_cross(&v1, &v2, &nList[i]);
	}*/
}

/*
Use the de Casteljau algorithm to subidivide the bezier surface divisions
times, then draw either the lines connecting the control points, if solid is
0, or draw triangles using the four corner control points.
*/
void module_bezierSurface(Module *m, BezierSurface *b, int divisions, int solid){
	Polygon *p;
	Line l;
	BezierSurface s, t, u, v;
	Point vlistR[16], vlistInR[4];
	Point vlistQ[16], vlistInQ[4];
	Point vlistS[16];
	Point vlistT[16];
	Point vlistU[16];
	Point vlistV[16];
	Point vlistPoly[3];
	Vector v1, v2, v3, v4;
	Vector vList[3];
	int i, j, k;
	
	p = polygon_create();
	polygon_init(p);	

	if(divisions == 0){
		if(solid == 0){
			for(i=0; i<=12; i+=4){
				line_set(&l, b->controlPt[i], b->controlPt[i+1]);
				module_line(m, &l);
				line_set(&l, b->controlPt[i+1], b->controlPt[i+2]);
				module_line(m, &l);
				line_set(&l, b->controlPt[i+2], b->controlPt[i+3]);
				module_line(m, &l);
			}
			//draw in other direction now
			for(j=0; j<4; j++){
				line_set(&l, b->controlPt[j], b->controlPt[j+4]);
				module_line(m, &l);
				line_set(&l, b->controlPt[j+4], b->controlPt[j+8]);
				module_line(m, &l);
				line_set(&l, b->controlPt[j+8], b->controlPt[j+12]);
				module_line(m, &l);
			}
		}
		else{
			//put in polygon triangles
			//first one is a special case
			vlistPoly[0] = b->controlPt[0];
			vlistPoly[1] = b->controlPt[3];
			vlistPoly[2] = b->controlPt[12];
			polygon_set(p, 3, &vlistPoly);
			polygon_setSided(p, 0);
			
			// find normals
			vector_set(&v1, vlistPoly[1].val[0] - vlistPoly[0].val[0],
							vlistPoly[1].val[1] - vlistPoly[0].val[1],
							vlistPoly[1].val[2] - vlistPoly[0].val[2]);
			vector_set(&v2, vlistPoly[2].val[0] - vlistPoly[0].val[0],
							vlistPoly[2].val[1] - vlistPoly[0].val[1],
							vlistPoly[2].val[2] - vlistPoly[0].val[2]);
			vector_cross(&v1, &v2, &vList[0]);
			
			// normal for control point 3
			vector_set(&v1, vlistPoly[0].val[0] - vlistPoly[1].val[0],
							vlistPoly[0].val[1] - vlistPoly[1].val[1],
							vlistPoly[0].val[2] - vlistPoly[1].val[2]);
			vector_set(&v2, vlistPoly[2].val[0] - vlistPoly[1].val[0],
							vlistPoly[2].val[1] - vlistPoly[1].val[1],
							vlistPoly[2].val[2] - vlistPoly[1].val[2]);
			vector_cross(&v1, &v2, &vList[1]);
			
			// normal for control point 12
			vector_set(&v1, vlistPoly[0].val[0] - vlistPoly[2].val[0],
							vlistPoly[0].val[1] - vlistPoly[2].val[1],
							vlistPoly[0].val[2] - vlistPoly[2].val[2]);
			vector_set(&v2, vlistPoly[1].val[0] - vlistPoly[2].val[0],
							vlistPoly[1].val[1] - vlistPoly[2].val[1],
							vlistPoly[1].val[2] - vlistPoly[2].val[2]);
			vector_cross(&v1, &v2, &vList[2]);
			polygon_setNormals(p, 3, &vList);
			module_polygon(m, p);
			
			vlistPoly[0] = b->controlPt[15];
			vlistPoly[1] = b->controlPt[3];
			vlistPoly[2] = b->controlPt[12];
			polygon_set(p, 3, &vlistPoly);
			polygon_setSided(p, 0);
			
			// normal for control point 15
			vector_set(&v1, vlistPoly[1].val[0] - vlistPoly[0].val[0],
							vlistPoly[1].val[1] - vlistPoly[0].val[1],
							vlistPoly[1].val[2] - vlistPoly[0].val[2]);
			vector_set(&v2, vlistPoly[2].val[0] - vlistPoly[0].val[0],
							vlistPoly[2].val[1] - vlistPoly[0].val[1],
							vlistPoly[2].val[2] - vlistPoly[0].val[2]);
			vector_cross(&v1, &v2, &vList[0]);
			polygon_setNormals(p, 3, &vList);
			module_polygon(m, p);
		}
	}
	else{
		for(i=0; i<4; i++){
			deCasteljau(&(b->controlPt[i*4]), &(vlistQ[i*4]), &(vlistR[i*4]));
		}
		for(j=0; j<4; j++){
			for(k=0; k<4; k++){
				vlistInQ[k] = vlistQ[j+k*4];
				vlistInR[k] = vlistR[j+k*4];
			}
			deCasteljau(vlistInQ, &(vlistS[j*4]), &(vlistT[j*4]));
			deCasteljau(vlistInR, &(vlistU[j*4]), &(vlistV[j*4]));
			//deCasteljau(&(vlistS[i*4]), &(vlistW[i*4]), &(vlistX[i*4]));
		}

		bezierSurface_set(&s, vlistS);
		bezierSurface_set(&t, vlistT);
		bezierSurface_set(&u, vlistU);
		bezierSurface_set(&v, vlistV);

		module_bezierSurface(m, &s, divisions - 1, solid);
		module_bezierSurface(m, &t, divisions - 1, solid);
		module_bezierSurface(m, &u, divisions - 1, solid);
		module_bezierSurface(m, &v, divisions - 1, solid);
		printf("bezier surface added to model\n");
	}
	polygon_free(p);
}

// makes and adds a sphere out of bezier surfaces
void module_sphere(Module *md, int divisions, int solid){
	Point pt[16];
	BezierSurface bs;
	
	point_set3D(&pt[0], 0, 0, .5);
	point_set3D(&pt[3], .5*cos(22.5*M_PI/180.0)*sin(0),.5*sin(22.5*M_PI/180.0)*sin(0), .5*cos(0));
	point_set3D(&pt[6], .5*cos(M_PI/4.0)*sin(0),.5*sin(M_PI/4.0)*sin(0), .5*cos(0));
	point_set3D(&pt[9], .5*cos(67.5*M_PI/180.0)*sin(0),.5*sin(67.5*M_PI/180.0)*sin(0), .5*cos(0));
	point_set3D(&pt[12], .5, 0, 0);


	point_set3D(&pt[1], .5*cos(0)*sin(M_PI/4.0),.5*sin(0)*sin(M_PI/4.0), .5*cos(M_PI/4.0));
	point_set3D(&pt[10], .5*cos(67.5*M_PI/180.0)*sin(M_PI/4.0),.5*sin(67.5*M_PI/180.0)*sin(M_PI/4.0), .5*cos(M_PI/4.0));

	point_set3D(&pt[7], .5*cos(M_PI/4.0)*sin(M_PI/4.0),.5*sin(M_PI/4.0)*sin(M_PI/4.0), .5*cos(M_PI/4.0));	
	point_set3D(&pt[4], .5*cos(22.5*M_PI/180.0)*sin(M_PI/4.0),.5*sin(22.5*M_PI/180.0)*sin(M_PI/4.0), .5*cos(M_PI/4.0));	

	point_set3D(&pt[13], .5*cos(M_PI/2.0)*sin(M_PI/4.0),.5*sin(M_PI/2.0)*sin(M_PI/4.0), .5*cos(M_PI/4.0));

/*
	point_set3D(&pt[2], .5*cos(0)*sin(M_PI/3.0),.5*sin(0)*sin(M_PI/3.0), .5*cos(M_PI/3.0));
	point_set3D(&pt[6], .5*cos(22.5*M_PI/180.0)*sin(M_PI/3),.5*sin(22.5*M_PI/180.0)*sin(M_PI/3.0), .5*cos(M_PI/6));
	point_set3D(&pt[9], .5*cos(M_PI/4.0)*sin(M_PI/3.0),.5*sin(M_PI/4.0)*sin(M_PI/3.0), .5*cos(M_PI/3.0));
	point_set3D(&pt[12], .5*cos(67.5*M_PI/180.0)*sin(M_PI/3.0),.5*sin(67.5*M_PI/180.0)*sin(M_PI/3.0), .5*cos(M_PI/3));
	point_set3D(&pt[15], .5*cos(M_PI/2.0)*sin(M_PI/3),.5*sin(M_PI/2.0)*sin(M_PI/3.0), .5*cos(M_PI/3.0));
*/

	point_set3D(&pt[2], 0, .5, 0);
	point_set3D(&pt[5], 0, .5, 0);
	point_set3D(&pt[8], 0, .5, 0);
	point_set3D(&pt[11], 0, .5, 0);
	point_set3D(&pt[14], 0, .5, 0);
	point_set3D(&pt[15], 0, .5, 0);

	bezierSurface_set(&bs, pt);
	module_bezierSurface(md, &bs, divisions, solid);
}
