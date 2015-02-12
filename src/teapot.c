/*
Margaux LeBlanc
Graphics 
11/10/14
This file creates the Utah Teapot!
*/

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
#include "bezier.h"

int main(int argc, char *argv[]) {
	Point p[118];
	Point rim[16], body1[16], body2[16], lid1[16], lid2[16], handle1[16];
	Point handle2[16], spout1[16], spout2[16];
	BezierSurface bc;
	DrawState ds;
	Module *curve;
	View3D view;
	Matrix VTM, GTM;
	int divisions = 2;
	int rows = 300, cols = 400, frame;
	Color green;

	color_set(&green, .043, .917, .090 );
	Image *src = image_create(rows, cols);

	curve = module_create();

	point_set3D(&p[0], 0.2000,  0.0000, 2.70000); 
	point_set3D(&p[1], 0.2000, -0.1120, 2.70000);
	point_set3D(&p[2], 0.1120, -0.2000, 2.70000);
	point_set3D(&p[3], 0.0000, -0.2000, 2.70000);
	point_set3D(&p[4], 1.3375,  0.0000, 2.53125 ); 
	point_set3D(&p[5], 1.3375, -0.7490, 2.53125);
	point_set3D(&p[6], 0.7490, -1.3375, 2.53125);
	point_set3D(&p[7], 0.0000, -1.3375, 2.53125);
	point_set3D(&p[8], 1.4375,  0.0000, 2.53125 ); 
	point_set3D(&p[9], 1.4375, -0.8050, 2.53125);
	point_set3D(&p[10], 0.8050, -1.4375, 2.53125 );
	point_set3D(&p[11], 0.0000, -1.4375, 2.53125 );
	point_set3D(&p[12], 1.5000,  0.0000, 2.40000); 
	point_set3D(&p[13], 1.5000, -0.8400, 2.40000);
	point_set3D(&p[14], 0.8400, -1.5000, 2.40000 );
	point_set3D(&p[15], 0.0000, -1.5000, 2.40000);
	point_set3D(&p[16], 1.7500,  0.0000, 1.87500); 
	point_set3D(&p[17], 1.7500, -0.9800, 1.87500 );
	point_set3D(&p[18], 0.9800, -1.7500, 1.87500 );
	point_set3D(&p[19], 0.0000, -1.7500, 1.87500);
	point_set3D(&p[20], 2.0000,  0.0000, 1.35000); 
	point_set3D(&p[21], 2.0000, -1.1200, 1.35000);
	point_set3D(&p[22], 1.1200, -2.0000, 1.35000);
	point_set3D(&p[23], 0.0000, -2.0000, 1.35000 );
	point_set3D(&p[24], 2.0000,  0.0000, 0.90000); 
	point_set3D(&p[25], 2.0000, -1.1200, 0.90000);
	point_set3D(&p[26], 1.1200, -2.0000, 0.90000 );
	point_set3D(&p[27], 0.0000, -2.0000, 0.90000 );
	point_set3D(&p[28], -2.0000,  0.0000, 0.90000 ); 
	point_set3D(&p[29], 2.0000,  0.0000, 0.45000);
	point_set3D(&p[30], 2.0000, -1.1200, 0.45000);
	point_set3D(&p[31], 1.1200, -2.0000, 0.45000);
	point_set3D(&p[32], 0.0000, -2.0000, 0.45000 ); 
	point_set3D(&p[33], 1.5000,  0.0000, 0.22500);
	point_set3D(&p[34], 1.5000, -0.8400, 0.22500);
	point_set3D(&p[35], 0.8400, -1.5000, 0.22500);
	point_set3D(&p[36], 0.0000, -1.5000, 0.22500); 
	point_set3D(&p[37], 1.5000,  0.0000, 0.15000);
	point_set3D(&p[38], 1.5000, -0.8400, 0.15000 );
	point_set3D(&p[39], 0.8400, -1.5000, 0.15000);
	point_set3D(&p[40], 0.0000, -1.5000, 0.15000 ); 
	point_set3D(&p[41], -1.6000,  0.0000, 2.02500 );
	point_set3D(&p[42], -1.6000, -0.3000, 2.02500 );
	point_set3D(&p[43], -1.5000, -0.3000, 2.25000 );
	point_set3D(&p[44], -1.5000,  0.0000, 2.25000); 
	point_set3D(&p[45], -2.3000,  0.0000, 2.02500);
	point_set3D(&p[46], -2.3000, -0.3000, 2.02500  );
	point_set3D(&p[47], -2.5000, -0.3000, 2.25000);
	point_set3D(&p[48], -2.5000,  0.0000, 2.25000); 
	point_set3D(&p[49], -2.7000,  0.0000, 2.02500 );
	point_set3D(&p[50], -2.7000, -0.3000, 2.02500 );
	point_set3D(&p[51], -3.0000, -0.3000, 2.25000);
	point_set3D(&p[52], -3.0000,  0.0000, 2.25000  ); 
	point_set3D(&p[53], -2.7000,  0.0000, 1.80000 );
	point_set3D(&p[54], -2.7000, -0.3000, 1.80000);
	point_set3D(&p[55], -3.0000, -0.3000, 1.80000);
	point_set3D(&p[56], -3.0000,  0.0000, 1.80000 ); 
	point_set3D(&p[57], -2.7000,  0.0000, 1.57500 );
	point_set3D(&p[58], -2.7000, -0.3000, 1.57500);
	point_set3D(&p[59], -3.0000, -0.3000, 1.35000 );
	point_set3D(&p[60], -3.0000,  0.0000, 1.35000); 
	point_set3D(&p[61], -2.5000,  0.0000, 1.12500);
	point_set3D(&p[62], -2.5000, -0.3000, 1.12500 );
	point_set3D(&p[63], -2.6500, -0.3000, 0.93750 );
	point_set3D(&p[64], -2.6500,  0.0000, 0.93750 ); 
	point_set3D(&p[65], -2.0000, -0.3000, 0.90000);
	point_set3D(&p[66], -1.9000, -0.3000, 0.60000 );
	point_set3D(&p[67], -1.9000,  0.0000, 0.60000);
	point_set3D(&p[68], 1.7000,  0.0000, 1.42500); 
	point_set3D(&p[69], 1.7000, -0.6600, 1.42500);
	point_set3D(&p[70], 1.7000, -0.6600, 0.60000);
	point_set3D(&p[71], 1.7000,  0.0000, 0.60000);
	point_set3D(&p[72], 2.6000,  0.0000, 1.42500); 
	point_set3D(&p[73], 2.6000, -0.6600, 1.42500);
	point_set3D(&p[74], 3.1000, -0.6600, 0.82500 );
	point_set3D(&p[75], 3.1000,  0.0000, 0.82500);
	point_set3D(&p[76], 2.3000,  0.0000, 2.10000); 
	point_set3D(&p[77], 2.3000, -0.2500, 2.10000 );
	point_set3D(&p[78], 2.4000, -0.2500, 2.02500 );
	point_set3D(&p[79], 2.4000,  0.0000, 2.02500 );
	point_set3D(&p[80], 2.7000,  0.0000, 2.40000); 
	point_set3D(&p[81], 2.7000, -0.2500, 2.40000);
	point_set3D(&p[82], 3.3000, -0.2500, 2.40000);
	point_set3D(&p[83], 3.3000,  0.0000, 2.40000);
	point_set3D(&p[84], 2.8000,  0.0000, 2.47500 ); 
	point_set3D(&p[85], 2.8000, -0.2500, 2.47500 );
	point_set3D(&p[86], 3.5250, -0.2500, 2.49375);
	point_set3D(&p[87], 3.5250,  0.0000, 2.49375);
	point_set3D(&p[88], 2.9000,  0.0000, 2.47500 ); 
	point_set3D(&p[89], 2.9000, -0.1500, 2.47500 );
	point_set3D(&p[90], 3.4500, -0.1500, 2.51250 );
	point_set3D(&p[91], 3.4500,  0.0000, 2.51250  );
	point_set3D(&p[92], 2.8000,  0.0000, 2.40000); 
	point_set3D(&p[93], 2.8000, -0.1500, 2.40000);
	point_set3D(&p[94], 3.2000, -0.1500, 2.40000  );
	point_set3D(&p[95], 3.2000,  0.0000, 2.40000);
	point_set3D(&p[96], 0.0000,  0.0000, 3.15000 );
	point_set3D(&p[97], 0.8000,  0.0000, 3.15000  ); 
	point_set3D(&p[98], 0.8000, -0.4500, 3.15000);
	point_set3D(&p[99],  0.4500, -0.8000, 3.15000 );
	point_set3D(&p[100], 0.0000, -0.8000, 3.15000 );
	point_set3D(&p[101], 0.0000,  0.0000, 2.85000 ); 
	point_set3D(&p[102], 1.4000,  0.0000, 2.40000);
	point_set3D(&p[103], 1.4000, -0.7840, 2.40000);
	point_set3D(&p[104], 0.7840, -1.4000, 2.40000 );
	point_set3D(&p[105], 0.0000, -1.4000, 2.40000 ); 
	point_set3D(&p[106], 0.4000,  0.0000, 2.55000);
	point_set3D(&p[107], 0.4000, -0.2240, 2.55000);
	point_set3D(&p[108], 0.2240, -0.4000, 2.55000 );
	point_set3D(&p[109], 0.0000, -0.4000, 2.55000); 
	point_set3D(&p[110], 1.3000,  0.0000, 2.55000 );
	point_set3D(&p[111], 1.3000, -0.7280, 2.55000);
	point_set3D(&p[112], 0.7280, -1.3000, 2.55000);
	point_set3D(&p[113], 0.0000, -1.3000, 2.55000 ); 
	point_set3D(&p[114], 1.3000,  0.0000, 2.40000);
	point_set3D(&p[115], 1.3000, -0.7280, 2.40000);
	point_set3D(&p[116], 0.7280, -1.3000, 2.40000);
	point_set3D(&p[117], 0.0000, -1.3000, 2.40000); 

	//set color of module
	module_color(curve, &green);

	//create rim
	rim[0] = p[102];
	rim[1] = p[103];
	rim[2] = p[104];
	rim[3] = p[105];
	rim[4] = p[4];
	rim[5] = p[5];
	rim[6] = p[6];
	rim[7] = p[7];
	rim[8] = p[8];
	rim[9] = p[9];
	rim[10] = p[10];
	rim[11] = p[11];
	rim[12] = p[12];
	rim[13] = p[13];
	rim[14] = p[14];
	rim[15] = p[15];
	bezierSurface_set(&bc, rim);
	module_bezierSurface(curve, &bc, divisions, 0);

	//now add body1
	body1[0] = p[12];
	body1[1] = p[13];
	body1[2] = p[14];
	body1[3] = p[15];
	body1[4] = p[16];
	body1[5] = p[17];
	body1[6] = p[18];
	body1[7] = p[19];
	body1[8] = p[20];
	body1[9] = p[21];
	body1[10] = p[22];
	body1[11] = p[23];
	body1[12] = p[24];
	body1[13] = p[25];
	body1[14] = p[26];
	body1[15] = p[27];
	bezierSurface_set(&bc, body1);
	module_bezierSurface(curve, &bc, divisions, 0);

	//now add body2
	body2[0] = p[24];
	body2[1] = p[25];
	body2[2] = p[26];
	body2[3] = p[27];
	body2[4] = p[29];
	body2[5] = p[30];
	body2[6] = p[31];
	body2[7] = p[32];
	body2[8] = p[33];
	body2[9] = p[34];
	body2[10] = p[35];
	body2[11] = p[36];
	body2[12] = p[37];
	body2[13] = p[38];
	body2[14] = p[39];
	body2[15] = p[40];
	bezierSurface_set(&bc, body2);
	module_bezierSurface(curve, &bc, divisions, 0);

	//now add Lid1
	lid1[0] = p[96];
	lid1[1] = p[96];
	lid1[2] = p[96];
	lid1[3] = p[96];
	lid1[4] = p[97];
	lid1[5] = p[98];
	lid1[6] = p[99];
	lid1[7] = p[100];
	lid1[8] = p[101];
	lid1[9] = p[101];
	lid1[10] = p[101];
	lid1[11] = p[101];
	lid1[12] = p[0];
	lid1[13] = p[1];
	lid1[14] = p[2];
	lid1[15] = p[3];
	bezierSurface_set(&bc, lid1);
	module_bezierSurface(curve, &bc, divisions, 0);

	//now add Lid2
	lid2[0] = p[0];
	lid2[1] = p[1];
	lid2[2] = p[2];
	lid2[3] = p[3];
	lid2[4] = p[106];
	lid2[5] = p[107];
	lid2[6] = p[108];
	lid2[7] = p[109];
	lid2[8] = p[110];
	lid2[9] = p[111];
	lid2[10] = p[112];
	lid2[11] = p[113];
	lid2[12] = p[114];
	lid2[13] = p[115];
	lid2[14] = p[116];
	lid2[15] = p[117];
	bezierSurface_set(&bc, lid2);
	module_bezierSurface(curve, &bc, divisions, 0);

	//draw rest of the body by rotating 90 degrees
	module_rotateZ(curve, cos(M_PI/2.0), sin(M_PI/2.0));
	bezierSurface_set(&bc, body1);
	module_bezierSurface(curve, &bc, divisions, 0);
	bezierSurface_set(&bc, body2);
	module_bezierSurface(curve, &bc, divisions, 0);
	bezierSurface_set(&bc, lid1);
	module_bezierSurface(curve, &bc, divisions, 0);
	bezierSurface_set(&bc, lid2);
	module_bezierSurface(curve, &bc, divisions, 0);
	bezierSurface_set(&bc, rim);
	module_bezierSurface(curve, &bc, divisions, 0);

	//draw rest of the body by rotating 180 degrees
	module_rotateZ(curve, cos(M_PI), sin(M_PI));
	bezierSurface_set(&bc, body1);
	module_bezierSurface(curve, &bc, divisions, 0);
	bezierSurface_set(&bc, body2);
	module_bezierSurface(curve, &bc, divisions, 0);
	bezierSurface_set(&bc, lid1);
	module_bezierSurface(curve, &bc, divisions, 0);
	bezierSurface_set(&bc, lid2);
	module_bezierSurface(curve, &bc, divisions, 0);
	bezierSurface_set(&bc, rim);
	module_bezierSurface(curve, &bc, divisions, 0);

	//draw rest of the body by rotating 270 degrees
	module_rotateZ(curve, cos(3*M_PI/2), sin(3*M_PI/2));
	bezierSurface_set(&bc, body1);
	module_bezierSurface(curve, &bc, divisions, 0);
	bezierSurface_set(&bc, body2);
	module_bezierSurface(curve, &bc, divisions, 0);
	bezierSurface_set(&bc, lid1);
	module_bezierSurface(curve, &bc, divisions, 0);
	bezierSurface_set(&bc, lid2);
	module_bezierSurface(curve, &bc, divisions, 0);
	bezierSurface_set(&bc, rim);
	module_bezierSurface(curve, &bc, divisions, 0);

	//now add spout1
	spout1[0] = p[68];
	spout1[1] = p[69];
	spout1[2] = p[70];
	spout1[3] = p[71];
	spout1[4] = p[72];
	spout1[5] = p[73];
	spout1[6] = p[74];
	spout1[7] = p[75];
	spout1[8] = p[76];
	spout1[9] = p[77];
	spout1[10] = p[78];
	spout1[11] = p[79];
	spout1[12] = p[80];
	spout1[13] = p[81];
	spout1[14] = p[82];
	spout1[15] = p[83];
	bezierSurface_set(&bc, spout1);
	module_bezierSurface(curve, &bc, divisions, 0);

	//now add spout2
	spout2[0] = p[80];
	spout2[1] = p[81];
	spout2[2] = p[82];
	spout2[3] = p[83];
	spout2[4] = p[84];
	spout2[5] = p[85];
	spout2[6] = p[86];
	spout2[7] = p[87];
	spout2[8] = p[88];
	spout2[9] = p[89];
	spout2[10] = p[90];
	spout2[11] = p[91];
	spout2[12] = p[92];
	spout2[13] = p[93];
	spout2[14] = p[94];
	spout2[15] = p[95];
	bezierSurface_set(&bc, spout2);
	module_bezierSurface(curve, &bc, divisions, 0);

	//now add handle1
	handle1[0] = p[41];
	handle1[1] = p[42];
	handle1[2] = p[43];
	handle1[3] = p[44];
	handle1[4] = p[45];
	handle1[5] = p[46];
	handle1[6] = p[47];
	handle1[7] = p[48];
	handle1[8] = p[49];
	handle1[9] = p[50];
	handle1[10] = p[51];
	handle1[11] = p[52];
	handle1[12] = p[53];
	handle1[13] = p[54];
	handle1[14] = p[55];
	handle1[15] = p[56];
	bezierSurface_set(&bc, handle1);
	module_bezierSurface(curve, &bc, divisions, 0);

	//now add handle2 There is a wrong point here
	handle2[0] = p[53];
	handle2[1] = p[54];
	handle2[2] = p[55];
	handle2[3] = p[56];
	handle2[4] = p[57];
	handle2[5] = p[58];
	handle2[6] = p[59];
	handle2[7] = p[60];
	handle2[8] = p[61];
	handle2[9] = p[62];
	handle2[10] = p[63];
	handle2[11] = p[64];
	handle2[12] = p[28];
	handle2[13] = p[65];
	handle2[14] = p[66];
	handle2[15] = p[67];
	bezierSurface_set(&bc, handle2);
	module_bezierSurface(curve, &bc, divisions, 0);

	//mirror parts
	module_scale(curve, 1, -1, 1);
	bezierSurface_set(&bc, spout1);
	module_bezierSurface(curve, &bc, divisions, 0);
	bezierSurface_set(&bc, spout2);
	module_bezierSurface(curve, &bc, divisions, 0);
	bezierSurface_set(&bc, handle1);
	module_bezierSurface(curve, &bc, divisions, 0);
	bezierSurface_set(&bc, handle2);
	module_bezierSurface(curve, &bc, divisions, 0);
	

	//set up draw state
	drawstate_setColor(&ds, green);
	ds.shade = ShadeConstant;

	// set up the view
	point_set3D(&(view.vrp), 4.0, 4.0, 5 );
	vector_set( &(view.vpn), -4, -4, -5 );
	vector_set( &(view.vup), 0.0, 0.0, 1.0 );
	view.d = .5;
	view.du = 1.0;
	view.dv = 1.0*rows/cols;
	view.screeny = rows;
	view.screenx = cols;
	view.f = 0.0;
	view.b = 3.0;

	matrix_setView3D( &VTM, &view );
	matrix_identity( &GTM );
	printf("set up camera\n");

	// Create the animation by adjusting the GTM
	for(frame=0;frame<60;frame++) {
		image_fillrgb(src, .074, .094, .286);
		char buffer[256];
		
		matrix_rotateZ(&GTM, cos(M_PI/8), sin(M_PI/8) );
		module_draw( curve, &VTM, &GTM, &ds, NULL, src );

		sprintf(buffer, "../images/teapot/teapot-frame%03d.ppm", frame);
		image_write(src, buffer);
		//printf("wrote image\n");
		image_reset(src);
	}

	// clean up
	image_free( src );

	module_delete( curve );


	return(0);
}
