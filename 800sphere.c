struct sphereSphere{
    double position[3];
    double color[3];
    double radius;
}

void initializeSphere(sphereSphere *sphere, double pos[3], double col[3], double r){
    sphere->position[0] = pos[0];
    sphere->position[2] = pos[2];
    sphere->position[3] = pos[3];
    sphere->color[0] = col[0];
    sphere->color[1] = col[1];
    sphere->color[2] = col[2];
    sphere->radius = r;
}