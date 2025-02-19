Main: ./src/main.cpp
<<<<<<< HEAD
	g++ -std=c++11 ./src/main.cpp ./src/object.cpp -I/Library/Frameworks/SDL2.framework/Headers -F/Library/Frameworks -framework SDL2 -o ./bin/raytracing2d -rpath /Library/Frameworks -Ofast
=======
	g++ ./src/main.cpp -I/Library/Frameworks/SDL2.framework/Headers -F/Library/Frameworks -framework SDL2 -o ./bin/raytracing2d -rpath /Library/Frameworks -O3
>>>>>>> c4b63d07266767917bde7913c6f140e0a32c1f65
