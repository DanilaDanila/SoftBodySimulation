#include "renderer.h"
#include <memory>

int main(int argc, char *args[]) {
  std::unique_ptr<Window> window(new SDLWindow(640, 480));

  window->Clear();
  window->DrawLine(0, 0, 200, 200);
  window->DrawLine(200, 0, 0, 200);
  window->Present();

  SDL_Delay(2000);
  return 0;
}
