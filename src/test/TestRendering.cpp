#include <core/Window.h>
#include <core/Texture.h>
#include <core/Button.h>
#include <stdio.h>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

const int BUTTON_WIDTH = 40;
const int BUTTON_HEIGHT = 20;

struct TestRendering : public Window {
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
			window = SDL_CreateWindow("Test rendering!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
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
			printf("Failed to load \"crackman\" font! Error: %s\n", TTF_GetError());
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
		} else {
			const int WIDTH = 40;
			const int HEIGHT = 20;
			for (int i = 0; i < NUM_BUTTONS; i++) {
				buttonClips[i].x = i * WIDTH;
				buttonClips[i].y = 0;
				buttonClips[i].w = WIDTH;
				buttonClips[i].h = HEIGHT;
			}
			for (int i = 0; i < NUM_BUTTONS; i++) {
				buttons[i].setSize(BUTTON_WIDTH, BUTTON_HEIGHT);
				buttons[i].setPosition(130 + i * 120, 440);
			}
		}

		return success;
	}

	void run() override {
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

	void close() override {
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

	SDL_Renderer* renderer = nullptr;
	TTF_Font* font = nullptr;

	SDL_Rect characterClips[NUM_CHARACTERS];
	Texture characterSpriteSheetTexture;
	Texture sunTexture;
	Texture nameTexture;
	Texture backgroundTexture;

	SDL_Rect buttonClips[NUM_BUTTONS];
	Button buttons[NUM_BUTTONS];
	Texture buttonSpriteSheetTexture;
};


int main(int argc, char** argv) {
	TestRendering mainWindow;
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