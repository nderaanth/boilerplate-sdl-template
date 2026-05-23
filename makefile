ifeq ($(OS),Windows_NT)
    DETECTED_OS := Windows
    RM = del /Q /F
    RMD_DIR = rmdir /S /Q
    MKDIR = mkdir
    EXE_EXT := .exe
    FIX_PATH = $(subst /,\,$1)
    # Silence colors on Windows cmd unless explicitly supported
    CLR_CXX := 
    CLR_LD  := 
    CLR_RST := 
else
    DETECTED_OS := $(shell uname -s)
    RM = rm -f
    RMD_DIR = rm -rf
    MKDIR = mkdir -p
    EXE_EXT :=
    FIX_PATH = $1
    # ANSI Color Escapes for Linux/macOS
    CLR_CXX := \033[1;34m
    CLR_LD  := \033[1;32m
    CLR_RST := \033[0m
endif

# --- Project Paths ---
TARGET_NAME := IndustrialGradeCoolant
TARGET      := $(TARGET_NAME)$(EXE_EXT)

SRC_DIR     := src
INC_DIR     := include
BUILD_DIR   := build
OBJ_DIR     := $(BUILD_DIR)/obj

# --- Compiler & Flags ---
CXX      := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -O2
CPPFLAGS := -I$(INC_DIR) -MMD -MP

# --- OS-Specific Dependencies ---
ifeq ($(DETECTED_OS),Windows)
    LDFLAGS  := -lmingw32 -lSDL3 -mwindows
else ifeq ($(DETECTED_OS),Darwin)
    LDFLAGS  := $(shell pkg-config --libs sdl3)
    CPPFLAGS += $(shell pkg-config --cflags sdl3)
else
    LDFLAGS  := $(shell pkg-config --libs sdl3)
    CPPFLAGS += $(shell pkg-config --cflags sdl3)
endif

# --- File Discovery ---
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))
DEPS := $(OBJS:.o=.d)

# --- Build Rules ---
.PHONY: all clean info

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo -e "$(CLR_LD)LD$(CLR_RST)  $@"
	@$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@$(if $(wildcard $(OBJ_DIR)),,$(MKDIR) $(call FIX_PATH,$(OBJ_DIR)))
	@echo -e "$(CLR_CXX)CXX$(CLR_RST) $<"
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

-include $(DEPS)

# --- Diagnostics & Clean ---
info:
	@echo "Detected OS: $(DETECTED_OS)"
	@echo "Target:      $(TARGET)"
	@echo "Sources:     $(SRCS)"
	@echo "Objects:     $(OBJS)"

clean:
	@echo "Cleaning build artifacts..."
ifeq ($(OS),Windows_NT)
	@if exist $(TARGET) $(RM) $(TARGET)
	@if exist $(BUILD_DIR) $(RMD_DIR) $(call FIX_PATH,$(BUILD_DIR))
else
	@$(RM) $(TARGET)
	@$(RMD_DIR) $(BUILD_DIR)
endif
