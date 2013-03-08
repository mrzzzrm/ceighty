#include "input.hpp"

using namespace std;

static Uint8 hold[SDLK_LAST + 1];
static Uint8 pressed[SDLK_LAST + 1];
static Uint8 released[SDLK_LAST + 1];
static time_t hold_since[SDLK_LAST + 1];
static list<int> pressed_keys;


void input_init()
{
  for(int k = 0; k <= SDLK_LAST; k++)
  {
    hold_since[k] = 0;
  }
}

void input_clear()
{
  for(int k = 0; k <= SDLK_LAST; k++)
  {
    pressed[k] = 0;
    released[k] = 0;
  }

  pressed_keys.clear();
}


void input_update()
{
  SDL_Event event;

  while(SDL_PollEvent(&event))
  {
    switch(event.type)
    {
      case SDL_QUIT:
        return throw C8_QUIT;
      break;
    }
  }

  for(int k = 0; k <= SDLK_LAST; k++)
  {
    if(SDL_GetKeyState(NULL)[k] && !hold[k])
    {
      pressed[k] = 1;
      hold_since[k] = SDL_GetTicks();

      pressed_keys.push_back(k);
    }

    if(!SDL_GetKeyState(NULL)[k] && hold[k])
    {
      released[k] = 1;
      hold_since[k] = 0;
    }
  }

  memcpy(hold, SDL_GetKeyState(NULL), SDLK_LAST);
}


bool input_key_hold(int key)
{
  assert(key >= 0 && key <= SDLK_LAST);
  return hold[key];
}

bool input_key_pressed(int key)
{
  assert(key >= 0 && key <= SDLK_LAST);
  return pressed[key];
}

bool input_key_released(int key)
{
  assert(key >= 0 && key <= SDLK_LAST);
  return released[key];
}

time_t input_key_hold_since(int key)
{
  assert(key >= 0 && key <= SDLK_LAST);
  return hold_since[key];
}

void input_set_key_pressed(int key, bool is)
{
  assert(key >= 0 && key <= SDLK_LAST);
  pressed[key] = is;
}

const std::list<int> &input_pressed_keys()
{
  return pressed_keys;
}

string input_SDLK_name(int key)
{
  switch(key)
  {
    case SDLK_UP: return "Up"; break;
    case SDLK_DOWN: return "Down"; break;
    case SDLK_LEFT: return "Left"; break;
    case SDLK_RIGHT: return "Right"; break;
    case SDLK_SPACE: return "Space"; break;
    case SDLK_RCTRL: return "RCtrl"; break;
    case SDLK_RALT: return "R"; break;
    case SDLK_RSHIFT: return "L"; break;
    case SDLK_END: return "B"; break;
    case SDLK_PAGEDOWN: return "X"; break;
    case SDLK_PAGEUP: return "Y"; break;
    case SDLK_HOME: return "A"; break;
    case SDLK_RETURN: return "Enter"; break;
    case SDLK_DELETE: return "Delete"; break;
    case SDLK_INSERT: return "Insert"; break;
    case SDLK_BACKSPACE: return "Backs."; break;
  }

  if(isprint((char)key))
  {
    return string().append((char*)&key);
  }
  else
  {
    return "???";
  }
}
