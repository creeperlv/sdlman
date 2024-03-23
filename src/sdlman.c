#include "sdlman.h"

FILE *__sdl_LogFile;

TTF_Font *font = NULL;
TTF_Font *font_Large;
SDL_Surface *screen = NULL;
SDL_Event event;

SDL_Surface *ASCII_FONT[256];
SDL_Surface *ASCII_FONT_LARGE[256];
ManBuffer buffer;
DIR *CurrentDir;
FILE *OpendFile;
char *OpendFileName;
void SetDir(char *dir) { CurrentDir = opendir(dir); }
Vec2 DetermineBufferSize(int W, int H, int CW, int CH) {
  Vec2 result = {W / CW, H / CH};
  return result;
}
UIData _ui_data;
bool WillLoadPage = false;
int Target_LineSegment_Count = DEFAULT_LIEN_COUNT;

void (*ProcessInput)(SDLKey) = NULL;
void ReaderUpdateInput(SDLKey k);
void FMUpdateInput(SDLKey k);

bool InitUIData() {
  _ui_data = malloc(sizeof(_UIData));
  if (_ui_data == NULL) {
    return false;
  }
  _ui_data->Scene = 0;
  _ui_data->CurrentListSelection = 0;
  _ui_data->CurrentMaxListCount = 0;
  _ui_data->CurrentListCountPerPage = 0;
  _ui_data->MaxScene = 2;
  ProcessInput = FMUpdateInput;
  return true;
}
void SelectDirectEntry(int id) {
  rewinddir(CurrentDir);
  int count = 0;
  struct dirent *ep;
  while ((ep = readdir(CurrentDir)) != NULL) {
    if (count == id) {
      if (ep->d_type == DT_DIR) {
        chdir(ep->d_name);
        closedir(CurrentDir);
        CurrentDir = opendir(".");
      } else if (ep->d_type == DT_REG) {
        if (OpendFile != NULL) {
          fclose(OpendFile);
        }
        OpendFile = fopen(ep->d_name, "rb");
        ProcessInput = ReaderUpdateInput;
        OpendFileName = ep->d_name;
        WillLoadPage = true;
        buffer->CurrentPos = 0;
        _ui_data->Scene = SCREEN_READING;
      }
    }
    count++;
  }
}
void FMUpdateInput(SDLKey k) {
  switch (k) {

  case SDLK_UP:
    _ui_data->CurrentListSelection--;
    if (_ui_data->CurrentListSelection < 0)
      _ui_data->CurrentListSelection = _ui_data->CurrentMaxListCount - 1;
    break;
  case SDLK_DOWN:

    _ui_data->CurrentListSelection++;
    if (_ui_data->CurrentListSelection >= _ui_data->CurrentMaxListCount)
      _ui_data->CurrentListSelection = 0;

    if (_ui_data->CurrentListSelection < 0)
      _ui_data->CurrentListSelection = 0;
    break;
  case SDLK_LEFT: {
    if (_ui_data->CurrentListSelection == 0) {
      _ui_data->CurrentListSelection = _ui_data->CurrentMaxListCount - 1;
    } else {

      _ui_data->CurrentListSelection -= _ui_data->CurrentListCountPerPage;
      if (_ui_data->CurrentListSelection < 0)
        _ui_data->CurrentListSelection = 0;
    }
  } break;
  case SDLK_RIGHT: {
    if (_ui_data->CurrentListSelection == _ui_data->CurrentMaxListCount - 1) {
      _ui_data->CurrentListSelection = 0;
    } else {

      _ui_data->CurrentListSelection += _ui_data->CurrentListCountPerPage;
      if (_ui_data->CurrentListSelection >= _ui_data->CurrentMaxListCount)
        _ui_data->CurrentListSelection = _ui_data->CurrentMaxListCount - 1;
    }

  } break;
  case KEY_GOBACK: {
    chdir("..");
    closedir(CurrentDir);
    CurrentDir = opendir(".");
  } break;
  case KEY_SELECT_0:
  case KEY_SELECT_1: {
    SelectDirectEntry(_ui_data->CurrentListSelection);
  } break;
  default:
    break;
  }
}
void ReaderUpdateInput(SDLKey k) {
  switch (k) {

  case SDLK_UP:
    GoBack(false);
    break;
  case SDLK_DOWN:
    GoNextSegment();
    break;
  case SDLK_LEFT:
    GoBack(true);
    break;
  case SDLK_RIGHT:
    GoNextPage();
    break;
  case KEY_GOBACK:
    _ui_data->Scene = SCREEN_FILE_BROWSER;
    ProcessInput = FMUpdateInput;
    break;
  default:
    break;
  }
}
void InitBuffer() {
  WriteLine("InitBuffer...");
  buffer = malloc(sizeof(man_buffer));
  Vec2 buf_size = DetermineBufferSize(300, 240 - LARGE_TEXT_SIZE - TEXT_SIZE,
                                      (TEXT_SIZE - 2) * 0.6, TEXT_SIZE);
  int BufSize = buf_size.x * buf_size.y;
  buffer->buffer = malloc(sizeof(char) * BufSize);
  buffer->W = buf_size.x;
  buffer->H = buf_size.y;
  buffer->S = BufSize;
  WriteLine("InitBuffer...Done");
  Log("Determined Buffer:W=%d,H=%d,Size=%d\n", buffer->W, buffer->H, buffer->S);
}
void InitASCII() {
  SDL_Color c = FOREGROUND_SDLCOLOR;
  char *str = malloc(sizeof(char) * 2);
  str[1] = 0;
  printf("%s\n", "Init ASCII");
  for (int i = 0; i < 256; i++) {
    str[0] = i;
    ASCII_FONT[i] = TTF_RenderText_Blended(font, str, c);
    ASCII_FONT_LARGE[i] = TTF_RenderText_Blended(font_Large, str, c);
  }
  free(str);
}

void SetLogFile(FILE *output) { __sdl_LogFile = output; }
void __print(char *fmt, va_list args) { vfprintf(__sdl_LogFile, fmt, args); }
void Log(char *format, ...) {
  va_list l;
  va_start(l, format);
  __print(format, l);
  va_end(l);
  fflush(__sdl_LogFile);
}
void WriteLine(char *str) {
  fprintf(__sdl_LogFile, "%s\n", str);
  fflush(__sdl_LogFile);
}
bool __ShouldClose = false;
int InitSDL() {
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    Log("%s, failed to SDL_Init\n", __func__);
    return -1;
  }
  TTF_Init();
  font = TTF_OpenFont("default.ttf", TEXT_SIZE - 2);
  font_Large = TTF_OpenFont("default.ttf", LARGE_TEXT_SIZE);
  if (NULL == font) {
    Log("%s, failed to TTF_OpenFont: %s\n", __func__, TTF_GetError());
    return -1;
  }
  screen = SDL_SetVideoMode(320, 240, 16, SDL_HWSURFACE | SDL_DOUBLEBUF);
  if (screen == NULL) {
    printf("%s, failed to SDL_SetVideoMode\n", __func__);
    return -1;
  }
  return 0;
}
int MeasureString(char *str, int Length) {
  size_t len = Length;
  if (Length < 0)
    len = strlen(str);
  int x = 0;
  for (size_t i = 0; i < len; i++) {
    SDL_Surface *char_surf = ASCII_FONT[(int)str[i]];
    x += char_surf->w;
  }
  return x;
}
int MeasureStringLarge(char *str, int Length) {
  size_t len = Length;
  if (Length < 0)
    len = strlen(str);
  int x = 0;
  for (size_t i = 0; i < len; i++) {
    SDL_Surface *char_surf = ASCII_FONT_LARGE[(int)str[i]];
    x += char_surf->w;
  }
  return x;
}
void DrawString(char *str, int Length, int x, int y) {
  size_t len = Length;
  if (Length < 0)
    len = strlen(str);
  SDL_Rect rect;
  rect.x = x;
  rect.y = y;
  for (size_t i = 0; i < len; i++) {
    SDL_Surface *char_surf = ASCII_FONT[(int)str[i]];
    rect.w = char_surf->w;
    rect.h = char_surf->h;
    SDL_BlitSurface(char_surf, NULL, screen, &rect);
    rect.x += rect.w;
  }
}
void DrawRectangle(int x, int y, int w, int h, int R, int G, int B) {

  SDL_Rect rect;
  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;
  Uint32 col;
  col = SDL_MapRGB(screen->format, R, G, B);
  SDL_FillRect(screen, &rect, col);
}
void DrawStringLarge(char *str, int Length, int x, int y) {
  size_t len = Length;
  if (Length < 0)
    len = strlen(str);
  SDL_Rect rect;
  rect.x = x;
  rect.y = y;
  for (size_t i = 0; i < len; i++) {
    SDL_Surface *char_surf = ASCII_FONT_LARGE[(int)str[i]];
    rect.w = char_surf->w;
    rect.h = char_surf->h;
    SDL_BlitSurface(char_surf, NULL, screen, &rect);
    rect.x += rect.w;
  }
}
void DrawFileList(DIR *dir, int x, int y, int w, int h) {
  if (dir != NULL) {
    int Offset = y;
    Offset += MIN(h - (_ui_data->CurrentListSelection + 1) * TEXT_SIZE, 0);
    struct dirent *ep;
    rewinddir(dir);
    int Count = 0;
    _ui_data->CurrentListCountPerPage = h / TEXT_SIZE;
    while ((ep = readdir(dir)) != NULL) {
      if (Count == _ui_data->CurrentListSelection) {
        SDL_Rect rect;
        rect.x = x;
        rect.y = Offset;
        rect.w = w;
        rect.h = TEXT_SIZE;
        Uint32 col;
        col = SDL_MapRGB(screen->format, 0x33, 0x33, 0x33);
        SDL_FillRect(screen, &rect, col);
      }
      if (Offset < 240) {
        if (ep->d_type == DT_DIR)
          DrawString("[D]", -1, x, Offset);
        else
          DrawString("[F]", -1, x, Offset);
        DrawString(ep->d_name, -1, x + TEXT_SIZE * 2, Offset);
      }
      Count++;
      Offset += TEXT_SIZE;
      _ui_data->CurrentMaxListCount = Count;
    }
  }
}

void GoBack(bool IsPage) {

  int LAST_POS = buffer->CurrentPos;
  // printf("Try reach:%d\n", LAST_POS);
  //  CurrentPos -= 1;
  while (1) {
    buffer->CurrentPos -= 1;
    if (buffer->CurrentPos < 0) {
      buffer->CurrentPos = 0;
      LoadBuffer();
      return;
    }
    LoadBuffer();
    if (!IsPage) {
      if (buffer->CurrentLineEnd < LAST_POS) {
        return;
      }
    } else if (buffer->CurrentPageEnd < LAST_POS) {
      return;
    }
    if (buffer->CurrentPos < 0) {
      buffer->CurrentPos = 0;
      LoadBuffer();
      return;
    }
  }
}
void GoNextSegment() {
  buffer->CurrentPos = buffer->CurrentLineEnd;
  WillLoadPage = true;
}
void GoNextPage() {
  buffer->CurrentPos = buffer->CurrentPageEnd;
  WillLoadPage = true;
}
void DrawFileManager() {
  DrawFileList(CurrentDir, 0, LARGE_TEXT_SIZE, 320, 230 - LARGE_TEXT_SIZE);
  DrawRectangle(0, 0, 320, LARGE_TEXT_SIZE, TINT_COLOR);
  DrawStringLarge(TITLE, -1, 0, 0);
}
void ClearScreen() {
  SDL_FillRect(screen, &screen->clip_rect,
               SDL_MapRGB(screen->format, BACKGROUND));
}
void LoadBuffer() {
  if (OpendFile == NULL)
    return;

  int BufferWidth = buffer->W;
  int BufferHeight = buffer->H;
  fseek(OpendFile, buffer->CurrentPos, SEEK_SET);
  for (int y = 0; y < BufferHeight; y++) {
    for (int x = 0; x < BufferWidth; x++) {
      buffer->buffer[y * BufferWidth + x] = ' ';
    }
  }
  for (int y = 0; y < BufferHeight; y++) {
    if (y == Target_LineSegment_Count) {
      buffer->CurrentLineEnd = ftell(OpendFile);
    }
    for (int x = 0; x < BufferWidth; x++) {
      int c = fgetc(OpendFile);
      if (c == EOF)
        goto CLEANUP;
      if (c == '\t') {
        x += 4;
        continue;
      }
      switch (c) {
      case '\t':
        x += 4;
        break;
      case '\n':
        goto LINEBREAK;
      case '\r':
        break;
      default:
        buffer->buffer[y * BufferWidth + x] = c;
        break;
      }
    }
  LINEBREAK:;
  }
CLEANUP:
  buffer->CurrentPageEnd = ftell(OpendFile);
  WillLoadPage = false;
}
void DrawBuffer(int X, int Y) {
  if (WillLoadPage) {
    LoadBuffer();
  }
  int BufferWidth = buffer->W;
  int BufferHeight = buffer->H;
  SDL_Rect rect;
  rect.x = X;
  rect.y = Y;
  for (int y = 0; y < BufferHeight; y++) {
    rect.y = Y + y * TEXT_SIZE;
    rect.x = X;
    for (int x = 0; x < BufferWidth; x++) {
      char ch = buffer->buffer[y * BufferWidth + x];
      SDL_Surface *char_surf = ASCII_FONT[(int)ch];
      if (char_surf == NULL) {
        char_surf = ASCII_FONT[(int)' '];
        continue;
      }
      rect.w = char_surf->w;
      rect.h = char_surf->h;
      if (ch != ' ')
        SDL_BlitSurface(char_surf, NULL, screen, &rect);
      rect.x += rect.w;
    }
  }
}
void DrawReading() {
  int w = MeasureStringLarge(OpendFileName, -1);
  DrawStringLarge(OpendFileName, -1, (320 - w) / 2, 0);
  DrawBuffer(10, LARGE_TEXT_SIZE);
}
void DrawUI() {
  ClearScreen();
  switch (_ui_data->Scene) {
  case SCREEN_FILE_BROWSER:
    DrawFileManager();
    break;
  case SCREEN_READING:
    DrawReading();
    break;
  }
  SDL_Flip(screen);
}
void UpdateInput() {

  if (SDL_WaitEvent(&event)) {
    switch (event.type) {
    case SDL_KEYDOWN: {
      SDLKey k = event.key.keysym.sym;
      switch (k) {
      case KEY_EXIT:
        __ShouldClose = true;

        break;
      default:
        if (ProcessInput != NULL) {
          ProcessInput(k);
        }
        break;
      }
      DrawUI();
    } break;
    case SDL_QUIT:
      __ShouldClose = true;
      break;
    default:
      break;
    }
  }
}
bool ShouldClose() { return __ShouldClose; }
