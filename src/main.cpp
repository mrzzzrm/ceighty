#include "main.hpp"

using namespace std;


C8Y_STATE state;


static void init()
{
  /* Init SDL */
  SDL_Init(SDL_INIT_EVERYTHING);
  #ifdef PANDORA
  SDL_SetVideoMode(800, 480, 16, SDL_SWSURFACE | SDL_FULLSCREEN | SDL_DOUBLEBUF);
  #else
  SDL_SetVideoMode(800, 480, 16, SDL_SWSURFACE);
  #endif
  SDL_ShowCursor(false);

  /* Init TTF */
  TTF_Init();

  /* Load media */
  c8y_media_load();

  /* Init subsystems */
  emu_init();
  load_init();
  options_init();
  controls_init();
  config_init();
  input_init();


  state = C8Y_EMU;
  emu_load("Trip8 Demo (2008) [Revival Studios].ch8");
}


static void run()
{
  C8Y_STATE tmp_state;

  while(true)
  {
    input_clear();
    input_update();

    /* Switch beween states */
    tmp_state = state;
    switch(state)
    {
      case C8Y_EMU:
        state = emu_target_state();
      break;
      case C8Y_LOAD:
        state = load_target_state();

        if(state == C8Y_EMU && load_new_rom_selected())
          emu_load(string("rom/") + load_new_rom_name());
      break;
      case C8Y_OPTIONS:
        state = options_target_state();
      break;
      case C8Y_CONTROLS:
        state = controls_target_state();
      break;
      default: break;
    }
    if(tmp_state != state)
    {
      switch(state)
      {
        case C8Y_EMU:      emu_activate();      break;
        case C8Y_LOAD:     load_activate();     break;
        case C8Y_OPTIONS:  options_activate();  break;
        case C8Y_CONTROLS: controls_activate(); break;
        default: break;
      }
    }


    /* Draw background */
    SDL_BlitSurface(c8y_media().back, NULL, SDL_GetVideoSurface(), NULL);

    /* Main calculating and drawing stuff */
    switch(state)
    {
      case C8Y_EMU:      emu_update();  emu_draw();      break;
      case C8Y_LOAD:     load_update();     load_draw();     break;
      case C8Y_OPTIONS:  options_update();  options_draw();  break;
      case C8Y_CONTROLS: controls_update(); controls_draw(); break;
      default: break;
    }



    /* Flip */
    SDL_Flip(SDL_GetVideoSurface());

    /* Terminate application */
    if(state == C8Y_EXIT)
      break;
  }
}

static void close()
{
  c8y_media_unload();

  controls_close();
  load_close();
  options_close();
}

int main(int argc, char **argv)
{
  try
  {
    init();
    run();
    close();
  }
  catch(C8_EXCEPTION ex)
  {
    if(ex == C8_QUIT)
      return 0;
    else
      return -1;
  }

  return 0;
}
