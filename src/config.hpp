#ifndef CONFIG_HPP
#define CONFIG_HPP

  #include <string>
  #include <iostream>
  #include <SDL/SDL.h>
  #include <stdio.h>


  /* Exception */
  enum CONFIG_EXCEPTION
  {
    CONFIG_FILE_ERROR,
    CONFIG_SYNTAX_ERROR
  };


  /* Config struct */
  struct Config
  {
    int keymap[0x10];
    int freq;
    SDL_Color fg, bg;
  };


  /* Standart functions */
  void config_init();


  /* Special functions */
  void config_load(const std::string &path);
  void config_load_options(const std::string &path);
  void config_load_controls(const std::string &path);

  void config_load_default();
  void config_load_default_options();
  void config_load_default_controls();

  void config_save_options(const std::string &path);
  void config_save_controls(const std::string &path);

  void config_save_default_options();
  void config_save_default_controls();

  void config_create(const std::string &path);

  Config &config();


#endif
