all: rasm

CC := gcc
LDFLAGS := -lm

ifeq ($(OS),Windows_NT)
    TARGET := rasm.exe
else
    TARGET := rasm
endif

rasm: rasm_v0120.c
	$(CC) $< -s -O2 $(LDFLAGS) -o $@
	cp $(TARGET) ../../bin
	rm -f $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean

