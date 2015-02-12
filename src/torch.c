/*
 * Tyler Harley
 * torch.c
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <color.h>
#include <image.h>
#include <list.h>
#include <math.h>
#include "graphics.h"
#include "drawstate.h"
#include "light.h"
#include "polygon.h"
#include "matrix.h"
#include "view.h"
#include "model.h"
#include "bezier.h"

int main(int argc, char *argv[]){
	Image *src;
	int cols = 600, rows = 500;
	Color ambient, light, brown, yellow, flame, torchlight, ground;
	DrawState *ds;
	Module *fire, *base, *torch, *floor, *scene;
	BezierSurface bs;
	Point p[16];
	Point v[4];
	Vector vList[4];
	Polygon fl;
	Lighting *lighting;
	
	View3D view;
	Matrix gtm, vtm;
	int i;
	
	color_set(&ambient, .05, .05, .05);
	color_set(&light, .5, .5, .5);
	color_set(&brown, 135/255.0, 102/255.0, 51/255.0);
	color_set(&flame, 1, 102/255.0, 0);
	color_set(&yellow, 1, 194/255.0, 102/255.0);
	color_set(&torchlight, 1, .9, .5);
	color_set(&ground, 80/255.0, 53/255.0, 25/255.0);
	
	base = module_create();
	module_bodyColor(base, &brown);
	module_scale(base, .5, -2, .5);
	module_shear2D(base, .25, 0);
	module_rotateY(base, -cos(M_PI/2.0), -sin(M_PI/2.0));
	module_cube(base, 1);
	
	fire = module_create();
	
	point_set3D(&p[0], 0.0, 1, 0.0); 
	point_set3D(&p[1], 0.3, .6, 0.0);
	point_set3D(&p[2], 0.5, .3, 0.0);
	point_set3D(&p[3], 0.0, 0.0, 0.0);
	
	point_set3D(&p[4], 0.0, 1, 0.0); 
	point_set3D(&p[5], 0.2, .6, 0.1);
	point_set3D(&p[6], 0.3, .3, 0.1);
	point_set3D(&p[7], 0.0, 0.0, 0.0);
	
	point_set3D(&p[8], 0.0, 1, 0.0);
	point_set3D(&p[9], 0.1, .6, 0.02);
	point_set3D(&p[10], 0.1, .3, 0.3);
	point_set3D(&p[11], 0.0, 0.0, 0.0);
	
	point_set3D(&p[12], 0.0, 1, 0.0); 
	point_set3D(&p[13], 0.0, .6, 0.3);
	point_set3D(&p[14], 0.0, .3, 0.5);
	point_set3D(&p[15], 0.0, 0.0, 0.0);
	bezierSurface_set(&bs, p);

	module_bodyColor(fire, &flame);
	module_bezierSurface(fire, &bs, 4, 1);
	module_scale(fire, -1, 1, 1);
	module_bezierSurface(fire, &bs, 4, 1);
	module_scale(fire, 1, 1, -1);
	module_bezierSurface(fire, &bs, 4, 1);
	module_scale(fire, -1, 1, 1);
	module_bezierSurface(fire, &bs, 4, 1);
	
	torch = module_create();
	module_module(torch, base);
	module_scale(torch, 1.7, 1.7, 1.7);
	module_translate(torch, 0, 1, -.3);
	module_module(torch, fire);
	
	/*floor = module_create();
	
	module_bodyColor(floor, &ground);
	polygon_init(&fl);
	point_set3D(&v[0], 0, -8, 0);
	point_set3D(&v[1], 40, -8, 0);
	point_set3D(&v[2], 40, -8, -4);
	point_set3D(&v[3], 0, -8, -4);
	polygon_set(&fl, 4, v);
	for(i = 0; i<4; i++){
		vector_set(&vList[i], 0, 1, 0);
	}
	polygon_setNormals(&fl, 4, vList);
	module_polygon(floor, &fl);
	*/
	
	
	
	scene = module_create();
	//module_module(scene, floor);
	
	module_translate(scene, 0, 0, -.3);
	module_module(scene, torch);
	module_translate(scene, 0, 0, .3);
	point_set3D(&p[0], -5, -8, 0.0); 
	point_set3D(&p[1], -5, -4, 0.0);
	point_set3D(&p[2], -5, 4, 0.0);
	point_set3D(&p[3], -5, 8, 0.0);

	point_set3D(&p[4], 0, -8, 0.0); 
	point_set3D(&p[5], 0, -4, 0);
	point_set3D(&p[6], 0, 4, 0);
	point_set3D(&p[7], 0, 8, 0.0);
	
	point_set3D(&p[8], 5, -8, 0.0);
	point_set3D(&p[9], 5, -4, 0.0);
	point_set3D(&p[10], 5, 4, 0.0);
	point_set3D(&p[11], 5, 8, 0.0);
	
	point_set3D(&p[12], 10, -8, 0.0); 
	point_set3D(&p[13], 10, -4, 0.0);
	point_set3D(&p[14], 10, 4, 0.0);
	point_set3D(&p[15], 10, 8, 0.0);
	bezierSurface_set(&bs, p);
	module_bodyColor(scene, &yellow);

	module_bezierSurface(scene, &bs, 5, 1);
	module_translate2D(scene, 4, 0);
	module_module(scene, base);
	module_translate2D(scene, 10, 0);
	module_bezierSurface(scene, &bs, 5, 1);
	module_module(scene, torch);
	module_translate2D(scene, 15, 0);
	module_bezierSurface(scene, &bs, 5, 1);

	src = image_create(rows, cols);
	
	point_set3D(&view.vrp, 0, 2, 0);
	vector_set(&view.vpn, .4, 0, 1);
	vector_set(&view.vup, 0, 1, 0);
	
	view.d = 4;
    view.du = 15;
    view.dv = view.du * (float)rows / cols;
    view.f = 1;
    view.b = 60;
    view.screenx = cols;
    view.screeny = rows;

    matrix_setView3D( &vtm, &view );
    matrix_identity( &gtm );
    
    ds = drawstate_create();
    point_copy(&ds->viewer, &view.vrp);
	ds->shade = ShadeGouraud;
	
	point_set3D(&p[0], 0, 1.5, 0);
	lighting = lighting_create();
	lighting_add(lighting, LightAmbient, &ambient, NULL, NULL, 0, 0);
	lighting_add(lighting, LightPoint, &light, NULL, &p[0], 0, 0);
	point_copy(&p[1], &view.vpn);
	point_set3D(&p[1], p[1].val[0], p[1].val[1] + 2, p[1].val[2]);
	lighting_add(lighting, LightPoint, &torchlight, NULL, &p[1], 0, 0);
	
	point_set3D(&p[2], 14, 1.5, 0);
	point_set3D(&p[3], p[1].val[0] + 14, p[1].val[1], p[1].val[2]);
	//lighting_add(lighting, LightPoint, &light, NULL, &p[2], 0, 0);
	lighting_add(lighting, LightPoint, &torchlight, NULL, &p[3], 0, 0);
	module_draw(scene, &vtm, &gtm, ds, lighting, src);
	image_write(src, "../images/torch3.ppm");
	
	//polygon_clear(&fl);
	image_free(src);
	module_delete(base);
	module_delete(fire);
	module_delete(torch);
	//module_delete(floor);
	module_delete(scene);
	
	return(0);
}