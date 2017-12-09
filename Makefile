Q = @
CC = g++
CXXVERSION = -std=c++1z

OUT_DIR := bin
SRC_DIR := src
OBJ_DIR := obj
INC_DIR := inc
PACKAGES := eigen3 zlib
CFLAGS := $(shell pkg-config $(PACKAGES) --cflags) -I $(INC_DIR) -g -pipe -Wall -march=native -mtune=native -msse2 -O2 -fPIC
LFLAGS := $(shell pkg-config $(PACKAGES) --libs) -ldl -Wl,--rpath=.

OUT_FILE := $(OUT_DIR)/main
CPP_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(addprefix $(OBJ_DIR)/,$(notdir $(CPP_FILES:.cpp=.o)))

GEN_DIR = src/generator
GEN_CPP = $(wildcard $(GEN_DIR)/*.cpp)
GEN_OUT = $(addprefix $(OUT_DIR)/,$(notdir $(GEN_CPP:.cpp=.so)))


all: build
	
build: $(OBJ_FILES)
	@echo "	LD $(OUT_FILE)"
	$(Q)mkdir -p $(OUT_DIR)
	$(Q)$(CC) $(OBJ_FILES) -o $(OUT_FILE) $(LFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "	CC $<"
	$(Q)mkdir -p $(OBJ_DIR)
	$(Q)$(CC) -c $< -o $@ $(CFLAGS)

gen: $(GEN_OUT)

$(OUT_DIR)/%.so: $(GEN_DIR)/%.cpp
	@echo "	CC $<"
	@echo "	LD $@"
	$(Q)mkdir -p $(OUT_DIR)
	$(Q)$(CC) $< -o $@ $(CFLAGS) -shared

clean:
	rm obj -R
