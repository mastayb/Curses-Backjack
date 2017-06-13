
CXXFLAGS+=-g -Wall -Wextra -pthread
CXXFLAGS+=-std=c++14

LDFLAGS+=-lncurses

all: hello 

hello: hello_cards.cpp
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) -o $@ 

clean:
	rm -f hello
