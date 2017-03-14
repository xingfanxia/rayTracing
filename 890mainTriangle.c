
/* compile by 
     clang 880mainAbstracted.c 000pixel.o -lglfw -framework opengl;
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
#include "890triangle.c"
#include "890pointVary.c"

lightLight light;
triangleTriangle triangle[3];
triangleTriangle triOne, triTwo, triThree;
int width = 512;
int height = 512;
int objectNum;
double camPos[3] = {0.0, 0.0, 0.0};
double camTarget[3];
double camRho;
double camPhi;
double camTheta;

#include "860camera.c"
#include "890ray.c"

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
    //triangle one
    double aPos[3] = {2.0, 1.0, -5.0};
    double aCol[3] = {1.0, 0.0, 0.0};
    double bPos[3] = {5.0, 2.0, -5.0};
    double bCol[3] = {0.0, 1.0, 0.0};
    double cPos[3] = {3.0, 5.0, -5.0};
    double cCol[3] = {0.0, 0.0, 1.0};
    triangleInitialize(&triOne, aPos, aCol, bPos, bCol, cPos, cCol);
    
    //triangle two
    aPos[0] = -4.0;
    aPos[1] = 1.0;
    aPos[2] = -5.0;
    aCol[0] = 1.0;
    aCol[1] = 0.0;
    aCol[2] = 0.0;
    
    bPos[0] = 0.0;
    bPos[1] = 0.0;
    bPos[2] = -5.0;
    bCol[0] = 0.0;
    bCol[1] = 1.0;
    bCol[2] = 0.0;
    
    cPos[0] = -2.0;
    cPos[1] = 4.0;
    cPos[2] = -5.0;
    cCol[0] = 0.0;
    cCol[1] = 0.0;
    cCol[2] = 1.0;
    
    triangleInitialize(&triTwo, aPos, aCol, bPos, bCol, cPos, cCol);

    //triangle three
    aPos[0] = -4.0;
    aPos[1] = 1.0;
    aPos[2] = -5.0;
    aCol[0] = 1.0;
    aCol[1] = 0.0;
    aCol[2] = 0.0;
    
    bPos[0] = 0.0;
    bPos[1] = 0.0;
    bPos[2] = -5.0;
    bCol[0] = 0.0;
    bCol[1] = 1.0;
    bCol[2] = 0.0;
    
    cPos[0] = -2.0;
    cPos[1] = 4.0;
    cPos[2] = -5.0;
    cCol[0] = 0.0;
    cCol[1] = 0.0;
    cCol[2] = 1.0;
    
    triangleInitialize(&triThree, aPos, aCol, bPos, bCol, cPos, cCol);
    
    objectNum = 3;
    triangle[0] = triOne;
    triangle[1] = triTwo;
    triangle[2] = triThree;
    
    double position[3];
    double color[3];
    position[0] = 0.0;
    position[1] = 0.0;
    position[2] = 5.0;
    color[0] = 1.0;
    color[1] = 1.0;
    color[2] = 1.0;
    lightInitialize(&light, position, color);
    
    camRho = 5.0;
    camPhi = 0.0;
    camTheta = 0.0;
    camTarget[0] = 0.0;
    camTarget[1] = 0.0;
    camTarget[2] = 0.0;
    camUpdateViewing(&cam);
}

/* Applies diffuse and specular lighting to each ray that's drawn */
void lighting(double colorinfo[3], rayRay ray, pointVary *vary){
    colorinfo[0] = vary->color[0];
    colorinfo[1] = vary->color[1];
    colorinfo[2] = vary->color[2];
    printf("vary color: %f, %f, %f\n", vary->color[0], vary->color[1], vary->color[2]);
    printf("light color: %f, %f, %f\n", light.color[0], light.color[1], light.color[2]);
    printf("color: %f, %f, %f\n", colorinfo[0], colorinfo[1], colorinfo[2]);

    
    double lightNormal[3];
    double unitLightNormal[3];
    vecSubtract(3, light.translation, ray.intersection, lightNormal);
    vecUnit(3, lightNormal, unitLightNormal);

    double difIntensity;
    double dot = vecDot(3, unitLightNormal, ray.normal);
                    
    if(dot < 0.1)
        difIntensity = 0.1;
    else
        difIntensity = dot;
                                                
    /*double camDir[3];
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
    printf("specIntensity: %f\n", specIntensity);
    printf("difIntensity: %f\n", difIntensity);*/
                        
    //colorinfo[0] = colorinfo[0] * difIntensity * light.color[0];// + specIntensity;
    //colorinfo[1] = colorinfo[1] * difIntensity * light.color[1];// + specIntensity;
    //colorinfo[2] = colorinfo[2] * difIntensity * light.color[2];// + specIntensity;
    printf("color: %f, %f, %f\n", colorinfo[0], colorinfo[1], colorinfo[2]);
}

void updateVaryings(void){
    for(int i = 0; i < objectNum; i += 1){
        double transformVec[4] = {triangle[i].aPosition[0], triangle[i].aPosition[1], 
            triangle[i].aPosition[2], 1.0};
        double vary[4];
        mat441Multiply(cam.viewing, transformVec, vary);
        triangle[i].aVarying[0] = vary[0];
        triangle[i].aVarying[1] = vary[1];
        triangle[i].aVarying[2] = vary[2];
        
        transformVec[0] = triangle[i].bPosition[0];
        transformVec[1] = triangle[i].bPosition[1];
        transformVec[2] = triangle[i].bPosition[2];
        transformVec[3] = 1.0;
        mat441Multiply(cam.viewing, transformVec, vary);
        triangle[i].bVarying[0] = vary[0];
        triangle[i].bVarying[1] = vary[1];
        triangle[i].bVarying[2] = vary[2];
        
        transformVec[0] = triangle[i].cPosition[0];
        transformVec[1] = triangle[i].cPosition[1];
        transformVec[2] = triangle[i].cPosition[2];
        transformVec[3] = 1.0;
        mat441Multiply(cam.viewing, transformVec, vary);
        triangle[i].cVarying[0] = vary[0];
        triangle[i].cVarying[1] = vary[1];
        triangle[i].cVarying[2] = vary[2];
    }
    double lightVec[4] = {light.translation[0], light.translation[1], 
        light.translation[2], 1.0};
    double lightVary[4];
    mat441Multiply(cam.viewing, lightVec, lightVary);
    light.varying[0] = lightVary[0];
    light.varying[1] = lightVary[1];
    light.varying[2] = lightVary[2];
    //printf("light: %f, %f, %f\n", light.varying[0], light.varying[1], light.varying[2]);
}

void traceRay(rayRay ray, int index, int depth, double rgb[3], pointVary *vary){
    int depthPotential;
    
    for(int i = 0; i < objectNum; i += 1){
        depthPotential = makeCounterClockwise(&ray, triangle[i].aVarying, 
            triangle[i].bVarying, triangle[i].cVarying, vary);
        if(depthPotential != -1 && depthPotential < depth){
            depth = depthPotential;
            lighting(rgb, ray, vary);
        }
    }
    //printf("depth potential: %d\n", depthPotential);
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
            pointVary vary;
            traceRay(ray, 0, 1000000, rgb, &vary);       
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