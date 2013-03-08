#ifndef INPUT_HPP
#define INPUT_HPP

  #include <list>
  #include <string>
  #include <sstream>
  #include <cctype>
  #include <string.h>
  #include <iostream>
  #include <stdlib.h>
  #include <assert.h>
  #include <SDL/SDL.h>
  #include "c8_exception.hpp"


  void input_init();
  void input_clear();
  void input_update();
  bool input_key_hold(int key);
  bool input_key_pressed(int key);
  bool input_key_released(int key);
  time_t input_key_hold_since(int key);
  void input_set_key_pressed(int key, bool is);
  const std::list<int> &input_pressed_keys();
  std::string input_SDLK_name(int key);


#endif
