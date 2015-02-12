/*
 * Tyler Harley & Margaux LeBlanc
 * This is a library for managing and drawing graphics primitives
 * 9/25/14
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <color.h>
#include <list.h>
#include <image.h>
#include "color.h"
#include "graphics.h"

/**********POINTS**********/

/*sets the first two values of p to x and y*/
void point_set2D(Point *p, double x, double y){
	p->val[0] = x;
	p->val[1] = y;
	p->val[2] = 1.0;
	p->val[3] = 1.0;
}

/*sets the first three values of p to x, y, and z*/
void point_set3D(Point *p, double x, double y, double z){
	p->val[0] = x;
	p->val[1] = y;
	p->val[2] = z;
	p->val[3] = 1.0;
}

/*sets the values of p to x, y, z, and h*/
void point_set(Point *p, double x, double y, double z, double h){
	p->val[0] = x;
	p->val[1] = y;
	p->val[2] = z;
	p->val[3] = h;
}

void point_average(Point *p, Point *pList, int n){
	int i;
	float x = 0;
	float y = 0;
	float z = 0;
	for(i=0; i<n; i++){
		x+=pList[i].val[0];
		y+=pList[i].val[1];
		z+=pList[i].val[2];
	}
	point_set3D(p, x/n, y/n, z/n);
}

void point_print(Point *p){
	printf("[%f , %f, %f]\n", p->val[0], p->val[1], p->val[2]);
}

/*copies the Point from into Point to*/
void point_copy(Point *to, Point *from){
	to->val[0] = from->val[0];
	to->val[1] = from->val[1];
	to->val[2] = from->val[2];
	to->val[3] = from->val[3];
}

//Normalize the x and y values of a point by it's homogenous coordinate
void point_normalize(Point *p){
	double x = p->val[0];
	double y = p->val[1];
	p->val[0] = x/p->val[3];
	p->val[1] = y/p->val[3];
} 

/*draws p into src with Color c*/ 
void point_draw(Point *p, Image *src, Color c){
	printf("entering point_draw\n");
	float z = p->val[2];
	FPixel *cur = image_getf(src, (int)p->val[1], (int)p->val[0]);
	if(cur && 1.0/z > cur->z){
		src->data[(int)p->val[1]][(int)p->val[0]].rgb[0] = c.c[0];
		src->data[(int)p->val[1]][(int)p->val[0]].rgb[1] = c.c[1];
		src->data[(int)p->val[1]][(int)p->val[0]].rgb[2] = c.c[2];
		image_setz(src, (int)p->val[1], (int)p->val[0], 1.0/z);
	}
	printf("entering point_draw\n");
}

/*draws p into src through FPixel c*/
void point_drawf(Point *p, Image *src, FPixel c){
	src->data[(int)p->val[1]][(int)p->val[0]] = c;
}

/**********LINES**********/

/*initializes a 2d line*/
void line_set2D(Line *l, double x0, double y0, double x1, double y1){
	Point a = {{x0,y0,0,1}};
	Point b = {{x1,y1,0,1}};
	l-> a = a;
	l-> b = b;
	l-> zBuffer = 1;
	//printf("in line_set2D points are: %f, %f, %f, %f\n",x0, y0, x1, y1);
}
/*initialize a line to ta and tb*/
void line_set(Line *l, Point ta, Point tb){
	l-> a = ta;
	l-> b = tb;
	l-> zBuffer = 1;
}
/*set the zBuffer flag to the given value*/
void line_zBuffer(Line *l, int flag){
	l-> zBuffer = flag;
}
/*copy the line data structure*/
void line_copy(Line *to, Line *from){
	to-> a = from-> a;
	to-> b = from-> b;
	to-> zBuffer = from-> zBuffer;
}

//normalize line by homogenous coordinate
void line_normalize(Line *l){
	double x, y, x2, y2;
	//divide the x and y components of the points by homogenous coordinate
	x = l->a.val[0];
	y = l->a.val[1];
	x2 = l->b.val[0];
	y2 = l->b.val[1];
	l->a.val[0] = x/l->a.val[3];
	l->a.val[1] = y/l->a.val[3];
	l->b.val[0] = x2/l->b.val[3];
	l->b.val[1] = y2/l->b.val[3];
}

/*draw the line into src using color c*/
void line_draw(Line *l, Image *src, Color c, int zBufferFlag){
	//bresenham's
	int startx, starty, dx, dy, updatedx, updatedy, tempdx, i, stepx;
	int e;
	float inverseZ, dInverseZ, zDifference;
	FPixel *curPix;
    
	//l->b.val[0] is x1 and l->a.val[0] = x0
	dx = (int)(l->b.val[0]+.5) - (int)(l->a.val[0]+.5);
	dy = (int)(l->b.val[1]+.5) - (int)(l->a.val[1]+.5);
	updatedx = dx;
	updatedy = dy;
	startx = (int)(l->a.val[0]+.5); //x0
	starty = (int)(l->a.val[1]+.5); //y0
	stepx = 1;
	e = 3*dy - 2*dx;

  	zDifference = 1.0/l->b.val[2] - 1.0/l->a.val[2];
	inverseZ = 1.0/l->a.val[2];

	// check for swapping endpoints
	if (dy < 0){
		dx = (int)(l->a.val[0]+.5) - (int)(l->b.val[0]+.5);
		dy = (int)(l->a.val[1]+.5) - (int)(l->b.val[1]+.5);
		updatedx = dx;
		updatedy = dy;
		// these duplicating shifting below

		startx = (int)(l->b.val[0]+.5); //x1
		starty = (int)(l->b.val[1]+.5); //y1

		e = 3*dy - 2*dx;
		
		inverseZ = 1.0/l->b.val[2];
	}
	
	// handle vertical lines 
	if (dx == 0){
		dInverseZ = zDifference/dy;
		for(i=0; i<dy+1; i++){
			image_setColor(src, starty, startx, c);
			starty++;
		}
	}
    
	// handle horizontal lines
	else if(dy == 0){
		dInverseZ = zDifference/dx;
		for(i=0; i<abs(dx); i++){
			image_setColor(src, starty, startx, c);
			if(dx >0){
				startx+=stepx;
			}
			else{
				startx-=stepx;
			}
		}
	}
	//NOT Vertical or Horizontal
	else {
		if (dx > 0 ){
			if(dx < dy){ // 2nd octant           
				tempdx = updatedx; // swap dx and dy for update
				updatedx = updatedy;
				updatedy = tempdx;
				e = 3*dx - 2*dy;
				
				dInverseZ = zDifference/dy;
			}
			else{
				// calculate delta-inverseZ
				dInverseZ = zDifference/dx;
			}
		}
		else if(dx < 0){
			stepx = -1;
			startx--; //starts at x0-1, y0
			if(-dx < dy){ // 3rd octant
				tempdx = -updatedx; // swap dx and dy for update
				updatedx = updatedy;
				updatedy = tempdx;
				e = -3*dx - 2*dy;
				
				dInverseZ = zDifference/dy;  
			}
			else{ //4th octant
				updatedx = -updatedx;
				e = 3*dy + 2*dx;
				
				// calculate delta-inverseZ
				dInverseZ = zDifference/dx;
			}
		}
		int range = labs(dx) > dy ? labs(dx) : dy;
		for(i=0; i<range; i++){
			
			// test if current 1/z value is greater than the current zBuffer
			curPix = image_getf(src, starty, startx);

			if(curPix && (inverseZ > curPix->z || zBufferFlag == 0)){
				image_setColor(src, starty, startx, c); // draw the pixel
				image_setz(src, starty, startx, inverseZ); // update zBuffer
			}

			// update inverseZ
			inverseZ+= dInverseZ;
			
			if(e>0){
				if((dx>0 && dx >= dy) || (dx<0 && dy <= -dx)){ // 1st or 4th octant
					starty++;
				}
				else{ // 2nd or 3rd octant
					startx+= stepx;
				}
				e = e-2*updatedx;
			}
			if((dx>0 && dx >= dy) || (dx<0 && dy <= -dx)){ // 1st or 4th octant
				startx+= stepx;
			 }
			else{ // 2nd or 3rd octant
				starty++;
			}
			e = e + 2*updatedy;
		}
	}
}

/*draw a dashed line into src using color c*/
void line_draw_dash(Line *l, Image *src, Color c){
	//bresenham's
	int startx, starty, dx, dy, updatedx, updatedy, tempdx, i, stepx;
	int e;
    
	//l->b.val[0] is x1 and l->a.val[0] = x0
	dx = (int)(l->b.val[0]+.5) - (int)(l->a.val[0]+.5);
	dy = (int)(l->b.val[1]+.5) - (int)(l->a.val[1]+.5);
	updatedx = dx;
	updatedy = dy;
	// This if statement ends up duplicating a shift you do later

	startx = (int)(l->a.val[0]+.5); //x0
	starty = (int)(l->a.val[1]+.5); //y0
	stepx = 1;

	e = 3*dy - 2*dx;

	// check for swapping endpoints
	if (dy < 0){
		dx = (int)(l->a.val[0]+.5) - (int)(l->b.val[0]+.5);
		dy = (int)(l->a.val[1]+.5) - (int)(l->b.val[1]+.5);
		updatedx = dx;
		updatedy = dy;

		startx = (int)(l->b.val[0]+.5); //x1
		starty = (int)(l->b.val[1]+.5); //y1

		e = 3*dy - 2*dx;
	}

	//printf("dx and dy are: %f, %f\n", dx, dy);
	// handle vertical lines 
	if (dx == 0){
		for(i=0; i<dy+1; i++){
			if(i%5 == 0){
				image_setColor(src, starty, startx, c);
			}
			starty++;
			printf("Vertical Line\n");
		}
	}
    
	// handle horizontal lines
	else if(dy == 0){
		for(i=0; i<abs(dx); i++){
			if(i%5 == 0){
				image_setColor(src, starty, startx, c);
			}
			if(dx >0){
				startx+=stepx;
			}
			else{
				startx-=stepx;
			}
			printf("Horizontal Lines\n");
		}
	}
	//NOT Vertical or Horizontal
	else {
		if (dx > 0 ){
			if(dx < dy){ // 2nd octant           
				tempdx = updatedx; // swap dx and dy for update
				updatedx = updatedy;
				updatedy = tempdx;
				e = 3*dx - 2*dy;
				printf("in the SECOND octant\n");
			}
		}
		else if(dx < 0){
			stepx = -1;
			startx--;
			if(-dx < dy){ // 3rd octant
				tempdx = -updatedx; // swap dx and dy for update
				updatedx = updatedy;
				updatedy = tempdx;
				e = -3*dx - 2*dy;
				printf("In the THIRD octant\n");	    
			}
			else{ //4th octant
				updatedx = -updatedx;
				e = 3*dy + 2*dx;
				printf("in the Fourth octant\n");
			}
		}
		int range = labs(dx) > dy ? labs(dx) : dy;
		for(i=0; i<range; i++){
			if(i%5 == 0){
				image_setColor(src, starty, startx, c);
			}
			if(e>0){
				if((dx>0 && dx >= dy) || (dx<0 && dy <= -dx)){ // 1st or 4th octant
					starty++;
				}
				else{ // 2nd or 3rd octant
					startx+= stepx;
				}
				e = e-2*updatedx;
			}
			if((dx>0 && dx >= dy) || (dx<0 && dy <= -dx)){ // 1st or 4th octant
				startx+= stepx;
			 }
			else{ // 2nd or 3rd octant
				starty++;
			}
			e = e + 2*updatedy;
		}
	}
}


/**********POLYLINES**********/
/*returns an allocated Polyline pointer initialized so that num Vertex is
	0 and vertex is NULL*/
Polyline *polyline_create(void){
	Polyline *poly = NULL;
	poly = malloc(sizeof(Polyline));
	//check to make sure poly allocated
	if(!poly){
		fprintf(stderr, "Polyline unable to be allocated\n");
		return(NULL);
	}
	//initialize polyline 
	poly->zBuffer = 0;
	poly->numVertex = 0;
	poly->vertex=NULL;
	return(poly);
}

/*returns an allocated Polyline pointer with vertex list initialized
	to the points in vlist*/
Polyline *polyline_createp(int numV, Point *vlist){
	Polyline *poly = NULL;
	int i;
	poly = malloc(sizeof(Polyline));
	//check to make sure poly allocated
	if(!poly){
		fprintf(stderr, "Polyline unable to be allocated\n");
		return(NULL);
	}
	//copy vlist to vertex data
	poly -> vertex = malloc(sizeof(Point)*numV);
	for( i = 0; i<numV; i++){
		poly->vertex[i] = vlist[i];
	}
	//initialize polyline 
	poly->zBuffer = 0;
	poly->numVertex = numV;
	return(poly);
}

/*frees the internal data and the Polyline Pointer*/
void polyline_free(Polyline *p){
	if(p){
		if(p->vertex){
			free(p->vertex);
		}
		free(p);
	}
}

/*initializes the pre-existing Polyline to an empty Polyline structure*/
void polyline_init(Polyline *p){
	//initialize polyline 
	p->zBuffer = 0;
	p->numVertex = 0;
	p->vertex = NULL;
}

/*initializes the vertex list to the points in vlist.
	De allocates/allocates the vertex list for p, as necessary*/
void polyline_set(Polyline *p, int numV, Point *vlist){
	int i;
	//check if p is there and vertex already has data
	if(p != NULL){
		if(p->vertex){
			//clear previous vertex list. Keep polyline structure
			free(p->vertex);
		}
		p->numVertex = numV;
		//copy vlist into vertex data
		p -> vertex = malloc(sizeof(Point)*numV);
		for( i = 0; i<numV; i++){
			p->vertex[i] = vlist[i];
		}	
	}
	else{
		return;
	}
}

/*Frees the internal data for Polyline, if necessary, and sets
	numVertex to 0 and vertex to NULL*/
void polyline_clear(Polyline *p){
	//check if p is there and vertex already has data
	if(p != NULL){
		if(p->vertex){
			//clear previous vertex list. Keep polyline structure
			free(p->vertex);
		}	
		p->numVertex = 0;
		p->vertex = NULL;
	}
	else{
		return;
	}
}

/*sets the z buffer flag to the given value*/
void polyline_zBuffer(Polyline *p, int flag){
	p->zBuffer = flag;
}

/*de allocates/allocates space as necessary in the destination
	Polyline data structure and copies the vertex data from the source
	polyline (from) to the destination (to)*/
void polyline_copy(Polyline *to, Polyline *from){
	int i;
	//check if to exists and if so check vertex list
	if(to != NULL){
		if(to->vertex){
			//free vertex list if it's there.
			free(to->vertex);
		}
		//malloc size of from vertex list so it will fit in to
		to->vertex = malloc(sizeof(Point)*from->numVertex);
		for(i=0; i<from->numVertex; i++){
			//copy
			to->vertex[i] = from->vertex[i];
		}
		to->numVertex = from->numVertex;
		to->zBuffer = from->zBuffer;
	}
	else{
		return;
	}
}

/*prints the polyline data to the stream designated 
	by the FILE pointer*/
void polyline_print(Polyline *p, FILE *fp){
	//print out vertices 1,2,3 \n 1,2,3. loop
	int i;	
	printf("file: %p\n", fp);
	for( i = 0; i< (p->numVertex); i++){
		//Trying to write to a file
		fwrite(&p->vertex[i], sizeof(Point), p->numVertex, fp);
		printf("vertex %i x,y: %f , %f\n", i, p->vertex[i].val[0], p->vertex[i].val[1]);
	}
}

//Normalizes the x and y values of each vertex by the homogenous coordinate
void polyline_normalize(Polyline *p){
	int i;
	double x, y;
	for(i=0; i<p->numVertex; i++){
		//normalize
		x = p->vertex[i].val[0];
		y = p->vertex[i].val[1];
		p->vertex[i].val[0] = x/p->vertex[i].val[3];
		p->vertex[i].val[1] = y/p->vertex[i].val[3];
	}
}

/*draw the polyline using color c*/
void polyline_draw(Polyline *p, Image *src, Color c, int zBufferFlag){
	int i;
	//to draw polyline need to connect vertices using lines
	//loop through vertices and draw a line element through each
	//1->2, 2->3, 3->4, ect
	for(i=0; i<p->numVertex-1; i++){
		Line l;		
		line_set(&(l), p->vertex[i], p->vertex[i+1]);
		line_draw(&(l), src, c, zBufferFlag);
		
	}
}

/**********CIRCLES**********/
/*initialize to center tc and radius tr*/
void circle_set(Circle *c, Point tc, double tr){
    c->r = tr;
    c->c = tc;
}

/*draw the circle into src using color p*/
void circle_draw(Circle *c, Image *src, Color p){
    int x = -1;
    int y = -1 * (int)c->r;
    int u = (int)(1 - c->r);
    int cx = c->c.val[0];
    int cy = c->c.val[1];

    // plot first point and its reflections
    image_setColor(src, cy+x, cx+y, p);
    image_setColor(src, cy-x-1, cx+y, p);
    image_setColor(src, cy+x, cx-y-1, p);
    image_setColor(src, cy-x-1, cx-y-1, p);
    image_setColor(src, cy+y, cx+x, p);
    image_setColor(src, cy-y-1, cx+x, p);
    image_setColor(src, cy+y, cx-x-1, p);
    image_setColor(src, cy-y-1, cx-x-1, p);

    while(x>y){
        x--;
        if(u<0){ // if inside circle
            u += 1-2*x;
        }
        else{
            y++;
            u += 1-2*(x - y);
        }
	// plot next point and its reflections
        image_setColor(src, cy+x, cx+y, p);
        image_setColor(src, cy-x-1, cx+y, p);
        image_setColor(src, cy+x, cx-y-1, p);
        image_setColor(src, cy-x-1, cx-y-1, p);
        image_setColor(src, cy+y, cx+x, p);
        image_setColor(src, cy-y-1, cx+x, p);
        image_setColor(src, cy+y, cx-x-1, p);
        image_setColor(src, cy-y-1, cx-x-1, p);
    }
}

/*draw a filled circle into src using color p*/
void circle_drawFill(Circle *c, Image *src, Color p){
    int x = -1;
    int y = -1 * (int)c->r;
    int i;
    int u = (int)(1 - c->r);
    int cx = c->c.val[0];
    int cy = c->c.val[1];

    // plot first point and its reflections
    // color across each row
    image_setColor(src, cy+x, cx+y, p);
    for(i=cx+y+1; i<cx-y; i++){
        image_setColor(src, cy+x, i, p);
    }
    image_setColor(src, cy+y, cx+x, p);
    for(i=cx+x+1; i<cx-x; i++){
        image_setColor(src, cy+y, i, p);
    }
    image_setColor(src, cy-x-1, cx+y, p);
    for(i=cx+y+1; i<cx-y; i++){
        image_setColor(src, cy-x-1, i, p);
    }
    image_setColor(src, cy-y-1, cx+x, p);
    for(i=cx+x+1; i<cx-x; i++){
        image_setColor(src, cy-y-1, i, p);
    }

    while(x>y){
        x--;
        if(u<0){ // if inside circle
            u += 1-2*x;
        }
        else{
            y++;
            u += 1-2*(x - y);
        }
        
        // plot next point and its reflections
        // color across each row
        image_setColor(src, cy+x, cx+y, p);
        for(i=cx+y+1; i<cx-y; i++){
            image_setColor(src, cy+x, i, p);
        }
        image_setColor(src, cy+y, cx+x, p);
        for(i=cx+x+1; i<cx-x; i++){
            image_setColor(src, cy+y, i, p);
        }
        image_setColor(src, cy-x-1, cx+y, p);
        for(i=cx+y+1; i<cx-y; i++){
            image_setColor(src, cy-x-1, i, p);
        }
        image_setColor(src, cy-y-1, cx+x, p);
        for(i=cx+x+1; i<cx-x; i++){
            image_setColor(src, cy-y-1, i, p);
        }
    }
}

//Sets up an arc that goes on for 2 octants
//Takes in a circle c, center point tc, and radius tr
void arc_set(Circle *c, Point tc, double tr){
    c->r = tr;
    c->c = tc;
}

//draws an arc that goes on for 2 octants in Image src, colored Color p,
//using circle c
void arc_draw(Circle *c, Image *src, Color p){
   int x = -1;
    int y = -1 * (int)c->r;
    int u = (int)(1 - c->r);
    int cx = c->c.val[0];
    int cy = c->c.val[1];

    // plot first point and its reflections
    image_setColor(src, cy-y-1, cx+x, p);
    image_setColor(src, cy-y-1, cx-x-1, p);

    while(x>y){
        x--;
        if(u<0){ // if inside circle
            u += 1-2*x;
        }
        else{
            y++;
            u += 1-2*(x - y);
        }

	// plot next point and its reflections
        image_setColor(src, cy-y-1, cx+x, p);
        image_setColor(src, cy-y-1, cx-x-1, p);
    }

}

/**********ELLIPSES**********/

/*initialize an ellipse to location tc and radii ta and tb*/
void ellipse_set(Ellipse *e, Point tc, double ta, double tb){
    e->ra = ta;
    e->rb = tb;
    e->c = tc;
}

/*draw into src using color p*/
void ellipse_draw(Ellipse *e, Image *src, Color p){
    double rx = e->ra;
    double ry = e->rb;
    int x = -1;
    int y = -1 * (int)ry;
    int ux = (int)(2*ry*ry);
    int uy = (int)(2*rx*rx*-y);
    int u = (int)(ry*ry - rx*rx*ry + rx*rx/4 + ry*ry + ux);
    int cx = e->c.val[0];
    int cy = e->c.val[1];

    // plot first point and its reflections
    image_setColor(src, cy+y, cx+x, p);
    image_setColor(src, cy-y-1, cx+x, p);
    image_setColor(src, cy+y, cx-x-1, p);
    image_setColor(src, cy-y-1, cx-x-1, p);
    
    while(ux < uy){ // while stepping across x
        x--;
        ux += 2*ry*ry;
        if(u < 0){
            u += ry*ry + ux;
        }
        else{
            y++;
            uy -= 2*rx*rx;
            u += ry*ry + ux - uy;
        }

        // plot next point and its reflections
        image_setColor(src, cy+y, cx+x, p);
        image_setColor(src, cy-y-1, cx+x, p);
        image_setColor(src, cy+y, cx-x-1, p);
        image_setColor(src, cy-y-1, cx-x-1, p);
    }

    // switch from stepping in x to stepping in y
    u = (int)((ry*ry) * (x*x + x) + (rx*rx) * (y*y -2*y + 1) - (rx*rx) * ry*ry + rx*rx - uy);
    
    while(y < 0){ // while stepping across y
        y++;
        uy -= 2*rx*rx;
        if(u > 0){
            u += rx*rx - uy;
        }
        else{
            x--;
            ux += 2*ry*ry;
            u += rx*rx - uy + ux;
        }

        // plot next point and its reflections
        image_setColor(src, cy+y, cx+x, p);
        image_setColor(src, cy-y-1, cx+x, p);
        image_setColor(src, cy+y, cx-x-1, p);
        image_setColor(src, cy-y-1, cx-x-1, p);
    } 
}

/*draw a filled ellipse into src using color p*/
void ellipse_drawFill(Ellipse *e, Image *src, Color p){
    double rx = e->ra;
    double ry = e->rb;
    int i;
    int x = -1;
    int y = -1 * (int)ry;
    int ux = (int)(2*ry*ry);
    int uy = (int)(2*rx*rx*-y);
    int u = (int)(ry*ry - rx*rx*ry + rx*rx/4 + ry*ry + ux);
    int cx = e->c.val[0];
    int cy = e->c.val[1];

    // plot first point and its reflections
    image_setColor(src, cy+y, cx+x, p);
    for(i=cx+x+1; i<cx-x; i++){
        image_setColor(src, cy+y, i, p);
    }
    image_setColor(src, cy-y-1, cx+x, p);
    for(i=cx+x+1; i<cx-x; i++){
        image_setColor(src, cy-y-1, i, p);
    }
    
    while(ux < uy){ // while stepping across x
        x--;
        ux += 2*ry*ry;
        if(u < 0){
            u += ry*ry + ux;
        }
        else{
            y++;
            uy -= 2*rx*rx;
            u += ry*ry + ux - uy;
        }

        // plot next point and its reflections
	
       image_setColor(src, cy+y, cx+x, p);
       for(i=cx+x+1; i<cx-x; i++){
           image_setColor(src, cy+y, i, p);
       }
       image_setColor(src, cy-y-1, cx+x, p);
       for(i=cx+x+1; i<cx-x; i++){
           image_setColor(src, cy-y-1, i, p);
       }
    }

    // switch from stepping in x to stepping in y
    u = (int)((ry*ry) * (x*x + x) + (rx*rx) * (y*y -2*y + 1) - (rx*rx) * ry*ry + rx*rx - uy);
    
    while(y < 0){ // while stepping across y
        y++;
        uy -= 2*rx*rx;
        if(u > 0){
            u += rx*rx - uy;
        }
        else{
            x--;
            ux += 2*ry*ry;
            u += rx*rx - uy + ux;
        }
	printf("filling\n");
        image_setColor(src, cy+y, cx+x, p);
        for(i=cx+x+1; i<cx-x; i++){
            image_setColor(src, cy+y, i, p);
        } 
        image_setColor(src, cy-y-1, cx+x, p);
        for(i=cx+x+1; i<cx-x; i++){
            image_setColor(src, cy-y-1, i, p);
        }
    }
}

void half_ellipse_draw(Ellipse *e, Image *src, Color p){
    double rx = e->ra;
    double ry = e->rb;
    int x = -1;
    int y = -1 * (int)ry;
    int ux = (int)(2*ry*ry);
    int uy = (int)(2*rx*rx*-y);
    int u = (int)(ry*ry - rx*rx*ry + rx*rx/4 + ry*ry + ux);
    int cx = e->c.val[0];
    int cy = e->c.val[1];

    // plot first point 
    image_setColor(src, cy-y-1, cx+x, p);
    image_setColor(src, cy-y-1, cx-x-1, p);
    
    while(ux < uy){ // while stepping across x
        x--;
        ux += 2*ry*ry;
        if(u < 0){
            u += ry*ry + ux;
        }
        else{
            y++;
            uy -= 2*rx*rx;
            u += ry*ry + ux - uy;
        }

        // plot next point 
        image_setColor(src, cy-y-1, cx+x, p);
        image_setColor(src, cy-y-1, cx-x-1, p);
    }

    // switch from stepping in x to stepping in y
    u = (int)((ry*ry) * (x*x + x) + (rx*rx) * (y*y -2*y + 1) - (rx*rx) * ry*ry + rx*rx - uy);
    
    while(y < 0){ // while stepping across y
        y++;
        uy -= 2*rx*rx;
        if(u > 0){
            u += rx*rx - uy;
        }
        else{
            x--;
            ux += 2*ry*ry;
            u += rx*rx - uy + ux;
        }

        // plot curve
        image_setColor(src, cy-y-1, cx+x, p);
        image_setColor(src, cy-y-1, cx-x-1, p);
    } 
}


	

