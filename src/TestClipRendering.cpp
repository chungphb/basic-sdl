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
		if (!spriteSheetTexture.loadFromFile(renderer, "image/foos.png")) {
			printf("Failed to load \"foos\" texture image!\n");
			success = false;
		} else {
			clips[0].x = 0;
			clips[0].y = 0;
			clips[0].w = 100;
			clips[0].h = 150;

			clips[1].x = 100;
			clips[1].y = 0;
			clips[1].w = 100;
			clips[1].h = 150;

			clips[2].x = 0;
			clips[2].y = 150;
			clips[2].w = 100;
			clips[2].h = 150;

			clips[3].x = 100;
			clips[3].y = 150;
			clips[3].w = 100;
			clips[3].h = 150;
		}
		return success;
	}

	void run() {
		bool quit = false;
		SDL_Event e;
		Uint8 r = 255, g = 255, b = 255;
		while (!quit) {
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				} else if (e.type == SDL_KEYDOWN) {
					switch (e.key.keysym.sym) {
						case SDLK_q: {
							r += 15;
							break;
						}
						case SDLK_w: {
							g += 15;
							break;
						}
						case SDLK_e: {
							b += 15;
							break;
						}
						case SDLK_r: {
							r -= 15;
							break;
						}
						case SDLK_t: {
							g -= 15;
							break;
						}
						case SDLK_y: {
							b -= 15;
							break;
						}
						default: {
							break;
						}
					}
				}
			}
			SDL_SetRenderDrawColor(renderer, 0x59, 0x59, 0x59, 0xFF);
			SDL_RenderClear(renderer);
			spriteSheetTexture.setColor(r, g, b);
			spriteSheetTexture.render(renderer, 80, 200, &clips[0]);
			spriteSheetTexture.render(renderer, 200, 200, &clips[1]);
			spriteSheetTexture.render(renderer, 320, 200, &clips[2]);
			spriteSheetTexture.render(renderer, 440, 200, &clips[3]);
			SDL_RenderPresent(renderer);
		}
	}

	void close() {
		spriteSheetTexture.free();
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
		SDL_DestroyWindow(window);
		window = nullptr;
		IMG_Quit();
		SDL_Quit();
	}

private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	struct Texture {
	public:
		Texture() {
			texture = nullptr;
			width = 0;
			height = 0;
		}

		~Texture() {
			free();
		}

		bool loadFromFile(SDL_Renderer* renderer, std::string path) {
			free();
			SDL_Texture* newTexture = nullptr;
			SDL_Surface* loadedSurface = IMG_Load(path.c_str());
			if (!loadedSurface) {
				printf("Unable to load image %s! Error: %s\n", path.c_str(), IMG_GetError());
			} else {
				SDL_SetColorKey(loadedSurface, true, SDL_MapRGB(loadedSurface->format, 0x00, 0xFF, 0xFF));
				newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
				if (!newTexture) {
					printf("Unable to create texture from %s! Error: %s\n", path.c_str(), SDL_GetError());
				} else {
					width = loadedSurface->w;
					height = loadedSurface->h;
				}
				SDL_FreeSurface(loadedSurface);
			}
			texture = newTexture;
			return texture != nullptr;
		}

		void setColor(Uint8 r, Uint8 g, Uint8 b) {
			SDL_SetTextureColorMod(texture, r, g, b);
		}

		void free() {
			if (texture) {
				SDL_DestroyTexture(texture);
				texture = nullptr;
				width = 0;
				height = 0;
			}
		}

		void render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip = nullptr) {
			SDL_Rect renderQuad{x, y, width, height};
			if (clip) {
				renderQuad.w = clip->w;
				renderQuad.h = clip->h;
			}
			SDL_RenderCopy(renderer, texture, clip, &renderQuad);
		}

		int getWidth() {
			return width;
		}

		int getHeight() {
			return height;
		}

	private:
		SDL_Texture* texture;
		int width;
		int height;
	};
	SDL_Rect clips[4];
	Texture spriteSheetTexture;
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