#ifndef __sdlman_constants
#define __sdlman_constants
#define TEXT_SIZE 12
#define LARGE_TEXT_SIZE 14
#define FOREGROUND 0xFF, 0xFF, 0xFF
#define TINT_COLOR 0x22, 0x88, 0xEE
#define FOREGROUND_SDLCOLOR                                                    \
  { FOREGROUND, 0x0 }

#define BACKGROUND 0x0, 0x0, 0x0
#define BACKGROUND_SDLCOLOR                                                    \
  { BACKGROUND, 0 }

#define TITLE "SDLMAN"
#define HINT "Up/Down - Line Scroll;Left/Right - Page Scroll"
#define SCREEN_FILE_BROWSER 0x00
#define SCREEN_READING 0x01
#define DEFAULT_LIEN_COUNT 1

#if defined(KEYMAP_01)
// RetroFW
#define KEY_EXIT SDLK_ESCAPE
#define KEY_GOBACK SDLK_LALT
#define KEY_SELECT_0 SDLK_LCTRL
#define KEY_SELECT_1 SDLK_RETURN
#define KEYMAP "Keymap RetroFW"

#elif defined(KEYMAP_02)
// PC
#define KEY_EXIT SDLK_ESCAPE
#define KEY_GOBACK SDLK_BACKSPACE
#define KEY_SELECT_0 SDLK_LCTRL
#define KEY_SELECT_1 SDLK_RETURN
#define KEYMAP "Keymap PC"
#else
//#elif defined(KEYMAP_00)
// Miyoo
#define KEY_EXIT SDLK_RCTRL
#define KEY_GOBACK SDLK_LCTRL
#define KEY_SELECT_0 SDLK_LALT
#define KEY_SELECT_1 SDLK_RETURN
#define KEYMAP "Keymap Miyoo"

#endif
#endif
