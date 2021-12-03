all: pyramid cross scroll

export PATH := $(PWD)/../bin:$(PATH)
	
pyramid: bin	
	make -C pyramid
	
cross: cross	
	make -C cross
	
scroll: scroll	
	make -C scroll		

.PHONY: clean cleanall pyramid cross scroll
clean:
	make -C pyramid clean
	make -C cross clean
	make -C scroll clean

bin:
	mkdir -p ./bin

cleanall: clean
	rm -rf ./bin