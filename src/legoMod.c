//Margaux LeBlanc
//Scene with Modules
//Graphics
//10/28/14

//Makes a picture of many lego men out of modules

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

/*
  Adds to the module a unit cylinder, aligned along the Y-axis

  Takes in the number of subdivisions to use when creating the cylinder
 */
void cylinder( Module *mod, int sides );
void cylinder( Module *mod, int sides ) {
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

    x1 = cos( i * M_PI * 2.0 / sides );
    z1 = sin( i * M_PI * 2.0 / sides );
    x2 = cos( ( (i+1)%sides ) * M_PI * 2.0 / sides );
    z2 = sin( ( (i+1)%sides ) * M_PI * 2.0 / sides );

    point_copy( &pt[0], &xtop );
    point_set3D( &pt[1], x1, 1.0, z1 );
    point_set3D( &pt[2], x2, 1.0, z2 );

    polygon_set( &p, 3, pt );
    module_polygon( mod, &p );

    point_copy( &pt[0], &xbot );
    point_set3D( &pt[1], x1, 0.0, z1 );
    point_set3D( &pt[2], x2, 0.0, z2 );

    polygon_set( &p, 3, pt );
    module_polygon( mod, &p );

    point_set3D( &pt[0], x1, 0.0, z1 );
    point_set3D( &pt[1], x2, 0.0, z2 );
    point_set3D( &pt[2], x2, 1.0, z2 );
    point_set3D( &pt[3], x1, 1.0, z1 );
    
    polygon_set( &p, 4, pt );
    module_polygon( mod, &p );
  }

  polygon_clear( &p );
}

// adds a cube centered on the origin of width 2x2x2 to the module
void cube( Module *mod );
void cube( Module *mod ) {
  Point pt[4];
  Polygon p;

  polygon_init( &p );
  point_set3D( &pt[0], -1, -1, -1 );
  point_set3D( &pt[1], -1, -1,  1 );
  point_set3D( &pt[2], -1,  1,  1 );
  point_set3D( &pt[3], -1,  1, -1 );
  polygon_set( &p, 4, pt );
  module_polygon( mod, &p );

  point_set3D( &pt[0], 1, -1, -1 );
  point_set3D( &pt[1], 1, -1,  1 );
  point_set3D( &pt[2], 1,  1,  1 );
  point_set3D( &pt[3], 1,  1, -1 );
  polygon_set( &p, 4, pt );
  module_polygon( mod, &p );

  point_set3D( &pt[0], -1, -1, -1 );
  point_set3D( &pt[1], -1, -1,  1 );
  point_set3D( &pt[2],  1, -1,  1 );
  point_set3D( &pt[3],  1, -1, -1 );
  polygon_set( &p, 4, pt );
  module_polygon( mod, &p );

  point_set3D( &pt[0], -1, 1, -1 );
  point_set3D( &pt[1], -1, 1,  1 );
  point_set3D( &pt[2],  1, 1,  1 );
  point_set3D( &pt[3],  1, 1, -1 );
  polygon_set( &p, 4, pt );
  module_polygon( mod, &p );

  point_set3D( &pt[0], -1, -1, -1 );
  point_set3D( &pt[1], -1,  1, -1 );
  point_set3D( &pt[2],  1,  1, -1 );
  point_set3D( &pt[3],  1, -1, -1 );
  polygon_set( &p, 4, pt );
  module_polygon( mod, &p );

  point_set3D( &pt[0], -1, -1, 1 );
  point_set3D( &pt[1], -1,  1, 1 );
  point_set3D( &pt[2],  1,  1, 1 );
  point_set3D( &pt[3],  1, -1, 1 );
  polygon_set( &p, 4, pt );
  module_polygon( mod, &p );

}

//Makes a picture of many lego men out of modules
int main(int argc, char *argv[]) {
	Image *src;
	//scene is going to be at least 3 layers deep
	Module *scene, *formation, *man, *top, *bottom, *leg, *arm, *head, *body, *underpants;
    View3D view;
    Matrix vtm, gtm;
    DrawState *ds;
	int cols = 533;
	int rows = 800;
    Color Blue = { { .027, 0.152, 0.572 } };
    Color Red =  { { .674, 0.047, 0.047 } };
    Color Yellow =  { { .901, 0.768, 0.039 } };

	//set up the view
    point_set3D( &(view.vrp), -10, 0, -10);
	vector_set(&(view.vpn), -view.vrp.val[0], -view.vrp.val[1], -view.vrp.val[2]);
    vector_set( &(view.vup), 0, 1, 0 );
    view.du = 1;
    view.dv = 1;
    view.f = 1;
    view.b = 15;
    view.screenx = cols;
    view.screeny = rows;

    matrix_setView3D( &vtm, &view );
    matrix_identity( &gtm );
	printf("finished setting up view\n");

	//define arm module
	arm = module_create();
	module_color(arm, &Blue);
	module_scale(arm, .5, 1.25, .5);
	cube(arm);

	
	//define leg module
	leg = module_create();
	module_color(leg, &Blue);
	module_scale(leg, .55, 1.5, .5);
	cube(leg);


	//define body
	body = module_create();
	module_color(body, &Red);
	module_scale(body, 1.5, 1.25, .5);
	cube(body);


	//define head
	head = module_create();
	module_color(head, &Yellow);
	module_scale(head, 1.25, 1.25, 1.25);
	cylinder(head, 10);

	//define underpants
	underpants = module_create();
	module_color(underpants, &Red);
	module_scale(underpants, 1.5, .25, .5);
	cube(underpants);
	module_identity(underpants);
	module_translate(underpants, 0, -.5, 0);
	module_scale(underpants, .25, .5, .5);
	cube(underpants);

	//define top: head, body, 2 arms
	top = module_create();
	module_translate(top, 2, 0, 0);
	module_module(top, arm);
	module_identity(top);
	module_module(top, body);
	module_translate(top, -2, 0, 0);
	module_module(top, arm);
	module_identity(top);
	module_translate(top, 0, 1.35, 0);
	module_module(top, head);

	//define bottom: legs
	bottom = module_create();
	module_translate(bottom, .9, 0, 0);
	module_module(bottom, leg);
	module_identity(bottom);
	module_translate(bottom, 0, 1.25, 0);
	module_module(bottom, underpants);
	module_identity(bottom);
	module_translate(bottom, -.9, 0, 0);
	module_module(bottom, leg);

	//define man: top and bottom
	man = module_create();
	module_translate(man, 0, -2.75, 0);
	module_module(man, bottom);
	module_identity(man);
	module_module(man, top);

	//define formation: 3 men
	formation = module_create();
	module_translate(formation, 3, -2, -3);
	module_module(formation, man);
	module_identity(formation);
	module_translate(formation, 2, 8, -6);
	module_scale(formation, .5, .5, .5);
	module_module(formation, man);
	module_identity(formation);
	module_translate(formation, 0, 0, 2);
	module_scale(formation, 1.5, 1.5, 1.5);
	module_module(formation, man);

	//define scene: three formations
	scene = module_create();
	module_rotateZ(scene, cos(0), -sin(.5));
	module_translate(scene, -10, 10, -2);
	module_scale(scene, .5, .5, .5);
	module_module(scene, formation);
	module_identity(scene);
	module_rotateZ(scene, cos(0), sin(.75));
	module_translate(scene, 2, 0, 0);
	module_module(scene, formation);
	module_identity(scene);
	module_rotateZ(scene, cos(0), sin(.25));
	module_translate(scene, -15, -15, 0);
	module_scale(scene, .25, .25, .25);
	module_module(scene, formation);
	

	    // create the image and drawstate
    src = image_create(rows, cols);
	image_fillrgb(src,.69, .949, .941); 
    ds = drawstate_create();
    ds->shade = ShadeConstant;
	printf("created images and draw state\n");
    // draw into the scene
    module_draw( scene, &vtm, &gtm, ds, NULL, src );
	printf("Mountains are drawn\n");
	// write out the scene
    image_write( src, "../images/legoManScene.ppm" );
	printf("image is written\n");

	// free the modules
	module_delete(man);
	printf("module lego man has been deleted\n");

	// free the drawstate
    free(ds);

	// free the image
    image_free( src );
	printf("images and draw state freed\n");

	return(0);
}
