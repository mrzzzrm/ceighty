#include "./controls.hpp"

using namespace std;

static C8Y_STATE target_state;
static SDL_Surface *title;
static int selected_key;
static bool selected_activated;
static vector<int> mapped_keys;
static vector<SDL_Surface*> mapped_keys_labels;



void controls_init()
{
  mapped_keys.resize(16);
  mapped_keys_labels.resize(16, NULL);

  title = TTF_RenderText_Blended(c8y_media().font_0, "Press 'Select' to map and 'Esc' to cancel mapping.", (SDL_Color){64, 196, 32});
    assert(title != NULL);
}

 void controls_close()
 {
   SDL_FreeSurface(title);
   for(int kl = 0; kl < (int)mapped_keys_labels.size(); kl++)
   {
     SDL_FreeSurface(mapped_keys_labels[kl]);
   }
 }

void controls_activate()
{
  target_state = C8Y_CONTROLS;

  selected_key = 0;
  selected_activated = 0;

  for(int k = 0; k < 16; k++)
  {
    mapped_keys[k] = config().keymap[controls_translate_id(k)];
    controls_render_mapped_key_label(k);
  }
}

void controls_update()
{
  /* Move cursor */
  if(!selected_activated)
  {
    if(input_key_pressed(SDLK_RIGHT))
    {
      if(selected_key%4 == 3)
        selected_key -= 3;
      else
        selected_key++;
    }
    if(input_key_pressed(SDLK_LEFT))
    {
      if(selected_key%4 == 0)
        selected_key += 3;
      else
        selected_key--;
    }
    if(input_key_pressed(SDLK_DOWN))
    {
      if(selected_key/4 == 3)
        selected_key -= 3*4;
      else
        selected_key += 4;
    }
    if(input_key_pressed(SDLK_UP))
    {
      if(selected_key/4 == 0)
        selected_key += 3*4;
      else
        selected_key -= 4;
    }

    /* Activate selection */
    if(input_key_pressed(SDLK_LCTRL))
      selected_activated = true;

    if(input_key_pressed(SDLK_PAGEDOWN))
      target_state = C8Y_EMU;

    if(input_key_pressed(SDLK_END))
    {
      for(int k = 0; k < 16; k++)
      {
        config().keymap[controls_translate_id(k)] = mapped_keys[k];
      }

      config_save_controls(c8_rompath() + ".conf");
      target_state = C8Y_EMU;
    }
  }
  else
  {
    if(input_pressed_keys().size() != 0)
    {
      int key = input_pressed_keys().front();

      if(key == SDLK_ESCAPE)
        selected_activated = false;

      if
        (
          key != SDLK_LCTRL &&
          key != SDLK_LALT &&
          key != SDLK_ESCAPE &&
          key != SDLK_HOME
        )
        {
          mapped_keys[selected_key] = key;
          controls_render_mapped_key_label(selected_key);

          selected_activated = false;
        }
    }
  }


  /* Load/Save Defaults */
  if(input_key_pressed(SDLK_HOME))
  {
    config_load_default_controls();
    controls_activate();
  }
  if(input_key_pressed(SDLK_PAGEUP))
  {
    for(int k = 0; k < 16; k++)
    {
      config().keymap[controls_translate_id(k)] = mapped_keys[k];
    }
    config_save_controls(c8_rompath() + ".conf");
    config_save_default_controls();
  }
}


void controls_draw()
{
  SDL_Rect dst;


  /* Title */
  dst.x = 400  - title->w/2;
  dst.y = 20;
  SDL_BlitSurface(title, NULL, SDL_GetVideoSurface(), &dst);


  /* Main field */
  dst.x = 220;
  dst.y = 43;
  SDL_BlitSurface(c8y_media().controls, NULL, SDL_GetVideoSurface(), &dst);

  for(int k = 0; k < 16; k++)
  {
    int x = (k%4) * 90;
    int y = (k/4) * 90;

    if(k == selected_key)
    {
      dst.x = x + 220;
      dst.y = y + 43;
      SDL_BlitSurface(c8y_media().controls_main_cursor, NULL, SDL_GetVideoSurface(), &dst);

      if(selected_activated && SDL_GetTicks()%800 < 400)
      {
        dst.x = x + 293;
        dst.y = y + 93;
        SDL_BlitSurface(c8y_media().controls_sub_cursor, NULL, SDL_GetVideoSurface(), &dst);
      }
    }

    if(mapped_keys_labels[k] == NULL || (selected_activated && k == selected_key))
      continue;

    dst.x = x + 295 - mapped_keys_labels[k]->w;
    dst.y = y + 98;
    SDL_BlitSurface(mapped_keys_labels[k], NULL, SDL_GetVideoSurface(), &dst);
  }


  /* Apply and Discard */
  dst.y = 400;
  dst.x = 0;   SDL_BlitSurface(c8y_media().button_discard, NULL, SDL_GetVideoSurface(), &dst);
  dst.x = 200; SDL_BlitSurface(c8y_media().button_load_defaults, NULL, SDL_GetVideoSurface(), &dst);
  dst.x = 400; SDL_BlitSurface(c8y_media().button_save_as_defaults, NULL, SDL_GetVideoSurface(), &dst);
  dst.x = 600; SDL_BlitSurface(c8y_media().button_apply, NULL, SDL_GetVideoSurface(), &dst);
}


C8Y_STATE controls_target_state()
{
  return target_state;
}

void controls_render_mapped_key_label(int num)
{
  mapped_keys_labels[num] = TTF_RenderText_Blended(c8y_media().font_0, input_SDLK_name(mapped_keys[num]).c_str(), (SDL_Color){0, 255, 0});
    assert(mapped_keys_labels[num] != NULL);
}

/* Translate from 0...F to C8 Keymap */
int controls_translate_id(int id)
{
  if(id%4 == 3)
    return id/4 + 12;
  else if(id <= 2)
    return id + 1;
  else if(id >= 4 && id <= 7)
    return id;
  else if(id >= 8 && id <= 0xB)
    return id - 1;
  else if(id == 12)
    return 0xA;
  else if(id == 13)
    return 0;
  else if(id == 14)
    return 0xB;

  cerr << "Unknown id " << id << endl;
  assert(0);
}

