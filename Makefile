recursive: 851mainRecursive.c
	clang 851mainRecursive.c 000pixel.o -lglfw -framework opengl

recursive.o: 851mainRecursive.c
	clang 851mainRecursive.c 000pixel.o -lglfw -framework opengl; ./a.out

effect: 861effect.c
	clang 861effect.c 000pixel.o -lglfw -framework opengl

effect.o: 861effect.c
	clang 861effect.c 000pixel.o -lglfw -framework opengl; ./a.out

reflection: 840mainReflection.c
	clang 840mainReflection.c 000pixel.o -lglfw -framework opengl

reflection.o: 840mainReflection.c
	clang 840mainReflection.c 000pixel.o -lglfw -framework opengl; ./a.out

run: a.out
	./a.out