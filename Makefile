recursive: 851mainRecursive.c
	clang 851mainRecursive.c 000pixel.o -lglfw -framework opengl

recursive.o: 851mainRecursive.c
	clang 851mainRecursive.c 000pixel.o -lglfw -framework opengl; ./a.out

spaceworm: 861effect.c
	clang 861effect.c 000pixel.o -lglfw -framework opengl

spaceworm.o: 861effect.c
	clang 861effect.c 000pixel.o -lglfw -framework opengl; ./a.out

reflection: 840mainReflection.c
	clang 840mainReflection.c 000pixel.o -lglfw -framework opengl

reflection.o: 840mainReflection.c
	clang 840mainReflection.c 000pixel.o -lglfw -framework opengl; ./a.out

camera: 860mainCamera.c
	clang 860mainCamera.c 000pixel.o -lglfw -framework opengl

spiral: 861mainCamera.c
	clang 861mainCamera.c 000pixel.o -lglfw -framework opengl

hexspiral: 862mainDNA.c
	clang 862mainDNA.c 000pixel.o -lglfw -framework opengl
	
run: a.out
	./a.out