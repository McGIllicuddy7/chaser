make: main.cpp runtime.cpp entity.cpp player.cpp box.cpp coltree.cpp runtime_texture.cpp
	g++ main.cpp  runtime.cpp entity.cpp player.cpp box.cpp coltree.cpp runtime_texture.cpp -l raylib -std=c++20 -fsanitize=address -g3
