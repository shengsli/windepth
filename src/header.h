#if defined(_MSC_VER)

#include "SDL.h"
#undef main
#include "SDL_ttf.h"

#include <cstdio>
#include <cstring>
#include <cstdlib>

#else

#include "SDL/SDL.h"
#include "SDL_ttf.h"

#endif

// These are used for backward compatibility with SDL 1.2 to get the keycodes to work
//#define SDL12_SDLK_FIRST 0
#define SDL12_SDLK_NORMAL 127
#define SDL12_SDLK_LAST 255
