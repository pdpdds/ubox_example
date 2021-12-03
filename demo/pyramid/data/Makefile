OUTPUT := ../generated
GENERATED := $(OUTPUT)/tiles.h $(OUTPUT)/player.h $(OUTPUT)/enemy.h $(OUTPUT)/foothold.h  $(OUTPUT)/map.h $(OUTPUT)/map_summary.h

all: $(GENERATED)

$(OUTPUT)/tiles.h: tiles.png
	python3.9 ../tools/png2tiles.py -i tiles $< > $@

$(OUTPUT)/player.h: player.png
	python3.9 ../tools/png2sprites.py -i player_sprite $< > $@

$(OUTPUT)/enemy.h: enemy.png
	python3.9 ../tools/png2sprites.py -i enemy_sprite $< > $@

$(OUTPUT)/foothold.h: foothold.png
	python3.9 ../tools/png2sprites.py -i foothold $< > $@

$(OUTPUT)/map.h:
	python3.9 ../tools/map_generate.py > $@

$(OUTPUT)/map_summary.h:
	python3.9 ../tools/map_summary.py map_summary > $@

.PHONY: all clean
clean:
	rm -f $(OUTPUT)/*

