/* compile by 
     clang 830mainSpecular.c 000pixel.o -lglfw -framework opengl;
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
#include "800sphere.c"
#include "810light.c"

lightLight light;
sphereSphere sphereOne;
sphereSphere sphereTwo;
sphereSphere sphereThree;
sphereSphere sphereFour;
sphereSphere sphereFive;
sphereSphere sphere[5];
int width = 512;
int height = 512;
int objectNum;

#include "820ray.c"

rayRay ray;

void initialize(void){
    //red
    double position[3] = {2.0, 1.8, -11.0};
    double color[3] = {1.0, 0.0, 0.0};
    sphereInitialize(&sphereOne, position, color, 2.0);
    
    //green
    position[0] = -3.0;
    position[1] = -4.0;
    position[2] = -16.0;
    color[0] = 0.0;
    color[1] = 1.0;
    color[2] = 0.0;
    sphereInitialize(&sphereTwo, position, color, 3.0);

    //blue
    position[0] = 2.0;
    position[1] = -1.3;
    position[2] = -13.0;
    color[0] = 0.0;
    color[1] = 0.0;
    color[2] = 1.0;
    sphereInitialize(&sphereThree, position, color, 2.0);
    
    //yellow
    position[0] = -1.0;
    position[1] = 1.0;
    position[2] = -21.0;
    color[0] = 1.0;
    color[1] = 1.0;
    color[2] = 0.0;
    sphereInitialize(&sphereFour, position, color, 4.0);
    
    //purple
    position[0] = -8.0;
    position[1] = 9.0;
    position[2] = -24.0;
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
    
    position[0] = -5.0;
    position[1] = -5.0;
    position[2] = 5.0;
    color[0] = 1.0;
    color[1] = 1.0;
    color[2] = 1.0;
    lightInitialize(&light, position, color);
}

void render(void){
    /* Two for loops to go over every pixel */
    for(int i = 0; i < height; i += 1){
        for(int j = 0; j < width; j += 1){
            double orig[3] = {0.0, 0.0, 0.0};
            double screen[3] = {(2.0 * ((double)i / (double)height)) - 1.0, (2.0 * ((double)j / (double)width)) - 1.0, -1};
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
                    
                        //printf("color: %f, %f, %f\n", rgb[0], rgb[1], rgb[2]);
                    
                        double lightNormal[3];
                        double unitLightNormal[3];
                        vecSubtract(3, light.translation, ray.intersection, lightNormal);
                        vecUnit(3, lightNormal, unitLightNormal);
                        //printf("light normal: %f, %f, %f\n", unitLightNormal[0], unitLightNormal[1], unitLightNormal[2]);
                        //printf("object normal: %f, %f, %f\n", ray.normal[0], ray.normal[1], ray.normal[2]);
                        double difIntensity;
                        double dot = vecDot(3, unitLightNormal, ray.normal);
                        //printf("difIntensity: %f\n", difIntensity);
                    
                        if(dot < 0)
                            difIntensity = 0.1;
                        else
                            difIntensity = dot;
                                                
                        double camDir[3];
                        double unitCamDir[3];
                        
                        vecSubtract(3, ray.origin, ray.intersection, camDir);
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
                        
                        rgb[0] = rgb[0] * difIntensity * light.color[0] + specIntensity;
                        rgb[1] = rgb[1] * difIntensity * light.color[1] + specIntensity;
                        rgb[2] = rgb[2] * difIntensity * light.color[2] + specIntensity;
                        //printf("color: %f, %f, %f\n", rgb[0], rgb[1], rgb[2]);
                        pixSetRGB(i, j, rgb[0], rgb[1], rgb[2]);
                    }
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