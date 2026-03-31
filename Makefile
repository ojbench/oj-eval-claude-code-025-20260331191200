CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra

all: code

code: code.cpp
	$(CXX) $(CXXFLAGS) -o code code.cpp

clean:
	rm -f code

.PHONY: all clean
