typedef struct sphereSphere sphereSphere;
struct sphereSphere{
    double position[3];
    double color[3];
    double radius;
    double reflection;
};

/* Initializes the sphere with all of the relevant information. the reflection value
has to be between 0 and 1 so there's a check for it. */
void sphereInitialize(sphereSphere *sphere, double pos[3], double col[3], double r, double reflect){
    sphere->position[0] = pos[0];
    sphere->position[1] = pos[1];
    sphere->position[2] = pos[2];
    sphere->color[0] = col[0];
    sphere->color[1] = col[1];
    sphere->color[2] = col[2];
    sphere->radius = r;
    if(reflect > 1.0)
        sphere->reflection = 1.0;
    else if(reflect < 0.0)
        sphere->reflection = 0.0;
    else
        sphere->reflection = reflect;
}