#include "config.hpp"

using namespace std;

static Config __config;


void config_init()
{
  FILE *file;
  SDL_Color fg, bg;

  file = fopen("default.conf", "r");
  if(file == NULL)
  {
    file = fopen("default.conf", "w");
    if(file == NULL)
    {
      cerr << "Error loading default config " << endl;
      throw CONFIG_FILE_ERROR;
    }

    int keys[] =
      {
        SDLK_x,
        SDLK_1, SDLK_2, SDLK_3,
        SDLK_q, SDLK_w, SDLK_e,
        SDLK_a, SDLK_s, SDLK_d,
        SDLK_y, SDLK_c,
        SDLK_4, SDLK_r, SDLK_f, SDLK_v
      };
    fwrite(keys, 1, sizeof(keys), file);

    int freq = 400;
    fwrite(&freq, 1, sizeof(freq), file);

    fg.r = 255;
    fg.g = 255;
    fg.b = 255;
    fwrite(&fg, 1, sizeof(fg), file);

    bg.r = 0;
    bg.g = 0;
    bg.b = 0;
    fwrite(&bg, 1, sizeof(bg), file);

    fclose(file);
  }
  else
  {
    fclose(file);
  }
}

void config_load(const std::string &path)
{
  config_load_options(path);
  config_load_controls(path);
}

void config_load_options(const std::string &path)
{
  FILE *file;
  size_t size;


  file = fopen(path.c_str(), "r");
  if(file == NULL)
    config_create(path);
  else
    fclose(file);

  file = fopen(path.c_str(), "r");
  if(file == NULL)
  {
    cerr << "Error loading config " << path << endl;
    throw CONFIG_FILE_ERROR;
  }

  if(fseek(file, sizeof(__config.keymap), SEEK_SET))
  {
    fclose(file);
    cerr << "Error while seeking in " << path << endl;
    throw CONFIG_SYNTAX_ERROR;
  }

  size = fread(&__config.freq, 1, sizeof(__config) - sizeof(__config.keymap), file);
  fclose(file);
  if(size != sizeof(__config) - sizeof(__config.keymap))
  {
    cerr << "Couldn't load config from " << path << ". File is too small." << endl;
    throw CONFIG_SYNTAX_ERROR;
  }
}

void config_load_controls(const std::string &path)
{
  FILE *file;
  size_t size;


  file = fopen(path.c_str(), "r");
  if(file == NULL)
    config_create(path);
  else
    fclose(file);

  file = fopen(path.c_str(), "r");
  if(file == NULL)
  {
    cerr << "Error loading config " << path << endl;
    throw CONFIG_FILE_ERROR;
  }

  size = fread(&__config.keymap, 1, sizeof(__config.keymap), file);
  fclose(file);
  if(size != sizeof(__config.keymap))
  {
    cerr << "Couldn't load config from " << path << ". File is too small." << endl;
    throw CONFIG_SYNTAX_ERROR;
  }
}

void config_load_default()
{
  config_load_default_options();
  config_load_default_controls();
}

void config_load_default_options()
{
  config_load_options("default.conf");
}


void config_load_default_controls()
{
  config_load_controls("default.conf");
}

void config_save_options(const std::string &path)
{
  FILE *file;

  file = fopen(path.c_str(), "r");
  if(file == NULL)
    config_create(path);
  else
    fclose(file);

  file = fopen(path.c_str(), "w");
  if(file == NULL)
  {
    cerr << "Error opening file " << path << endl;
    throw CONFIG_FILE_ERROR;
  }
  fwrite(&__config, 1, sizeof(__config), file);
  fclose(file);
}


void config_save_controls(const std::string &path)
{
  FILE *file;

  file = fopen(path.c_str(), "r");
  if(file == NULL)
    config_create(path);
  else
    fclose(file);

  file = fopen(path.c_str(), "w");
  if(file == NULL)
  {
    cerr << "Error opening file " << path << endl;
    throw CONFIG_FILE_ERROR;
  }
  fwrite(&__config, 1, sizeof(__config), file);
  fclose(file);
}

void config_save_default_options()
{
  config_save_options("default.conf");
}

void config_save_default_controls()
{
  config_save_controls("default.conf");
}

void config_create(const std::string &path)
{
  size_t size;
  Config tmp_config;
  FILE *src_file, *dst_file;


  dst_file = fopen(path.c_str(), "w");
  if(dst_file == NULL)
  {
    cerr << "Error opening file " << path << endl;
    throw CONFIG_FILE_ERROR;
  }

  src_file = fopen("default.conf", "r");
  if(src_file == NULL)
  {
    cerr << "Error opening file " << path << endl;
    throw CONFIG_FILE_ERROR;
  }


  size = fread(&tmp_config, 1, sizeof(tmp_config), src_file);
  if(size != sizeof(tmp_config))
  {
    cerr << "Couldn't load config from default.conf. File is too small." << endl;
    throw CONFIG_SYNTAX_ERROR;
  }

  size = fwrite(&tmp_config, 1, sizeof(tmp_config), dst_file);
  if(size != sizeof(tmp_config))
  {
    cerr << "Couldn't write config to " << path << endl;
    throw CONFIG_SYNTAX_ERROR;
  }

  fclose(src_file);
  fclose(dst_file);
}

Config &config()
{
  return __config;
}

