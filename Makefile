all:
	emcc main.cpp -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' --emrun --preload-file resources -o hello.html
