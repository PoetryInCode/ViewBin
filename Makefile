all: build ;

build: viewbin.c
	cc $^ -o viewbin

install:
	sudo cp viewbin /usr/local/bin/
