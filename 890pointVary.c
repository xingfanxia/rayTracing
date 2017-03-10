typedef struct pointVary pointVary;
struct pointVary{
    double position[3];
    double color[3];
};

void pointVaryInitialize(pointVary *point, double pos[3], double col[3]){
    point->position[0] = pos[0];
    point->position[1] = pos[1];
    point->position[2] = pos[2];
    point->color[0] = col[0];
    point->color[1] = col[1];
    point->color[2] = col[2];
}

void makePointA(pointVary *vary, double a[], double b[], double c[], double point[]){
    int varyDim = 6;
    if(point[0] <= c[0]){
        double toInv[2][2] = {{(b[0] - a[0]), (c[0] - a[0])}, {(b[1] - a[1]), (c[1] - a[1])}};
        double xMinusA[2] = {(point[0] - a[0]), (point[1] - a[1])};
        double inv[2][2];
        if(mat22Invert(toInv, inv) <= 0)
            return;
        double pq[2];
        mat22Multiply(inv, xMinusA, pq);
                    
        double attr[varyDim];
        double pTimesBetaMinusAlpha[varyDim];
        vecSubtract(varyDim, b, a, pTimesBetaMinusAlpha);
        vecScale(varyDim, pq[0], pTimesBetaMinusAlpha, pTimesBetaMinusAlpha);
                    
        double qTimesGammaMinusAlpha[varyDim];
        vecSubtract(varyDim, c, a, qTimesGammaMinusAlpha);
        vecScale(varyDim, pq[1], qTimesGammaMinusAlpha, qTimesGammaMinusAlpha);
                    
        vecAdd(varyDim, a, pTimesBetaMinusAlpha, attr);
        vecAdd(varyDim, attr, qTimesGammaMinusAlpha, attr);
        
        vary->position[0] = attr[0];
        vary->position[1] = attr[1];
        vary->position[2] = attr[2];
        vary->color[0] = attr[3];
        vary->color[1] = attr[4];
        vary->color[2] = attr[5];
    }
    else{
        double toInv[2][2] = {{(b[0] - a[0]), (c[0] - a[0])}, {(b[1] - a[1]), (c[1] - a[1])}};
        double xMinusA[2] = {(point[0] - a[0]), (point[1] - a[1])};
        double inv[2][2];
        if(mat22Invert(toInv, inv) <= 0)
            return;
        double pq[2];
        mat22Multiply(inv, xMinusA, pq);
                    
        double attr[varyDim];
        double pTimesBetaMinusAlpha[varyDim];
        vecSubtract(varyDim, b, a, pTimesBetaMinusAlpha);
        vecScale(varyDim, pq[0], pTimesBetaMinusAlpha, pTimesBetaMinusAlpha);
                    
        double qTimesGammaMinusAlpha[varyDim];
        vecSubtract(varyDim, c, a, qTimesGammaMinusAlpha);
        vecScale(varyDim, pq[1], qTimesGammaMinusAlpha, qTimesGammaMinusAlpha);
                    
        vecAdd(varyDim, a, pTimesBetaMinusAlpha, attr);
        vecAdd(varyDim, attr, qTimesGammaMinusAlpha, attr);
        
        vary->position[0] = attr[0];
        vary->position[1] = attr[1];
        vary->position[2] = attr[2];
        vary->color[0] = attr[3];
        vary->color[1] = attr[4];
        vary->color[2] = attr[5];
    }
}

void makePointB(pointVary *vary, double a[], double b[], double c[], double point[]){
    int varyDim = 6;
    if(point[0] < b[0]){
        double toInv[2][2] = {{(b[0] - a[0]), (c[0] - a[0])}, {(b[1] - a[1]), (c[1] - a[1])}};
        double xMinusA[2] = {(point[0] - a[0]), (point[1] - a[1])};
        double inv[2][2];
        if(mat22Invert(toInv, inv) <= 0)
            return;
        double pq[2];
        mat22Multiply(inv, xMinusA, pq);
        
        double attr[varyDim];
        double pTimesBetaMinusAlpha[varyDim];
        vecSubtract(varyDim, b, a, pTimesBetaMinusAlpha);
        vecScale(varyDim, pq[0], pTimesBetaMinusAlpha, pTimesBetaMinusAlpha);
                    
        double qTimesGammaMinusAlpha[varyDim];
        vecSubtract(varyDim, c, a, qTimesGammaMinusAlpha);
        vecScale(varyDim, pq[1], qTimesGammaMinusAlpha, qTimesGammaMinusAlpha);
                    
        vecAdd(varyDim, a, pTimesBetaMinusAlpha, attr);
        vecAdd(varyDim, attr, qTimesGammaMinusAlpha, attr);
        
        vary->position[0] = attr[0];
        vary->position[1] = attr[1];
        vary->position[2] = attr[2];
        vary->color[0] = attr[3];
        vary->color[1] = attr[4];
        vary->color[2] = attr[5];
    }
    else{
        double toInv[2][2] = {{(b[0] - a[0]), (c[0] - a[0])}, {(b[1] - a[1]), (c[1] - a[1])}};
        double xMinusA[2] = {(point[0] - a[0]), (point[1] - a[1])};
        double inv[2][2];
        if(mat22Invert(toInv, inv) <= 0)
            return;
        double pq[2];
        mat22Multiply(inv, xMinusA, pq);
                    
        double attr[varyDim];
        double pTimesBetaMinusAlpha[varyDim];
        vecSubtract(varyDim, b, a, pTimesBetaMinusAlpha);
        vecScale(varyDim, pq[0], pTimesBetaMinusAlpha, pTimesBetaMinusAlpha);
                    
        double qTimesGammaMinusAlpha[varyDim];
        vecSubtract(varyDim, c, a, qTimesGammaMinusAlpha);
        vecScale(varyDim, pq[1], qTimesGammaMinusAlpha, qTimesGammaMinusAlpha);
                    
        vecAdd(varyDim, a, pTimesBetaMinusAlpha, attr);
        vecAdd(varyDim, attr, qTimesGammaMinusAlpha, attr);
        
        vary->position[0] = attr[0];
        vary->position[1] = attr[1];
        vary->position[2] = attr[2];
        vary->color[0] = attr[3];
        vary->color[1] = attr[4];
        vary->color[2] = attr[5];
    }
}

void makePointVary(pointVary *vary, double a[], double b[], double c[], double point[]){
    if(c[0] < b[0])
            makePointA(vary, a, b, c, point);
        else
            makePointB(vary, a, b, c, point);
}