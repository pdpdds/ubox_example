OUTPUT := ../generated
GENERATED := $(OUTPUT)/tiles.h $(OUTPUT)/player.h $(OUTPUT)/enemy.h $(OUTPUT)/map.h

all: $(GENERATED)

$(OUTPUT)/tiles.h: tiles.png
	python ../../tools/png2tiles.py -i tiles $< > $@

$(OUTPUT)/player.h: player.png
	python ../../tools/png2sprites.py -i player_sprite $< > $@

$(OUTPUT)/enemy.h: enemy.png
	python ../../tools/png2sprites.py -i enemy_sprite $< > $@

$(OUTPUT)/map.h: map.json map_conf.json
	python ../../tools/map.py --aplib --max-ents 11 --room-height 21 map.json map > $@

.PHONY: all clean
clean:
	rm -f $(OUTPUT)/*

