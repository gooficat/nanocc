#
#
#Copyright 2026 Hasan Sabri
#
#Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
#
#The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
#
#THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#
CC := clang

TARGET := $(shell basename $(shell pwd))

SUBTARGETS :=

PREDEPS :=

EXEC_PREFIX :=
EXEC_SUFFIX := elf


CFLAGS := @compile_flags.txt

SRC_DIR := src
INC_DIR := src
BIN_DIR ?= bin
OBJ_DIR := obj

OUT_CFLAGS := -L$(BIN_DIR) -Wl,-rpath,'$$ORIGIN'


SRCS := $(shell find $(SRC_DIR) -name "*.*")
OBJS := $(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

INSIDE_LINKS := $(patsubst %,$(BIN_DIR)/lib%.so,$(SUBTARGETS))
LINKS := $(INSIDE_LINKS) $(PREDEPS)

OUT = $(BIN_DIR)/$(EXEC_PREFIX)$(TARGET).$(EXEC_SUFFIX)

.PHONY: $(TARGET) clean FORCE

.PHONY: $(LINKS) clean

$(TARGET): $(OUT)

$(INSIDE_LINKS): $(BIN_DIR)/lib%.so: FORCE
	$(MAKE) -C $* BIN_DIR=$(abspath $(BIN_DIR))

FORCE:


$(OUT): $(LINKS) $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) -o $@ $(OBJS) $(CFLAGS) $(OUT_CFLAGS) $(patsubst %,-l%,$(LINKS))

$(OBJ_DIR)/%.c.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) -c -o $@ $< $(CFLAGS) -MMD -MP

$(OBJ_DIR)/%.s.o: $(SRC_DIR)/%.s
	mkdir -p $(dir $@)
	$(CC) -c -o $@ $<

.PHONY: clean

clean:
	rm -f $(OBJS) $(DEPS) $(OUT)
	for d in $(SUBTARGETS); do $(MAKE) -C $$d clean; done

-include $(DEPS)
