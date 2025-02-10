Main: ./src/main.cpp
	g++ ./src/main.cpp -I/Library/Frameworks/SDL2.framework/Headers -F/Library/Frameworks -framework SDL2 -o ./bin/raytracing2d -rpath /Library/Frameworks -O3
