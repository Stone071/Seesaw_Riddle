OUT_DIR = ./bin
DEBUG_FLAG = DEBUG_MODE
COMP_FLAGS = -Wall -Wextra

play: game
	$(OUT_DIR)/game

game:out_dir
	gcc $(COMP_FLAGS) main.c screen.c -o $(OUT_DIR)/game

debug_game: out_dir
	gcc $(COMP_FLAGS) -D$(DEBUG_FLAG) main.c screen.c -o $(OUT_DIR)/debug_game

out_dir:
	mkdir -p $(OUT_DIR)

shared:out_dir
# No need for any screen logic
	gcc $(COMP_FLAGS) -shared -fPIC  -DSHARED_LIB main.c -o $(OUT_DIR)/gamelib.so

clean:
	rm -f $(OUT_DIR)/game
	rm -f $(OUT_DIR)/debug_game
	rm -f $(OUT_DIR)/gamelib.so
	