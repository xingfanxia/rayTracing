/* has an origin, a direction, an intersection point, and a normal vector */
typedef struct rayRay rayRay;
struct rayRay{
    double origin[3];
    double direction[3];
    double intersection[3];
    double normal[3];
};

/* intersection point and normal vector aren't set until the intersection attempt */
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

/* Tests for intersection with a sphere and finds the intersection point in space and 
the normal vector on the surface of the sphere. The depth of the intersection is returned */
int rayIntersectionAttempt(rayRay *ray, sphereSphere *sphere){
    double l[3];
    vecSubtract(3, sphere->varying, ray->origin, l);
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
    vecAdd(3, ray->origin, tTimesDir, ray->intersection);
    vecSubtract(3, ray->intersection, sphere->varying, normal);
    vecUnit(3, normal, ray->normal);
    double vecDepth[3];
    vecSubtract(3, camPos, ray->intersection, vecDepth);
    return vecLength(3, vecDepth);
}

/* Assumes that the ray is pointing into a sphere and returns tOne instead of tZero 
tZero is the origin of the ray that's passed in. This method is only used by refractionRay
because the two rays are required. One for entering the spheres and one exiting. */
int rayInternalIntersection(rayRay *ray, sphereSphere *sphere){
    double l[3];
    vecSubtract(3, sphere->varying, ray->origin, l);
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
    vecScale(3, tOne, ray->direction, tTimesDir);
    vecAdd(3, ray->origin, tTimesDir, ray->intersection);
    vecSubtract(3, ray->intersection, sphere->varying, normal);
    vecUnit(3, normal, ray->normal);
    double vecDepth[3];
    vecSubtract(3, camPos, ray->intersection, vecDepth);
    return vecLength(3, vecDepth);
}