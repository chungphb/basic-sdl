#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

struct MainWindow {
public:
	bool init() {
		bool success = true;
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			printf("SDL could not initialize! Error: %s\n", SDL_GetError());
			success = false;
		} else {
			window = SDL_CreateWindow("Hello world!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
			if (!window) {
				printf("Window could not be created! Error: %s\n", SDL_GetError());
				success = false;
			} else {
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags)) {
					printf("SDL2_image could not initialize! Error: %s\n", IMG_GetError());
					success = false;
				} else {
					screenSurface = SDL_GetWindowSurface(window);
				}
			}
		}
		return success;
	}

	bool loadMedia() {
		bool success = true;
		pngSurface = loadSurface("image/1.PNG");
		if (!pngSurface) {
			printf("Failed to load PNG image!\n");
			success = false;
		}
		return success;
	}

	void run() {
		bool quit = false;
		SDL_Event e;
		while (!quit) {
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				}
			}
			SDL_BlitSurface(pngSurface, nullptr, screenSurface, nullptr);
			SDL_UpdateWindowSurface(window);
		}
	}

	void close() {
		SDL_FreeSurface(pngSurface);
		pngSurface = nullptr;
		SDL_DestroyWindow(window);
		window = nullptr;
		screenSurface = nullptr;
		IMG_Quit();
		SDL_Quit();
	}

	SDL_Surface* loadSurface(std::string path) {
		SDL_Surface* optimizedSurface = nullptr;
		SDL_Surface* loadedSurface = IMG_Load(path.c_str());
		if (!loadedSurface) {
			printf("Unable to load image %s! Error: %s\n", path.c_str(), IMG_GetError());
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
	SDL_Surface* pngSurface = nullptr;
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