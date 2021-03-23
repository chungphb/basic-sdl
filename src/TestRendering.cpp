#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

const int BUTTON_WIDTH = 40;
const int BUTTON_HEIGHT = 20;

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

		if (!characterSpriteSheetTexture.loadFromFile(renderer, "image/characters.png")) {
			printf("Failed to load \"characters\" texture image!\n");
			success = false;
		} else {
			const int WIDTH = 100;
			const int HEIGHT = 150;
			for (int i = 0; i < NUM_CHARACTERS; i++) {
				characterClips[i].x = (i % 2) * WIDTH;
				characterClips[i].y = (i / 2) * HEIGHT;
				characterClips[i].w = WIDTH;
				characterClips[i].h = HEIGHT;
			}
			characterSpriteSheetTexture.setBlendMode(SDL_BLENDMODE_BLEND);
		}

		if (!sunTexture.loadFromFile(renderer, "image/sun.png")) {
			printf("Failed to load \"sun\" texture image!\n");
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
			if (!nameTexture.loadFromRenderedText(renderer, font, "Kitty!", textColor)) {
				printf("Failed to render text texture!\n");
				success = false;
			}
		}

		if (!buttonSpriteSheetTexture.loadFromFile(renderer, "image/buttons.png")) {
			printf("Failed to load \"buttons\" texture image!\n");
			success = false;
		}
		else {
			const int WIDTH = 40;
			const int HEIGHT = 20;
			for (int i = 0; i < NUM_CHARACTERS; i++) {
				buttonClips[i].x = i * WIDTH;
				buttonClips[i].y = 0;
				buttonClips[i].w = WIDTH;
				buttonClips[i].h = HEIGHT;
			}
			buttons[0].setPosition(130, 440);
			buttons[1].setPosition(250, 440);
			buttons[2].setPosition(370, 440);
			buttons[3].setPosition(490, 440);
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
				} else {
					for (int i = 0; i < NUM_BUTTONS; i++) {
						buttons[i].handleEvent(&e);
					}
				}
			}
			SDL_SetRenderDrawColor(renderer, 0x59, 0x59, 0x59, 0xFF);
			SDL_RenderClear(renderer);
			backgroundTexture.setColor(r, g, b);
			backgroundTexture.render(renderer, 0, 0);
			nameTexture.render(renderer, 320, 180);
			sunTexture.setColor(r, g, b);
			sunTexture.render(renderer, WINDOW_WIDTH - 100, 10, nullptr, angle, nullptr, flip);
			characterSpriteSheetTexture.setColor(r, g, b);
			characterSpriteSheetTexture.setAlpha(a);
			characterSpriteSheetTexture.render(renderer, 100, 280, &characterClips[0]);
			characterSpriteSheetTexture.render(renderer, 220, 280, &characterClips[1]);
			characterSpriteSheetTexture.render(renderer, 340, 278, &characterClips[2]);
			characterSpriteSheetTexture.render(renderer, 460, 278, &characterClips[3]);
			for (int i = 0; i < NUM_BUTTONS; i++) {
				buttons[i].render(renderer, &buttonSpriteSheetTexture, buttonClips);
			}
			SDL_RenderPresent(renderer);
		}
	}

	void close() {
		characterSpriteSheetTexture.free();
		sunTexture.free();
		nameTexture.free();
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
	static constexpr int NUM_CHARACTERS = 4;
	static constexpr int NUM_BUTTONS = 4;

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

	SDL_Rect characterClips[NUM_CHARACTERS];
	Texture characterSpriteSheetTexture;
	Texture sunTexture;
	Texture nameTexture;
	Texture backgroundTexture;

	enum ButtonSprite {
		BUTTON_SPRITE_MOUSE_OUT = 0,
		BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
		BUTTON_SPRITE_MOUSE_DOWN = 2,
		BUTTON_SPRITE_MOUSE_UP = 3,
		BUTTON_SPRITE_TOTAL = 4
	};

	struct Button {
	public:
		Button() {
			position.x = 0;
			position.y = 0;
			currentSprite = BUTTON_SPRITE_MOUSE_OUT;
		}

		void setPosition(int x, int y) {
			position.x = x;
			position.y = y;
		}

		void handleEvent(SDL_Event* e) {
			if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP) {
				int x, y;
				SDL_GetMouseState(&x, &y);
				bool inside = true;
				if (x < position.x) {
					inside = false;
				} else if (x > position.x + BUTTON_WIDTH) {
					inside = false;
				} else if (y < position.y) {
					inside = false;
				} else if (y > position.y + BUTTON_HEIGHT) {
					inside = false;
				}
				if (!inside) {
					currentSprite = BUTTON_SPRITE_MOUSE_OUT;
				}
				else {
					switch (e->type) {
						case SDL_MOUSEMOTION: {
							currentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
							break;
						}
						case SDL_MOUSEBUTTONDOWN: {
							currentSprite = BUTTON_SPRITE_MOUSE_DOWN;
							break;
						}
						case SDL_MOUSEBUTTONUP: {
							currentSprite = BUTTON_SPRITE_MOUSE_UP;
							break;
						}
						default: {
							break;
						}
					}
				}
			}
		}

		void render(SDL_Renderer* renderer, Texture* buttonSpriteSheetTexture, SDL_Rect* buttonClips) {
			buttonSpriteSheetTexture->render(renderer, position.x, position.y, &buttonClips[currentSprite]);
		}

	private:
		SDL_Point position;
		ButtonSprite currentSprite;
	};

	Button buttons[NUM_BUTTONS];
	SDL_Rect buttonClips[NUM_BUTTONS];
	Texture buttonSpriteSheetTexture;
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