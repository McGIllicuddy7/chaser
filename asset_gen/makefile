 LFLAGS = 
ifeq ($(shell uname),Linux)
LFLAGS = -lm
endif
ifeq ($(shell uname),Darwin)
LFLAGS = -lm -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
endif
make: main.c
	gcc main.c ../raylib-master/raylib/libraylib.a $(LFLAGS) -g3 -std=c2x -fsanitize=address