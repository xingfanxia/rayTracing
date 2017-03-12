
/* compile by 
     clang 910mainShadow.c 000pixel.o -lglfw -framework opengl;
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
#include "860sphere.c"
#include "860light.c"

sphereSphere light;
sphereSphere sphereOne;
sphereSphere sphereTwo;
sphereSphere sphereThree;
sphereSphere sphere[3];
int width = 512;
int height = 512;
int objectNum;
double camPos[3] = {0.0, 0.0, 0.0};
double camTarget[3];
double camRho;
double camPhi;
double camTheta;
int bounce = 3;

#include "860camera.c"
#include "860ray.c"

rayRay ray;
camCamera cam;
int update = 0;

void handleKeyUp(int key, int shiftIsDown, int controlIsDown,
		int altOptionIsDown, int superCommandIsDown) {
	printf("key up %d, shift %d, control %d, altOpt %d, supComm %d\n",
		key, shiftIsDown, controlIsDown, altOptionIsDown, superCommandIsDown);
		//d
		if(key == 68){
		    camPhi += 0.2;
		}
		//a
		else if(key == 65){
		    camPhi -= 0.2;
		}
		//s
		else if(key == 83){
		    camTheta += 0.2;
		}
		//w
		else if(key == 87){
		    camTheta -= 0.2;
		}
		//q
		else if(key == 69){
		    camRho += 1.0;
		}
		//e
		else if(key == 81){
		    camRho -= 1.0; 
		}
		//f
		else if(key == 70){
		    if(update == 0)
		        update = 1;
		    else
		        update = 0;
		}
}

void initialize(void){
    //red
    double position[3] = {2.0, 2.0, -5.0};
    double color[3] = {1.0, 0.1, 0.1};
    sphereInitialize(&sphereOne, position, color, 2.0, 0.0);
    
    //green
    position[0] = -3.0;
    position[1] = -4.0;
    position[2] = -10.0;
    color[0] = 0.1;
    color[1] = 1.0;
    color[2] = 0.1;
    sphereInitialize(&sphereTwo, position, color, 3.0, 0.0);

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
    sphereInitialize(&light, position, color, 2.0, 0.0);
    
    camRho = 5.0;
    camPhi = 0.0;
    camTheta = 0.0;
    camTarget[0] = 0.0;
    camTarget[1] = 0.0;
    camTarget[2] = -10.0;
    camUpdateViewing(&cam);
}

/* Applies diffuse and specular lighting to each ray that's drawn */
void lighting(double colorinfo[3], rayRay *ray, int k){
    colorinfo[0] = sphere[k].color[0];
    colorinfo[1] = sphere[k].color[1];
    colorinfo[2] = sphere[k].color[2];
                
    double lightNormal[3];
    double unitLightNormal[3];
    vecSubtract(3, light.position, ray->intersection, lightNormal);
    vecUnit(3, lightNormal, unitLightNormal);

    double difIntensity;
    double dot = vecDot(3, unitLightNormal, ray->normal);
                    
    if(dot < 0.1)
        difIntensity = 0.1;
    else
        difIntensity = dot;
                                                
    double camDir[3];
    double unitCamDir[3];
                        
    vecSubtract(3, camPos, ray->intersection, camDir);
    vecUnit(3, camDir, unitCamDir);
                        
    double p[3];
    double r[3];
    vecScale(3, dot, ray->normal, p);
    vecScale(3, 2.0, p, p);
    vecSubtract(3, p, unitLightNormal, r);
    double specIntensity;
    double unitR[3];
    vecUnit(3, r, unitR);
    double orig[3] = {ray->intersection[0], ray->intersection[1], ray->intersection[2]};
    rayRay rayTwo;
    rayInitialize(&rayTwo, orig, unitR);
    int intersect = rayIntersectionAttempt(&rayTwo, &light);
    if(intersect != -1){
        specIntensity = 2.0;
    }
    else
        specIntensity = 0.1;
                        
    specIntensity = pow(specIntensity, 50);
    
    double dir[3];
    double unitDir[3];
    vecSubtract(3, light.position, ray->intersection, dir);
    vecUnit(3, dir, unitDir);
    rayInitialize(&rayTwo, orig, unitDir);
    for(int i = 0; i < objectNum; i += 1){
        intersect = rayIntersectionAttempt(&rayTwo, &sphere[i]);
    }
    if(intersect != -1){
        specIntensity = 0.1;
        difIntensity = 0.1;
    }
                        
    colorinfo[0] = colorinfo[0] * difIntensity * light.color[0] + specIntensity;
    colorinfo[1] = colorinfo[1] * difIntensity * light.color[1] + specIntensity;
    colorinfo[2] = colorinfo[2] * difIntensity * light.color[2] + specIntensity;
}

void updateVaryings(void){
    for(int i = 0; i < objectNum; i += 1){
        double transformVec[4] = {sphere[i].position[0], sphere[i].position[1], 
            sphere[i].position[2], 1.0};
        double vary[4];
        mat441Multiply(cam.viewing, transformVec, vary);
        sphere[i].varying[0] = vary[0];
        sphere[i].varying[1] = vary[1];
        sphere[i].varying[2] = vary[2];
    }
    double lightVec[4] = {light.position[0], light.position[1], 
        light.position[2], 1.0};
    double lightVary[4];
    mat441Multiply(cam.viewing, lightVec, lightVary);
    light.varying[0] = lightVary[0];
    light.varying[1] = lightVary[1];
    light.varying[2] = lightVary[2];
}

/* initializes the ray so that it's new origin is where it intersected the object it's now
reflecting off of. the direction of the new ray is the reflection of direction value across
the normal vector */
void getReflectionRay(rayRay *ray, rayRay *rayTwo){
    double orig[3] = {ray->intersection[0], ray->intersection[1], ray->intersection[2]};
    
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
    rayInitialize(rayTwo, orig, finalDir);            
}

/* This combines the reflection color and the point color of the sphere that's been
intersected. The reflection color is scaled by the reflectiveness of the sphere in question. */
void combineColors(double pointCol[3], double reflectionCol[3], sphereSphere sphere,
        int reflection, double rgbCol[3], rayRay *ray){
    if(reflection == 1){
        rgbCol[0] = pointCol[0] * (reflectionCol[0] * sphere.reflection);
        rgbCol[1] = pointCol[1] * (reflectionCol[1] * sphere.reflection);
        rgbCol[2] = pointCol[2] * (reflectionCol[2] * sphere.reflection);
     }
    else{
        rgbCol[0] = pointCol[0];
        rgbCol[1] = pointCol[1];
        rgbCol[2] = pointCol[2];
    }
}

int traceRay(rayRay *ray, int index, int depth, double rgb[3]){
    int toReturn = 0;
    rayRay rayTwo;
    int reflection = 0;
    int depthPotential;
    
    for(int i = 0; i < objectNum; i += 1){
        double pointColor[3] = {0.0, 0.0, 0.0};
        double reflectionColor[3] = {0.0, 0.0, 0.0};
        depthPotential = rayIntersectionAttempt(ray, &sphere[i]);
        if(depthPotential != -1 && depthPotential < depth){
    
            toReturn = 1;
            depth = depthPotential;
            lighting(pointColor, ray, i);

            if(sphere[i].reflection > 0.0){
                getReflectionRay(ray, &rayTwo);
                reflection = traceRay(&rayTwo, 0, 10000000, reflectionColor);
            }
            combineColors(pointColor, reflectionColor, sphere[i], reflection, rgb, ray);

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
            rayInitialize(&ray, orig, finalDir);

            double rgb[3] = {0.0, 0.0, 0.0};
            
            /* Loops over every sphere in the program and tests whether each ray intersects.
            The ray intersection method returns the depth of the object. The depth is used
            to know which object to draw at each pixel. */
            traceRay(&ray, 0, 1000000, rgb);       
            pixSetRGB(i, j, rgb[0], rgb[1], rgb[2]);
        }
    }
}

void handleTimeStep(double oldTime, double newTime) {
	if (floor(newTime) - floor(oldTime) >= 1.0){
		printf("handleTimeStep: %f frames/sec\n", 1.0 / (newTime - oldTime));
	}
	if(update == 1){
	    camTheta += 0.1;
	}
	camUpdateViewing(&cam);
	updateVaryings();
	pixClearRGB(0.0, 0.0, 0.0);
	render();
}

int main(void){
    pixSetTimeStepHandler(handleTimeStep);
	pixSetKeyUpHandler(handleKeyUp);
    if (pixInitialize(width, height, "Ray Tracing") != 0)
		return 1;
	else {
	    pixClearRGB(0.0, 0.0, 0.0);
	    printf("cleared\n");
	    
	    
        initialize();
        printf("initialized\n");
        camUpdateViewing(&cam);
	    updateVaryings();
        render();
        printf("rendered\n");
	    	    
        pixRun();

        return 0;
    }
}