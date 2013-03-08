CC = 		g++
CFLAGS = 	-Wall 
LDFLAGS = 	-lSDL -lSDL_ttf -lSDL_image

OBJ = 	src/chip8.o src/controls.o src/input.o  src/main.o src/c8y_media.o \
		src/config.o src/emu.o src/load.o src/options.o

.PHONY = clean
       
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
	 
ceighty: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LDFLAGS)


clean:
	rm src/*.o ceighty

