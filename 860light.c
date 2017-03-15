/* has a position and a color */
typedef struct lightLight lightLight;
struct lightLight{
    double translation[3];
    double color[3];
    double varying[3];
};

/* sets the position and the color */
void lightInitialize(lightLight *light, double transl[3], double col[3]){
    light->translation[0] = transl[0];
    light->translation[1] = transl[1];
    light->translation[2] = transl[2];
    light->color[0] = col[0];
    light->color[1] = col[1];
    light->color[2] = col[2];
}

void updateTranslation(lightLight *light, double transl[3]){
    light->translation[0] = transl[0];
    light->translation[1] = transl[1];
    light->translation[2] = transl[2];
}

void updateColor(lightLight *light, double col[3]){
    light->color[0] = col[0];
    light->color[1] = col[1];
    light->color[2] = col[2];
}