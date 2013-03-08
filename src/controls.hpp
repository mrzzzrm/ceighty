#ifndef CONTROLS_HPP
#define CONTROLS_HPP


  #include <iostream>
  #include <vector>
  #include <SDL/SDL.h>
  #include <SDL/SDL_ttf.h>
  #include "input.hpp"
  #include "config.hpp"
  #include "chip8.h"
  #include "c8y_media.hpp"
  #include "c8y_state.hpp"


  /* Normal functions */
  void controls_init();
  void controls_close();
  void controls_activate();
  void controls_update();
  void controls_draw();
  C8Y_STATE controls_target_state();


  /* Internals */
  void controls_render_mapped_key_label(int num);
  int controls_translate_id(int id);

#endif
