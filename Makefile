EXEC = main
CC = clang++

SRC_DIR = src
BUILD_DIR = build
INCLUDE_DIR = include

LDFLAGS = -ldl -lglfw -lGL -lassimp
CFLAGS = -Wall -Werror -std=c++20 -g -I$(INCLUDE_DIR)

SRCS = $(shell find $(SRC_DIR) -name '*.cpp')
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm $(BUILD_DIR)/*.o $(EXEC)
