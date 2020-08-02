CC = g++
NAME = chess
TERMINAL = terminal
TEST = test

LOGIC_SRC = $(wildcard Logic/src/**/*.cpp)
LOGIC_OBJ = $(LOGIC_SRC:Logic/src/%.cpp=Logic/obj/%.o)

GRAPHICS_SRC = $(wildcard Graphics/src/*.cpp)
GRAPHICS_OBJ = $(GRAPHICS_SRC:Graphics/src/%.cpp=Graphics/obj/%.o)

TERMINAL_SRC = $(wildcard Terminal/src/*.cpp)
TERMINAL_OBJ = $(TERMINAL_SRC:Terminal/src/%.cpp=Terminal/obj/%.o)

TESTS_SRC = $(wildcard Tests/src/*.cpp)
TESTS_OBJ = $(TERMINAL_SRC:Tests/src/%.cpp=tests/obj/%.o)

WFLAGS = -Wall -Wextra -Werror -Wpedantic -Wno-unused-variable -Wno-unused-function -Wno-unused-parameter
LFLAGS = -Ilogic/inc -ITerminal/inc -IGraphics/inc -Itests/inc -ISFML-2.5.1/include
OFLAGS = -Ofast -march=native -fomit-frame-pointer -flto
# OFLAGS = -O0 -g3 -ftrapv -fno-omit-frame-pointer
SFML = -LSFML-2.5.1/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network -lsfml-audio

COMPILE = $(CC) -std=c++17 -pipe $(WFLAGS) $(LFLAGS) $(OFLAGS)
RM = rm -rf

all: $(NAME)

test: $(TEST)

terminal: $(TERMINAL)

$(GRAPHICS_OBJ)%.o: $(GRAPHICS_SRC)%.cpp
	@$(COMPILE) -c $< -o $@

$(LOGIC_OBJ)%.o: $(LOGIC_SRC)%.cpp
	@$(COMPILE) -c $< -o $@

$(TERMINAL_OBJ)%.o: $(TERMINAL_SRC)%.cpp
	@$(COMPILE) -c $< -o $@

$(TESTS_OBJ)%.o: $(TESTS_SRC)%.cpp
	@$(COMPILE) -c $< -o $@

$(NAME): $(LOGIC_OBJ) $(GRAPHICS_OBJ)
	@$(COMPILE) $^ -o $@ $(SFML)

$(TERMINAL): $(LOGIC_OBJ) $(TERMINAL_OBJ)
	@$(COMPILE) $^ -o $@

$(TEST): $(LOGIC_OBJ) $(TESTS_OBJ)
	@$(COMPILE) $^ -o $@

clean:
	@$(RM) $(GRAPHICS_OBJ) $(LOGIC_OBJ) $(TERMINAL_OBJ) $(TESTS_OBJ)

uninstall: clean
	@$(RM) $(NAME) $(TEST)

reinstall: uninstall all

.PHONY: all clean test uninstall reinstall
