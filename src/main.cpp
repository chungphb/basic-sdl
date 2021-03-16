#include <SDL.h>
#include <stdio.h>
#include <string>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

enum KeyPressSurfaces {
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

struct MainWindow {
public:
	bool init() {
		bool success = true;
		if (SDL_Init(SDL_INIT_VIDEO)) {
			printf("SDL could not initialize! Error: %s\n", SDL_GetError());
			success = false;
		} else {
			window = SDL_CreateWindow("Hello world!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
			if (!window) {
				printf("Window could not be created! Error: %s\n", SDL_GetError());
				success = false;
			} else {
				screenSurface = SDL_GetWindowSurface(window);
			}
		}
		return success;
	}

	bool loadMedia() {
		bool success = true;
		keyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = loadSurface("image/default.bmp");
		if (!keyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT]) {
			printf("Failed to load default image!\n");
			success = false;
		}
		keyPressSurfaces[KEY_PRESS_SURFACE_UP] = loadSurface("image/up.bmp");
		if (!keyPressSurfaces[KEY_PRESS_SURFACE_UP]) {
			printf("Failed to load up image!\n");
			success = false;
		}
		keyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = loadSurface("image/down.bmp");
		if (!keyPressSurfaces[KEY_PRESS_SURFACE_DOWN]) {
			printf("Failed to load up image!\n");
			success = false;
		}
		keyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = loadSurface("image/left.bmp");
		if (!keyPressSurfaces[KEY_PRESS_SURFACE_LEFT]) {
			printf("Failed to load left image!\n");
			success = false;
		}
		keyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = loadSurface("image/right.bmp");
		if (!keyPressSurfaces[KEY_PRESS_SURFACE_RIGHT]) {
			printf("Failed to load right image!\n");
			success = false;
		}
		return success;
	}

	void run() {
		bool quit = false;
		currentSurface = keyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
		SDL_Event e;
		while (!quit) {
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				} else if (e.type == SDL_KEYDOWN) {
					switch (e.key.keysym.sym) {
						case SDLK_UP: {
							currentSurface = keyPressSurfaces[KEY_PRESS_SURFACE_UP];
							break;
						}
						case SDLK_DOWN: {
							currentSurface = keyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
							break;
						}
						case SDLK_LEFT: {
							currentSurface = keyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
							break;
						}
						case SDLK_RIGHT: {
							currentSurface = keyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
							break;
						}
						default: {
							currentSurface = keyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
							break;
						}
					}
				}
			}
			SDL_Rect stretchRect;
			stretchRect.x = WINDOW_WIDTH >> 2;
			stretchRect.y = WINDOW_HEIGHT >> 2;
			stretchRect.w = WINDOW_WIDTH >> 1;
			stretchRect.h = WINDOW_HEIGHT >> 1;
			SDL_BlitScaled(currentSurface, nullptr, screenSurface, &stretchRect);
			SDL_UpdateWindowSurface(window);
		}
	}

	void close() {
		for (int i = 0; i < KEY_PRESS_SURFACE_TOTAL; i++) {
			SDL_FreeSurface(keyPressSurfaces[i]);
			keyPressSurfaces[i] = nullptr;
		}
		SDL_DestroyWindow(window);
		window = nullptr;
		screenSurface = nullptr;
		SDL_Quit();
	}

	SDL_Surface* loadSurface(std::string path) {
		SDL_Surface* optimizedSurface = nullptr;
		SDL_Surface* loadedSurface = SDL_LoadBMP(path.c_str());
		if (!loadedSurface) {
			printf("Unable to load image %s! Error: %s\n", path.c_str(), SDL_GetError());
		} else {
			optimizedSurface = SDL_ConvertSurface(loadedSurface, screenSurface->format, 0);
			if (!optimizedSurface) {
				printf("Unable to optimize image %s! Error: %s\n", path.c_str(), SDL_GetError());
			}
			SDL_FreeSurface(loadedSurface);
		}
		return optimizedSurface;
	}

private:
	SDL_Window* window = nullptr;
	SDL_Surface* screenSurface = nullptr;
	SDL_Surface* keyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];
	SDL_Surface* currentSurface = nullptr;
};


int main(int argc, char** argv) {
	MainWindow mainWindow;
	if (!mainWindow.init()) {
		printf("Failed to initialize!\n");
	} else {
		if (!mainWindow.loadMedia()) {
			printf("Failed to load media!\n");
		} else {
			mainWindow.run();
		}
	}
	mainWindow.close();
	return 0;
}