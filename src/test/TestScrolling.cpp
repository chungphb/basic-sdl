#include <core/Window.h>
#include <core/Texture.h>
#include <stdio.h>
#include <vector>
#include <math.h>

const int LEVEL_WIDTH = 960;
const int LEVEL_HEIGHT = 720;

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

struct Dot {
public:
	static constexpr int DOT_WIDTH = 20;
	static constexpr int DOT_HEIGHT = 20;
	static constexpr int DOT_VEL = 10;

	Dot() {
		posX = 0;
		posY = 0;
		velX = 0;
		velY = 0;
	}

	void handleEvent(SDL_Event& e) {
		if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
			switch (e.key.keysym.sym) {
				case SDLK_UP: {
					velY -= DOT_VEL; break;
				}
				case SDLK_DOWN: {
					velY += DOT_VEL; break;
				}
				case SDLK_LEFT: {
					velX -= DOT_VEL; break;
				}
				case SDLK_RIGHT: {
					velX += DOT_VEL; break;
				}
				default: {
					break;
				}
			}
		}
		else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
			switch (e.key.keysym.sym) {
				case SDLK_UP: {
					velY += DOT_VEL; break;
				}
				case SDLK_DOWN: {
					velY -= DOT_VEL; break;
				}
				case SDLK_LEFT: {
					velX += DOT_VEL; break;
				}
				case SDLK_RIGHT: {
					velX -= DOT_VEL; break;
				}
				default: {
					break;
				}
			}
		}
	}

	void move() {
		posX += velX;
		if (posX < 0 || posX + DOT_WIDTH > LEVEL_WIDTH) {
			posX -= velX;
		}
		posY += velY;
		if (posY < 0 || posY + DOT_HEIGHT > LEVEL_HEIGHT) {
			posY -= velY;
		}
	}

	void render(SDL_Renderer* renderer, Texture* dotTexture, SDL_Point cam) {
		dotTexture->render(renderer, posX - cam.x, posY - cam.y);
	}

	int getPosX() {
		return posX;
	}

	int getPosY() {
		return posY;
	}

private:
	int posX, posY;
	int velX, velY;
};

struct TestScrolling : public Window {
public:
	bool init() override {
		bool success = true;
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			printf("SDL could not initialize! Error: %s\n", SDL_GetError());
			success = false;
		}
		else {
			if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
				printf("Warning: Linear texture filtering is not enabled!");
			}
			window = SDL_CreateWindow("Test motion!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
			if (!window) {
				printf("Window could not be created! Error: %s\n", SDL_GetError());
				success = false;
			}
			else {
				renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
				if (!renderer) {
					printf("Renderer could not be created! Error: %s\n", SDL_GetError());
					success = false;
				}
				else {
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

	bool loadMedia() override {
		bool success = true;
		if (!dotTexture.loadFromFile(renderer, "image/dot.png")) {
			printf("Failed to load \"dot\" texture image!\n");
			success = false;
		}
		if (!backgroundTexture.loadFromFile(renderer, "image/background2.png")) {
			printf("Failed to load \"background2\" texture image!\n");
			success = false;
		}
		return success;
	}

	void run() override {
		bool quit = false;
		SDL_Event e;
		Dot dot;
		SDL_Rect cam{0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
		while (!quit) {
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				}
				else {
					dot.handleEvent(e);
				}
			}
			dot.move();
			cam.x = (dot.getPosX() - Dot::DOT_WIDTH / 2) - WINDOW_WIDTH / 2;
			cam.y = (dot.getPosY() - Dot::DOT_HEIGHT / 2) - WINDOW_HEIGHT / 2;
			if (cam.x < 0) {
				cam.x = 0;
			}
			if (cam.y < 0) {
				cam.y = 0;
			}
			if (cam.x > LEVEL_WIDTH - cam.w) {
				cam.x = LEVEL_WIDTH - cam.w;
			}
			if (cam.y > LEVEL_HEIGHT - cam.h) {
				cam.y = LEVEL_HEIGHT - cam.h;
			}
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(renderer);
			SDL_SetRenderDrawColor(renderer, 0xC0, 0x00, 0x00, 0xFF);
			backgroundTexture.render(renderer, 0, 0, &cam);
			dot.render(renderer, &dotTexture, {cam.x, cam.y});
			SDL_RenderPresent(renderer);
		}
	}

	void close() override {
		dotTexture.free();
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
	Texture dotTexture;
	Texture backgroundTexture;
};

int main(int argc, char** argv) {
	TestScrolling mainWindow;
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