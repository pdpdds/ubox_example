OUTPUT := ../generated
GENERATED := $(OUTPUT)/tiles.h $(OUTPUT)/player.h $(OUTPUT)/enemy.h $(OUTPUT)/map.h $(OUTPUT)/map2.h  $(OUTPUT)/map3.h  $(OUTPUT)/map4.h $(OUTPUT)/map5.h

all: $(GENERATED)

$(OUTPUT)/tiles.h: tiles.png
	python3.9 ../../../tools/png2tiles.py -i tiles $< > $@

$(OUTPUT)/player.h: player.png
	python3.9 ../../../tools/png2sprites.py -i player_sprite $< > $@

$(OUTPUT)/enemy.h: enemy.png
	python3.9 ../../../tools/png2sprites.py -i enemy_sprite $< > $@

$(OUTPUT)/map.h: map.json map_conf.json
	python3.9 ../tools/map.py --aplib --room-height 21 map.json map > $@

$(OUTPUT)/map2.h: map2.json map_conf.json
	python3.9 ../tools/map.py --aplib --room-height 21 map2.json map2 > $@

$(OUTPUT)/map3.h: map3.json map_conf.json
	python3.9 ../tools/map.py --aplib --room-height 21 map3.json map3 > $@
	
$(OUTPUT)/map4.h: map4.json map_conf.json
	python3.9 ../tools/map.py --aplib --room-width 32 --room-height 21 map4.json map4 > $@	

$(OUTPUT)/map5.h: map5.json map_conf.json
	python3.9 ../tools/map.py --aplib --room-width 32 --room-height 21 map5.json map5 > $@	

.PHONY: all clean
clean:
	rm -f $(OUTPUT)/*

