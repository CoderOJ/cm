CXX := g++
CPPFLAGS += -std=c++17
CPPFLAGS += -g -Wall -Wextra -Wconversion -Wno-sign-conversion 
CPPFLAGS += -fsanitize=address,undefined -fno-sanitize-recover 
CPPFLAGS += -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC 
CPPFLAGS += -DCM_DEBUG

SRC := $(wildcard *.hpp)
GCH := $(patsubst %.hpp, %.hpp.gch, ${SRC})

all: $(GCH)

clean:
	rm $(GCH)

%.gch: %
	${CXX} ${CPPFLAGS} $<
