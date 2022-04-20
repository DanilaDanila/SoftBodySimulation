#ifndef RENDERER_H
#define RENDERER_H

#include <phys.h>

#ifdef NO_SDL2_DIR
#include <SDL.h>
#else // NO_SDL2_DIR
#include <SDL2/SDL.h>
#endif // NO_SDL2_DIR

class Window {
public:
  virtual void Clear() = 0;

  virtual void DrawLine(int from_x, int from_y, int to_x, int to_y) = 0;

  virtual void DrawPoint(int x, int y) = 0;

  virtual void Present() = 0;

  virtual void drawBody(const Body *const) = 0;
};

class SDLWindow : public Window {
public:
  SDLWindow(int width = 640, int height = 480)
      : _width(width), _height(height) {
    SDL_Init(SDL_INIT_VIDEO);
    _window = SDL_CreateWindow("SoftBody", SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED, width, height,
                               SDL_WINDOW_SHOWN);
    _renderer = SDL_CreateRenderer(_window, -1, 0);
  }

  virtual void Clear() {
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
    SDL_RenderClear(_renderer);
    SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
  }

  virtual void DrawLine(int from_x, int from_y, int to_x, int to_y) {
    SDL_RenderDrawLine(_renderer, from_x, from_y, to_x, to_y);
  }

  virtual void DrawPoint(int x, int y) { SDL_RenderDrawPoint(_renderer, x, y); }

  virtual void Present() { SDL_RenderPresent(_renderer); }

  virtual void drawBody(const Body *const body) {
    for (int i = 0; i < body->points_count; ++i) {
      int j = (i + 1) % body->points_count;

      int from_x = body->points[i].current_position.x * _width;
      int from_y = body->points[i].current_position.y * _height;
      int to_x = body->points[j].current_position.x * _width;
      int to_y = body->points[j].current_position.y * _height;
      SDL_RenderDrawLine(_renderer, from_x, from_y, to_x, to_y);
    }
  }

  virtual ~SDLWindow() {
    SDL_DestroyWindow(_window);
    SDL_Quit();
  }

private:
  int _width;
  int _height;

  SDL_Window *_window;
  SDL_Renderer *_renderer;
};

#endif // RENDERER_H
