typedef struct rayRay rayRay;
struct rayRay{
    double origin[3];
    double direction[3];
    double point[3];
    double normal[3];
};

void rayInitialize(rayRay *ray, double orig[3], double dir[3]){
    ray->origin[0] = orig[0];
    ray->origin[1] = orig[1];
    ray->origin[2] = orig[2];
    ray->direction[0] = dir[0];
    ray->direction[1] = dir[1];
    ray->direction[2] = dir[2];
    ray->point[0] = 0.0;
    ray->point[1] = 0.0;
    ray->point[2] = 0.0;
    ray->normal[0] = 0.0;
    ray->normal[1] = 0.0;
    ray->normal[2] = 0.0;
}

int rayIntersection(rayRay *ray, sphereSphere *sphere){
    double c[3];
    vecSubtract(3, sphere->position, ray->origin, c);
    double v = vecDot(3, (c), ray->direction);
    double d = (sphere->radius * sphere->radius) - (vecDot(3, (c), (c)) - (v * v));
    if(d < 0){
        //printf("no dice\n");
        return -1;
    }
    else{
        //printf("an intersection!\n");
        double disc = sqrt(d);
        double vMinusDiscTimesV[3];
        double vMinusDisc = v - disc;
        vecScale(3, vMinusDisc, ray->direction, vMinusDiscTimesV); 
        vecAdd(3, ray->origin, vMinusDiscTimesV, ray->point);
        vecSubtract(3, ray->point, sphere->position, ray->normal);
        return 0;
    }
}

int rayIntersectionAttempt(rayRay *ray, sphereSphere *sphere){
    double l[3];
    vecSubtract(3, sphere->position, ray->origin, l);
    double tca = vecDot(3, l, ray->direction);
    if(tca < 0)
        return -1;
    double dTwo = vecDot(3, l, l) - (tca * tca);
    double radiusSquared = sphere->radius * sphere->radius;
    if(dTwo > radiusSquared)
        return -1;
    double thc = sqrt(radiusSquared - dTwo);
    double tZero = tca - thc; 
    double tOne = tca + thc;
    if(tZero > tOne){
        double temp = tZero;
        tZero = tOne;
        tOne = temp;
    }
    double tTimesDir[3];
    double normal[3];
    vecScale(3, tZero, ray->direction, tTimesDir);
    vecAdd(3, ray->origin, tTimesDir, ray->point);
    vecSubtract(3, ray->point, sphere->position, normal);
    vecUnit(3, normal, ray->normal);
    return 0;
}