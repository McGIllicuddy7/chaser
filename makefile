make: main.cpp runtime.cpp entity.cpp player.cpp box.cpp
	g++ main.cpp  runtime.cpp entity.cpp player.cpp box.cpp  -l raylib -std=c++20 -fsanitize=address -g3
