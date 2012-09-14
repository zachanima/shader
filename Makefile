PROJECT = shader
IN      = src/*.cpp src/**/*.cpp
OUT     = bin/$(PROJECT)

CC      = g++
CFLAGS  = -O2 -ansi -pedantic -g -I/usr/include/ImageMagick # -Wall
LFLAGS  = -lGL -lGLU -lSDL -lnoise -lMagick++

all: $(PROJECT)

$(PROJECT): $(IN)
	$(CC) $(CFLAGS) $(IN) -o "$(OUT)" $(LFLAGS)

