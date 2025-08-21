include config.mk

ifeq ($(OS),Windows_NT)
    EXE    := .exe
    RMFILE := del /q
    RMDIR  := rmdir /s /q
    MKDIR  = if not exist "$(subst /,\,$1)" mkdir "$(subst /,\,$1)"
else
    EXE    :=
    RMFILE := rm -f
    RMDIR  := rm -rf
    MKDIR  = mkdir -p $1
endif

BIN := frontend$(EXE)

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJ_FILES)
	$(CXX) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(ROOT_DIR)/src/%.cc
	$(call MKDIR,$(dir $@))
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RMDIR) $(OBJ_DIR)
	$(RMFILE) $(BIN)
