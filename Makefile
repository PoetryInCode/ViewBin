CC=cc
INSTALL_DIR=/usr/local/bin/
OUT=viewbin

all: build ;

build: viewbin.c
	cc $^ -o $(OUT)

install:
	sudo cp viewbin $(INSTALL_DIR)

uninstall:
	sudo rm $(INSTALL_DIR)$(OUT)
