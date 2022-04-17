#include "phys.h"
#include "renderer.h"
#include <memory>

int main(int argc, char *args[]) {
  BodyCreator bc;
  bc.addPoint(Vec2(.1f, .1f));
  bc.addPoint(Vec2(.9f, .1f));
  bc.addPoint(Vec2(.9f, .6f));
  bc.addPoint(Vec2(.5f, .8f));
  bc.addPoint(Vec2(.1f, .6f));

  Body body = bc.createBody();

  std::shared_ptr<Window> window(new SDLWindow(640, 640));

  window->Clear();
  window->drawBody(&body);
  window->Present();

  SDL_Delay(2000);
  return 0;
}
