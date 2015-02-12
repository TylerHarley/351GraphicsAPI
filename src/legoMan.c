//Margaux LeBlanc
//Graphics
//10/4/14
//Creates an image of a legoman that can be placed at any point in an image

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <list.h>
#include "color.h"
#include "image.h"
#include "graphics.h"
#include "polygon.h"


/*
  Program to draw a lego man. 
*/
int main(int argc, char *argv[]) {
  Point center;
  Image *src;
  Polygon *p;
  Color Yellow, lightYellow, Black, Red, Blue, lightBlue;
  Point pt[61];
  Ellipse eye1, eye2;
  Circle mouth;

/*Originally I wanted to have this draw a lego man starting from
an origin passed in by the user. Then as I started to make a lego_man
function in our library I realized that I didn't know what I would return
to the main program. I don't know how to draw a list of polygons (apparently
that is in project 6). So here is my code with the origin concept in hopes
that at one point I will be able to finally make this a function 
in my library and draw lego men wherever I want in an image*/

  //set the origin
  point_set2D(&center, 100, 900);

  //set colors
  color_set(&Red, 0.941, 0.211, 0.078 );
  color_set(&Black, 0.0, 0.0, 0.0);
  color_set(&Blue, .058, .137, .623);
  color_set(&lightBlue, .286, .478, .854);
  color_set(&Yellow, .839, .674, .019);
  color_set(&lightYellow, .882, .803, .215);

  //create image and fill it with lime green cause it's cool
  printf("Creating image\n");
  src = image_create( 1000, 1000 );
  printf("created image\n");
  image_fillrgb(src, .231, .890, .086);

  //Points for left foot
  point_set2D(&pt[1], center.val[0], center.val[1]);
  point_set2D(&pt[2], center.val[0]+150, center.val[1]);
  point_set2D(&pt[3], center.val[0]+150, center.val[1]-50);
  point_set2D(&pt[4], center.val[0], center.val[1]-50);
  point_set2D(&pt[5], center.val[0], center.val[1]);

  p = polygon_createp(6, pt);
  polygon_drawFill(p, src, Blue);

  //points for left leg
  point_set2D(&pt[6], center.val[0], center.val[1]-50);
  point_set2D(&pt[7], center.val[0], center.val[1]-300 );
  point_set2D(&pt[8], center.val[0]+150, center.val[1]-300);
  point_set2D(&pt[9], center.val[0]+150, center.val[1]-50);
  point_set2D(&pt[10], center.val[0], center.val[1]-50);

  polygon_set(p, 6, pt+6);
  polygon_drawFill(p, src, lightBlue);

  //draw underwear
  point_set2D(&pt[11], center.val[0], center.val[1]-300);
  point_set2D(&pt[12], center.val[0], center.val[1]-350);
  point_set2D(&pt[13], center.val[0]+325, center.val[1]-350);
  point_set2D(&pt[14], center.val[0]+325, center.val[1]-300);
  point_set2D(&pt[15], center.val[0]+175, center.val[1]-300);  
  point_set2D(&pt[16], center.val[0]+175, center.val[1]-200);
  point_set2D(&pt[17], center.val[0]+150, center.val[1]-200);
  point_set2D(&pt[18], center.val[0]+150, center.val[1]-300);
  point_set2D(&pt[19], center.val[0], center.val[1]-300);

  polygon_set(p, 10, pt+11);
  polygon_drawFill(p, src, Blue);

  //Points for the right foot
  point_set2D(&pt[20], center.val[0]+175, center.val[1]);
  point_set2D(&pt[21], center.val[0]+325, center.val[1]);
  point_set2D(&pt[22], center.val[0]+325, center.val[1]-50);
  point_set2D(&pt[23], center.val[0]+175, center.val[1]-50);
  point_set2D(&pt[24], center.val[0]+175, center.val[1]);

  polygon_set(p, 6, pt+20);
  polygon_drawFill(p, src, Blue);

  //Points for the right leg
  point_set2D(&pt[25], center.val[0]+175, center.val[1]-50);
  point_set2D(&pt[26], center.val[0]+325, center.val[1]-50);
  point_set2D(&pt[27], center.val[0]+325, center.val[1]-300);
  point_set2D(&pt[28], center.val[0]+175, center.val[1]-300);
  point_set2D(&pt[29], center.val[0]+175, center.val[1]-50);

  polygon_set(p, 6, pt+25);
  polygon_drawFill(p, src, lightBlue);

  //Points for the torso
  point_set2D(&pt[30], center.val[0], center.val[1]-350);
  point_set2D(&pt[31], center.val[0]+325, center.val[1]-350);
  point_set2D(&pt[32], center.val[0]+250, center.val[1]-600);
  point_set2D(&pt[33], center.val[0]+75, center.val[1]-600);
  point_set2D(&pt[34], center.val[0], center.val[1]-350);

  polygon_set(p, 6, pt+30);
  polygon_drawFill(p, src, Red);

  //Points for the left arm
  point_set2D(&pt[35], center.val[0], center.val[1]-350);
  point_set2D(&pt[36], center.val[0], center.val[1]-600);
  point_set2D(&pt[37], center.val[0]+75, center.val[1]-600);
  point_set2D(&pt[38], center.val[0], center.val[1]-350);

  polygon_set(p, 5, pt+35);
  polygon_drawFill(p, src, lightBlue);

  //Points for the right arm
  point_set2D(&pt[39], center.val[0]+325, center.val[1]-350);
  point_set2D(&pt[40], center.val[0]+250, center.val[1]-600);
  point_set2D(&pt[41], center.val[0]+325, center.val[1]-600);
  point_set2D(&pt[42], center.val[0]+325, center.val[1]-350);

  polygon_set(p, 5, pt+39);
  polygon_drawFill(p, src, lightBlue);


  //Points for neck
  point_set2D(&pt[43], center.val[0]+125, center.val[1]-600);
  point_set2D(&pt[44], center.val[0]+200, center.val[1]-600);
  point_set2D(&pt[45], center.val[0]+200, center.val[1]-610);
  point_set2D(&pt[46], center.val[0]+125, center.val[1]-610);
  point_set2D(&pt[47], center.val[0]+125, center.val[1]-600);

  polygon_set(p, 6, pt+43);
  polygon_drawFill(p, src, Yellow);

  //points for head
  point_set2D(&pt[48], center.val[0]+75, center.val[1]-610);
  point_set2D(&pt[49], center.val[0]+75, center.val[1]-775);
  point_set2D(&pt[50], center.val[0]+250, center.val[1]-775);
  point_set2D(&pt[51], center.val[0]+250, center.val[1]-610);
  point_set2D(&pt[52], center.val[0]+75, center.val[1]-610);

  polygon_set(p, 6, pt+48);
  polygon_drawFill(p, src, lightYellow);

  //draw top of head
  point_set2D(&pt[53], center.val[0]+125, center.val[1]-795);
  point_set2D(&pt[54], center.val[0]+125, center.val[1]-775);
  point_set2D(&pt[55], center.val[0]+200, center.val[1]-775);
  point_set2D(&pt[56], center.val[0]+200, center.val[1]-795);
  point_set2D(&pt[57], center.val[0]+125, center.val[1]-795);

  //Draw top of head
  polygon_set(p, 6, pt+53);
  polygon_drawFill(p, src, Yellow);

  //set center points for ellipses 
  point_set2D(&pt[58], center.val[0]+130, center.val[1]-720);
  point_set2D(&pt[59], center.val[0]+190, center.val[1]-720);

  //draw the eyes
  ellipse_set(&eye1, pt[58], 10, 10);
  ellipse_set(&eye2, pt[59], 10, 10);
  ellipse_drawFill(&eye1, src, Black);
  ellipse_drawFill(&eye2, src, Black); 

  //draw the mouth
  point_set2D(&pt[60], center.val[0]+160, center.val[1]-715);
  arc_set(&mouth, pt[60], 65);
  arc_draw(&mouth, src, Black); 
	
  //write the image for lego man
  image_write( src, "legoman.ppm");
  printf("Cleanup\n");
  image_free( src );
  printf("freed image\n");
  return(0);
}
