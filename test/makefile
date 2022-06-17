CXX := g++
CPPFLAGS += -std=c++17
CPPFLAGS += -g -Wall -Wextra -Wconversion -Wno-sign-conversion 
CPPFLAGS += -fsanitize=address,undefined -fno-sanitize-recover 
CPPFLAGS += -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC 
CPPFLAGS += -DCM_DEBUG

SRC  := $(wildcard *.cpp)
OBJ  := $(patsubst %.cpp, %, ${SRC})
TEST := $(patsubst %.cpp, %_test, ${SRC})

all: ${OBJ}

clean:
	rm ${OBJ}

test: ${TEST}

%_test: %
	@echo -e "\033[1;34m--- $^ ---\033[0m"
	@./$^
	@echo 
	@echo 
