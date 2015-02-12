//Margaux LeBlanc
//Scene with Modules
//Graphics
//10/28/14

//This file draws a picture of a mountain range with clouds

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <color.h>
#include <image.h>
#include <list.h>
#include <math.h>
#include "graphics.h"
#include "polygon.h"
#include "matrix.h"
#include "view.h"
#include "drawstate.h"
#include "model.h"

//Add a unit pyramid 
void pyramid(Module *mod);
void pyramid(Module *mod){
	Point pt[5];
	Point tpt[3];
	Polygon side[5];	
	int i;
	
	//initialize polygon
	for(i=0;i<6;i++) {
		polygon_init( &side[i] );
	}
	// points of a pyramid
	point_set3D( &pt[0], 1, 0, 0 );
	point_set3D( &pt[1],  0, 0, 1 );
	point_set3D( &pt[2],  -1,  0, 0 );
	point_set3D( &pt[3], 0,  0, -1 );
	point_set3D( &pt[4], 0, 1,  0 );
	// bottom
	polygon_set( &side[0], 4, &(pt[0]) );
	module_polygon( mod, &side[0] );

	// front side
	point_copy( &tpt[0], &pt[2] );
	point_copy( &tpt[1], &pt[3] );
	point_copy( &tpt[2], &pt[4] );

	polygon_set( &side[1], 3, tpt );
	module_polygon( mod, &side[1] );

	//left side
	point_copy( &tpt[0], &pt[1] );
	point_copy( &tpt[1], &pt[2] );
	point_copy( &tpt[2], &pt[4] );

	polygon_set( &side[2], 3, tpt );
	module_polygon( mod, &side[2] );

	// right side
	point_copy( &tpt[0], &pt[3] );
	point_copy( &tpt[1], &pt[0] );
	point_copy( &tpt[2], &pt[4] );

	polygon_set( &side[3], 3, tpt );
	module_polygon( mod, &side[3] );

	// back side
	point_copy( &tpt[0], &pt[0] );
	point_copy( &tpt[1], &pt[1] );
	point_copy( &tpt[2], &pt[4] );

	polygon_set( &side[4], 3, tpt );
	module_polygon( mod, &side[4] );
	printf("set polygon pyramid\n");
}

//Draws a mountain range with clouds
int main(int argc, char *argv[]) {
	Image *src, *pattern;
	Module *mountains; //has five mountain modules in it.
    View3D view;
    Matrix vtm, gtm;
    DrawState *ds;
	int cols = 300;
	int rows = 200;

	//set up the view
    point_set3D( &(view.vrp), 2, .5, -1 );
	vector_set(&(view.vpn), -view.vrp.val[0], -view.vrp.val[1], -view.vrp.val[2]);
    vector_set( &(view.vup), 0, 1, 0 );
    view.d = 1;
    view.du = 1.6;
    view.dv = 0.9;
    view.f = 1;
    view.b = 5;
    view.screenx = cols;
    view.screeny = rows;

    matrix_setView3D( &vtm, &view );
    matrix_identity( &gtm );
	printf("finished setting up view\n");

    // 3 mountains
	mountains = module_create();
    pyramid( mountains );

	module_identity(mountains);
	module_translate(mountains, 1, 0, 1); //never change y bc should be on same ground level
	pyramid(mountains);

	module_identity(mountains);
	module_translate(mountains, -1.5, 0, 0); 
	pyramid(mountains);

	
	printf("set up mountains\n");

    // create the image and drawstate
    src = image_read("../images/iceSheetS.ppm");
	pattern = image_read("../images/smallMtnText.ppm");
    ds = drawstate_create();
    ds->shade = ShadeConstant;
	printf("created images and draw state\n");
    // draw into the scene
    module_drawPattern( mountains, &vtm, &gtm, ds, NULL, src, pattern );
	printf("Mountains are drawn\n");
	// write out the scene
    image_write( src, "../images/mountains.ppm" );
	printf("image is written\n");

	// free the modules
	module_delete(mountains);
	printf("module mountains have been deleted\n");

	// free the drawstate
    free(ds);

	// free the image
    image_free( src );
	image_free(pattern);
	printf("images and draw state freed\n");

	return(0);
}
