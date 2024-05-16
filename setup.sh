#/bin/bash
cd raylib-master 
cmake .
cd raylib
make
cd ..
cd ..

cd engine
make 
cd ..



cd game 
make 
cd ..

