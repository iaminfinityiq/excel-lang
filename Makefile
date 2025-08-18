include config.mk

# Detect platform and define commands
ifeq ($(OS),Windows_NT)
	SHELL := cmd.exe
	MKDIR = if not exist "$(subst /,\,$1)" mkdir "$(subst /,\,$1)"
	RMDIR = if exist "$(subst /,\,$1)" rmdir /s /q "$(subst /,\,$1)"
	DEL   = if exist "$(subst /,\,$1)" del /q "$(subst /,\,$1)"
	EXE   = .exe
	PATHSEP = \\
else
	SHELL := /bin/sh
	MKDIR = mkdir -p $1
	RMDIR = rm -rf $1
	DEL   = rm -f $1
	EXE   =
	PATHSEP = /
endif

BIN := frontend$(EXE)

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJ_FILES)
	$(call MKDIR,$(@D))
	$(CXX) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(call MKDIR,$(dir $@))
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	$(call MKDIR,$(dir $@))
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cxx
	$(call MKDIR,$(dir $@))
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(call RMDIR,$(OBJ_DIR))
	$(call DEL,$(BIN))