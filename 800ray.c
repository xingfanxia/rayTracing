/* has an origin and a direction */
typedef struct rayRay rayRay;
struct rayRay{
    double origin[3];
    double direction[3];
};

/* sets the origin and direction */
void rayInitialize(rayRay *ray, double orig[3], double dir[3]){
    ray->origin[0] = orig[0];
    ray->origin[1] = orig[1];
    ray->origin[2] = orig[2];
    ray->direction[0] = dir[0];
    ray->direction[1] = dir[1];
    ray->direction[2] = dir[2];
}

/* one version of ray intersection of a sphere. Returns 0 if there's an intersection and 
-1 if there's not. Taken from https://www.cs.unc.edu/~rademach/xroads-RT/RTarticle.html */
int rayIntersection(rayRay *ray, sphereSphere *sphere){
    double c[3];
    vecSubtract(3, sphere->position, ray->origin, c);
    double v = vecDot(3, (c), ray->direction);
    double d = (sphere->radius * sphere->radius) - (vecDot(3, (c), (c)) - (v * v));
    if(d < 0){
        printf("no dice\n");
        return -1;
    }
    else{
        printf("an intersection!\n");
        return 0;
        double disc = sqrt(d);
        double vMinusDiscTimesV[3];
        double vMinusDisc = v - disc;
        vecScale(3, vMinusDisc, ray->direction, vMinusDiscTimesV); 
        vecAdd(3, ray->origin, vMinusDiscTimesV, objectPoint);
        vecSubtract(3, objectPoint, sphere->position, objectNormal);
        return 0;
    }
}

/* Another attempt to intersect a vector with a sphere */
int rayIntersectionAttempt(rayRay *ray, sphereSphere *sphere){
    double l[3];
    vecSubtract(3, sphere->position, camPos, l);
    double tca = vecDot(3, l, ray->direction);
    if(tca < 0)
        return -1;
    double dTwo = vecDot(3, l, l) - (tca * tca);
    double radiusSquared = sphere->radius * sphere->radius;
    if(dTwo > radiusSquared)
        return -1;
    return 0;
}