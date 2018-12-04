CFLAGS = -I ./include
LFLAGS = -lrt -lX11 -lGLU -lGL -lm -std=c++11
PFILES = justinS.cpp austinR.cpp isaacL.cpp alexH.cpp

all: main

main: themilkman.cpp isaacL.cpp 
	g++ $(CFLAGS) themilkman.cpp log.cpp $(PFILES) libggfonts.a -Wall -Wextra $(LFLAGS) -othemilkman

clean:
	rm -f themilkman
	rm -f *.o

