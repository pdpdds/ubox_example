all: 01_hello 02_consolehello 03_timer 04_clibrary 05_sound 06_effect 07_snakebyte 08_breakout 09_socoban 10_tetris 11_demo

export PATH := $(PWD)/../bin:$(PATH)
	
01_hello: bin	
	make -C 01_hello
	
02_consolehello: bin
	make -C 02_consolehello
	
03_timer: bin	
	make -C 03_timer

04_clibrary: bin	
	make -C 04_clibrary

05_sound: bin	
	make -C 05_sound

06_effect: bin	
	make -C 06_effect

07_snakebyte: bin	
	make -C 07_snakebyte	

08_breakout: bin	
	make -C 08_breakout	


09_socoban: bin	
	make -C 09_socoban

10_tetris: bin	
	make -C 10_tetris

11_demo: bin	
	make -C 11_demo

.PHONY: clean cleanall 01_hello 02_consolehello 03_timer 04_clibrary 05_sound 06_effect 07_snakebyte 08_breakout 09_socoban 10_tetris 11_demo
clean:
	make -C 01_hello clean
	make -C 02_consolehello clean
	make -C 03_timer clean
	make -C 04_clibrary clean
	make -C 05_sound clean
	make -C 06_effect clean
	make -C 07_snakebyte clean
	make -C 08_breakout clean
	make -C 09_socoban clean
	make -C 10_tetris clean
	make -C 11_demo clean

bin:
	mkdir -p ./bin

cleanall: clean
	rm -rf ./bin