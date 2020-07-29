CC = g++
NAME = chess
TEST = test

LOGIC_SRC = $(wildcard logic/src/**/*.cpp)
LOGIC_OBJ = $(LOGIC_SRC:logic/src/%.cpp=logic/obj/%.o)

VIEW_SRC = $(wildcard view/src/console/*.cpp)
VIEW_OBJ = $(VIEW_SRC:view/src/%.cpp=logic/obj/%.o)

TESTS_SRC = $(wildcard tests/src/*.cpp)
TESTS_OBJ = $(VIEW_SRC:tests/src/%.cpp=logic/obj/%.o)

WFLAGS = -Wall -Wextra -Werror -Wpedantic -Wno-unused-variable -Wno-unused-function -Wno-unused-parameter
LFLAGS = -Ilogic/inc -Iview/inc -Itests/inc
OFLAGS = -Ofast -march=native -fomit-frame-pointer -flto
# OFLAGS = -O0 -g3 -ftrapv -fno-omit-frame-pointer
SFML = -ISFML-2.5.1/include -LSFML-2.5.1/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -lsfml-audio

COMPILE = $(CC) -std=c++17 -pipe $(WFLAGS) $(LFLAGS) $(OFLAGS)
RM = rm -rf

all: $(NAME)

$(LOGIC_OBJ)%.o: $(LOGIC_SRC)%.cpp
	@$(COMPILE) -c $< -o $@

$(VIEW_OBJ)%.o: $(VIEW_SRC)%.cpp
	@$(COMPILE) -c $< -o $@

$(TESTS_OBJ)%.o: $(TESTS_SRC)%.cpp
	@$(COMPILE) -c $< -o $@

$(NAME): $(LOGIC_OBJ) $(VIEW_OBJ)
	@$(COMPILE) $^ -o $@

test: $(TEST)

$(TEST): $(LOGIC_OBJ) $(TESTS_OBJ)
	@$(COMPILE) $^ -o $@

clean:
	@$(RM) $(LOGIC_OBJ) $(VIEW_OBJ) $(TESTS_OBJ)

uninstall: clean
	@$(RM) $(NAME)

reinstall: uninstall all

.PHONY: all clean test uninstall reinstall

sfml:
	@$(COMPILE) ./view/graphics/main.cpp -o sfml
