all: bin build
	make -C src

export PATH := $(PWD)/../../bin:$(PATH)

bin:
	mkdir -p ./bin

build:
	mkdir -p ./build

.PHONY: all clean
clean:
	rm -rf ./bin ./build
	make -C src clean
	

