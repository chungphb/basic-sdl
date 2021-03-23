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
				}
			}
		}
		return success;
	}

	bool loadMedia() {
		bool success = true;
		texture = loadTexture("image/landscape.png");
		if (!texture) {
			printf("Failed to load texture image!\n");
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

			// IMAGE RENDERING

			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, texture, nullptr, nullptr);

			// GEOMETRY RENDERING

			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(renderer);

			SDL_Rect rect;
			rect.x = WINDOW_WIDTH / 4;
			rect.y = WINDOW_HEIGHT / 4;
			rect.w = WINDOW_WIDTH / 2;
			rect.h = WINDOW_HEIGHT / 2;
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
			SDL_RenderFillRect(renderer, &rect);

			SDL_Rect otherRect;
			otherRect.x = WINDOW_WIDTH / 6;
			otherRect.y = WINDOW_HEIGHT / 6;
			otherRect.w = 2 * WINDOW_WIDTH / 3;
			otherRect.h = 2 * WINDOW_HEIGHT / 3;
			SDL_SetRenderDrawColor(renderer, 0xFF, 0x69, 0xB4, 0xFF);
			SDL_RenderDrawRect(renderer, &otherRect);

			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderDrawLine(renderer, WINDOW_WIDTH / 3, WINDOW_HEIGHT / 2, 2 * WINDOW_WIDTH / 3, WINDOW_HEIGHT / 2);
			for (int i = WINDOW_HEIGHT / 3; i <= 2 * WINDOW_HEIGHT / 3; i += WINDOW_HEIGHT / 120) {
				SDL_RenderDrawPoint(renderer, WINDOW_WIDTH / 2, i);
			}

			// VIEWPORT

			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(renderer);

			SDL_Rect topLeftViewPort;
			topLeftViewPort.x = 0;
			topLeftViewPort.y = 0;
			topLeftViewPort.w = WINDOW_WIDTH / 2;
			topLeftViewPort.h = WINDOW_HEIGHT / 2;
			SDL_RenderSetViewport(renderer, &topLeftViewPort);
			SDL_RenderCopy(renderer, texture, nullptr, nullptr);

			SDL_Rect topRightViewPort;
			topRightViewPort.x = WINDOW_WIDTH / 2;
			topRightViewPort.y = 0;
			topRightViewPort.w = WINDOW_WIDTH / 2;
			topRightViewPort.h = WINDOW_HEIGHT / 2;
			SDL_RenderSetViewport(renderer, &topRightViewPort);
			SDL_RenderCopy(renderer, texture, nullptr, nullptr);

			SDL_Rect bottomViewPort;
			bottomViewPort.x = 0;
			bottomViewPort.y = WINDOW_HEIGHT / 2;
			bottomViewPort.w = WINDOW_WIDTH;
			bottomViewPort.h = WINDOW_HEIGHT / 2;
			SDL_RenderSetViewport(renderer, &bottomViewPort);
			SDL_RenderCopy(renderer, texture, nullptr, nullptr);

			SDL_RenderPresent(renderer);
		}
	}

	void close() {
		SDL_DestroyTexture(texture);
		texture = nullptr;
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
		SDL_DestroyWindow(window);
		window = nullptr;
		IMG_Quit();
		SDL_Quit();
	}

	SDL_Texture* loadTexture(std::string path) {
		SDL_Texture* newTexture = nullptr;
		SDL_Surface* loadedSurface = IMG_Load(path.c_str());
		if (!loadedSurface) {
			printf("Unable to load image %s! Error: %s\n", path.c_str(), IMG_GetError());
		} else {
			newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
			if (!newTexture) {
				printf("Unable to create texture from %s! Error: %s\n", path.c_str(), SDL_GetError());
			}
			SDL_FreeSurface(loadedSurface);
		}
		return newTexture;
	}

private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* texture = nullptr;
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