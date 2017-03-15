typedef struct camCamera camCamera;
struct camCamera {
    double cameraRotation[3][3];
    double cameraTranslation[3];
    double viewing[4][4];
};

/* Taken from the software engine renRenderer */
void camLookAt(camCamera *cam, double target[3], double rho, double phi, 
        double theta) {
    double z[3], y[3], yStd[3] = {0.0, 1.0, 0.0}, zStd[3] = {0.0, 0.0, 1.0};
    vec3Spherical(1.0, phi, theta, z);
    vec3Spherical(1.0, M_PI / 2.0 - phi, theta + M_PI, y);
    mat33BasisRotation(yStd, zStd, y, z, cam->cameraRotation);
    vecScale(3, rho, z, cam->cameraTranslation);
    vecAdd(3, target, cam->cameraTranslation, cam->cameraTranslation);
}

/* updates the four by four viewing matrix */
void camUpdateViewing(camCamera *cam) {
    camLookAt(cam, camTarget, camRho, camPhi, camTheta);   
    mat44InverseIsometry(cam->cameraRotation, cam->cameraTranslation, cam->viewing);
}