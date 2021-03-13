#include <SDL.h>
#include <stdio.h>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

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
		imageSurface = SDL_LoadBMP("image/1.bmp");
		if (!imageSurface) {
			printf("Unable to load image %s! Error: %s\n", "image/1.bmp", SDL_GetError());
			success = false;
		}
		return success;
	}

	void applyMedia() {
		SDL_BlitSurface(imageSurface, nullptr, screenSurface, nullptr);
		SDL_UpdateWindowSurface(window);
		SDL_Delay(2000);
	}

	void close() {
		SDL_FreeSurface(imageSurface);
		imageSurface = nullptr;
		SDL_DestroyWindow(window);
		window = nullptr;
		screenSurface = nullptr;
		SDL_Quit();
	}

private:
	SDL_Window* window = nullptr;
	SDL_Surface* screenSurface = nullptr;
	SDL_Surface* imageSurface = nullptr;
};


int main(int argc, char** argv) {
	MainWindow mainWindow;
	if (!mainWindow.init()) {
		printf("Failed to initialize!\n");
	} else {
		if (!mainWindow.loadMedia()) {
			printf("Failed to load media!\n");
		} else {
			mainWindow.applyMedia();
		}
	}
	mainWindow.close();
	return 0;
}