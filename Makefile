CC=gcc
CFLAGS=-w
STD=c23

SRC := $(wildcard examples/*.c)
BIN := $(patsubst examples/%.c, build/%, $(SRC))
SHOW_TARGETS := $(patsubst examples/%.c, show/%, $(SRC))

.PHONY: nix build-examples show-examples clean $(SHOW_TARGETS)

nix:
	nix-shell -p libgccjit gnumake gdb binutils

build-examples:
	@for src in $(SRC); do \
		target="build/$$(basename $$src .c)"; \
		echo "Building $$target..."; \
		$(MAKE) "$$target" || echo "[ERROR]: Failed to build $$target"; \
	done

show-examples:
	@echo "Available examples:"
	@for bin in $(BIN); do \
		echo "  - $$(basename $$bin)"; \
	done
	@echo ""
	@echo "Run with: make show/<example_name>"

show/%: build/%
	@echo "[INFO]: Running $<..."
	@./$<

build/%: examples/%.c | build
	$(CC) $(CFLAGS) -std=$(STD) -o $@ $<
	chmod +x $@
	@echo "[INFO]: built $@"

build:
	mkdir -p build

clean:
	rm -rf build