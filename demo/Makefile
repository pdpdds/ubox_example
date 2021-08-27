all: 11_demo

export PATH := $(PWD)/../bin:$(PATH)
	
11_demo: bin	
	make -C 11_demo

.PHONY: clean cleanall 11_demo
clean:
	make -C 11_demo clean

bin:
	mkdir -p ./bin

cleanall: clean
	rm -rf ./bin