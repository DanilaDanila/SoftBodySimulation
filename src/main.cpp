#include "phys.h"
#include "renderer.h"
#include <cassert>
#include <chrono>
#include <cmath>
#include <memory>
#include <thread>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 640

#define _2PI 2 * 3.1415926

const Vec2 pix2coords(int x, int y) {
  return Vec2(float(x) / WINDOW_WIDTH, 1. - float(y) / WINDOW_HEIGHT);
}

void drawAndExit(Window *window, const Body &body) {
  window->Clear();
  window->drawBody(&body);
  window->Present();

  SDL_Delay(2000);
  exit(1);
}

void NGon(BodyCreator &bc, int N, const Vec2 &center, float radius) {
  bc.clear();
  for (int i = 0; i < N; ++i) {
    const Vec2 dp =
        Vec2(radius * cos(i * _2PI / N), radius * sin(i * _2PI / N));
    const Vec2 p = center + dp;

    bc.addPoint(p);
  }

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (i == j) {
        continue;
      }

      bc.addEdge(i, j);
    }
  }
}

int main(int argc, char *args[]) {
  Sandbox sandbox;
  sandbox.setAcceleration(Vec2(.0f, .1f));

  BodyCreator bcreator;

  // some body
  bcreator.addPoint(Vec2(.4f, .4f));
  bcreator.addPoint(Vec2(.6f, .4f));
  bcreator.addPoint(Vec2(.6f, .6f));
  bcreator.addPoint(Vec2(.4f, .6f));
  auto bd = bcreator.createBody();
  auto p = Point(Vec2(.5, .5));
  assert(isPointInsideBody(&p, &bd));

  // bottom border
  bcreator.clear();
  bcreator.addPoint(pix2coords(0, 0));
  bcreator.addPoint(pix2coords(WINDOW_WIDTH, 0));
  bcreator.addPoint(pix2coords(WINDOW_WIDTH, 120));
  bcreator.addPoint(pix2coords(0, 120));
  // drawAndExit(&*window, bcreator.createBody());
  sandbox.addStaticBody(bcreator.createBody());

  // top border
  bcreator.clear();
  bcreator.addPoint(pix2coords(0, WINDOW_HEIGHT));
  bcreator.addPoint(pix2coords(WINDOW_WIDTH, WINDOW_HEIGHT));
  bcreator.addPoint(pix2coords(WINDOW_WIDTH, WINDOW_HEIGHT - 20));
  bcreator.addPoint(pix2coords(0, WINDOW_HEIGHT - 20));
  // drawAndExit(&*window, bcreator.createBody());
  sandbox.addStaticBody(bcreator.createBody());

  // left border
  bcreator.clear();
  bcreator.addPoint(pix2coords(0, 0));
  bcreator.addPoint(pix2coords(20, 0));
  bcreator.addPoint(pix2coords(20, WINDOW_HEIGHT));
  bcreator.addPoint(pix2coords(0, WINDOW_HEIGHT));
  // drawAndExit(&*window, bcreator.createBody());
  sandbox.addStaticBody(bcreator.createBody());

  // right border
  bcreator.clear();
  bcreator.addPoint(pix2coords(WINDOW_WIDTH, 0));
  bcreator.addPoint(pix2coords(WINDOW_WIDTH - 20, 0));
  bcreator.addPoint(pix2coords(WINDOW_WIDTH - 20, WINDOW_HEIGHT));
  bcreator.addPoint(pix2coords(WINDOW_WIDTH, WINDOW_HEIGHT));
  // drawAndExit(&*window, bcreator.createBody());
  sandbox.addStaticBody(bcreator.createBody());

  // 'ball'
  bcreator.clear();
  NGon(bcreator, 4, pix2coords(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2), .1f);
  // drawAndExit(&*window, bcreator.createBody());
  sandbox.addDynamicBody(bcreator.createBody());

  // std::shared_ptr<Window> window(new SDLWindow(640, 640));

  std::shared_ptr<Window> window(new SDLWindow(640, 640));

  SDL_Event event;

  bool running = true;
  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
      }
    }

    sandbox.update(.02);

    window->Clear();

    for (const Body &body : sandbox.getStaticBodys()) {
      window->drawBody(&body);
    }

    for (const Body &body : sandbox.getDynamicBodys()) {
      window->drawBody(&body);
    }

    window->Present();

    // SDL_Delay(20);
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
  }

  return 0;
}
