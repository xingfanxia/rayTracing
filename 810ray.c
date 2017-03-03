 typedef struct rayRay rayRay;
struct rayRay{
    double origin[3];
    double direction[3];
    double intersection[3];
    double normal[3];
};

void rayInitialize(rayRay *ray, double orig[3], double dir[3]){
    ray->origin[0] = orig[0];
    ray->origin[1] = orig[1];
    ray->origin[2] = orig[2];
    ray->direction[0] = dir[0];
    ray->direction[1] = dir[1];
    ray->direction[2] = dir[2];
    ray->intersection[0] = 0.0;
    ray->intersection[1] = 0.0;
    ray->intersection[2] = 0.0;
    ray->normal[0] = 0.0;
    ray->normal[1] = 0.0;
    ray->normal[2] = 0.0;
}


int rayIntersectionAttempt(rayRay *ray, sphereSphere *sphere){
    double l[3];
    vecSubtract(3, sphere->position, ray->origin, l);
    double tca = vecDot(3, l, ray->direction);
    if(tca < 0)
        return -1;
    double d_square = vecDot(3, l, l) - (tca * tca);
    double radiusSquared = sphere->radius * sphere->radius;
    if(d_square > radiusSquared)
        return -1;
    double thc = sqrt(radiusSquared - d_square);
    double tZero = tca - thc; 
    double tOne = tca + thc;
    if(tZero > tOne){
        double temp = tZero;
        tZero = tOne;
        tOne = temp;
    }

    if (tZero < 0) {
        tZero = tOne;
        if (tZero < 0) return -1;
    }


    double tTimesDir[3];
    double normal[3];
    vecScale(3, tZero, ray->direction, tTimesDir);
    vecAdd(3, ray->origin, tTimesDir, ray->intersection);
    vecSubtract(3, ray->intersection, sphere->position, normal);
    vecUnit(3, normal, ray->normal);
    return 0;
}