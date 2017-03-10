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


int raySphereIntersection(rayRay *ray, sphereSphere *sphere){
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
    return tZero;
}

int rayTriangleIntersection(rayRay *ray, double a[], double b[], double c[], pointVary *vary){
    double ab[3]; 
    double ac[3];
    vecSubtract(3, b, a, ab);
    vecSubtract(3, c, a, ac);
    double normal[3];
    vec3Cross(ab, ac, normal);
    double nDotRayDir = vecDot(3, normal, ray->direction);
    if(nDotRayDir == 0){
        printf("parallel\n");
        return -1;
    }
    
    double d = vecDot(3, normal, a);
    double t = (vecDot(3, normal, ray->origin)) / nDotRayDir;
    t += d;
    if(t > 0){
        printf("triangle is behind\n");
        return -1;
    }
    
    double tTimesDir[3];
    double point[3];
    vecScale(3, t, ray->direction, tTimesDir);
    vecAdd(3, tTimesDir, ray->origin, point);
    double perpindicular[3];
    
    double edgeZero[3];
    vecSubtract(3, b, a, edgeZero);
    double pointZero[3];
    vecSubtract(3, point, a, pointZero);
    vec3Cross(edgeZero, pointZero, perpindicular);
    if(vecDot(3, normal, perpindicular) < 0){
        //printf("P is on the right side\n");
        return -1;
    }
    
    double edgeOne[3];
    vecSubtract(3, c, b, edgeOne);
    double pointOne[3];
    vecSubtract(3, point, b, pointOne);
    vec3Cross(edgeOne, pointOne, perpindicular);
    if(vecDot(3, normal, perpindicular) < 0){
        //printf("P is on the right side\n");
        return -1;
    }
        
    double edgeTwo[3];
    vecSubtract(3, a, c, edgeTwo);
    double pointTwo[3];
    vecSubtract(3, point, a, pointTwo);
    vec3Cross(edgeTwo, pointTwo, perpindicular);
    if(vecDot(3, normal, perpindicular) < 0){
        //printf("P is on the right side\n");
        return -1;
    }
        
    ray->normal[0] = normal[0];
    ray->normal[1] = normal[1];
    ray->normal[2] = normal[2];
    ray->intersection[0] = point[0];
    ray->intersection[1] = point[1];
    ray->intersection[2] = point[2];
    
    makePointVary(vary, a, b, c, point);
    
    printf("Intersection!\n");
    printf("a: %f, %f, %f\n", a[0], a[1], a[2]);
    printf("b: %f, %f, %f\n", b[0], b[1], b[2]);
    printf("c: %f, %f, %f\n", c[0], c[1], c[2]);
    return t;
}

int makeCounterClockwise(rayRay *ray, double a[], double b[], double c[], pointVary *vary){
        if(a[0] < b[0]){
            if(a[0] <= c[0])
                return rayTriangleIntersection(ray, a, b, c, vary);
            else
                return rayTriangleIntersection(ray, c, a, b, vary);
        }
        else if (b[0] < a[0]){
            if(b[0] < c[0])
                return rayTriangleIntersection(ray, b, c, a, vary);
            else
                return rayTriangleIntersection(ray, c, a, b, vary);
        }
        else{
            if(a[0] < c[0])
                return rayTriangleIntersection(ray, b, c, a, vary);
            else
                return rayTriangleIntersection(ray, c, a, b, vary);
        }
    }