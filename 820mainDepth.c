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
#include "810light.c"

/* All of the global variables. The objects to be drawn, the size of the screen, and a few
arrays for use later. Now includes a lightLight struct for lighting. */
lightLight light;
sphereSphere sphereOne;
sphereSphere sphereTwo;
sphereSphere sphereThree;
sphereSphere sphereFour;
sphereSphere sphereFive;
sphereSphere sphere[5];
int width = 1024;
int height = 1024;
int objectNum;

#include "820ray.c"

/* The one ray will be reinitialized for every pixel in the screen */
rayRay ray;

/* Initializes all of the spheres and adds them to the array of spheres. Also initializes
the light in the scene */
void initialize(void){
    //red
    double position[3] = {2.0, 1.8, -5.0};
    double color[3] = {1.0, 0.0, 0.0};
    sphereInitialize(&sphereOne, position, color, 2.0);
    
    //green
    position[0] = -3.0;
    position[1] = -4.0;
    position[2] = -10.0;
    color[0] = 0.0;
    color[1] = 1.0;
    color[2] = 0.0;
    sphereInitialize(&sphereTwo, position, color, 3.0);

    //blue
    position[0] = 2.0;
    position[1] = 1.3;
    position[2] = -6.0;
    color[0] = 0.0;
    color[1] = 0.0;
    color[2] = 1.0;
    sphereInitialize(&sphereThree, position, color, 2.0);
    
    //yellow
    position[0] = 1.0;
    position[1] = -1.0;
    position[2] = -15.0;
    color[0] = 1.0;
    color[1] = 1.0;
    color[2] = 0.0;
    sphereInitialize(&sphereFour, position, color, 4.0);
    
    //purple
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
    
    //light
    position[0] = -5.0;
    position[1] = -5.0;
    position[2] = 5.0;
    color[0] = 1.0;
    color[1] = 1.0;
    color[2] = 1.0;
    lightInitialize(&light, position, color);
}

/* initializes one ray for every pixel and tests whether or not it intersects a sphere.
If it does the color is treated with diffuse lighting so the spheres will look 3 
dimensional. */
void render(void){
    /* Two for loops to go over every pixel */
    for(int i = 0; i < height; i += 1){
        for(int j = 0; j < width; j += 1){
            double orig[3] = {0.0, 0.0, 0.0};
            double screen[3] = {(2.0 * ((double)i / (double)height)) - 1.0, (2.0 * ((double)j / (double)width)) - 1.0, -1.0};
            double dir[3];
            vecSubtract(3, screen, orig, dir);
            double finalDir[3];
            vecUnit(3, dir, finalDir);
            rayInitialize(&ray, orig, finalDir);
            double depth = 1000000.0;
            double depthPotential;
            
            /* Loops over every sphere in the program and tests whether each ray intersects.
            The ray intersection method returns the depth of the object. The depth is used
            to know which object to draw at each pixel. */
            for(int k = 0; k < objectNum; k += 1){    
                depthPotential = rayIntersectionAttempt(&ray, &sphere[k]);
                if(depthPotential != -1){
                    if(depthPotential < depth){
                        depth = depthPotential;
                    
                        double rgb[3];
                        rgb[0] = sphere[k].color[0];
                        rgb[1] = sphere[k].color[1];
                        rgb[2] = sphere[k].color[2];
                                      
                        //applies diffuse lighting to the intersection point               
                        double lightNormal[3];
                        double unitLightNormal[3];
                        vecSubtract(3, light.translation, ray.intersection, lightNormal);
                        vecUnit(3, lightNormal, unitLightNormal);
                        double difIntensity = vecDot(3, unitLightNormal, ray.normal);
                    
                        if(difIntensity < 0.1){
                            difIntensity = 0.1;
                        }
                    
                        rgb[0] = rgb[0] * difIntensity * light.color[0];
                        rgb[1] = rgb[1] * difIntensity * light.color[1];
                        rgb[2] = rgb[2] * difIntensity * light.color[2];
                        pixSetRGB(i, j, rgb[0], rgb[1], rgb[2]);
                    }
                }
            }
        }
    }
}

/* The main method creates the window, sets it all to black, calls the initialize method
and the render method. */
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