#include "sdlman.h"
//
int main() {
  SetLogFile(stdout);
  WriteLine("Init");
  Log("KeyMap:%s\n", KEYMAP);
  InitSDL();
  InitBuffer();
  InitASCII();
  SetDir(".");
  if (!InitUIData()) {
    return 0x1001;
  }

  DrawUI();
  while (!ShouldClose()) {
    UpdateInput();
    DrawUI();
  }
  return 0;
}
