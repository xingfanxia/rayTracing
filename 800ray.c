struct rayRay{
    double origin[3];
    double direction[3];
}

void rayInitialize(rayRay *ray, double orig[3], double dir[3]){
    ray->origin[0] = orig[0];
    ray->origin[1] = orig[1];
    ray->origin[2] = orig[2];
    ray->direction[0] = dir[0];
    ray->direction[1] = dir[1];
    ray->direction[2] = dir[2];
}

int rayIntersection(rayRay *ray, sphereSphere *sphere){
    double c[3];
    vecSubtract(3, ray->origin, sphere->position, c);
    double v = vecDot(3, (c), ray->direction);
    double d = (sphere->radius * sphere->radius) - (vecDot(3, (c), (c)) - (v * v));
    //no intersection
    if(d < 0)
        return -1;
    else{
        double disc = sqrt(d);
        double vMinusDisc[3];
        double vMinusDiscTimesV[3];
        vecSubtract(3, v, disc, vMinusDisc);
        vecScale(3, vMinusDisc, ray->direction, vMinusDiscTimesV); 
        vecAdd(3, ray->origin, vMinusDiscTimesV, objectPoint);
        vecSubtract(3, sphere->position, objectPoint, objectNormal);
        return 0;
    }
}