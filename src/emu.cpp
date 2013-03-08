#include "./emu.hpp"

using namespace std;


static const int FPS = 60;
static C8Y_STATE target_state = C8Y_EMU;



void emu_init()
{

}

void emu_activate()
{
  target_state = C8Y_EMU;

  /* Unpause */
  c8_pause(false);

  /* Update config - just in case it was changed */
  c8_set_keymap(config().keymap);
  c8_set_fg(config().fg);
  c8_set_bg(config().bg);
}

void emu_update()
{
  /* Select menu item */
  if(input_key_pressed(SDLK_SPACE))
    target_state = C8Y_LOAD;
  if(input_key_pressed(SDLK_LALT))
    target_state = C8Y_OPTIONS;
  if(input_key_pressed(SDLK_LCTRL))
    target_state = C8Y_CONTROLS;
  if(input_key_pressed(SDLK_ESCAPE))
    target_state = C8Y_EXIT;

  /* Pause the emulation if another state is entered */
  if(target_state != C8Y_EMU)
    c8_pause(true);

  /* Execute frame */
  c8_exec_frame(config().freq, FPS);
}

void emu_draw()
{
  SDL_Rect dst;


  /* Render the 4 buttons */
  dst.y = 400;
  dst.x = 0;   SDL_BlitSurface(c8y_media().button_load, NULL, SDL_GetVideoSurface(), &dst);
  dst.x = 200; SDL_BlitSurface(c8y_media().button_options, NULL, SDL_GetVideoSurface(), &dst);
  dst.x = 400; SDL_BlitSurface(c8y_media().button_controls, NULL, SDL_GetVideoSurface(), &dst);
  dst.x = 600; SDL_BlitSurface(c8y_media().button_exit, NULL, SDL_GetVideoSurface(), &dst);

  /* Render main frame */
  c8_render(16, 16, 768, 383);
  c8_delay(FPS);
}

C8Y_STATE emu_target_state()
{
  return target_state;
}

void emu_load(const std::string &path)
{
  int re;

  c8_reset();

  re = c8_load_rom(path.c_str());
  if(re < 0)
  {
    cerr << "Error loading ROM " << path << endl;
    throw EMU_FILE_ERROR;
  }
  else
  {
    cout << "Loaded ROM " << path << ". Size: " << re << endl;
  }
  config_load(path + string(".conf"));
  emu_activate();
}

