/* compile by 
    camera: 860mainCamera.c
    clang 860mainCamera.c 000pixel.o -lglfw -framework opengl
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
/* All of the global variables. The objects to be drawn, the size of the screen, and a few
arrays for use later. Now includes a lightLight struct for lighting and a camera object
for interaction. */
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
double camPos[3] = {0.0, 0.0, 0.0};
double camTarget[3];
double camRho;
double camPhi;
double camTheta;

#include "860camera.c"
#include "860ray.c"

camCamera cam;
int update = 0;

/* The one ray will be reinitialized for every pixel in the screen */
rayRay ray;

/* The key handler from our software engine. It rotates and translates the camera when the
wasdeq buttons are pressed. */
void handleKeyUp(int key, int shiftIsDown, int controlIsDown,
		int altOptionIsDown, int superCommandIsDown) {
	printf("key up %d, shift %d, control %d, altOpt %d, supComm %d\n",
		key, shiftIsDown, controlIsDown, altOptionIsDown, superCommandIsDown);
		//d
		if(key == 68){
		    camPhi += 0.2;
		    update = 1;
		}
		//a
		else if(key == 65){
		    camPhi -= 0.2;
		    update = 1;
		}
		//s
		else if(key == 83){
		    camTheta += 0.2;
		    update = 1;
		}
		//w
		else if(key == 87){
		    camTheta -= 0.2;
		    update = 1;
		}
		//q
		else if(key == 69){
		    camRho += 1.0;
		    update = 1;
		}
		//e
		else if(key == 81){
		    camRho -= 1.0; 
		    update = 1;
		}
}

/* Initializes all of the spheres and adds them to the array of spheres. Also initializes
the light in the scene */
void initialize(void){
    //red
    double position[3] = {2.0, 1.8, -10.0};
    double color[3] = {1.0, 0.0, 0.0};
    sphereInitialize(&sphereOne, position, color, 2.0, 0.0);
    
    //green
    position[0] = -3.0;
    position[1] = -4.0;
    position[2] = -15.0;
    color[0] = 0.0;
    color[1] = 1.0;
    color[2] = 0.0;
    sphereInitialize(&sphereTwo, position, color, 4.0, 0.0);

    //blue
    position[0] = 2.0;
    position[1] = -1.3;
    position[2] = -8.0;
    color[0] = 0.0;
    color[1] = 0.0;
    color[2] = 1.0;
    sphereInitialize(&sphereThree, position, color, 2.0, 0.0);
    
    //yellow
    position[0] = -1.0;
    position[1] = 1.0;
    position[2] = -18.0;
    color[0] = 1.0;
    color[1] = 1.0;
    color[2] = 0.0;
    sphereInitialize(&sphereFour, position, color, 4.0, 0.0);
    
    //purple
    position[0] = -8.0;
    position[1] = 9.0;
    position[2] = -22.0;
    color[0] = 1.0;
    color[1] = 0.0;
    color[2] = 1.0;
    sphereInitialize(&sphereFive, position, color, 5.0, 0.0);
    
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
    
    //camera
    camRho = 5.0;
    camPhi = 0.0;
    camTheta = 0.0;
    camTarget[0] = 0.0;
    camTarget[1] = 0.0;
    camTarget[2] = -10.0;
    camUpdateViewing(&cam);
}

/* initializes one ray for every pixel and tests whether or not it intersects a sphere.
If it does the color is treated with diffuse lighting and now specular lighting so the
 spheres will look 3 dimensional. */
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
                                        
                        double lightNormal[3];
                        double unitLightNormal[3];
                        vecSubtract(3, light.varying, ray.intersection, lightNormal);
                        vecUnit(3, lightNormal, unitLightNormal);
                        double difIntensity;
                        double dot = vecDot(3, unitLightNormal, ray.normal);
                    
                        if(dot < 0.1)
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
                        pixSetRGB(i, j, rgb[0], rgb[1], rgb[2]);
                    }
                }
            }
        }
    }
}

/* updates the varyings in the sphere and light structs based on the viewing matrix
contained in the camera struct */
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
    double lightVec[4] = {light.translation[0], light.translation[1], 
        light.translation[2], 1.0};
    double lightVary[4];
    mat441Multiply(cam.viewing, lightVec, lightVary);
    light.varying[0] = lightVary[0];
    light.varying[1] = lightVary[1];
    light.varying[2] = lightVary[2];
}

/* The handle time step from the software engine. Every time it updates the viewing matrix
of the camera if there's been a change indicated by the update variable. it also updates
the varyings, clears the screen, and renders again. */
void handleTimeStep(double oldTime, double newTime) {
	if (floor(newTime) - floor(oldTime) >= 1.0){
		printf("handleTimeStep: %f frames/sec\n", 1.0 / (newTime - oldTime));
	}
	if(update == 1){
	    update = 0;
	    camTheta += 0.1;
	    camUpdateViewing(&cam);
	    updateVaryings();
	    pixClearRGB(0.0, 0.0, 0.0);
	    render();
	}
}

/* The main method creates the window, sets it all to black, calls the initialize method
and the render method. Now includes the update viewing and update varyings calls. */
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