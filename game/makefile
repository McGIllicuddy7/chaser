LFLAGS = 
ifeq ($(shell uname),Linux)
LFLAGS = -lm 
endif
ifeq ($(shell uname),Darwin)
LFLAGS = -lm -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL  
endif

make: background.cpp manager.cpp bullet.cpp enemy.cpp init_script.cpp player.cpp weapons.cpp particles.cpp missile.cpp ../engine/engine.a 
	g++ background.cpp manager.cpp bullet.cpp enemy.cpp init_script.cpp player.cpp weapons.cpp particles.cpp missile.cpp ../engine/engine.a ../raylib-master/raylib/libraylib.a $(LFLAGS) -G3 -std=c++20 -fsanitize=address 
