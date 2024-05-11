#/bin/bash
cd engine
make 
cd ..

cd raylib-master 
cmake .
cd raylib
make
cd ..
cd ..

cd game 
make 
cd ..

