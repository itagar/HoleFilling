CXX= g++
CXXFLAGS= -c -Wextra -Wall -Wvla -std=c++11 -DNDEBUG
CODEFILES= HoleFilling.tar HoleFilling.cpp Pixel.cpp Pixel.h Hole.cpp Hole.h HoleException.h Makefile README


# Default
default: HoleFilling


# Executables
HoleFilling: HoleFilling.o Hole.o Pixel.o
	$(CXX) HoleFilling.o Pixel.o Hole.o -o HoleFilling `pkg-config --cflags --libs opencv`


# Object Files
HoleFilling.o: HoleFilling.cpp Pixel.h Hole.h HoleException.h
	$(CXX) $(CXXFLAGS) HoleFilling.cpp -o HoleFilling.o

Pixel.o: Pixel.cpp Pixel.h
	$(CXX) $(CXXFLAGS) Pixel.cpp -o Pixel.o

Hole.o: Hole.cpp Hole.h Pixel.h
	$(CXX) $(CXXFLAGS) Hole.cpp -o Hole.o


# tar
tar:
	tar -cvf $(CODEFILES)


# Other Targets
clean:
	-rm -vf *.o HoleFilling
