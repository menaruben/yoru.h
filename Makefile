CC=gcc
CFLAGS=-Wall -Wextra -Wno-unused-but-set-parameter
STD=c23

SRC := $(wildcard examples/*.c)
BIN := $(patsubst examples/%.c, build/%, $(SRC))

.PHONY: nix build-examples clean

nix:
	nix-shell -p libgccjit gnumake gdb binutils

build-examples: $(BIN)

build/%: examples/%.c | build
	$(CC) $(CFLAGS) -std=$(STD) -o $@ $<
	chmod +x $@
	@echo "[INFO]: built $@"

build:
	mkdir -p build

clean:
	rm -rf build
