all: libs

export PATH := $(PWD)/bin:$(PATH)

libs: lib ubox spman mplayer ap

game: bin libs
	make -C tools
	make -C game
	
hello: bin libs	
	make -C hello
	
consolehello: bin libs	
	make -C consolehello
	
timer: bin libs	
	make -C timer

docs:
	make -C docs

ubox:
	make -C src/ubox

spman:
	make -C src/spman

mplayer:
	make -C src/mplayer

ap:
	make -C src/ap

bin/apultra: bin
	make -C tools ../bin/apultra

.PHONY: clean cleanall docs libs game hello consolehello
clean:
	make -C src/ubox clean
	make -C src/spman clean
	make -C src/mplayer clean
	make -C src/ap clean
	make -C game clean

lib:
	mkdir -p ./lib

bin:
	mkdir -p ./bin

cleanall: clean
	rm -rf ./bin ./lib
	make -C docs clean
