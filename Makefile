CC	= g++

TARGET	= game

$(TARGET):	main.o physics.o 
	$(CC) -o $(TARGET) main.o physics.o `sdl-config --libs` -lGL -lGLU -lm

main.o:		main.c main.h
	$(CC) -c main.c `sdl-config --cflags`

physics.o:	physics.c main.h
	$(CC) -c physics.c `sdl-config --cflags`

clean:
	rm -f $(TARGET) main.o physics.o core
