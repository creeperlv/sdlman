#ifndef __sdlman__
#define __sdlman__
#include "sdlman_constants.h"
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <dirent.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MIN(a, b) (a) > (b) ? (b) : (a)
#define MAX(a, b) (a) < (b) ? (b) : (a)
typedef struct __man_buffer {
  char *buffer;
  int W;
  int H;
  int S;
  int CurrentPos;
  int CurrentLineEnd;
  int CurrentPageEnd;
} man_buffer;
typedef man_buffer *ManBuffer;
typedef struct vec2 {
  int x;
  int y;
} Vec2;
typedef struct __uiData {
  size_t Scene;
  size_t MaxScene;
  int CurrentListSelection;
  int CurrentMaxListCount;
  int CurrentListCountPerPage;
} _UIData;
typedef _UIData *UIData;
bool InitUIData();
void SetDir(char *dir);
void GoNextPage();
void LoadBuffer();
void GoNextSegment();
void GoBack(bool IsPage);
void SetLogFile(FILE *output);
void WriteLine(char *str);
void Log(char *format, ...);
int InitSDL();
void DrawRectangle(int x, int y, int w, int h, int R, int G, int B);
int MeasureString(char *str, int Length);
int MeasureStringLarge(char *str, int Length);
void DrawString(char *str, int Length, int x, int y);
void DrawStringLarge(char *str, int Length, int x, int y);
void DrawUI();
void UpdateInput();
bool ShouldClose();
void InitASCII();
void InitBuffer();
#endif
