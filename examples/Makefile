all: 01_hello 02_consolehello 03_timer

export PATH := $(PWD)/../bin:$(PATH)
	
01_hello: bin	
	make -C 01_hello
	
02_consolehello: bin
	make -C 02_consolehello
	
03_timer: bin	
	make -C 03_timer

04_clibrary: bin	
	make -C 04_clibrary

.PHONY: clean cleanall 01_hello 02_consolehello 03_timer
clean:
	make -C 01_hello clean
	make -C 02_consolehello clean
	make -C 03_timer clean
	make -C 04_clibrary

bin:
	mkdir -p ./bin

cleanall: clean
	rm -rf ./bin