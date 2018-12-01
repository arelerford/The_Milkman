CFLAGS = -I ./include
LFLAGS = -lrt -lX11 -lGLU -lGL -pthread -lm -std=c++11

all: milkman

milkman: milkman.cpp log.cpp
	g++ $(CFLAGS) milkman.cpp log.cpp justinS.cpp austinR.cpp isaacL.cpp alexH.cpp libggfonts.a -Wall -Wextra $(LFLAGS) -omilkman

clean:
	rm -f milkman
	rm -f *.o

