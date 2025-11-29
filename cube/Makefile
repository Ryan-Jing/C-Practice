.PHONY: clean run install
.SILENT:

# Installation prefix (default: /usr/local)
PREFIX ?= /usr/local
# For user installation, use: make install PREFIX=~

cube.o: cube.c
	gcc -o $@ $< -lm

run: cube.o
	./$<

clean:
	rm -rf cube.o

install: cube.o
	@echo "Installing cube to $(PREFIX)/bin..."
	mkdir -p $(PREFIX)/bin
	cp cube.o $(PREFIX)/bin/cube
	chmod +x $(PREFIX)/bin/cube
	@echo "Installation complete. Run 'cube' to start."