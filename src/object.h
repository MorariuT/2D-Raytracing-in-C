#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include "SDL2/SDL.h"
#include <stdio.h>

class Object
{
    public:
    SDL_Renderer *s;
    Uint8 R, G, B, A;

    int x, y;

    std::vector<std::pair<int, int> > points;

    Object(SDL_Renderer *s);
    void render(std::string shape, std::vector<int> args);
    void set_coords(int x, int y);
    void set_color(Uint8 R, Uint8 G, Uint8 B, Uint8 A);

    private:

    void render_square(int x, int y, int side_length);
    void render_circle(int x, int y, int radius);
    void render_rectangle(int x, int y, int l, int L);
    void render_triangle(int x, int y, int s1, int s2, int s3);

    double get_distance(int x1, int y1, int x2, int y2);

};