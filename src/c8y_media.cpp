#include "c8y_media.hpp"

C8Y_Media __c8y_media;



C8Y_Media &c8y_media()
{
  return __c8y_media;
}

void c8y_media_load()
{
  SDL_Surface *tmp;

  tmp = IMG_Load("dat/back.png");                    assert(tmp != NULL);__c8y_media.back =                    SDL_DisplayFormat(tmp);      SDL_FreeSurface(tmp);
  tmp = IMG_Load("dat/arrow_left.png");              assert(tmp != NULL);__c8y_media.arrow_left =              SDL_DisplayFormatAlpha(tmp); SDL_FreeSurface(tmp);
  tmp = IMG_Load("dat/arrow_right.png");             assert(tmp != NULL);__c8y_media.arrow_right =             SDL_DisplayFormatAlpha(tmp); SDL_FreeSurface(tmp);
  tmp = IMG_Load("dat/arrow_right_small.png");       assert(tmp != NULL);__c8y_media.arrow_right_small =       SDL_DisplayFormatAlpha(tmp); SDL_FreeSurface(tmp);
  tmp = IMG_Load("dat/button_apply.png");            assert(tmp != NULL);__c8y_media.button_apply =            SDL_DisplayFormat(tmp);      SDL_FreeSurface(tmp);
  tmp = IMG_Load("dat/button_controls.png");         assert(tmp != NULL);__c8y_media.button_controls =         SDL_DisplayFormat(tmp);      SDL_FreeSurface(tmp);
  tmp = IMG_Load("dat/button_controls.png");         assert(tmp != NULL);__c8y_media.button_controls =         SDL_DisplayFormat(tmp);      SDL_FreeSurface(tmp);
  tmp = IMG_Load("dat/button_discard.png");          assert(tmp != NULL);__c8y_media.button_discard =          SDL_DisplayFormat(tmp);      SDL_FreeSurface(tmp);
  tmp = IMG_Load("dat/button_exit.png");             assert(tmp != NULL);__c8y_media.button_exit =             SDL_DisplayFormat(tmp);      SDL_FreeSurface(tmp);
  tmp = IMG_Load("dat/button_load.png");             assert(tmp != NULL);__c8y_media.button_load =             SDL_DisplayFormat(tmp);      SDL_FreeSurface(tmp);
  tmp = IMG_Load("dat/button_load_defaults.png");    assert(tmp != NULL);__c8y_media.button_load_defaults =    SDL_DisplayFormat(tmp);      SDL_FreeSurface(tmp);
  tmp = IMG_Load("dat/button_options.png");          assert(tmp != NULL);__c8y_media.button_options =          SDL_DisplayFormat(tmp);      SDL_FreeSurface(tmp);
  tmp = IMG_Load("dat/button_save_as_defaults.png"); assert(tmp != NULL);__c8y_media.button_save_as_defaults = SDL_DisplayFormat(tmp);      SDL_FreeSurface(tmp);
  tmp = IMG_Load("dat/controls.png");                assert(tmp != NULL);__c8y_media.controls =                SDL_DisplayFormatAlpha(tmp); SDL_FreeSurface(tmp);
  tmp = IMG_Load("dat/controls_main_cursor.png");    assert(tmp != NULL);__c8y_media.controls_main_cursor =    SDL_DisplayFormatAlpha(tmp); SDL_FreeSurface(tmp);
  tmp = IMG_Load("dat/controls_sub_cursor.png");     assert(tmp != NULL);__c8y_media.controls_sub_cursor =     SDL_DisplayFormatAlpha(tmp); SDL_FreeSurface(tmp);

  __c8y_media.font_0 = TTF_OpenFont("dat/FreeSansBoldOblique.ttf", 20);assert(__c8y_media.font_0 != NULL);
  __c8y_media.font_1 = TTF_OpenFont("dat/FreeSansBoldOblique.ttf", 16);assert(__c8y_media.font_1 != NULL);
}

void c8y_media_unload()
{
  SDL_FreeSurface(__c8y_media.back);
  SDL_FreeSurface(__c8y_media.arrow_left);
  SDL_FreeSurface(__c8y_media.arrow_right);
  SDL_FreeSurface(__c8y_media.arrow_right_small);
  SDL_FreeSurface(__c8y_media.button_apply);
  SDL_FreeSurface(__c8y_media.button_controls);
  SDL_FreeSurface(__c8y_media.button_discard);
  SDL_FreeSurface(__c8y_media.button_exit);
  SDL_FreeSurface(__c8y_media.button_load);
  SDL_FreeSurface(__c8y_media.button_load_defaults);
  SDL_FreeSurface(__c8y_media.button_options);
  SDL_FreeSurface(__c8y_media.button_save_as_defaults);
  SDL_FreeSurface(__c8y_media.controls);
  SDL_FreeSurface(__c8y_media.controls_main_cursor);
  SDL_FreeSurface(__c8y_media.controls_sub_cursor);

  TTF_CloseFont(__c8y_media.font_0);
  TTF_CloseFont(__c8y_media.font_1);
}
