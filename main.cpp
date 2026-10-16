/*
  Copyright (c) 2014 John Miner

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/
#include <algorithm>
#include <cmath>
#include <iostream>
#include "point2d.h"
#include <SDL2/SDL.h>
#include <vector>

using namespace std;

vector<Point2D> points;
SDL_Renderer *renderer;
SDL_Window *screen;

void render(void);

bool released = false;
bool curveonly = false;

void load()
{
  screen = SDL_CreateWindow("Bezier", SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED, 800, 800, 0);
  if (screen == NULL) {
    fprintf(stderr, "Failed to create window\r\n");
    exit(1);
  }
  renderer = SDL_CreateRenderer(screen, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
  if (renderer == NULL) {
    fprintf(stderr, "Failed to create renderer\r\n");
    exit(1);
  }
  SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
  //SDL_RenderSetLogicalSize(renderer, 800, 800);
}

void input()
{
  int mouseX = 0;
  int mouseY = 0;
  Uint32 buttons;
  SDL_Event e;
  SDL_PumpEvents();
  SDL_PollEvent(&e);
  if (e.type == SDL_QUIT)
    exit(0);
  buttons = SDL_GetMouseState(&mouseX, &mouseY);
  if((buttons & SDL_BUTTON_LMASK) == 0) {
    released = true;
  }
  if(released == true && (buttons & SDL_BUTTON_LMASK) != 0) {
    Point2D p(mouseX, mouseY);
    points.push_back(p);
    released = false;
  }
}

void atExitQuit()
{
  if (renderer != NULL)
    SDL_DestroyRenderer(renderer);
  if (screen != NULL)
    SDL_DestroyWindow(screen);
  SDL_Quit();
}

int main(int argc, char** argv)
{	
  if (argc >= 2 && strncmp(argv[1], "--curve-only", 12) == 0)
    curveonly = true;
  volatile int volatwhil = 0;
#ifndef _WIN32
  signal(SIGPIPE, SIG_IGN);
#endif
  SDL_Init(SDL_INIT_VIDEO); /* TODO check return value */
  atexit(atExitQuit);
  load();
  while(1) {
    if (volatwhil == 0)
      volatwhil = 1;
    input();
    render();
  }
	return 0;
}

int factorial(int n)
{
    if( n <= 0 )
        return 1;
    return n * factorial(n - 1);
}

int nChoosei(int n, int i)
{
    return factorial(n) / (factorial(i) * factorial(n - i));
}

void drawLine(int x1, int y1, int x2, int y2)
{
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void drawBezier(vector<Point2D> points)
{
    int n = points.size();

    if( n <= 1 )
        return;

    Point2D b;
    Point2D last;
    b.x = points[0].x;
    b.y = points[0].y;
    last.x = 0;
    last.y = 0;
    vector<Point2D> bezier;

    double t = 0.0;

    for(t = 0.0; t <= 1.0; t += .001)
    {
        last.x = 0;
        last.y = 0;
        for(int i = 0; i < n; ++i)
        {
            last.x += nChoosei(n - 1, i) * pow(1.0 - t, n - 1 - i) * pow(t, i) * points[i].x;
            last.y += nChoosei(n - 1, i) * pow(1.0 - t, n - 1 - i) * pow(t, i) * points[i].y;
        }
        drawLine(b.x, b.y, last.x, last.y);
        b.x = last.x;
        b.y = last.y;
    }

    int numPoints = bezier.size();
}

void drawBezierSegments()
{
    if(points.size() <= 2)
        return;
    int n = points.size();
    for(int i = 2; i < n; i += 2)
    {
      vector<Point2D> segment(3);
      copy(points.begin() + i - 2, points.begin() + i + 1, segment.begin());
      drawBezier(segment);
    }
}

void render()
{
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_Point sdlpoints[255];
  int i = 0;
  for( vector<Point2D>::iterator it = points.begin(); it != points.end(); ++it )
  {
    SDL_Point p = { it->x, it->y };
    sdlpoints[i] = p;
    i++;
  }
  if (curveonly == false)
    SDL_RenderDrawLines(renderer, sdlpoints, points.size());
  drawBezier(points);
  //drawBezierSegments();
  SDL_RenderPresent(renderer);
}

