
/* compile by 
     clang 852mainRecursive.c 000pixel.o -lglfw -framework opengl;
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

rayRay ray;

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
    position[1] = 2.0;
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
    //printf("sphere color: %f, %f, %f\n", colorinfo[0], colorinfo[1], colorinfo[2]);
                
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
void getReflectionRay(rayRay *ray, rayRay *rayTwo){
    double orig[3] = {ray->intersection[0], ray->intersection[1], ray->intersection[2]};
    printf("origin: %f, %f, %f\n", orig[0], orig[1], orig[2]);
    
    double prevRayDir[3] = {-ray->direction[0], -ray->direction[1], -ray->direction[2]};
    
    double dot = vecDot(3, prevRayDir, ray->normal);
    if (dot <= 0) {
        return;
    }
    double p[3];
    double r[3];
    vecScale(3, dot, ray->normal, p);
    vecScale(3, 2.0, p, p);
    vecSubtract(3, p, prevRayDir, r);
    
    double finalDir[3];
    vecUnit(3, r, finalDir);
    //printf("final Direction: %f, %f, %f\n", finalDir[0], finalDir[1], finalDir[2]);
    rayInitialize(rayTwo, orig, finalDir, 1);
    //printf("rayTwo origin in the initializer: %f, %f, %f\n", rayTwo.origin[0], rayTwo.origin[1], rayTwo.origin[2]);
    //printf("rayTwo direction in the initializer: %f, %f, %f\n", rayTwo.direction[0], rayTwo.direction[1], rayTwo.direction[2]);
            
}

/* This combines the reflection color and the point color of the sphere that's been
intersected. The reflection color is scaled by the reflectiveness of the sphere in question. */
void combineColors(double pointCol[3], double reflectionCol[3], sphereSphere sphere,
        int reflection, double rgbCol[3]){
    if(reflection == 1){
        //printf("yes\n");
        //printf("point color: %f, %f, %f\n", pointCol[0], pointCol[1], pointCol[2]);
        //printf("reflection color: %f, %f, %f\n", reflectionCol[0], reflectionCol[1], reflectionCol[2]);   
        rgbCol[0] = pointCol[0] * (reflectionCol[0] * sphere.reflection);
        //printf("rgb: %f, point: %f, reflection: %f\n", rgbCol[0], pointCol[0], reflectionCol[0]);
        rgbCol[1] = pointCol[1] * (reflectionCol[1] * sphere.reflection);
        rgbCol[2] = pointCol[2] * (reflectionCol[2] * sphere.reflection);
        // rgb[0] = (reflectionColor[0] * sphere.reflection);
        // rgb[1] = (reflectionColor[1] * sphere.reflection);
        // rgb[2] = (reflectionColor[2] * sphere.reflection);
        //printf("ultimate color: %f, %f, %f\n", rgbCol[0], rgbCol[1], rgbCol[2]);        
    }
    else{
        //printf("no\n");
        rgbCol[0] = pointCol[0];
        rgbCol[1] = pointCol[1];
        rgbCol[2] = pointCol[2];
        //printf("color: %f, %f, %f\n", rgbCol[0], rgbCol[1], rgbCol[2]);
    }
}

int traceRay(rayRay ray, int depth, double rgb[3]){
    int toReturn = 0;
    rayRay rayTwo;
    int reflection = 0;
    int depthPotential;
    int depthReflect = 100000000;
    int depthPotentialReflect;
    double pointColor[3] = {0.0, 0.0, 0.0};
    double reflectionColor[3] = {0.0, 0.0, 0.0};
    
    for(int i = 0; i < objectNum; i += 1){
    
        depthPotential = rayIntersectionAttempt(&ray, &sphere[i]);
        if(depthPotential != -1 && depthPotential < depth){
            toReturn = 1;
            depth = depthPotential;
            //printf("point Color before: %f, %f, %f\n", pointColor[0], pointColor[1], pointColor[2]);
            lighting(pointColor, ray, i);
            //printf("point Color after: %f, %f, %f\n", pointColor[0], pointColor[1], pointColor[2]);
        
            //printf("yes\n");   
            getReflectionRay(&ray, &rayTwo);
            printf("rayTwo origin in trace Ray: %f, %f, %f\n", rayTwo.origin[0], rayTwo.origin[1], rayTwo.origin[2]);
            printf("rayTwo direction in trace Ray: %f, %f, %f\n", rayTwo.direction[0], rayTwo.direction[1], rayTwo.direction[2]);
            
            for(int j = 0; j < objectNum; j += 1){
                depthPotentialReflect = rayIntersectionAttempt(&rayTwo, &sphere[j]);
                if(depthPotentialReflect != -1 && depthPotentialReflect < depthReflect){
                    reflection = 1;
                    depthReflect = depthPotentialReflect;
                    lighting(reflectionColor, rayTwo, j);
                    printf("reflection color after: %f, %f, %f\n", reflectionColor[0], reflectionColor[1], reflectionColor[2]);
                }
            }
            combineColors(pointColor, reflectionColor, sphere[i], reflection, rgb);
            //printf("rgb: %f, %f, %f\n", rgb[0], rgb[1], rgb[2]);
        }
    }
    return toReturn;
}

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

            double rgb[3] = {0.5, 0.1, 0.5};
            
            /* Loops over every sphere in the program and tests whether each ray intersects.
            The ray intersection method returns the depth of the object. The depth is used
            to know which object to draw at each pixel. */
            //printf("pixel: %d, %d\n", i, j);
            traceRay(ray, 1000000, rgb);       
            pixSetRGB(i, j, rgb[0], rgb[1], rgb[2]);
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