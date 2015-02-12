/*
 *Tyler Harley
 *ellipsetest.c
 *Simple test program that draws an ellipse
 */

#include <stdio.h>
#include <stdlib.h>
#include "color.h"
#include "image.h"
#include "graphics.h"

int main(){
    Color blue;
    Image *src;
    Point center;
    double radiusX = 50;
    double radiusY = 75;
    Ellipse ellipse;

    color_set(&blue, 0.0, 0.0, 1.0);
    src = image_create(500, 500);
    image_fillrgb(src, 1.0, 1.0, 1.0);

    point_set2D(&center, 290, 140);
    ellipse_set(&ellipse, center, radiusX, radiusY);

    ellipse_drawFill(&ellipse, src, blue);

    image_write(src, "../images/ellipse.ppm");

    image_free(src);
    return(0);
}
