PROJECT = shader
IN      = src/*.cpp src/**/*.cpp
OUT     = bin/$(PROJECT)

CC      = g++
CFLAGS  = -O2 -ansi -pedantic -g -Wall
LFLAGS  = -lGL -lGLU -lSDL -lnoise

all: $(PROJECT)

$(PROJECT): $(IN)
	$(CC) $(CFLAGS) $(IN) -o "$(OUT)" $(LFLAGS)

