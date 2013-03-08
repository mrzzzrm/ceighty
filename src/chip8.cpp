#include "./chip8.h"

using namespace std;

#define X ((op & 0x0F00) >> 8)
#define Y ((op & 0x00F0) >> 4)
#define N (op & 0x000F)
#define NN (op & 0x00FF)
#define NNN (op & 0x0FFF)
#define VX (c8.reg[X])
#define VY (c8.reg[Y])
#define VF (c8.reg[0xF])


static int keymap[0x10];
static bool paused;
static time_t paused_start;
static time_t paused_ticks;
static SDL_Color fg, bg;
static string rompath;


static const u8 font[] =
{
  0x60, 0xa0, 0xa0, 0xa0, 0xc0,/* 0 */
  0x40, 0xc0, 0x40, 0x40, 0xe0,/* 1 */
  0xc0, 0x20, 0x40, 0x80, 0xe0,/* 2 */
  0xc0, 0x20, 0x40, 0x20, 0xc0,/* 3 */
  0x20, 0xa0, 0xe0, 0x20, 0x20,/* 4 */
  0xe0, 0x80, 0xc0, 0x20, 0xc0,/* 5 */
  0x40, 0x80, 0xc0, 0xa0, 0x40,/* 6 */
  0xe0, 0x20, 0x60, 0x40, 0x40,/* 7 */
  0x40, 0xa0, 0x40, 0xa0, 0x40,/* 8 */
  0x40, 0xa0, 0x60, 0x20, 0x40,/* 9 */
  0x40, 0xa0, 0xe0, 0xa0, 0xa0,/* A */
  0xc0, 0xa0, 0xc0, 0xa0, 0xc0,/* B */
  0x60, 0x80, 0x80, 0x80, 0x60,/* C */
  0xc0, 0xa0, 0xa0, 0xa0, 0xc0,/* D */
  0xe0, 0x80, 0xc0, 0x80, 0xe0,/* E */
  0xe0, 0x80, 0xc0, 0x80, 0x80 /* F */
};


static const u8 sfont[] = {
  0x7c, 0xc6, 0xce, 0xde, 0xd6, 0xf6, 0xe6, 0xc6, 0x7c, 0x00, /* 0 */
 	0x10, 0x30, 0xf0, 0x30, 0x30, 0x30, 0x30, 0x30, 0xfc, 0x00, /* 1 */
 	0x78, 0xcc, 0xcc, 0xc,  0x18, 0x30, 0x60, 0xcc, 0xfc, 0x00, /* 2 */
 	0x78, 0xcc, 0x0c, 0x0c, 0x38, 0x0c, 0x0c, 0xcc, 0x78, 0x00, /* 3 */
 	0x0c, 0x1c, 0x3c, 0x6c, 0xcc, 0xfe, 0x0c, 0x0c, 0x1e, 0x00, /* 4 */
 	0xfc, 0xc0, 0xc0, 0xc0, 0xf8, 0x0c, 0x0c, 0xcc, 0x78, 0x00, /* 5 */
 	0x38, 0x60, 0xc0, 0xc0, 0xf8, 0xcc, 0xcc, 0xcc, 0x78, 0x00, /* 6 */
 	0xfe, 0xc6, 0xc6, 0x06, 0x0c, 0x18, 0x30, 0x30, 0x30, 0x00, /* 7 */
 	0x78, 0xcc, 0xcc, 0xec, 0x78, 0xdc, 0xcc, 0xcc, 0x78, 0x00, /* 8 */
 	0x7c, 0xc6, 0xc6, 0xc6, 0x7c, 0x18, 0x18, 0x30, 0x70, 0x00, /* 9 */
 	0x30, 0x78, 0xcc, 0xcc, 0xcc, 0xfc, 0xcc, 0xcc, 0xcc, 0x00, /* A */
 	0xfc, 0x66, 0x66, 0x66, 0x7c, 0x66, 0x66, 0x66, 0xfc, 0x00, /* B */
 	0x3c, 0x66, 0xc6, 0xc0, 0xc0, 0xc0, 0xc6, 0x66, 0x3c, 0x00, /* C */
 	0xf8, 0x6c, 0x66, 0x66, 0x66, 0x66, 0x66, 0x6c, 0xf8, 0x00, /* D */
 	0xfe, 0x62, 0x60, 0x64, 0x7c, 0x64, 0x60, 0x62, 0xfe, 0x00, /* E */
 	0xfe, 0x66, 0x62, 0x64, 0x7c, 0x64, 0x60, 0x60, 0xf0, 0x00  /* F */
};


struct C8
{
  u8 ram[0x1000];
  u8 reg[0x10];
  u16 i;
  u16 pc;

  u16 st[0x10];
  u16 stt;
  u16 stc;

  u8 scr[CHIP8_SCREEN_WIDTH*2][CHIP8_SCREEN_HEIGHT*2];

  u8 key[0x10];

  u8 td;
  u8 ts;

  u8 ak;
  u8 ak_reg;

  /* superchip */
  u8 xscr;
  u8 qf;

  u8 rpl[8];
}c8;




int c8_load_rom(const char *path)
{
  size_t fs;
  FILE *f;

  rompath = path;

  f = fopen(path, "rb");
  if(f == NULL)
  {
    return -1;
  }

  fs = fread(c8.ram + 0x200, 1, 0xFFF - 0x200, f);
  fclose(f);

  return fs;
}


void c8_init()
{
  /* Init chip8 */
  c8.pc = 0;
  c8.i = 0;
  c8.stt = 0;
  c8.stc = 0;
  c8.td = 0;
  c8.ts = 0;
  c8.ak = false;
  c8.ak_reg = 0;
  c8.xscr = false;
  c8.qf = 0;

  memset(c8.st, 0, sizeof(c8.st));
  memset(c8.reg, 0, sizeof(c8.reg));
  memset(c8.scr, 0, sizeof(c8.scr));
  memset(c8.key, 0, sizeof(c8.key));
  memset(c8.ram, 0, sizeof(c8.ram));

  paused = false;
  paused_start = 0;
  paused_ticks = 0;
}

void c8_reset()
{
  /* Reset chip8 */
  c8.pc = 0x200;
  c8.i = 0x0;
  c8.stt = 0;
  c8.stc = 0;
  c8.td = 0;
  c8.ts = 0;
  c8.ak = false;
  c8.ak_reg = 0;
  c8.xscr = false;
  c8.qf = 0;


  memset(c8.st, 0, sizeof(c8.st));
  memset(c8.reg, 0, sizeof(c8.reg));
  memset(c8.scr, 0, sizeof(c8.scr));
  memset(c8.key, 0, sizeof(c8.key));
  memset(c8.ram, 0, sizeof(c8.ram));

  paused = false;
  fg.r = 064; fg.g = 255; fg.b = 064;
  bg.r = 000; bg.g = 000; bg.b = 000;

  /* Preload RAM */
  memcpy(c8.ram, sfont, sizeof(sfont));
  memcpy(c8.ram + 160, font, sizeof(font));

  /* Init settings */
  memset(keymap, 0, sizeof(keymap));
}


void c8_set_keymap(const int *_keymap)
{
  for(int k = 0; k < 0x10; k++)
  {
    keymap[k] = _keymap[k];
  }
}


void c8_exec()
{
  static int timer = SDL_GetTicks() - paused_ticks;
  u16 op;


  /* Paused */
  if(paused)
    return;


  /* Input */
  for(u8 k = 0; k < 0x10; k++)
  {
    /* Pressed */
    if(input_key_pressed(keymap[k]))
    {
      c8.key[k] = 1;

      if(c8.ak)
      {
        c8.reg[c8.ak_reg] = k;
        input_set_key_pressed(keymap[k], false);
        c8.ak = false;
      }
    }

    /* Released */
    if(input_key_released(keymap[k]))
      c8.key[k] = 0;
  }


  /* Await keypress */
  if(c8.ak)
    return;


  /* Timers */
  while((SDL_GetTicks() - paused_ticks) - timer > 1000/60)
  {
    if(c8.td > 0)
      c8.td--;
    if(c8.ts > 0)
      c8.ts--;

    timer += 1000/60;
  }

  /* Opcode decode and exec */
  if(c8.pc + 1 > 0xFFF)
    c8_ram_illegal(__LINE__);

  op = c8_ram(c8.pc++) << 8;
  op |= c8_ram(c8.pc++);



//  printf("op %x at %x ", op, c8.pc);
//  for(int a = 0; a <= 0xf; a++)
//  {
//    printf("[%2x]", c8.reg[a]);
//  }
//  printf("\n");

  switch((op & 0xF000) >> 12)
  {
    case 0x0:
      if((op & 0x00F0) == 0xC0)
      {
        c8_op_00CN(op); /* superchip */
      }
      else
      {
        switch(op & 0xFF)
        {
          case 0xE0: c8_op_00E0(op); break;
          case 0xEE: c8_op_00EE(op); break;
          case 0xFB: c8_op_00FB(op); break; /* superchip */
          case 0xFC: c8_op_00FC(op); break; /* superchip */
          case 0xFD: c8_op_00FD(op); break; /* superchip */
          case 0xFE: c8_op_00FE(op); break; /* superchip */
          case 0xFF: c8_op_00FF(op); break; /* superchip */
          default: c8_op_0NNN(op); break;
        }
      }
    break;


    case 0x1: c8_op_1NNN(op); break;
    case 0x2: c8_op_2NNN(op); break;
    case 0x3: c8_op_3XNN(op); break;
    case 0x4: c8_op_4XNN(op); break;
    case 0x5: c8_op_5XY0(op); break;
    case 0x6: c8_op_6XNN(op); break;
    case 0x7: c8_op_7XNN(op); break;


    case 0x8:
      switch(op & 0xF)
      {
        case 0x0: c8_op_8XY0(op); break;
        case 0x1: c8_op_8XY1(op); break;
        case 0x2: c8_op_8XY2(op); break;
        case 0x3: c8_op_8XY3(op); break;
        case 0x4: c8_op_8XY4(op); break;
        case 0x5: c8_op_8XY5(op); break;
        case 0x6: c8_op_8XY6(op); break;
        case 0x7: c8_op_8XY7(op); break;
        case 0xE: c8_op_8XYE(op); break;
        default: c8_op_illegal(__LINE__); break;
      }
    break;


    case 0x9:
      if((op & 0xF) == 0x0)
        c8_op_9XY0(op);
      else
        c8_op_illegal(__LINE__);
    break;


    case 0xA: c8_op_ANNN(op); break;
    case 0xB: c8_op_BNNN(op); break;
    case 0xC: c8_op_CXNN(op); break;

    case 0xD:
      if((op & 0xf) == 0)
        c8_op_DXY0(op);
      else
        c8_op_DXYN(op);
    break;


    case 0xE:
      if((op & 0xFF) == 0x9E)
        c8_op_EX9E(op);
      else if((op & 0xFF) == 0xA1)
        c8_op_EXA1(op);
      else
        c8_op_illegal(__LINE__);
    break;


    case 0xF:
      switch(op & 0xFF)
      {
        case 0x07: c8_op_FX07(op); break;
        case 0x0A: c8_op_FX0A(op); break;
        case 0x15: c8_op_FX15(op); break;
        case 0x18: c8_op_FX18(op); break;
        case 0x1E: c8_op_FX1E(op); break;
        case 0x29: c8_op_FX29(op); break;
        case 0x39: c8_op_FX30(op); break; /* superchip */
        case 0x33: c8_op_FX33(op); break;
        case 0x55: c8_op_FX55(op); break;
        case 0x65: c8_op_FX65(op); break;
        case 0x75: c8_op_FX75(op); break;
        case 0x85: c8_op_FX85(op); break;
      }
    break;


    default:
      c8_op_illegal(__LINE__);
    break;
  }
}


void c8_exec_frame(int ops, int fps)
{
  static double carry_ops = 0;
  static time_t last_frame = SDL_GetTicks();
  int exec_ops;



  carry_ops += ((double)ops/1000.0f) * ((SDL_GetTicks() - paused_ticks) - last_frame);
  exec_ops = (int)carry_ops;
  carry_ops -= exec_ops;


  for(int op = 0; op < exec_ops; op++)
  {
    if(op%32 == 0)
      input_update();

    c8_exec();
  }

  last_frame = SDL_GetTicks() - paused_ticks;
}


void c8_render(int x, int y, int w, int h)
{
  int sw, sh;
  float pw, ph;
  SDL_Rect panel = {x, y, w, h};

  if(c8.xscr)
  {
    sw = CHIP8_SCREEN_WIDTH * 2;
    sh = CHIP8_SCREEN_HEIGHT * 2;
  }
  else
  {
    sw = CHIP8_SCREEN_WIDTH;
    sh = CHIP8_SCREEN_HEIGHT;
  }

  pw = (float)w / sw;
  ph = (float)h / sh;


  SDL_Surface *s = SDL_GetVideoSurface();
  SDL_FillRect(s, &panel, SDL_MapRGB(s->format, bg.r, bg.g, bg.b));

  float px1 = 0, px2 = pw;
  for(int sx = 0; sx < sw; sx++, px1 = px2, px2 += pw)
  {
    float py1 = 0, py2 = ph;
    for(int sy = 0; sy < sh; sy++, py1 = py2, py2 += ph)
    {
      if(c8.scr[sx][sy] == 0)
        continue;

      SDL_Rect tr;

      tr.x = (Sint16)(x + px1);
      tr.y = (Sint16)(y + py1);
      tr.w = (Sint16)(px2 - (int)px1);
      tr.h = (Sint16)(py2 - (int)py1);

      SDL_FillRect(s, &tr, SDL_MapRGB(s->format, fg.r, fg.g, fg.b));
    }
  }
}

void c8_delay(int fs)
{
  static int l = SDL_GetTicks();
  unsigned int t = 1000/fs;

  if(SDL_GetTicks() - l < t)
  {
    SDL_Delay(t - (SDL_GetTicks() - l));
  }

  l = SDL_GetTicks();
}

void c8_pause(bool is)
{
  if(is == paused)
    return;

  paused = is;

  if(is)
    paused_start = SDL_GetTicks();
  else
    paused_ticks += SDL_GetTicks() - paused_start;
}

void c8_set_fg(SDL_Color _fg)
{
  fg = _fg;
}

void c8_set_bg(SDL_Color _bg)
{
  bg = _bg;
}

const std::string &c8_rompath()
{
  return rompath;
}

void c8_st_push(u16 ad)
{
  c8.st[c8.stc] = ad;
  c8.stc++;
  c8.stc &= 0xf;
}

u16 c8_st_pop(void)
{
  c8.stc--;
  c8.stc &= 0xf;
  return c8.st[c8.stc];
}


u8 &c8_ram(int a)
{
  if(a < 0 || a >= 0xfff)
      c8_ram_illegal(__LINE__);

  return c8.ram[a];
}

void c8_op_0NNN(u16 op)
{
  c8_op_illegal(__LINE__);
}

void c8_op_00E0(u16 op)
{
  memset(c8.scr, 0, sizeof(c8.scr));
}

void c8_op_00EE(u16 op)
{
  c8.pc = c8_st_pop();
}

void c8_op_1NNN(u16 op)
{
  c8.pc = NNN;
}

void c8_op_2NNN(u16 op)
{
  c8_st_push(c8.pc);
  c8.pc = NNN;
}

void c8_op_3XNN(u16 op)
{
  if(VX == NN)
    c8.pc += 2;
}

void c8_op_4XNN(u16 op)
{
  if(VX != NN)
    c8.pc += 2;
}

void c8_op_5XY0(u16 op)
{
  if(VX == VY)
    c8.pc += 2;
}

void c8_op_6XNN(u16 op)
{
  VX = NN;
}

void c8_op_7XNN(u16 op)
{
  VX += NN;
}

void c8_op_8XY0(u16 op)
{
  VX = VY;
}

void c8_op_8XY1(u16 op)
{
  VX |= VY;
}

void c8_op_8XY2(u16 op)
{
  VX &= VY;
}

void c8_op_8XY3(u16 op)
{
  VX ^= VY;
}

void c8_op_8XY4(u16 op)
{
  if((u16)VX + (u16)VY > 0xFF)
    VF = 1;
  else
    VF = 0;
  VX += VY;
}

void c8_op_8XY5(u16 op)
{
  if(VX < VY)
    VF = 0;
  else
    VF = 1;

  VX -= VY;
}

void c8_op_8XY6(u16 op)
{
  VF = VX & 0x1;
  VX >>= 1;
}

void c8_op_8XY7(u16 op)
{
  VX = VY - VX;

  if(X > Y)
    VF = 0;
  else
    VF = 1;
}

void c8_op_8XYE(u16 op)
{
  VF = VX >> 7;
  VX <<= 1;
}

void c8_op_9XY0(u16 op)
{
  if(VX != VY)
    c8.pc += 2;
}

void c8_op_ANNN(u16 op)
{
  c8.i = NNN;
}

void c8_op_BNNN(u16 op)
{
  c8.pc = (NNN + c8.reg[0x0]) & 0xfff;
}

void c8_op_CXNN(u16 op)
{
  VX = rand() & NN;
}

void c8_op_DXYN(u16 op)
{
  u8 sw, sh;
  u8 h = N;
  u8 sx = VX;
  u8 sy = VY;

  if(c8.xscr)
  {
    sw = CHIP8_SCREEN_WIDTH * 2;
    sh = CHIP8_SCREEN_HEIGHT * 2;
  }
  else
  {
    sw = CHIP8_SCREEN_WIDTH;
    sh = CHIP8_SCREEN_HEIGHT;
  }


  VF = 0;
  for(u8 y = 0; y < h; y++)
  {
    for(u8 x = 0; x < 8; x++)
    {
      u8 p = (c8_ram(c8.i + y) & (1<<(7-x))) != 0 ? 1 : 0;

      if(!p)
        continue;

      int vx = (x + sx) % sw;
      int vy = (y + sy) % sh;

      if(p && c8.scr[vx][vy])
        VF = 1;

      c8.scr[vx][vy] ^= p;
    }
  }
}

void c8_op_EX9E(u16 op)
{
  if(c8.key[VX])
    c8.pc += 2;
}

void c8_op_EXA1(u16 op)
{
  if(!c8.key[VX])
    c8.pc += 2;
}

void c8_op_FX07(u16 op)
{
  VX = c8.td;
}

void c8_op_FX0A(u16 op)
{
  c8.ak = true;
  c8.ak_reg = X;
}

void c8_op_FX15(u16 op)
{
  c8.td = VX;
}

void c8_op_FX18(u16 op)
{
  c8.ts = VX;
}

void c8_op_FX1E(u16 op)
{
  c8.i += VX;
}

void c8_op_FX29(u16 op)
{
  c8.i = VX * 5 + 160;
}

void c8_op_FX33(u16 op)
{
  u8 val = VX;

  c8_ram(c8.i) = val / 100;
  c8_ram(c8.i+1) = (val / 10) % 10;
  c8_ram(c8.i+2) = val % 10;
}

void c8_op_FX55(u16 op)
{
  for(u8 r = 0; r <= X; r++)
  {
    c8_ram(c8.i + r) = c8.reg[r];
  }

  //c8.i += X+1;
}

void c8_op_FX65(u16 op)
{
  for(u8 r = 0; r <= X; r++)
  {
    c8.reg[r] = c8_ram(c8.i + r);
  }

  //c8.i += X+1;
}

/**
  SUPERCHIP OPCODES
**/
void c8_op_00CN(u16 op)
{
  for(int y = CHIP8_SCREEN_HEIGHT * 2 - 1; y >= N; y--)
  {
    for(int x = 0; x < CHIP8_SCREEN_WIDTH * 2; x++)
    {
      c8.scr[x][y] = c8.scr[x][y-N];
    }
  }

  for(int y = 0; y < N; y++)
  {
    for(int x = 0; x < CHIP8_SCREEN_WIDTH * 2; x++)
    {
      c8.scr[x][y] = 0;
    }
  }
}

void c8_op_00FB(u16 op)
{
  int scroll, sw, sh;

  if(c8.xscr)
  {
    scroll = 4;
    sw = CHIP8_SCREEN_WIDTH*2;
    sh = CHIP8_SCREEN_HEIGHT*2;
  }
  else
  {
    scroll =  2;
    sw = CHIP8_SCREEN_WIDTH;
    sh = CHIP8_SCREEN_HEIGHT;
  }

  for(int x = scroll; x < sw; x++)
  {
    for(int y = 0; y < sh; y++)
    {
      c8.scr[x-scroll][y] = c8.scr[x][y];
    }
  }
}

void c8_op_00FC(u16 op)
{
  int scroll, sw, sh;

  if(c8.xscr)
  {
    scroll = 4;
    sw = CHIP8_SCREEN_WIDTH*2;
    sh = CHIP8_SCREEN_HEIGHT*2;
  }
  else
  {
    scroll =  2;
    sw = CHIP8_SCREEN_WIDTH;
    sh = CHIP8_SCREEN_HEIGHT;
  }

  for(int x = sw - scroll - 1; x >= 0; x--)
  {
    for(int y = 0; y < sh; y++)
    {
      c8.scr[x-scroll][y] = c8.scr[x][y];
    }
  }
}

void c8_op_00FD(u16 op)
{
  c8.qf = true;
}

void c8_op_00FE(u16 op)
{
  c8.xscr = false;
}

void c8_op_00FF(u16 op)
{
  c8.xscr = true;
}

void c8_op_DXY0(u16 op)
{
  u8 sw, sh;
  u8 w;
  u8 sx = VX;
  u8 sy = VY;


  if(c8.xscr)
  {
    sw = CHIP8_SCREEN_WIDTH * 2;
    sh = CHIP8_SCREEN_HEIGHT * 2;
  }
  else
  {
    sw = CHIP8_SCREEN_WIDTH;
    sh = CHIP8_SCREEN_HEIGHT;
  }

  if(c8.xscr)
    w = 16;
  else
    w = 8;

  VF = 0;
  for(u8 y = 0; y < 16; y++)
  {
    for(u8 x = 0; x < w; x++)
    {
      u8 p;

      if(x < 0x8)
        p = (c8_ram(c8.i + y*(w/8)) & (1<<(7-x))) != 0 ? 1 : 0;
      else
        p = (c8_ram(c8.i + y*(w/8) + 1) & (1<<(7-(x-8)))) != 0 ? 1 : 0;

      if(!p)
        continue;

      int vx = (x + sx) % sw;
      int vy = (y + sy) % sh;

      if(p && c8.scr[vx][vy])
        VF = 1;

      c8.scr[vx][vy] ^= p;
    }
  }
}

void c8_op_FX30(u16 op)
{
  c8.i = X * 10;
}

void c8_op_FX75(u16 op)
{
  memcpy(c8.rpl, c8.reg, (X & 0x7) + 1);
}

void c8_op_FX85(u16 op)
{
  memcpy(c8.reg, c8.rpl, (X & 0x7) + 1);
}

/**
  /SUPERCHIP OPCODES
**/



void c8_op_illegal(int line)
{
  u16 op;

  if(c8.pc < 2)
  {
    printf("Illegal op detected in code line %i\n", line);
    throw C8_ILLEGAL_OPCODE;
  }

  op = c8_ram(c8.pc-2) << 8;
  op |= c8_ram(c8.pc-1);

  printf("Illegal op %X at %X detected in code line %i\n", (int)op, (int)(c8.pc-2), line);
  throw C8_ILLEGAL_OPCODE;
}


void c8_ram_illegal(int line)
{
  u16 op;


  if(c8.pc < 2)
  {
    printf("Illegal ram access detected in code line %i\n", line);
    throw C8_ILLEGAL_RAM_ACCESS;
  }

  op = c8_ram(c8.pc-2) << 8;
  op |= c8_ram(c8.pc-1);

  printf("Illegal ram access by op %X at %X detected in code line %i\n", (int)op, (int)(c8.pc-2), line);
  throw C8_ILLEGAL_RAM_ACCESS;
}

