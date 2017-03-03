### CS 331 Computer Graphics Project: Ray Tracing to draw still images

### What we have done:
1. We can draw spheres in a world from a top down camera
2. We can draw spheres with correct lighting (Diffuse, specular and ambient).
3. We added depth control.

### To-Dos:
1. Do it recursively so we can go down infinityly.

### Issues:
1. We have distortions with spheres the further they are away from the center.
2. Shadow cast randomly. (before reflection)
3. Randoms lines.
4. Reflection color is not combined in the manner or not being acquired successfully.
5. Weird coarse engine.

### Algorithm
```c
for every pixel in the scene {
    RayInitialize(from view point, to the pixel);
    Intialize NearestDepth = infinity;
    nearestObj = null;

    to everyObj in the scene {
        if (the light intersects the obj) {
            if (intersctiondepth(t) < NearestDepth) {
                NearestDepth = t;
                NearestObj = thisObj;
            }
        }
    }

    if (NearestObj == null) {
        setSphereColor(this.color);
    } else {
        checkIfObjInShadow();
        if (color is reflective) {
            RayInitialize(reflectiveRay);
            RecursiveCall;
        } else if (color is refractive) {
            RayInitialize(refractiveRay);
            RecursiveCall;
        }
        calculateColor();
        setColor();
    }
}
```
