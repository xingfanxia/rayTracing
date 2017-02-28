#define STB_IMAGE_IMPLEMENTATION
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <stdarg.h>
#include "000pixel.h"
#include "stb_image.h"
#include "800vector.c"
#include "800matrix.c"
#include "800sphere.c"


sphereSphere sphereOne;
sphereSphere sphereTwo;
sphereSphere sphereThree;
sphereSphere sphereFour;
sphereSphere sphereFive;
sphereSphere sphere[5];
int width = 512;
int height = 512;
int objectNum;
double objectNormal[3] = {0.0, 0.0, 0.0};
double objectPoint[3] = {0.0, 0.0, 0.0};
double camPos[3] = {0.0, 0.0, 0.0};

#include "800ray.c"

rayRay ray;

void initialize(void){
    double position[3] = {2.0, 5.0, -6.0};
    double color[3] = {1.0, 0.5, 0.0};
    sphereInitialize(&sphereOne, position, color, 2.0);
    
    position[0] = -3.0;
    position[1] = -4.0;
    position[2] = -10.0;
    color[0] = 0.0;
    color[1] = 1.0;
    color[2] = 0.0;
    sphereInitialize(&sphereTwo, position, color, 3.0);
    
    position[0] = 7.0;
    position[1] = 3.0;
    position[2] = -5.0;
    color[0] = 0.0;
    color[1] = 0.0;
    color[2] = 1.0;
    sphereInitialize(&sphereThree, position, color, 2.0);
    
    position[0] = 1.0;
    position[1] = -1.0;
    position[2] = -15.0;
    color[0] = 1.0;
    color[1] = 1.0;
    color[2] = 0.0;
    sphereInitialize(&sphereFour, position, color, 4.0);
    
    position[0] = -3.0;
    position[1] = 4.0;
    position[2] = -18.0;
    color[0] = 1.0;
    color[1] = 0.0;
    color[2] = 1.0;
    sphereInitialize(&sphereFive, position, color, 5.0);
    
    objectNum = 5;
    sphere[0] = sphereOne;
    sphere[1] = sphereTwo;
    sphere[2] = sphereThree;
    sphere[3] = sphereFour;
    sphere[4] = sphereFive;
}

void render(void){
    for(int i = 0; i < height; i += 1){
        for(int j = 0; j < width; j += 1){
            double orig[3] = {0.0, 0.0, 0.0};
            double screen[3] = {(2.0 * ((double)i / (double)height)) - 1.0, (2.0 * ((double)j / (double)width)) - 1.0, -1.0};
            double dir[3];
            vecSubtract(3, screen, camPos, dir);
            double finalDir[3];
            vecUnit(3, dir, finalDir);
            rayInitialize(&ray, orig, finalDir);
            
            for(int k = 0; k < objectNum; k += 1){    
                if(rayIntersection(&ray, &sphere[k]) != -1){
                    double rgb[3];
                    rgb[0] = sphere[k].color[0];
                    rgb[1] = sphere[k].color[1];
                    rgb[2] = sphere[k].color[2];
                    
                    pixSetRGB(i, j, rgb[0], rgb[1], rgb[2]);
                }
            }
        }
    }
}

int main(void){
    if (pixInitialize(width, height, "Ray Tracing") != 0)
		return 1;
	else {
	    pixClearRGB(0.0, 0.0, 0.0);
	    printf("cleared\n");
	    
	    
        initialize();
        printf("initialized\n");
        render();
        printf("rendered\n");
	    	    
        pixRun();

        return 0;
    }
}