IDIR = .
CC = gcc
CFLAGS = -I$(IDIR)

OUT_DIR = out

.PHONY: all clean

all: clean reader writer

reader: reader.c
	@mkdir -p $(OUT_DIR)
	@$(CC) -o $(OUT_DIR)/reader reader.c $(CFLAGS)

writer: writer.c
	@mkdir -p $(OUT_DIR)
	@$(CC) -o $(OUT_DIR)/writer writer.c $(CFLAGS)

clean:
	@rm -f $(OUT_DIR)/* *~ core $(IDIR)/*~