#ifndef C8Y_MEDIA
#define C8Y_MEDIA

  #include <assert.h>
  #include <SDL/SDL.h>
  #include <SDL/SDL_image.h>
  #include <SDL/SDL_ttf.h>


  struct C8Y_Media
  {
    SDL_Surface *back;
    SDL_Surface *arrow_left;
    SDL_Surface *arrow_right;
    SDL_Surface *arrow_right_small;
    SDL_Surface *button_apply;
    SDL_Surface *button_controls;
    SDL_Surface *button_discard;
    SDL_Surface *button_exit;
    SDL_Surface *button_load;
    SDL_Surface *button_load_defaults;
    SDL_Surface *button_options;
    SDL_Surface *button_save_as_defaults;
    SDL_Surface *controls;
    SDL_Surface *controls_main_cursor;
    SDL_Surface *controls_sub_cursor;

    TTF_Font *font_0;
    TTF_Font *font_1;
  };

  C8Y_Media &c8y_media();
  void c8y_media_load();
  void c8y_media_unload();

#endif
