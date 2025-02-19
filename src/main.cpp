#include "SDL2/SDL.h"
#include "defs.h"
#include "object.h"
#include <algorithm>
#include <mutex>
#include <stdio.h>
#include <thread>
#include <vector>

using namespace std;

vector<vector<pair<int, int>>> objects;

bool is_in_objects(int x, int y) {
  for (int i = 0; i < objects.size(); i++) {
    if (binary_search(objects[i].begin(), objects[i].end(), make_pair(x, y)))
      return true;
  }
  return false;
}

void render_chunk(vector<double> &diff_x1, vector<double> &diff_y1, double x2,
                  double y2, int start, int end,
                  vector<SDL_Point> &local_points) {
  for (int i = start; i < end; i++) {
    double step_x = diff_x1[i] / 2;
    double step_y = diff_y1[i] / 2;
    double x = x2, y = y2;

    if (step_x <= 0 && step_y <= 0) {
      while (x > 0 && y > 0) {
        x += step_x;
        y += step_y;
        if (is_in_objects(x, y))
          break;
        local_points.push_back({static_cast<int>(x), static_cast<int>(y)});
      }
    }

    x = x2;
    y = y2;
    if (step_x >= 0 && step_y <= 0) {
      while (x < WIDTH && y > 0) {
        x += step_x;
        y += step_y;
        if (is_in_objects(x, y))
          break;
        local_points.push_back({static_cast<int>(x), static_cast<int>(y)});
      }
    }

    x = x2;
    y = y2;
    if (step_x <= 0 && step_y >= 0) {
      while (x > 0 && y < HEIGHT) {
        x += step_x;
        y += step_y;
        if (is_in_objects(x, y))
          break;
        local_points.push_back({static_cast<int>(x), static_cast<int>(y)});
      }
    }

    x = x2;
    y = y2;
    if (step_x >= 0 && step_y >= 0) {
      while (x < WIDTH && y < HEIGHT) {
        x += step_x;
        y += step_y;
        if (is_in_objects(x, y))
          break;
        local_points.push_back({static_cast<int>(x), static_cast<int>(y)});
      }
    }
  }
}

void render_functions(SDL_Renderer *s, vector<double> &diff_x1,
                      vector<double> &diff_y1, double x2, double y2) {
  SDL_SetRenderDrawColor(s, 0, 0xFF, 0xFF, 0xFF);

  int num_threads = 8;
  vector<thread> threads;
  int totalRays = diff_x1.size();
  int chunk_size = totalRays / num_threads;

  vector<vector<SDL_Point>> thread_points(num_threads);

  for (int i = 0; i < num_threads; i++) {
    int start = i * chunk_size;
    int end = (i + 1) * chunk_size;

    threads.emplace_back([=, &diff_x1, &diff_y1, &thread_points]() {
      render_chunk(diff_x1, diff_y1, x2, y2, start, end, thread_points[i]);
    });
  }

  for (auto &t : threads)
    t.join();

  vector<SDL_Point> all_points;
  for (auto &vec : thread_points) {
    all_points.insert(all_points.end(), vec.begin(), vec.end());
  }

  SDL_RenderDrawPoints(s, all_points.data(), all_points.size());
  SDL_SetRenderDrawColor(s, 0x00, 0x00, 0x00, 0xFF);
}

int main() {
  SDL_Init(SDL_INIT_VIDEO);

  SDL_Window *window = SDL_CreateWindow(
      "2d raytracing", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH,
      HEIGHT, SDL_WINDOW_SHOWN);
  SDL_Renderer *s = SDL_CreateRenderer(
      window, 0, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

  bool running = true;
  SDL_Event event;

  int xMouse = 1;
  int yMouse = 1;

  int xSquare = 50;
  bool Square_direction = true;

  Object ms = Object(s);
  vector<int> args_ms = {50};
  ms.set_color(0x00, 0x00, 0xFF, 0xFF);

  Object cursor = Object(s);
  vector<int> args_cr = {50};
  cursor.set_color(0xFF, 0xFF, 0xFF, 0xFF);

  Object moving_cube = Object(s);
  vector<int> args_mc = {SQ_SIDE};
  moving_cube.set_color(0x00, 0xFF, 0x00, 0xFF);

  Object obstacle_circle = Object(s);
  vector<int> args_oc = {75};
  obstacle_circle.set_coords(400, 100);
  obstacle_circle.set_color(0xFF, 0x00, 0x00, 0xFF);

  vector<double> xMs;
  vector<double> yMs;

  for (int i = 0; i < NUMBER_OF_RAYS + 10; i++) {
    double angle = (2 * 3.1416 * i) / NUMBER_OF_RAYS;
    double dx = cos(angle);
    double dy = sin(angle);

    xMs.push_back(dx);
    yMs.push_back(dy);
  }

  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
      }
      if (event.type == SDL_MOUSEMOTION) {
        SDL_GetMouseState(&xMouse, &yMouse);
      }
    }

    SDL_RenderClear(s);

    moving_cube.set_coords(xSquare, 400);
    moving_cube.render("square", args_mc);
    objects.push_back(moving_cube.points);

    ms.set_coords(100, 100);
    ms.render("square", args_ms);
    objects.push_back(ms.points);

    obstacle_circle.render("circle", args_oc);
    objects.push_back(obstacle_circle.points);

    render_functions(s, xMs, yMs, xMouse, yMouse);

    cursor.set_coords(xMouse, yMouse);
    cursor.render("circle", args_cr);

    SDL_RenderPresent(s);
    SDL_RenderClear(s);
    objects.clear();

    if (Square_direction)
      xSquare += 2;
    else
      xSquare -= 2;

    if (xSquare == WIDTH - SQ_SIDE)
      Square_direction = false;
    else if (xSquare == 0)
      Square_direction = true;
  }

  SDL_DestroyRenderer(s);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
