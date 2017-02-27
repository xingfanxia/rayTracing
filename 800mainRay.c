#define STB_IMAGE_IMPLEMENTATION
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <stdarg.h>
#include "000pixel.h"
#include "stb_image.h"
#include "800light.c"
#include "800vector.c"
#include "800matrix.c"
#include "800sphere.c"


lightLight light;
sphereSphere sphereOne;
sphereSphere sphereTwo;
sphereSphere sphere[2];
int width = 512;
int height = 512;
int objectNum;
double objectNormal[3];
double objectPoint[3];
double camPos[3] = {0.0, 0.0, 0.0};
double shiny = 100.0;

#include "800ray.c"

void initialize(void){
    double position[3] = {2.0, 5.0, -6.0};
    double color[3] = {1.0, 0.0, 0.0};
    sphereInitialize(&sphereOne, position, color, 10.0);
    
    position[0] = -3.0;
    position[1] = -4.0;
    position[2] = -10.0;
    color[0] = 0.0;
    color[0] = 1.0;
    color[0] = 0.0;
    sphereInitialize(&sphereTwo, position, color, 8.0);
    
    objectNum = 2;
    sphere[0] = sphereOne;
    sphere[1] = sphereTwo;
    
    position[0] = 5.0;
    position[1] = 5.0;
    position[2] = 5.0;
    color[0] = 1.0;
    color[0] = 1.0;
    color[0] = 1.0;
    lightInitialize(&light, position, color);
}

void render(void){
    for(int i = 0; i < height; i += 1){
        for(int j = 0; j < width; j += 1){
            rayRay ray;
            double orig[3] = {0.0, 0.0, 0.0};
            double screen[3] = {(2 * (i / height)) - 1.0, (2 * (i / width)) - 1.0, -1.0};
            double dir[3];
            vecSubtract(3, screen, camPos, dir);
            double finalDir[3];
            vecUnit(3, dir, finalDir);
            rayInitialize(&ray, orig, finalDir);
            
            for(int k = 0; k < objectNum; k += 1){    
                printf("intersection?\n");
                if(rayIntersectionAttempt(&ray, &sphere[k]) != -1){
                    printf("there was an intersection\n");
                    double rgb[3];
                    rgb[0] = sphere[k].color[0];
                    rgb[1] = sphere[k].color[1];
                    rgb[2] = sphere[k].color[2];
    
                    double lightNormal[3] = {objectPoint[0] - light.translation[0],
                        objectPoint[1] - light.translation[1], objectPoint[2] - 
                        light.translation[2]};
                    
                    double unitLightNormal[3];
    
                    vecUnit(3, lightNormal, unitLightNormal);
    
                    double dot = vecDot(3, objectNormal, unitLightNormal);
                    double difIntensity;
    
                    if(dot > 0){
                        difIntensity = dot;
                    }
                    else{
                        difIntensity = 0;
                    }
    
                    double p[3];
                    double r[3];
                    vecScale(3, dot, objectNormal, p);
                    vecScale(3, 2.0, p, p);
                    vecSubtract(3, p, unitLightNormal, r);
                    double specIntensity;
    
                    double cameravec[3] = {camPos[0] - sphere[k].position[0], 
                    camPos[1] - sphere[k].position[1], camPos[2] - sphere[k].position[2]};
                
                    double unitCameravec[3];
                    vecUnit(3, cameravec, unitCameravec);
    
                    dot = vecDot(3, unitCameravec, r);
                    if(dot > 0){
                        specIntensity = dot;
                    }
                    else{
                        specIntensity = 0;
                    }
                    specIntensity = pow(specIntensity, shiny);
                    rgb[0] = rgb[0] * (difIntensity + 0.1) * light.color[0] + specIntensity;
                    rgb[1] = rgb[1] * (difIntensity + 0.1) * light.color[1] + specIntensity;
                    rgb[2] = rgb[2] * (difIntensity + 0.1) * light.color[2] + specIntensity;
                    
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