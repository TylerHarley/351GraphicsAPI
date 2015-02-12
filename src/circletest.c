/*
 *Tyler Harley
 *circletest.c
 *Simple test program that draws a circle
 */

#include <stdio.h>
#include <stdlib.h>
#include "color.h"
#include "image.h"
#include "graphics.h"

int main(){
    Color red;
    Image *src;
    Point center;
    double radius = 100;
    Circle circle;

    color_set(&red, 1.0, 0.0, 0.0);
    src = image_create(500, 500);
    image_fillrgb(src, 1.0, 1.0, 1.0);

    point_set2D(&center, 250, 220);
    circle_set(&circle, center, radius);

    circle_drawFill(&circle, src, red);

    image_write(src, "../images/circle.ppm");

    image_free(src);
    return(0);
}
