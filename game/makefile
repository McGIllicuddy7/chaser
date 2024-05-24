LFLAGS = 
ifeq ($(shell uname),Linux)
LFLAGS = -lm 
endif
ifeq ($(shell uname),Darwin)
LFLAGS = -lm -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL  
endif
make:bolt.cpp background.cpp manager.cpp bullet.cpp enemy.cpp init_script.cpp player.cpp weapons.cpp particles.cpp missile.cpp ../engine/engine.a 
	g++ bolt.cpp background.cpp manager.cpp bullet.cpp enemy.cpp init_script.cpp player.cpp weapons.cpp particles.cpp missile.cpp ../engine/engine.a ../raylib-master/raylib/libraylib.a $(LFLAGS)  -std=c++20 -o chaser -l profiler

