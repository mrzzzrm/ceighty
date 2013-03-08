#include "./load.hpp"

using namespace std;


struct DirElement
{
  unsigned char type;
  string name;
  SDL_Surface *label;

  bool operator<(const DirElement &element) const
  {
    return name < element.name;
  }
};


static C8Y_STATE target_state = C8Y_LOAD;
static SDL_Surface *title;
static vector<DirElement> directory;
static int selected_element;
static int selected_in_view;
static int max_elements_in_view;
static int elements_in_view;
static time_t last_update;
static bool new_rom_selected;
static string new_rom_name;


void load_init()
{
  DIR *dir;
  struct dirent *ent;
  string romdir;


  /* Romdir */
  romdir = getcwd(NULL, 0);
  romdir.append("/rom");

  /* Title */
  title = TTF_RenderText_Blended(c8y_media().font_0, "Place roms in '[appdata]/rom/' with filename-extension '.ch8'", (SDL_Color){64, 255, 32});
    assert(title != NULL);

  /* Render all elements */
  dir = opendir(romdir.c_str());
  if(dir != NULL)
  {
    while((ent = readdir(dir)) != NULL)
    {
      string name;
      DirElement element;

      name = ent->d_name;

      /* Check for '.ch8' extension */
      if(name.size() < 5 || name.substr(name.size()-4, 4) != ".ch8")
        continue;

      element.name = name;
      element.type = ent->d_type;
      element.label = TTF_RenderText_Blended(c8y_media().font_1, ent->d_name, (SDL_Color){64, 196, 32});
        assert(element.label != NULL);

      directory.push_back(element);
    }
    sort(directory.begin(), directory.end());
  }


  /* Init */
  last_update = 0;
  selected_element = 0;
  selected_in_view = 0;
  max_elements_in_view = 300/17;
  elements_in_view = (int)directory.size() < max_elements_in_view ? directory.size() : max_elements_in_view;
  new_rom_selected = false;
}

void load_close()
{
  SDL_FreeSurface(title);
  for(int i = 0; i < (int)directory.size(); i++)
  {
    SDL_FreeSurface(directory[i].label);
  }
}

void load_activate()
{
  last_update = SDL_GetTicks();
  target_state = C8Y_LOAD;
}

void load_update()
{
  new_rom_selected = false;

  /* Change selection */
  if(input_key_pressed(SDLK_DOWN))
  {
    selected_element++;
    selected_in_view++;
  }
  if(input_key_hold(SDLK_DOWN) && SDL_GetTicks() - input_key_hold_since(SDLK_DOWN) > 500 && (time_t)SDL_GetTicks()/30 != last_update/30)
  {
    selected_element += SDL_GetTicks()/30 - last_update/30;
    selected_in_view += SDL_GetTicks()/30 - last_update/30;
  }
  if(input_key_pressed(SDLK_UP))
  {
    selected_element--;
    selected_in_view--;
  }
  if(input_key_hold(SDLK_UP) && SDL_GetTicks() - input_key_hold_since(SDLK_UP) > 500 && (time_t)SDL_GetTicks()/30 != last_update/30)
  {
    selected_element -= SDL_GetTicks()/30 - last_update/30;
    selected_in_view -= SDL_GetTicks()/30 - last_update/30;
  }

  if(selected_element < 0)
    selected_element = 0;
  if(selected_element > (int)directory.size() - 1)
    selected_element = directory.size() - 1;

  if(selected_in_view < 0)
    selected_in_view = 0;
  if(selected_in_view > elements_in_view - 1)
    selected_in_view = elements_in_view - 1;


  /* Quit load */
  new_rom_selected = false;
  if(input_key_pressed(SDLK_END))
  {
    if(directory.size() > 0)
    {
      new_rom_selected = true;
      new_rom_name = directory[selected_element].name;
    }
    else
    {
      new_rom_selected = false;
    }

    target_state = C8Y_EMU;
  }
  if(input_key_pressed(SDLK_PAGEDOWN))
  {
    target_state = C8Y_EMU;
  }

  last_update = SDL_GetTicks();
}

void load_draw()
{
  SDL_Rect dst;


  /* Title */
  dst.x = 20;
  dst.y = 20;
  SDL_BlitSurface(title, NULL, SDL_GetVideoSurface(), &dst);


  /* Draw the Directory */
  int first_element = selected_element - selected_in_view;
  int num_elements = (int)directory.size() - first_element > elements_in_view ? elements_in_view : directory.size() - first_element;
  int last_element =  first_element + num_elements - 1;
  for(int e = first_element, y = 50; e <= last_element; e++, y += 20)
  {
    dst.x = 50;
    dst.y = y;
    SDL_BlitSurface(directory[e].label, NULL, SDL_GetVideoSurface(), &dst);

    /* Arrow */
    if(e == selected_element)
    {
      dst.x = 20;
      dst.y = y - 2;
      SDL_BlitSurface(c8y_media().arrow_right_small, NULL, SDL_GetVideoSurface(), &dst);
    }
  }


  /* Render the 2 buttons */
  dst.y = 400;
  dst.x = 0;   SDL_BlitSurface(c8y_media().button_discard, NULL, SDL_GetVideoSurface(), &dst);
  dst.x = 600; SDL_BlitSurface(c8y_media().button_apply, NULL, SDL_GetVideoSurface(), &dst);
}


C8Y_STATE load_target_state()
{
  return target_state;
}


bool load_new_rom_selected()
{
  return new_rom_selected;
}


string load_new_rom_name()
{
  return new_rom_name;
}


