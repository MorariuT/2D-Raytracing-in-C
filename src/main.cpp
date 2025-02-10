#include <iostream>
#include <vector>
#include <algorithm>
#include "SDL2/SDL.h"
#include <stdio.h>

#define WIDTH 750
#define HEIGHT 750
#define SQ_SIDE 100
#define NUMBER_OF_RAYS 500

using namespace std;

vector<vector<pair<int, int> > > objects;

void render_square(SDL_Renderer *s, int x, int y, int side_length, bool add_to_list)
{
    vector<pair<int, int> > obj;
    SDL_SetRenderDrawColor(s, 0xFF, 0xFF, 0xFF, 0xFF);
    for(int i = 0;i < side_length;i++)
    {
        for(int j = 0;j < side_length;j++)
        {
            SDL_RenderDrawPoint(s, x + i, y + j);
            if(add_to_list) obj.push_back(make_pair(x + i, y + j));
        }
    }
    if(add_to_list) 
    {
        // sort(obj.begin(), obj.end());
        objects.push_back(obj);
    }
    SDL_SetRenderDrawColor(s, 0x00, 0x00, 0x00, 0xFF);
}

double get_distance(int x1, int y1, int x2, int y2)
{
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

void render_circle(SDL_Renderer *s, int x, int y, int radius, bool add_to_list)
{
    vector<pair<int, int> > obj;
    SDL_SetRenderDrawColor(s, 0xFF, 0x00, 0xFF, 0xFF);
    int x_min = max(0, x - radius);
    int y_min = max(0, y - radius);

    int x_max = min(WIDTH, x + radius);
    int y_max = min(HEIGHT, y + radius);

    for(int i = x_min;i <= x_max;i++)
    {
        for(int j = y_min;j <= y_max;j++)
        {
            if(get_distance(i, j, x, y) <= radius) 
            {
                SDL_RenderDrawPoint(s, i, j);
                if(add_to_list) obj.push_back(make_pair(i, j));
            }
            
        }
    }
    if(add_to_list) 
    {
        // sort(obj.begin(), obj.end());
        objects.push_back(obj);
    }
    SDL_SetRenderDrawColor(s, 0x00, 0x00, 0x00, 0xFF);
}

bool is_in_objects(int x, int y)
{
    for(int i = 0;i < objects.size();i++)
    {
        if(binary_search(objects[i].begin(), objects[i].end(), make_pair(x, y))) return true;
    }
    return false;
}

void render_functions(SDL_Renderer *s, vector<double> &diff_x1, vector<double> &diff_y1, double x2, double y2)
{
    SDL_SetRenderDrawColor(s, 0, 0xFF, 0xFF, 0xFF);

    for(int i = 0;i < diff_x1.size();i++)
    {
        double step_x = diff_x1[i];
        double step_y = diff_y1[i];

        double x = x2;
        double y = y2;

        if(step_x <= 0 and step_y <= 0)
        {
            while(x > 0 and y > 0)
            {
                x += step_x;
                y += step_y;

                if(is_in_objects(x, y)) break;

                SDL_RenderDrawPoint(s, x, y);
            }
        }

        x = x2;
        y = y2;

        if(step_x >= 0 and step_y <= 0)
        {
            while(x < WIDTH and y > 0)
            {
                x += step_x;
                y += step_y;
                if(is_in_objects(x, y)) break;


                SDL_RenderDrawPoint(s, x, y);
            }
        }

        x = x2;
        y = y2;

        if(step_x <= 0 and step_y >= 0)
        {
            while(x > 0 and y < HEIGHT)
            {
                x += step_x;
                y += step_y;
                if(is_in_objects(x, y)) break;


                SDL_RenderDrawPoint(s, x, y);
            }
        }

        x = x2;
        y = y2;

        if(step_x >= 0 and step_y >= 0)
        {
            while(x < WIDTH and y < HEIGHT)
            {
                x += step_x;
                y += step_y;
                if(is_in_objects(x, y)) break;


                SDL_RenderDrawPoint(s, x, y);
            }
        }
    }
    SDL_SetRenderDrawColor(s, 0x00, 0x00, 0x00, 0xFF);
}

int main() 
{    
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("2d raytracing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *s = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC) ;

    bool running = true;
    SDL_Event event;

    int xMouse = 1;
    int yMouse = 1;

    int xSquare = 50;
    bool Square_direction = true;


    while (running) 
    {
        cout << xMouse << " " << yMouse << '\n';
        while (SDL_PollEvent(&event)) 
        {
            if (event.type == SDL_QUIT) 
            {
                running = false;
            }
            if(event.type == SDL_MOUSEMOTION)
            {
                SDL_GetMouseState(&xMouse,&yMouse);
            }
        }        
       
        SDL_RenderClear(s);

        render_square(s, xSquare, 500, SQ_SIDE, true);
        render_circle(s, 250, 400, 40, true);



        vector<double> xMs;
        vector<double> yMs;

        int nr_rays = sqrt(NUMBER_OF_RAYS);

        for(int i = -(nr_rays / 2);i <= nr_rays/2;i++)
        {
            for(int j = -(nr_rays / 2);j <= nr_rays/2;j++)
            {
                if(i == 0 and j == 0) continue;
                xMs.push_back(i);
                yMs.push_back(j);
            }
        }


        render_functions(s, xMs, yMs, xMouse, yMouse);

        render_circle(s, xMouse, yMouse, 40, false);


        SDL_RenderPresent(s);
        SDL_RenderClear(s);
        objects.clear();

        if(Square_direction) xSquare += 2;
        else xSquare -= 2;

        if(xSquare == WIDTH - SQ_SIDE) Square_direction = false;
        else if(xSquare == 0) Square_direction = true;
    }

    return 0;
}