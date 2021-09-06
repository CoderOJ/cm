CXX := clang++ -std=c++17
CPPFLAGS := -Wall -Wextra -Wconversion -Wno-sign-conversion -g -fsanitize=address,undefined -DCM_DEBUG
REMFLAGS := -O2

SRC := $(wildcard *.cpp)
OBJ := $(patsubst %.cpp, %, ${SRC})
RUN := $(patsubst %.cpp, %_run, ${SRC})
REM := $(patsubst %.cpp, %_rem, ${SRC})


all: $(OBJ)

%: %.cpp
	@$(CXX) $(CPPFLAGS) -o $@ $^


in:
	@touch in

.PHONY:run
run: in $(RUN)

%_run: %
	@echo -e "\033[1;34m--- $^ ---\033[0m"
	@time ./$^ < in > $^.out


.PHONY:remote
remote: $(REM)


%_rem: %
	$(CXX) $(REMFLAGS) -o $^ $^.cpp
