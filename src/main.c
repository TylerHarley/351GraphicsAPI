#include <stdio.h>
#include <stdlib.h>
#include "color.h"
#include "image.h"
#include "graphics.h"

int main(int argc, char *argv[]) {
	Image *src;
	Line l;
	double x0=50, y0=50, x1=60, y1=55;
	Color White;
	
	if(argc >= 5){
		x0 = atof(argv[1]);
		y0 = atof(argv[2]);
		x1 = atof(argv[3]);
		y1 = atof(argv[4]);
	}

	src = image_create(100, 100);

	printf("Drawing line (%d, %d) to (%d, %d)\n", (int)x0, (int)y0, (int)x1, (int)y1);
	line_set2D(&l, x0, y0, x1, y1 );

	color_set(&White, 1.0, 1.0, 1.0);

	line_draw(&l, src, White);

	image_write(src, "testline.ppm");

	image_free(src);
}

