Ray: 800mainRay.c
	clang 800mainRay.c 000pixel.o -lglfw -framework opengl; ./a.out

Lighting: 810mainLighting.c
	clang 810mainLighting.c 000pixel.o -lglfw -framework opengl; ./a.out

Depth: 820mainDepth.c
	clang 820mainDepth.c 000pixel.o -lglfw -framework opengl; ./a.out

specular: 830mainSpecular.c
	clang 830mainSpecular.c 000pixel.o -lglfw -framework opengl; ./a.out

recursive: 852mainRecursive.c
	clang 852mainRecursive.c 000pixel.o -lglfw -framework opengl; ./a.out

spaceworm: 861effect.c
	clang 861effect.c 000pixel.o -lglfw -framework opengl; ./a.out

camera: 860mainCamera.c
	clang 860mainCamera.c 000pixel.o -lglfw -framework opengl; ./a.out

spiral: 861mainCamera.c
	clang 861mainCamera.c 000pixel.o -lglfw -framework opengl; ./a.out

hexspiral: 862mainDNA.c
	clang 862mainDNA.c 000pixel.o -lglfw -framework opengl; ./a.out

refraction: 920mainRefraction.c
	clang 920mainRefraction.c 000pixel.o -lglfw -framework opengl; ./a.out

final: 940mainFinal.c
	clang 940mainFinal.c 000pixel.o -lglfw -framework opengl; ./a.out
	
run: a.out
	./a.out