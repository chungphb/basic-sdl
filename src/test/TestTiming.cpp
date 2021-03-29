#include <core/Window.h>
#include <core/Texture.h>
#include <core/Timer.h>
#include <stdio.h>
#include <sstream>
#include <iomanip>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

struct TestTiming : public Window {
public:
	bool init() override {
		bool success = true;
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			printf("SDL could not initialize! Error: %s\n", SDL_GetError());
			success = false;
		} else {
			if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
				printf("Warning: Linear texture filtering is not enabled!");
			}
			window = SDL_CreateWindow("Hello world!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
			if (!window) {
				printf("Window could not be created! Error: %s\n", SDL_GetError());
				success = false;
			} else {
				renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
				if (!renderer) {
					printf("Renderer could not be created! Error: %s\n", SDL_GetError());
					success = false;
				} else {
					SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
					int imgFlags = IMG_INIT_PNG;
					if (!(IMG_Init(imgFlags) & imgFlags)) {
						printf("SDL2_image could not initialize! Error: %s\n", IMG_GetError());
						success = false;
					}
					if (TTF_Init() == -1) {
						printf("SDL_ttf could not initialize! Error: %s\n", TTF_GetError());
						success = false;
					}
				}
			}
		}
		return success;
	}

	bool loadMedia() override {
		bool success = true;

		if (!characterTexture.loadFromFile(renderer, "image/character.png")) {
			printf("Failed to load \"character\" texture image!\n");
			success = false;
		}

		if (!backgroundTexture.loadFromFile(renderer, "image/background.png")) {
			printf("Failed to load \"background\" texture image!\n");
			success = false;
		}

		titleFont = TTF_OpenFont("font/crackman.ttf", 60);
		if (!titleFont) {
			printf("Failed to load \"gasalt\" font! Error: %s\n", TTF_GetError());
			success = false;
		} else {
			SDL_Color textColor{0xEF, 0x81, 0x96};
			if (!nameTexture.loadFromRenderedText(renderer, titleFont, "Kitty!", textColor)) {
				printf("Failed to render text texture!\n");
				success = false;
			}
		}

		timeFont = TTF_OpenFont("font/pixeboy.ttf", 32);
		if (!timeFont) {
			printf("Failed to load \"pixeboy\" font! Error: %s\n", TTF_GetError());
			success = false;
		}
		
		return success;
	}

	void run() override {
		bool quit = false;
		SDL_Event e;
		Timer timer;
		std::stringstream timeText;
		while (!quit) {
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				} else if (e.type == SDL_KEYDOWN) {
					switch (e.key.keysym.sym) {
						case SDLK_s: {
							if (timer.isStarted()) {
								timer.stop();
							} else {
								timer.start();
							}
							break;
						}
						case SDLK_p: {
							if (timer.isPaused()) {
								timer.unpause();
							} else {
								timer.pause();
							}
						}
					}
				}
			}

			timeText.str("");
			timeText << std::fixed << std::setprecision(2);
			timeText << "Time: " << std::setw(7) << timer.getTicks() / 1000.;
			if (!timeTexture.loadFromRenderedText(renderer, timeFont, timeText.str(), SDL_Color{0xEF, 0x81, 0x96})) {
				printf("Failed to render time texture!\n");
			}

			SDL_RenderClear(renderer);
			backgroundTexture.render(renderer, 0, 0);
			nameTexture.render(renderer, 320, 180);
			timeTexture.render(renderer, WINDOW_WIDTH - 180, 20);
			characterTexture.render(renderer, WINDOW_WIDTH / 2 + 40, WINDOW_HEIGHT / 2 + 40);
			SDL_RenderPresent(renderer);
		}
	}

	void close() override {
		characterTexture.free();
		backgroundTexture.free();
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
		SDL_DestroyWindow(window);
		window = nullptr;
		IMG_Quit();
		SDL_Quit();
	}

private:
	SDL_Renderer* renderer = nullptr;
	TTF_Font* titleFont = nullptr;
	TTF_Font* timeFont = nullptr;
	Texture characterTexture;
	Texture backgroundTexture;
	Texture nameTexture;
	Texture timeTexture;
};


int main(int argc, char** argv) {
	TestTiming mainWindow;
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