#include <core/Window.h>
#include <core/Texture.h>
#include <core/Timer.h>
#include <stdio.h>
#include <sstream>
#include <iomanip>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const int SCREEN_FPS = 60;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

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
			window = SDL_CreateWindow("Test timing!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
			if (!window) {
				printf("Window could not be created! Error: %s\n", SDL_GetError());
				success = false;
			} else {
				renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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

		if (!landscapeTexture.loadFromFile(renderer, "image/landscape.png")) {
			printf("Failed to load \"landscape\" texture image!\n");
			success = false;
		}

		titleFont = TTF_OpenFont("font/crackman.ttf", 60);
		if (!titleFont) {
			printf("Failed to load \"gasalt\" font! Error: %s\n", TTF_GetError());
			success = false;
		}

		timeFont = TTF_OpenFont("font/pixel.ttf", 28);
		if (!timeFont) {
			printf("Failed to load \"pixel\" font! Error: %s\n", TTF_GetError());
			success = false;
		}
		
		return success;
	}

	void run() override {
		bool quit = false;
		SDL_Event e;
		Timer timer, fpsTimer, capTimer;
		std::stringstream timeText, fpsText;
		int nFrames = 0;
		fpsTimer.start();
		while (!quit) {
			capTimer.start();
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
			if (!timeTexture.loadFromRenderedText(renderer, timeFont, timeText.str(), SDL_Color{0x00, 0x00, 0x44})) {
				printf("Failed to render time texture!\n");
			}

			float avgFPS = nFrames / (fpsTimer.getTicks() / 1000.);
			if (avgFPS > 2000000) {
				avgFPS = 0;
			}

			fpsText.str("");
			fpsText << std::fixed << std::setprecision(2);
			fpsText << "FPS : " << std::setw(7) << avgFPS;
			if (!fpsTexture.loadFromRenderedText(renderer, timeFont, fpsText.str(), SDL_Color{0x00, 0x00, 0x44})) {
				printf("Failed to render fps texture!\n");
			}

			SDL_RenderClear(renderer);
			landscapeTexture.render(renderer, 0, 0);
			timeTexture.render(renderer, WINDOW_WIDTH - 200, 20);
			fpsTexture.render(renderer, WINDOW_WIDTH - 200, 45);
			SDL_RenderPresent(renderer);

			nFrames++;

			int frameTicks = capTimer.getTicks();
			if (frameTicks < SCREEN_TICKS_PER_FRAME) {
				SDL_Delay(SCREEN_TICKS_PER_FRAME - frameTicks);
			}
		}
	}

	void close() override {
		landscapeTexture.free();
		timeTexture.free();
		fpsTexture.free();
		TTF_CloseFont(titleFont);
		TTF_CloseFont(timeFont);
		titleFont = nullptr;
		timeFont = nullptr;
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
		SDL_DestroyWindow(window);
		window = nullptr;
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
	}

private:
	SDL_Renderer* renderer = nullptr;
	TTF_Font* titleFont = nullptr;
	TTF_Font* timeFont = nullptr;
	Texture landscapeTexture;
	Texture timeTexture;
	Texture fpsTexture;
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