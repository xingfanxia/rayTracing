recursive: 850mainRecursive.c
	clang 850mainRecursive.c 000pixel.o -lglfw -framework opengl

recursive.o: 850mainRecursive.c
	clang 850mainRecursive.c 000pixel.o -lglfw -framework opengl; ./a.out

effect: 861effect.c
	clang 861effect.c 000pixel.o -lglfw -framework opengl

effect.o: 861effect.c
	clang 861effect.c 000pixel.o -lglfw -framework opengl; ./a.out