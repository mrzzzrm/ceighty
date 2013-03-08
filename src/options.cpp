#include "./options.hpp"

using namespace std;


C8Y_STATE target_state = C8Y_OPTIONS;

SDL_Surface *title;
SDL_Surface *label_freq, *label_freq_val;
SDL_Surface *label_fg, *label_fg_val;
SDL_Surface *label_bg, *label_bg_val;

int selected_element;
int freq;
int fg;
int bg;
time_t last_update;
vector<SDL_Color> color_vals;
vector<string> color_names;


void options_init()
{
  color_vals.push_back((SDL_Color){255, 255, 255}); color_names.push_back("White");
  color_vals.push_back((SDL_Color){0, 0, 0});       color_names.push_back("Black");
  color_vals.push_back((SDL_Color){128, 128, 128}); color_names.push_back("Grey");
  color_vals.push_back((SDL_Color){255, 0, 0});     color_names.push_back("Red");
  color_vals.push_back((SDL_Color){0, 255, 0});     color_names.push_back("Green");
  color_vals.push_back((SDL_Color){0, 0, 255});     color_names.push_back("Blue");

  /* Title */
  title = TTF_RenderText_Blended(c8y_media().font_0, "Select options", (SDL_Color){64, 255, 32});
    assert(title != NULL);

  /* Labels */
  label_freq = TTF_RenderText_Blended(c8y_media().font_1, "Frequency(Hz)", (SDL_Color){64, 196, 32});
    assert(label_freq != NULL);
  label_fg = TTF_RenderText_Blended(c8y_media().font_1, "Foreground", (SDL_Color){64, 196, 32});
    assert(label_fg != NULL);
  label_bg = TTF_RenderText_Blended(c8y_media().font_1, "Background", (SDL_Color){64, 196, 32});
    assert(label_bg != NULL);


  /* Init */
  label_freq_val = NULL;
  label_fg_val = NULL;
  label_bg_val = NULL;
  selected_element = 0;
  last_update = 0;
}

void options_close()
{
  SDL_FreeSurface(title);
  SDL_FreeSurface(label_freq);
  SDL_FreeSurface(label_freq_val);
  SDL_FreeSurface(label_fg);
  SDL_FreeSurface(label_fg_val);
  SDL_FreeSurface(label_bg);
  SDL_FreeSurface(label_bg_val);
}

void options_activate()
{
  target_state = C8Y_OPTIONS;
  last_update = SDL_GetTicks();

  freq = config().freq;

  /* Find ids for configurated color vals */
  fg = options_get_matching_fg(config().fg);
  bg = options_get_matching_bg(config().bg);

  options_render_freq_val();
  options_render_fg_val();
  options_render_bg_val();
}

void options_update()
{
  int sfreq, sfg, sbg;


  /* Change selection */
  if(input_key_pressed(SDLK_DOWN) && selected_element < 3-1)
    selected_element++;
  if(input_key_pressed(SDLK_UP) && selected_element > 0)
    selected_element--;


  /* Change values */
  sfreq = freq;
  sfg = fg;
  sbg = bg;
  switch(selected_element)
  {
    case 0:
      /* Change */
      if(input_key_pressed(SDLK_LEFT))
        freq--;
      if(input_key_hold(SDLK_LEFT) && SDL_GetTicks() - input_key_hold_since(SDLK_LEFT) > 500 && (time_t)SDL_GetTicks()/10 != last_update/10)
        freq -= (SDL_GetTicks()/10 - last_update/10) * 2;

      if(input_key_pressed(SDLK_RIGHT))
        freq++;
      if(input_key_hold(SDLK_RIGHT) && SDL_GetTicks() - input_key_hold_since(SDLK_RIGHT) > 500 && (time_t)SDL_GetTicks()/10 != last_update/10)
        freq += (SDL_GetTicks()/10 - last_update/10) * 2;


      /* Limit */
      if(freq < 1) freq = 1;
      if(freq > 9999) freq = 9999;

      /* Render */
      if(freq != sfreq)
        options_render_freq_val();

    break;
    case 1:
      /* Change */
      if(input_key_pressed(SDLK_LEFT))
        fg--;
      if(input_key_pressed(SDLK_RIGHT))
        fg++;

      /* Limit */
      if(fg < 0) fg = 0;
      if(fg >= (int)color_vals.size()) fg = color_vals.size() - 1;

      /* Render */
      if(fg != sfg)
        options_render_fg_val();
    break;
    case 2:
      /* Change */
      if(input_key_pressed(SDLK_LEFT))
        bg--;
      if(input_key_pressed(SDLK_RIGHT))
        bg++;

      /* Limit */
      if(bg < 0) bg = 0;
      if(bg >= (int)color_vals.size()) bg = color_vals.size() - 1;

      /* Render */
      if(bg != sbg)
        options_render_bg_val();
    break;
  }



  /* Apply, Discard... */
  if(input_key_pressed(SDLK_END))
  {
    config().freq = freq;
    config().fg = color_vals[fg];
    config().bg = color_vals[bg];
    target_state = C8Y_EMU;
    config_save_options(c8_rompath() + ".conf");
  }
  if(input_key_pressed(SDLK_PAGEDOWN))
  {
    freq = config().freq;
    fg = options_get_matching_fg(config().fg);
    bg = options_get_matching_bg(config().fg);
    target_state = C8Y_EMU;
  }


  /* Load/Save Defaults */
  if(input_key_pressed(SDLK_HOME))
  {
    config_load_default_options();
    options_activate();
  }
  if(input_key_pressed(SDLK_PAGEUP))
  {
    config().freq = freq;
    config().fg = color_vals[fg];
    config().bg = color_vals[bg];
    config_save_options(c8_rompath() + ".conf");
    config_save_default_options();
  }

  last_update = SDL_GetTicks();
}

void options_draw()
{
  SDL_Rect dst;


  /* Title */
  dst.x = 320;
  dst.y = 30;
  SDL_BlitSurface(title, NULL, SDL_GetVideoSurface(), &dst);


  /* Labels */
  dst.x = 50; dst.y = 100; SDL_BlitSurface(label_freq, NULL, SDL_GetVideoSurface(), &dst);
  if(selected_element == 0){
    dst.x = 20; dst.y -= 2; SDL_BlitSurface(c8y_media().arrow_right_small, NULL, SDL_GetVideoSurface(), &dst);
    }
  dst.x = 600; dst.y = 100; SDL_BlitSurface(label_freq_val, NULL, SDL_GetVideoSurface(), &dst);

  dst.x = 50; dst.y = 200; SDL_BlitSurface(label_fg, NULL, SDL_GetVideoSurface(), &dst);
  if(selected_element == 1){
    dst.x = 20; dst.y -= 2; SDL_BlitSurface(c8y_media().arrow_right_small, NULL, SDL_GetVideoSurface(), &dst);
    }
  dst.x = 600; dst.y = 200; SDL_BlitSurface(label_fg_val, NULL, SDL_GetVideoSurface(), &dst);

  dst.x = 50; dst.y = 300; SDL_BlitSurface(label_bg, NULL, SDL_GetVideoSurface(), &dst);
  if(selected_element == 2){
    dst.x = 20; dst.y -= 2; SDL_BlitSurface(c8y_media().arrow_right_small, NULL, SDL_GetVideoSurface(), &dst);
    }
  dst.x = 600; dst.y = 300; SDL_BlitSurface(label_bg_val, NULL, SDL_GetVideoSurface(), &dst);


  /* Draw the 4 buttons */
  dst.y = 400;
  dst.x = 0;   SDL_BlitSurface(c8y_media().button_discard, NULL, SDL_GetVideoSurface(), &dst);
  dst.x = 200; SDL_BlitSurface(c8y_media().button_load_defaults, NULL, SDL_GetVideoSurface(), &dst);
  dst.x = 400; SDL_BlitSurface(c8y_media().button_save_as_defaults, NULL, SDL_GetVideoSurface(), &dst);
  dst.x = 600; SDL_BlitSurface(c8y_media().button_apply, NULL, SDL_GetVideoSurface(), &dst);
}

C8Y_STATE options_target_state()
{
  return target_state;
}

void options_render_freq_val()
{
  stringstream ss;

  if(label_freq_val != NULL)
    SDL_FreeSurface(label_freq_val);

  ss << freq;
  label_freq_val = TTF_RenderText_Blended(c8y_media().font_0, ss.str().c_str(), (SDL_Color){64, 196, 32});
    assert(label_freq_val != NULL);
}

void options_render_fg_val()
{
  if(label_fg_val != NULL)
    SDL_FreeSurface(label_fg_val);

  label_fg_val = TTF_RenderText_Blended(c8y_media().font_0, color_names[fg].c_str(), (SDL_Color){64, 196, 32});
    assert(label_fg_val != NULL);
}

void options_render_bg_val()
{
  if(label_bg_val != NULL)
    SDL_FreeSurface(label_bg_val);

  label_bg_val = TTF_RenderText_Blended(c8y_media().font_0, color_names[bg].c_str(), (SDL_Color){64, 196, 32});
    assert(label_bg_val != NULL);
}


int options_get_matching_fg(SDL_Color color)
{
  int fg = -1;
  for(int c = 0; c < (int)color_vals.size(); c++)
  {
    if(color_vals[c].r == config().fg.r && color_vals[c].g == config().fg.g && color_vals[c].b == config().fg.b)
      fg = c;
  }
  if(fg == -1){
    color_vals.push_back(config().fg);
    color_names.push_back("Unknown BG");
    fg = color_vals.size() - 1;
  }

  return fg;
}


int options_get_matching_bg(SDL_Color color)
{
  int bg = -1;
  for(int c = 0; c < (int)color_vals.size(); c++)
  {
    if(color_vals[c].r == config().bg.r && color_vals[c].g == config().bg.g && color_vals[c].b == config().bg.b)
      bg = c;
  }
  if(bg == -1){
    color_vals.push_back(config().bg);
    color_names.push_back("Unknown FG");
    bg = color_vals.size() - 1;
  }

  return bg;
}

