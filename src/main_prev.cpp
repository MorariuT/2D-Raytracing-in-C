#include <iostream>
#include <vector>
#include <algorithm>
#include "SDL2/SDL.h"
#include <stdio.h>
#include "defs.h"
#include "object.h"

using namespace std;

vector<vector<pair<int, int> > > objects;

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
        double step_x = diff_x1[i] / 4;
        double step_y = diff_y1[i] / 4;

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

    Object ms = Object(s);
    vector<int> args_ms;
    args_ms.push_back(50);

    vector<int> args_cr;
    args_cr.push_back(50);



    vector<double> xMs;
    vector<double> yMs;

    int nr_rays = sqrt(NUMBER_OF_RAYS);

    for(int i = -(nr_rays / 2);i <= 0;i++)
    {
        for(int j = -(nr_rays / 2);j <= 0;j++)
        {
            if(i == 0 and j == 0) continue;
            xMs.push_back(i);
            yMs.push_back(j);
        }
    }



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

        ms.set_coords(100, 100);
        ms.render("square", args_ms);
        objects.push_back(ms.points);


        render_functions(s, xMs, yMs, xMouse, yMouse);

        Object cursor = Object(s);
        cursor.set_coords(xMouse, yMouse);

        cursor.render("circle", args_cr);

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
