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


TARGET := $(shell basename $(shell pwd))

EXEC_SUFFIX := elf

CFLAGS := @compile_flags.txt

SRC_DIR := src
INC_DIR := inc
BIN_DIR := bin
OBJ_DIR := $(BIN_DIR)/obj


HEADERS := $(shell find ./$(INC_DIR) -name "*.h")
SRCS := $(shell find ./$(SRC_DIR) -name "*.c")
OBJS := $(patsubst ./$(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

OUT := $(BIN_DIR)/$(TARGET).$(EXEC_SUFFIX)

$(TARGET): $(OUT)

$(OUT): $(OBJS) $(HEADERS)
	$(CC) -o $@ $(OBJS) $(CFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) -c -o $@ $< $(CFLAGS)

.PHONY: clean

clean:
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)
	rm -f $(OBJS)
	rm -f $(OUT)
