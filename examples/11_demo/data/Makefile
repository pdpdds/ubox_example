OUTPUT := ../generated
GENERATED := $(OUTPUT)/tiles.h $(OUTPUT)/player.h $(OUTPUT)/enemy.h $(OUTPUT)/foothold.h $(OUTPUT)/map1.h $(OUTPUT)/map2.h  $(OUTPUT)/map3.h  $(OUTPUT)/map4.h $(OUTPUT)/map5.h  $(OUTPUT)/map6.h $(OUTPUT)/map7.h \
		$(OUTPUT)/map8.h $(OUTPUT)/map9.h $(OUTPUT)/map_summary.h

all: $(GENERATED)

$(OUTPUT)/tiles.h: tiles.png
	python3.9 ../../../tools/png2tiles.py -i tiles $< > $@

$(OUTPUT)/player.h: player.png
	python3.9 ../../../tools/png2sprites.py -i player_sprite $< > $@

$(OUTPUT)/enemy.h: enemy.png
	python3.9 ../../../tools/png2sprites.py -i enemy_sprite $< > $@

$(OUTPUT)/foothold.h: foothold.png
	python3.9 ../../../tools/png2sprites.py -i foothold $< > $@

$(OUTPUT)/map1.h: map.json map_conf.json
	python3.9 ../tools/map.py --aplib --room-height 21 map.json map1 > $@

$(OUTPUT)/map2.h: map2.json map_conf.json
	python3.9 ../tools/map.py --aplib --room-height 21 map2.json map2 > $@

$(OUTPUT)/map3.h: map3.json map_conf.json
	python3.9 ../tools/map.py --aplib --room-height 21 map3.json map3 > $@
	
$(OUTPUT)/map4.h: map4.json map_conf.json
	python3.9 ../tools/map.py --aplib --room-width 32 --room-height 21 map4.json map4 > $@	

$(OUTPUT)/map5.h: map5.json map_conf.json
	python3.9 ../tools/map.py --aplib --room-width 32 --room-height 21 map5.json map5 > $@	
	
$(OUTPUT)/map6.h: map6.json map_conf.json
	python3.9 ../tools/map.py --aplib --room-width 32 --room-height 21 map6.json map6 > $@	
	
$(OUTPUT)/map7.h: map7.json map_conf.json
	python3.9 ../tools/map.py --aplib --room-width 32 --room-height 21 map7.json map7 > $@	

$(OUTPUT)/map8.h: map8.json map_conf.json
	python3.9 ../tools/map.py --aplib --room-width 32 --room-height 21 map8.json map8 > $@

$(OUTPUT)/map9.h: map9.json map_conf.json
	python3.9 ../tools/map.py --aplib --room-width 32 --room-height 21 map9.json map9 > $@

$(OUTPUT)/map_summary.h:
	python3.9 ../tools/map_summary.py map_summary > $@

.PHONY: all clean
clean:
	rm -f $(OUTPUT)/*

