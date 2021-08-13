OUTPUT := ../generated
GENERATED := $(OUTPUT)/tiles.h $(OUTPUT)/player.h $(OUTPUT)/enemy.h $(OUTPUT)/map.h $(OUTPUT)/map2.h

all: $(GENERATED)

$(OUTPUT)/tiles.h: tiles.png
	python3.7 ../../../tools/png2tiles.py -i tiles $< > $@

$(OUTPUT)/player.h: player.png
	python3.7 ../../../tools/png2sprites.py -i player_sprite $< > $@

$(OUTPUT)/enemy.h: enemy.png
	python3.7 ../../../tools/png2sprites.py -i enemy_sprite $< > $@

$(OUTPUT)/map.h: map.json map_conf.json
	python3.7 ../tools/map.py --aplib --room-height 21 map.json map > $@
	
$(OUTPUT)/map2.h: map2.json map2_conf.json
	python3.7 ../tools/map.py --aplib --room-width 32 --room-height 21 map2.json map2 > $@	

.PHONY: all clean
clean:
	rm -f $(OUTPUT)/*

