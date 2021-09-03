CXX := clang++ -std=c++17
CPPFLAGS := -Wall -Wextra -Wconversion -g -DCM_DEBUG -fsanitize=address,undefined

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
