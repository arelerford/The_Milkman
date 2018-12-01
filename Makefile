CFLAGS = -I ./include
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm -std=c++11

all: milkman

milkman: milkman.cpp
	g++ $(CFLAGS) milkman.cpp justinS.cpp austinR.cpp isaacL.cpp alexH.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -omilkman

clean:
	rm -f milkman
	rm -f *.o

