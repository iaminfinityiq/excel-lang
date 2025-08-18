# config.mk

CXX      = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -O2
LDFLAGS  =
INCLUDES = -Isrc

SRC_DIR  = src/frontend
OBJ_DIR  = build

# Function to recursively find C++ source files
define find_cpp_sources
$(wildcard $(1)/*.cpp) $(wildcard $(1)/*.cc) $(wildcard $(1)/*.cxx) \
$(foreach d,$(wildcard $(1)/*),$(call find_cpp_sources,$(d)))
endef

SRC_FILES := $(call find_cpp_sources,$(SRC_DIR))
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,\
               $(patsubst $(SRC_DIR)/%.cc,$(OBJ_DIR)/%.o,\
                 $(patsubst $(SRC_DIR)/%.cxx,$(OBJ_DIR)/%.o,$(SRC_FILES))))