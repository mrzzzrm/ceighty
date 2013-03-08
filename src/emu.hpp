#ifndef EMU_HPP
#define EMU_HPP

  #include <string>
  #include <sstream>
  #include <iostream>
  #include <SDL/SDL.h>
  #include <SDL/SDL_ttf.h>

  #include "chip8.h"
  #include "c8y_state.hpp"
  #include "c8y_media.hpp"
  #include "input.hpp"
  #include "config.hpp"



  /* Errors */
  enum EMU_EXCEPTION
  {
    EMU_FILE_ERROR
  };


  /* Normal functions */
  void emu_init();
  void emu_activate();
  void emu_update();
  void emu_draw();
  C8Y_STATE emu_target_state();


  /* Special functions */
  void emu_load(const std::string &path);


#endif
