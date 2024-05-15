make: coltree.cpp entity.cpp runtime_texture.cpp runtime.cpp main.cpp level_loader.cpp
	g++ -c coltree.cpp entity.cpp runtime_texture.cpp runtime.cpp main.cpp level_loader.cpp -std=c++20 -g3 -fsanitize=address
	ar rcs engine.a coltree.o entity.o runtime.o runtime_texture.o main.o level_loader.o
	rm *.o