OUT_DIR = ./bin
DEBUG_FLAG = DEBUG_MODE

play: game
	$(OUT_DIR)/game

game:out_dir
	gcc main.c screen.c -o $(OUT_DIR)/game

debug_game: out_dir
	gcc -D$(DEBUG_FLAG) main.c screen.c -o $(OUT_DIR)/debug_game

out_dir:
	mkdir -p $(OUT_DIR)

clean:
	rm -f $(OUT_DIR)/game
	rm -f $(OUT_DIR)/debug_game