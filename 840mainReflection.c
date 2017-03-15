
/* compile by 
     clang 840mainReflection.c 000pixel.o -lglfw -framework opengl;
*/
#define STB_IMAGE_IMPLEMENTATION
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <stdarg.h>
#include "000pixel.h"
#include "stb_image.h"
#include "800vector.c"
#include "800matrix.c"
#include "840sphere.c"
#include "810light.c"

/* All of the global variables. The objects to be drawn, the size of the screen, and a few
arrays for use later. Now includes a lightLight struct for lighting. */
lightLight light;
sphereSphere sphereOne;
sphereSphere sphereTwo;
sphereSphere sphereThree;
sphereSphere sphere[3];
int width = 512;
int height = 512;
int objectNum;
double camPos[3] = {0.0, 0.0, 0.0};

#include "840ray.c"

/* The one ray will be reinitialized for every pixel in the screen */
rayRay ray;

/* Initializes all of the spheres and adds them to the array of spheres. Also initializes
the light in the scene */
void initialize(void){
    //red
    double position[3] = {2.0, 2.0, -5.0};
    double color[3] = {1.0, 0.0, 0.0};
    sphereInitialize(&sphereOne, position, color, 2.0, 1.0);
    
    //green
    position[0] = -3.0;
    position[1] = -4.0;
    position[2] = -10.0;
    color[0] = 0.0;
    color[1] = 1.0;
    color[2] = 0.0;
    sphereInitialize(&sphereTwo, position, color, 3.0, 1.0);

    //blue
    position[0] = -2.0;
    position[1] = 1.3;
    position[2] = -6.0;
    color[0] = 1.0;
    color[1] = 1.0;
    color[2] = 1.0;
    sphereInitialize(&sphereThree, position, color, 2.0, 1.0);
    
    
    objectNum = 3;
    sphere[0] = sphereOne;
    sphere[1] = sphereTwo;
    sphere[2] = sphereThree;
    
    position[0] = -5.0;
    position[1] = -5.0;
    position[2] = 5.0;
    color[0] = 1.0;
    color[1] = 1.0;
    color[2] = 1.0;
    lightInitialize(&light, position, color);
}

/* Applies diffuse and specular lighting to each ray that's drawn */
void lighting(double colorinfo[3], rayRay ray, int k){
    colorinfo[0] = sphere[k].color[0];
    colorinfo[1] = sphere[k].color[1];
    colorinfo[2] = sphere[k].color[2];
                
    double lightNormal[3];
    double unitLightNormal[3];
    vecSubtract(3, light.translation, ray.intersection, lightNormal);
    vecUnit(3, lightNormal, unitLightNormal);

    double difIntensity;
    double dot = vecDot(3, unitLightNormal, ray.normal);
                    
    if(dot < 0)
        difIntensity = 0.1;
    else
        difIntensity = dot;
                                                
    double camDir[3];
    double unitCamDir[3];
                        
    vecSubtract(3, camPos, ray.intersection, camDir);
    vecUnit(3, camDir, unitCamDir);
                        
    double p[3];
    double r[3];
    vecScale(3, dot, ray.normal, p);
    vecScale(3, 2.0, p, p);
    vecSubtract(3, p, unitLightNormal, r);
    double specIntensity;
    
    dot = vecDot(3, unitCamDir, r);
    if(dot > 0){
         specIntensity = dot;
    }
    else{
        specIntensity = 0;
    }
                        
    specIntensity = pow(specIntensity, 50);
                        
    colorinfo[0] = colorinfo[0] * difIntensity * light.color[0] + specIntensity;
    colorinfo[1] = colorinfo[1] * difIntensity * light.color[1] + specIntensity;
    colorinfo[2] = colorinfo[2] * difIntensity * light.color[2] + specIntensity;
}

/* initializes the ray so that it's new origin is where it intersected the object it's now
reflecting off of. the direction of the new ray is the reflection of direction value across
the normal vector */
void getReflectionRay(rayRay ray){
    double orig[3] = {ray.intersection[0]+0.4, ray.intersection[1]+0.4, ray.intersection[2]+0.4};
    
    double prevRayDir[3];
    double unitPrevRayDir[3];
    vecSubtract(3, ray.origin, ray.intersection, prevRayDir);
    vecUnit(3, prevRayDir, unitPrevRayDir);
    
    double dot = vecDot(3, unitPrevRayDir, ray.normal);
    if (dot <= 0) {
        printf("if there should be reflection: %f\n", dot);
        return;
    }
    double p[3];
    double r[3];
    vecScale(3, dot, ray.normal, p);
    vecScale(3, 2.0, p, p);
    vecSubtract(3, p, unitPrevRayDir, r);
    
    double finalDir[3];
    vecUnit(3, r, finalDir);
    rayInitialize(&ray, orig, finalDir, 1);
}

/* This combines the reflection color and the point color of the sphere that's been
intersected. The reflection color is scaled by the reflectiveness of the sphere in question. */
void combineColors(double pointColor[3], double reflectionColor[3], sphereSphere sphere,
        double rgb[3]){
    if(sphere.reflection > 0.0){
        rgb[0] = pointColor[0] * (reflectionColor[0] * sphere.reflection);
        rgb[1] = pointColor[1] * (reflectionColor[1] * sphere.reflection);
        rgb[2] = pointColor[2] * (reflectionColor[2] * sphere.reflection);       
    }
    else{
        rgb[0] = pointColor[0];
        rgb[1] = pointColor[1];
        rgb[2] = pointColor[2];
    }
}

/* initializes one ray for every pixel and tests whether or not it intersects a sphere.
If it does the color is treated with diffuse lighting and now specular lighting so the
 spheres will look 3 dimensional. */
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
            rayInitialize(&ray, orig, finalDir, 0);
            
            double depth = 1000000.0;
            double depthPotential;
            double pointColor[3];
            double reflectionColor[3] = {0.0, 0.0, 0.0};
            double rgb[3];
            
            /* Loops over every sphere in the program and tests whether each ray intersects.
            The ray intersection method returns the depth of the object. The depth is used
            to know which object to draw at each pixel. */
            for(int k = 0; k < objectNum; k += 1){    
                depthPotential = rayIntersectionAttempt(&ray, &sphere[k]);
                if(depthPotential != -1 && depthPotential < depth){
                    depth = depthPotential;
                    lighting(pointColor, ray, k);
                    if(sphere[k].reflection > 0.0){
                        reflectionColor[0] = 0.0;
                        reflectionColor[1] = 0.0;
                        reflectionColor[2] = 0.0;
                        getReflectionRay(ray);
                        double depthReflect = 1000000.0;
                        for(int l = 0; l < objectNum; l += 1){
                            //if (l == k) continue;
                            double depthPotReflect = rayIntersectionAttempt(&ray, &sphere[l]);
                            if(depthPotReflect != -1 && depthPotReflect < depthReflect){
                                depthReflect = depthPotReflect;
                                lighting(reflectionColor, ray, l);
                                combineColors(pointColor, reflectionColor, sphere[k], rgb);
                            }
                        }
                    }
                }
            }       
            pixSetRGB(i, j, rgb[0], rgb[1], rgb[2]);
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