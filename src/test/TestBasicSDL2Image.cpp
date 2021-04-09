#include <util/TestBase.h>
#include <stdio.h>
#include <string>

struct TestBasicSDL2Image : public TestBase {
public:
	bool init() override {
		bool success = true;
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			printf("SDL could not initialize! Error: %s\n", SDL_GetError());
			success = false;
		} else {
			window = SDL_CreateWindow(name().c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
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

	bool loadMedia() override {
		bool success = true;
		pngSurface = loadSurface("image/landscape.png");
		if (!pngSurface) {
			printf("Failed to load PNG image!\n");
			success = false;
		}
		return success;
	}

	void run() override {
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

	void close() override {
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

	std::string name() override {
		return "Test Basic SDL2 Image";
	}

private:
	SDL_Surface* screenSurface = nullptr;
	SDL_Surface* pngSurface = nullptr;
};


int main(int argc, char** argv) {
	TestBasicSDL2Image mainWindow;
	mainWindow.test();
	return 0;
}