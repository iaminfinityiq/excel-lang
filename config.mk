CXX      ?= c++
CXXFLAGS ?= -Wall -Wextra -std=c++17 -O2
LDFLAGS  ?=

ROOT_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
SRC_DIR  := $(ROOT_DIR)/src
OBJ_DIR  := $(ROOT_DIR)/build

INCLUDES := -I$(ROOT_DIR)/src/frontend -I$(ROOT_DIR)/src

SRC_FILES := $(shell find $(SRC_DIR) -type f -name '*.cc')

OBJ_FILES := $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%, $(SRC_FILES:.cc=.o))