make: main.cpp runtime.cpp entity.cpp player.cpp box.cpp coltree.cpp
	g++ main.cpp  runtime.cpp entity.cpp player.cpp box.cpp coltree.cpp -l raylib -std=c++20 -fsanitize=address -g3
