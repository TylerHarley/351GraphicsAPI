/*
 * Tyler Harley
 * light.c
 * 11/26/14
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

// initializes the light to default values
void light_init(Light *light){
	if(light){
		light->type = LightNone;
		color_set(&(light->color), 0.0, 0.0, 0.0);
		vector_set(&(light->direction), 0.0, 0.0, 0.0);
		point_set3D(&(light->location), 0.0, 0.0, 0.0);
		light->cutoff = 0.0;
		light->sharpness = 1.0;
	}
}

// copy the light information
void light_copy(Light *to, Light *from){
	if(to && from){
		to->type = from->type;
		to->color = from->color;
		to->direction = from->direction;
		to->location = from->location;
		to->cutoff = from->cutoff;
		to->sharpness = from->sharpness;	
	}
}

// allocate and return a new lighting structure set to default values
Lighting *lighting_create(){
	Lighting *l;
	int i;
	l = malloc(sizeof(Lighting));
	
	if(!l){
		fprintf(stderr, "Lighting unable to be allocated\n");
		return(NULL);
	}
	
	l->nLights = 0;
	for(i=0; i<MAX_LIGHTS; i++){
		light_init(&(l->light[i]));
	}
	return(l);
}

// initializes the lighting structure to default values
void lighting_init(Lighting *l){
	int i;
	l->nLights = 0;
	for(i=0; i<MAX_LIGHTS; i++){
		light_init(&(l->light[i]));
	}
}

// add a new light to the Lighting structure given the parameters,
// some of which may be NULL, depending on the type
void lighting_add(Lighting *l, LightType type, Color *c, Vector *dir, Point *pos, float cutoff, float sharpness){
	if(l->nLights >= MAX_LIGHTS){
		fprintf(stderr, "Maximum lights allowed reached\n");
	}
	else{
		Light light;
		light.type = type;
		if(c){ light.color = *c; }
		if(dir){ light.direction = *dir; }
		if(pos){ light.location = *pos; }
		light.cutoff = cutoff;
		light.sharpness = sharpness;
		l->light[l->nLights] = light;
		l->nLights++;
	}
}

// calculate the proper color given the normal N, view vector V, 3D point P, 
// body color Cb, surface color Cs, sharpness value s, the lighting, and 
// whether the polygon is one-sided or two-sided
void lighting_shading(Lighting *l, Vector *N, Vector *V, Point *p, Color *Cb, Color *Cs, float s, int oneSided, Color *c){
	int i;
	float totalColor[3] = {0.0, 0.0, 0.0};
	Vector H, L;
	double hx, hy, hz;
	double bodyVectors, surfaceVectors, sigma;
	vector_normalize(N);
	
	for(i = 0; i < l->nLights; i++){
		Light *light = &l->light[i];
		switch(light->type){
			case LightNone:
				break;
			case LightAmbient:
				totalColor[0] += light->color.c[0] * Cb->c[0];
				totalColor[1] += light->color.c[1] * Cb->c[1];
				totalColor[2] += light->color.c[2] * Cb->c[2];
				break;
			case LightDirect:
				vector_copy(&L, &(light->direction));
				vector_normalize(V);
				vector_normalize(&L);			

				bodyVectors = vector_dot(&L, N);
				// if polygon is one-sided and light vector and surface normal are not within same hemisphere
				if(oneSided == 1 && bodyVectors < 0){ break; }

				sigma = vector_dot(V, N);
				// if they have different signs, break
				if((bodyVectors < 0 && sigma > 0) || (bodyVectors > 0 && sigma < 0)){ break; }

				// calculate halfway vector
				hx = (L.val[0] + V->val[0]) / 2.0;
				hy = (L.val[1] + V->val[1]) / 2.0;
				hz = (L.val[2] + V->val[2]) / 2.0;
				vector_set(&H, hx, hy, hz);	
				surfaceVectors = vector_dot(&H, N);

				// if normal is pointing the wrong way, fix it
				if(bodyVectors < 0){
					bodyVectors *= -1;
					surfaceVectors *= -1;
				}	
				
				totalColor[0] += (Cb->c[0] * light->color.c[0] * bodyVectors) +
									 (light->color.c[0] * Cs->c[0] * pow(surfaceVectors, s));
				totalColor[1] += (Cb->c[1] * light->color.c[1] * bodyVectors) +
									 (light->color.c[1] * Cs->c[1] * pow(surfaceVectors, s));
				totalColor[2] += (Cb->c[2] * light->color.c[2] * bodyVectors) +
									 (light->color.c[2] * Cs->c[2] * pow(surfaceVectors, s));
				break;
			case LightPoint:
				vector_set(&L, light->location.val[0] - p->val[0],
						light->location.val[1] - p->val[1],
						light->location.val[2] - p->val[2]);
				vector_normalize(V);
				vector_normalize(&L);
				
				bodyVectors = vector_dot(&L, N);
				// if polygon is one-sided and light vector and surface normal are not within same hemisphere
				if(oneSided == 1 && bodyVectors < 0){ break; }
				
				sigma = vector_dot(V, N);
				// if they have different signs, break
				if((bodyVectors < 0 && sigma > 0) || (bodyVectors > 0 && sigma < 0)){ break; }

				// calculate halfway vector
				hx = (L.val[0] + V->val[0]) / 2.0;
				hy = (L.val[1] + V->val[1]) / 2.0;
				hz = (L.val[2] + V->val[2]) / 2.0;
				vector_set(&H, hx, hy, hz);	
				surfaceVectors = vector_dot(&H, N);

				// if normal is pointing the wrong way, fix it
				if(bodyVectors < 0){
					bodyVectors *= -1;
					surfaceVectors *= -1;
				}	
				
				totalColor[0] += (Cb->c[0] * light->color.c[0] * bodyVectors) +
									 (light->color.c[0] * Cs->c[0] * pow(surfaceVectors, s));
				totalColor[1] += (Cb->c[1] * light->color.c[1] * bodyVectors) +
									 (light->color.c[1] * Cs->c[1] * pow(surfaceVectors, s));
				totalColor[2] += (Cb->c[2] * light->color.c[2] * bodyVectors) +
									 (light->color.c[2] * Cs->c[2] * pow(surfaceVectors, s));
				//printf("[%.2f, %.2f, %.2f]\n", totalColor[0], totalColor[1], totalColor[2]);
				break;
			case LightSpot:
				break;
		}
	}
	if(totalColor[0] > 1){ totalColor[0] = 1; }
	else if(totalColor[0] < 0){ totalColor[0] = 0; }
	if(totalColor[1] > 1){ totalColor[1] = 1; }
	else if(totalColor[1] < 0){ totalColor[1] = 0; }
	if(totalColor[2] > 1){ totalColor[2] = 1; }
	else if(totalColor[2] < 0){ totalColor[2] = 0; }
	
	//printf("%.2f, %.2f, %.2f\n", totalColor[0], totalColor[1], totalColor[2]);
	color_set(c, totalColor[0], totalColor[1], totalColor[2]);
}
