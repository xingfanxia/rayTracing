typedef struct triangleTriangle triangleTriangle;
struct triangleTriangle{
    double aPosition[3];
    double aVarying[6];
    double bPosition[3];
    double bVarying[6];
    double cPosition[3];
    double cVarying[6];
};

void triangleInitialize(triangleTriangle *tri, double apos[3], double acol[3], 
    double bpos[3], double bcol[3], double cpos[3], double ccol[3]){
    tri->aPosition[0] = apos[0];
    tri->aPosition[1] = apos[1];
    tri->aPosition[2] = apos[2];
    tri->aVarying[3] = acol[0];
    tri->aVarying[4] = acol[1];
    tri->aVarying[5] = acol[2];
    
    tri->bPosition[0] = bpos[0];
    tri->bPosition[1] = bpos[1];
    tri->bPosition[2] = bpos[2];
    tri->bVarying[3] = bcol[0];
    tri->bVarying[4] = bcol[1];
    tri->bVarying[5] = bcol[2];
    
    tri->cPosition[0] = cpos[0];
    tri->cPosition[1] = cpos[1];
    tri->cPosition[2] = cpos[2];
    tri->cVarying[3] = ccol[0];
    tri->cVarying[4] = ccol[1];
    tri->cVarying[5] = ccol[2];
}