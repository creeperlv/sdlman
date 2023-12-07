# sdlman

A simple SDL-based document reader for MiyooCFW and RetroFW devices.

Libraries used: `SDL`, `SDL_image` and `SDL_ttf`.

To build with toolchains, just specify the C compiler with CC environment. e.g.:

```
CC=/opt/miyoo/bin/arm-miyoo-linux-musleabi-gcc ./build.sh
```

Currently there are three control keymaps:

- KEYMAP_00 : MiyooCFW on PowKiddy V90 (Default)

- KEYMAP_01 : RetroFW on LDK (Horizontal)

- KEYMAP_02 : For PC (Enter for confirm, Backspace for back, Escape to exit)

To configure keymap, compile with `KEY` environment. e.g.:

```
KEY=KEYMAP_01 ./build.sh
```
