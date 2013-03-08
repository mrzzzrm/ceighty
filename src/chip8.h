#ifndef CHIP8_H
#define CHIP8_H

  #include <iostream>
  #include <string.h>
  #include <stdlib.h>
  #include <stdio.h>
  #include <inttypes.h>
  #include <SDL/SDL.h>

  #include "input.hpp"
  #include "c8_exception.hpp"


  typedef uint8_t u8;
  typedef uint16_t u16;
  typedef int16_t s16;


  #define CHIP8_SCREEN_WIDTH 64
  #define CHIP8_SCREEN_HEIGHT 32

  extern int c8_cycle_count;



  /* General functions */
  int c8_load_rom(const char *path);
  void c8_init(void);
  void c8_reset(void);
  void c8_set_keymap(const int *_keymap);

  void c8_exec(void);
  void c8_exec_frame(int ops, int fs);

  void c8_render(int x, int y, int w, int h);
  void c8_delay(int fs);

  void c8_pause(bool is);

  void c8_set_fg(SDL_Color _fg);
  void c8_set_bg(SDL_Color _bg);

  const std::string &c8_rompath();


  /* Stack */
  void c8z_st_push(u16 ad);
  u16 c8_st_pop(void);


  /* Memory access*/
  u8 &c8_ram(int a);


  /* Chip8 Opcodes */
  void c8_op_0NNN(u16 op);
  void c8_op_00E0(u16 op);
  void c8_op_00EE(u16 op);
  void c8_op_1NNN(u16 op);
  void c8_op_2NNN(u16 op);
  void c8_op_3XNN(u16 op);
  void c8_op_4XNN(u16 op);
  void c8_op_5XY0(u16 op);
  void c8_op_6XNN(u16 op);
  void c8_op_7XNN(u16 op);
  void c8_op_8XY0(u16 op);
  void c8_op_8XY1(u16 op);
  void c8_op_8XY2(u16 op);
  void c8_op_8XY3(u16 op);
  void c8_op_8XY4(u16 op);
  void c8_op_8XY5(u16 op);
  void c8_op_8XY6(u16 op);
  void c8_op_8XY7(u16 op);
  void c8_op_8XYE(u16 op);
  void c8_op_9XY0(u16 op);
  void c8_op_ANNN(u16 op);
  void c8_op_BNNN(u16 op);
  void c8_op_CXNN(u16 op);
  void c8_op_DXYN(u16 op);
  void c8_op_EX9E(u16 op);
  void c8_op_EXA1(u16 op);
  void c8_op_FX07(u16 op);
  void c8_op_FX0A(u16 op);
  void c8_op_FX15(u16 op);
  void c8_op_FX18(u16 op);
  void c8_op_FX1E(u16 op);
  void c8_op_FX29(u16 op);
  void c8_op_FX33(u16 op);
  void c8_op_FX55(u16 op);
  void c8_op_FX65(u16 op);


  /* SuperChip8 Opcodes */
  void c8_op_00CN(u16 op);
  void c8_op_00FB(u16 op);
  void c8_op_00FC(u16 op);
  void c8_op_00FD(u16 op);
  void c8_op_00FE(u16 op);
  void c8_op_00FF(u16 op);
  void c8_op_DXY0(u16 op);
  void c8_op_FX30(u16 op);
  void c8_op_FX75(u16 op);
  void c8_op_FX85(u16 op);


  /* Illegal instruction */
  void c8_op_illegal(int line);
  void c8_ram_illegal(int line);



#endif // CHIP8_H
