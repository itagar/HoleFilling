CXX= g++
CXXFLAGS= -c -Wextra -Wall -Wvla -std=c++11 -DNDEBUG
CODEFILES= HoleFilling.tar HoleFilling.hpp Makefile README


# Default
default: HoleFilling


# Executables
HoleFilling: HoleFilling.o Pixel.o
	$(CXX) HoleFilling.o Pixel.o -o HoleFilling


# Object Files
HoleFilling.o: HoleFilling.cpp Pixel.h HoleException.h
	$(CXX) $(CXXFLAGS) HoleFilling.cpp -o HoleFilling.o

Pixel.o: Pixel.cpp Pixel.h
	$(CXX) $(CXXFLAGS) Pixel.cpp -o Pixel.o


# tar
tar:
	tar -cvf $(CODEFILES)


# Other Targets
clean:
	-rm -vf *.o HoleFilling

