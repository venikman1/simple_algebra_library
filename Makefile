#
# Makefile
# Game prototype project
#
# Created by AnyKeyShik Rarity 2018-19-12
#

# Compilers
CC := g++

# Flags
CFLAGS := -g --std=c++17
LDFLAGS :=
INCLUDE_FLAGS := -I headers

# Folders
SRCDIR := src
TARGETDIR := bin

# Files
SRCTEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCTEXT))
DEBUG_EXECUTABLE := build_debug
EXECUTABLE := build

# Target
TARGET := $(TARGETDIR)/$(EXECUTABLE)
DEBUG_TARGET := $(TARGETDIR)/$(DEBUG_EXECUTABLE)

first: release

$(TARGET):
	@mkdir -p bin
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) $(SOURCES) $(LDFLAGS) -o $(TARGET)

$(DEBUG_TARGET):
	@mkdir -p bin
	$(CC) $(CFLAGS) $(INCLUDE_FLAGS) $(SOURCES) $(LDFLAGS) -g -o $(DEBUG_TARGET)

debug: $(DEBUG_TARGET)

release: $(TARGET)

clean:
	@echo "Cleaning binaries..."
	@rm -rf bin

.PHONY: first, test, release, clean
