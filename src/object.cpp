#include "object.h"
#include "defs.h"

Object::Object(SDL_Renderer *s)
{
    this->R = 0xFF;
    this-> G = 0x00;
    this-> B = 0xFF;
    this-> A = 0xFF;
    this->s = s;

    this->x = 0;
    this->y = 0;
}

void Object::set_color(Uint8 R, Uint8 G, Uint8 B, Uint8 A)
{
    this->R = R;
    this-> G = G;
    this-> B = B;
    this-> A = A;

}

void Object::set_coords(int x, int y)
{
    this->x = x;
    this->y = y;
}

void Object::render(std::string shape, std::vector<int> args)
{
    this->points.clear();

    if(shape == "square")
        this->render_square(this->x, this->y, args[0]);
    if(shape == "circle")
        this->render_circle(this->x, this->y, args[0]);
    if(shape == "rectangle")
        this->render_rectangle(this->x, this->y, args[0], args[1]);
    if(shape == "triangle")
        this->render_triangle(this->x, this->y, args[0], args[1], args[2]);
}

void Object::render_square(int x, int y, int side_length)
{
    SDL_SetRenderDrawColor(s, this->R, this->G, this->B, this->A);
    for(int i = 0;i < side_length;i++)
    {
        for(int j = 0;j < side_length;j++)
        {
            SDL_RenderDrawPoint(s, x + i, y + j);
            this->points.push_back(std::make_pair(x + i, y + j));
        }
    }
    SDL_SetRenderDrawColor(s, 0x00, 0x00, 0x00, 0xFF);
}

double Object::get_distance(int x1, int y1, int x2, int y2)
{
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

void Object::render_circle(int x, int y, int radius)
{
    using namespace std;
    SDL_SetRenderDrawColor(s, this->R, this->G, this->B, this->A);
    int x_min = max(0, x - radius);
    int y_min = max(0, y - radius);

    int x_max = min(WIDTH, x + radius);
    int y_max = min(HEIGHT, y + radius);

    for(int i = x_min;i <= x_max;i++)
    {
        for(int j = y_min;j <= y_max;j++)
        {
            if(Object::get_distance(i, j, x, y) <= radius)
            {
                SDL_RenderDrawPoint(s, i, j);
                this->points.push_back(make_pair(i, j));
            }

        }
    }

    SDL_SetRenderDrawColor(s, 0x00, 0x00, 0x00, 0xFF);

}

void Object::render_rectangle(int x, int y, int l, int L)
{
    SDL_SetRenderDrawColor(s, this->R, this->G, this->B, this->A);
    for(int i = 0;i < l;i++)
    {
        for(int j = 0;j < L;j++)
        {
            SDL_RenderDrawPoint(s, x + i, y + j);
            this->points.push_back(std::make_pair(x + i, y + j));
        }
    }
    SDL_SetRenderDrawColor(s, 0x00, 0x00, 0x00, 0xFF);
}

void Object::render_triangle(int x, int y, int s1, int s2, int s3)
{

}
