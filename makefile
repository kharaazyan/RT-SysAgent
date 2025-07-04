###############################################################################
# Nexus Agent - Professional Makefile
# Supports: Out-of-source builds, debug/release, dependency tracking, install
###############################################################################

# === Project Metadata ===
PROJECT      := agent
VERSION      := 1.0.0

# === Directories ===
SRC_DIR      := src
INC_DIR      := include
BUILD_DIR    := build
BIN_DIR      := bin
DIST_DIR     := dist
TEST_DIR     := tests

# === Tools ===
CXX          ?= g++
AR           ?= ar
RM           := rm -rf
MKDIR        := mkdir -p

# === Flags ===
CXXFLAGS     := -std=c++20 -Wall -Wextra -I$(INC_DIR)
LDFLAGS      :=
DEBUG_FLAGS  := -g -O0
RELEASE_FLAGS:= -O2

# === Build Type ===
BUILD        ?= debug
ifeq ($(BUILD),release)
    CXXFLAGS += $(RELEASE_FLAGS)
    BUILD_TYPE := Release
else
    CXXFLAGS += $(DEBUG_FLAGS)
    BUILD_TYPE := Debug
endif

# === Source/Objects/Deps ===
SRCS         := $(wildcard $(SRC_DIR)/*.cpp)
OBJS         := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
DEPS         := $(OBJS:.o=.d)

# === Executable(s) ===
TARGET       := $(BIN_DIR)/$(PROJECT)

# === Colors ===
GREEN        := \033[0;32m
YELLOW       := \033[1;33m
RED          := \033[0;31m
NC           := \033[0m

# === Verbosity ===
V            ?= 0
ifeq ($(V),0)
    Q := @
else
    Q :=
endif

###############################################################################
# Targets
###############################################################################

.PHONY: all clean rebuild install uninstall test lint format docs help

all: $(TARGET)
	@echo "$(GREEN)[✔] Build complete ($(BUILD_TYPE))$(NC)"

$(TARGET): $(OBJS) | $(BIN_DIR)
	@echo "$(YELLOW)[Linking] $@$(NC)"
	$(Q)$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@echo "$(YELLOW)[Compiling] $<$(NC)"
	$(Q)$(CXX) $(CXXFLAGS) -MMD -c $< -o $@

$(BUILD_DIR) $(BIN_DIR) $(DIST_DIR):
	$(Q)$(MKDIR) $@

-include $(DEPS)

clean:
	$(Q)$(RM) $(BUILD_DIR) $(BIN_DIR) $(DIST_DIR)
	@echo "$(GREEN)[✔] Cleaned$(NC)"

rebuild: clean all

help:
	@echo "Usage: make [target] [BUILD=debug|release] [V=1]"
	@echo ""
	@echo "Targets:"
	@echo "  all        - Build the project (default)"
	@echo "  clean      - Remove all build artifacts"
	@echo "  rebuild    - Clean and build"
	@echo "  help       - Show this help message"
	@echo ""
	@echo "Variables:"
	@echo "  BUILD      - debug (default) or release"
	@echo "  V          - Verbose build (V=1)"