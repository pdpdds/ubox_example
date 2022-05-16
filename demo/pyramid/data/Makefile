OUTPUT := ../generated
GENERATED := $(OUTPUT)/tiles.h $(OUTPUT)/player.h $(OUTPUT)/enemy.h $(OUTPUT)/foothold.h  $(OUTPUT)/map.h $(OUTPUT)/map_summary.h

all: $(GENERATED)

$(OUTPUT)/tiles.h: tiles.png
	python ../tools/png2tiles.py -i tiles $< > $@

$(OUTPUT)/player.h: player.png
	python ../tools/png2sprites.py -i player_sprite $< > $@

$(OUTPUT)/enemy.h: enemy.png
	python ../tools/png2sprites.py -i enemy_sprite $< > $@

$(OUTPUT)/foothold.h: foothold.png
	python ../tools/png2sprites.py -i foothold $< > $@

$(OUTPUT)/map.h:
	python ../tools/map_generate.py > $@

$(OUTPUT)/map_summary.h:
	python ../tools/map_summary.py map_summary > $@

.PHONY: all clean
clean:
	rm -f $(OUTPUT)/*

