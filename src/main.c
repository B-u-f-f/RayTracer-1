#define HYPATIA_IMPLEMENTATION

#include <stdio.h>
#include <assert.h>
#include <tgmath.h>
#include <stdbool.h>

#include "outfile.h"
#include "util.h"
#include "camera.h"
#include "sphere.h"
#include "hitRecord.h"

#include "hypatiaINC.h"

vec3 writeColor(vec3 pixel_color, int sample_per_pixel){
	float r = pixel_color.x;
	float g = pixel_color.y;
	float b = pixel_color.z;

	float scale = 1.0f/sample_per_pixel;

	r = sqrt(scale*r);
	g = sqrt(scale*g);
	b = sqrt(scale*b);

	vec3 temp;
	
	vector3_setf3(&temp, 255.999f*clamp(r,0.0f,0.999f),
					   255.999f*clamp(g,0.0f,0.999f),
				 	   255.999f*clamp(b,0.0f,0.999f));

	return temp;
}

HitRecord* hittableList(int n, Sphere *sphere[n], vec3 o, vec3 d, float t_min,float t_max){
	HitRecord* h = NULL;
	HitRecord* r = NULL;
	for(int i=0;i<n;i++){
		r = hit(*sphere[i], o, d, t_min, t_max);

		if(r != NULL)
			h = r;
	}
	
	return h;
}

vec3 ray_c(vec3 origin, vec3 direction, int n, Sphere* sphere[n], int depth){
	vec3 vertex;
	if(depth <= 0)
		return *(vector3_zero(&vertex));

	HitRecord* rec = hittableList(n, sphere, origin, direction, 0.001, INFINITY);
	if(rec != NULL){
		vec3 target = rec->p;
		vector3_add(&target, &rec->normal);
		vec3 rand = randomInUnitSphere();
		vector3_add(&target, &rand);
		vec3 inter2 = target;
		vector3_subtract(&inter2, &rec->p);
		vec3 inter1 = ray_c(rec->p, inter2, n, sphere, depth - 1);
		vector3_multiplyf(&inter1, 0.5);

		return inter1;
	}

	vec3 ud = direction;
	vector3_normalize(&ud);
	float t = 0.5 * (ud.y + 1.0);
	vec3 inter4;
	vector3_setf3(&inter4, 1.0 - t, 1.0 - t, 1.0 - t);
	vec3 inter3;
	vector3_setf3(&inter3, 0.5 * t, 0.7 * t, 1.0 * t);
	vector3_add(&inter3, &inter4);
	
	return inter3;
}
		


int main(int argc, char *argv[]){
	if(argc < 2){
		printf("FATAL ERROR: Output file name not provided.\n");
		printf("EXITING ...\n");
		return 0;
	}

	printf("Using Hypatia Version:%s\n", HYPATIA_VERSION);

	const int HEIGHT = 256;
	const int WIDTH = 256;
	const int SAMPLES_PER_PIXEL = 1;
	const int MAX_DEPTH = 10;
	
	vec3* image = (vec3*) malloc(sizeof(vec3) * HEIGHT * WIDTH);
	
	Sphere * s[2];
	vec3 cen;
	vector3_setf3(&cen, 0.0f, -100.5f, -1.0f);
	s[0] = setSphere(cen, 100);
	vector3_setf3(&cen, 0.0f, 0.0f, -1.0f);
	s[1] = setSphere(cen, 0.5f);

	Camera* c = createCamera();
	vec3 pixel_color;
	vec3 o;
	vec3 d;
	vec3 temp;
	

	for (int j = HEIGHT - 1; j >= 0; j--){
		for (int i = 0; i < WIDTH; i++){
			printf("%d %d \t", i, j);

			vector3_zero(&pixel_color);

			for(int k = 0; k < SAMPLES_PER_PIXEL; k++){
				float u = ((float)i + randomFloat(0.0f, 1.0f)) / (WIDTH - 1);
				float v = ((float)j + randomFloat(0.0f, 1.0f)) / (HEIGHT - 1);
				getRay(c, u, v, &o, &d);
				temp = ray_c(o, d, 2, s, MAX_DEPTH);
				vector3_add(&pixel_color, &temp);	
			}

			temp = writeColor(pixel_color, SAMPLES_PER_PIXEL);
			vector3_set(&image[i + HEIGHT * j], &temp);
		}
	}

	writeToPPM(argv[1], WIDTH, HEIGHT, image);
	free(image);
	free(c);
	free(s[0]);
	free(s[1]);

	return 0;
}
