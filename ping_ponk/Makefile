CC ?= gcc
CFLAGS ?= -Wall -Wextra -std=c11 -O2

SDL3_CFLAGS := $(shell pkg-config --cflags sdl3 2>/dev/null)
SDL3_LIBS := $(shell pkg-config --libs sdl3 2>/dev/null)
ifeq ($(strip $(SDL3_LIBS)),)
SDL3_LIBS := -lSDL3
endif

SDL3_IMAGE_CFLAGS := $(shell pkg-config --cflags sdl3_image 2>/dev/null)
SDL3_IMAGE_LIBS := $(shell pkg-config --libs sdl3_image 2>/dev/null)
ifeq ($(strip $(SDL3_IMAGE_LIBS)),)
SDL3_IMAGE_LIBS := -lSDL3_image
endif

BUILD_DIR := build
TARGETS := $(BUILD_DIR)/client $(BUILD_DIR)/server $(BUILD_DIR)/pingo

.PHONY: all clean

all: $(TARGETS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/client: client.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/server: server.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/pingo: pingo.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SDL3_CFLAGS) $(SDL3_IMAGE_CFLAGS) -lm $< -o $@ $(SDL3_LIBS) $(SDL3_IMAGE_LIBS)

clean:
	rm -rf $(BUILD_DIR)
