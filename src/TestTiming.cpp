#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <sstream>

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

	void run() {
		bool quit = false;
		SDL_Event e;
		Uint32 startTime = 0;
		std::stringstream timeText;
		while (!quit) {
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				} else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN) {
					startTime = SDL_GetTicks();
				}
			}

			timeText.str("");
			timeText << "Time: " << SDL_GetTicks() - startTime << "ms";
			if (!timeTexture.loadFromRenderedText(renderer, timeFont, timeText.str(), SDL_Color{0xEF, 0x81, 0x96})) {
				printf("Failed to render time texture!\n");
			}

			SDL_RenderClear(renderer);
			backgroundTexture.render(renderer, 0, 0);
			nameTexture.render(renderer, 320, 180);
			timeTexture.render(renderer, WINDOW_WIDTH - 200, 20);
			characterTexture.render(renderer, WINDOW_WIDTH / 2 + 40, WINDOW_HEIGHT / 2 + 40);
			SDL_RenderPresent(renderer);
		}
	}

	void close() {
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
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	TTF_Font* titleFont = nullptr;
	TTF_Font* timeFont = nullptr;

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

	Texture characterTexture;
	Texture backgroundTexture;
	Texture nameTexture;
	Texture timeTexture;
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