#ifndef OPTIONS_HPP
#define OPTIONS_HPP

  #include <sstream>
  #include <vector>
  #include <string>
  #include <iostream>
  #include "c8y_state.hpp"
  #include "c8y_media.hpp"
  #include "input.hpp"
  #include "config.hpp"
  #include "chip8.h"


  /* Normal functions */
  void options_init();
  void options_close();
  void options_activate();
  void options_update();
  void options_draw();
  C8Y_STATE options_target_state();


  /* Internals */
  void options_render_freq_val();
  void options_render_fg_val();
  void options_render_bg_val();
  int options_get_matching_fg(SDL_Color color);
  int options_get_matching_bg(SDL_Color color);

#endif
