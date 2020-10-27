IDIR = .
CC = gcc
CFLAGS = -I$(IDIR)

OUT_DIR = out
OUTPUT = $(OUT_DIR)/proceso

.PHONY: all clean

all: clean main

main: main.c
	@mkdir -p $(OUT_DIR)
	@$(CC) -o $(OUTPUT) main.c $(CFLAGS)

clean:
	@rm -f $(OUTPUT) *~ core $(IDIR)/*~