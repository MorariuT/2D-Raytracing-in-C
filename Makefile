# VSCode won't use this file, but it's here if YOU want to use it.
Main: ./src/main.cpp
	g++ -std=c++11 ./src/main.cpp ./src/object.cpp -I/Library/Frameworks/SDL2.framework/Headers -F/Library/Frameworks -framework SDL2 -o ./bin/raytracing2d -rpath /Library/Frameworks -Ofast
