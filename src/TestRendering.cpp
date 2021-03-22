#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
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
					if (TTF_Init() == -1) {
						printf("SDL_ttf could not initialize! Error: %s\n", TTF_GetError());
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

			spriteSheetTexture.setBlendMode(SDL_BLENDMODE_BLEND);
		}
		if (!fooTexture.loadFromFile(renderer, "image/foo2.png")) {
			printf("Failed to load \"foo2\" texture image!\n");
			success = false;
		}
		if (!backgroundTexture.loadFromFile(renderer, "image/background.png")) {
			printf("Failed to load \"background\" texture image!\n");
			success = false;
		}
		
		font = TTF_OpenFont("font/crackman.ttf", 60);
		if (!font) {
			printf("Failed to load \"gasalt\" font! Error: %s\n", TTF_GetError());
			success = false;
		} else {
			SDL_Color textColor{0xE3, 0xA7, 0xC0};
			if (!textTexture.loadFromRenderedText(renderer, font, "Kitty!", textColor)) {
				printf("Failed to render text texture!\n");
				success = false;
			}
		}

		return success;
	}

	void run() {
		bool quit = false;
		SDL_Event e;
		Uint8 r = 255, g = 255, b = 255;
		Uint8 a = 255;
		double angle = 0;
		SDL_RendererFlip flip = SDL_FLIP_NONE;
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
						case SDLK_a: {
							if (a + 32 > 255) {
								a = 255;
							} else {
								a += 32;
							}
							break;
						}
						case SDLK_s: {
							if (a - 32 < 0) {
								a = 0;
							}
							else {
								a -= 32;
							}
							break;
						}
						case SDLK_1: {
							angle += 60;
							break;
						}
						case SDLK_2: {
							angle -= 60;
							break;
						}
						case SDLK_3: {
							flip = SDL_FLIP_NONE;
							break;
						}
						case SDLK_4: {
							flip = SDL_FLIP_HORIZONTAL;
							break;
						}
						case SDLK_5: {
							flip = SDL_FLIP_VERTICAL;
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
			backgroundTexture.setColor(r, g, b);
			backgroundTexture.render(renderer, 0, 0);
			textTexture.render(renderer, 320, 180);
			fooTexture.setColor(r, g, b);
			fooTexture.render(renderer, WINDOW_WIDTH - 100, 10, nullptr, angle, nullptr, flip);
			spriteSheetTexture.setColor(r, g, b);
			spriteSheetTexture.setAlpha(a);
			spriteSheetTexture.render(renderer, 100, 280, &clips[0]);
			spriteSheetTexture.render(renderer, 220, 280, &clips[1]);
			spriteSheetTexture.render(renderer, 340, 278, &clips[2]);
			spriteSheetTexture.render(renderer, 460, 278, &clips[3]);
			SDL_RenderPresent(renderer);
		}
	}

	void close() {
		spriteSheetTexture.free();
		fooTexture.free();
		textTexture.free();
		backgroundTexture.free();
		TTF_CloseFont(font);
		font = nullptr;
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
		SDL_DestroyWindow(window);
		window = nullptr;
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
	}

private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	TTF_Font* font = nullptr;
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

		bool loadFromRenderedText(SDL_Renderer* renderer, TTF_Font* font, std::string textureText, SDL_Color textColor) {
			free();
			SDL_Texture* newTexture = nullptr;
			SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
			if (!textSurface) {
				printf("Unable to render text surface! Error: %s\n", TTF_GetError());
			} else {
				newTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
				if (!newTexture) {
					printf("Unable to create texture from rendered text! Error: %s\n", SDL_GetError());
				} else {
					width = textSurface->w;
					height = textSurface->h;
				}
				SDL_FreeSurface(textSurface);
			}
			texture = newTexture;
			return texture != nullptr;
		}

		void setColor(Uint8 r, Uint8 g, Uint8 b) {
			SDL_SetTextureColorMod(texture, r, g, b);
		}

		void setBlendMode(SDL_BlendMode blendMode) {
			SDL_SetTextureBlendMode(texture, blendMode);
		}

		void setAlpha(Uint8 alpha) {
			SDL_SetTextureAlphaMod(texture, alpha);
		}

		void free() {
			if (texture) {
				SDL_DestroyTexture(texture);
				texture = nullptr;
				width = 0;
				height = 0;
			}
		}

		void render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip = nullptr, double angle = 0.0, SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE) {
			SDL_Rect renderQuad{x, y, width, height};
			if (clip) {
				renderQuad.w = clip->w;
				renderQuad.h = clip->h;
			}
			SDL_RenderCopyEx(renderer, texture, clip, &renderQuad, angle, center, flip);
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
	Texture fooTexture;
	Texture textTexture;
	Texture backgroundTexture;
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