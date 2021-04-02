#include <core/Window.h>
#include <core/Texture.h>
#include <stdio.h>

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
		collisonBox.w = DOT_HEIGHT;
		collisonBox.h = DOT_WIDTH;
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
		} else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
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

	void move(SDL_Rect& wall) {
		posX += velX;
		collisonBox.x = posX;
		if (posX < 0 || (posX + DOT_WIDTH > WINDOW_WIDTH) || (checkCollision(wall, collisonBox))) {
			posX -= velX;
			collisonBox.x = posX;
		}
		posY += velY;
		collisonBox.y = posY;
		if (posY < 0 || (posY + DOT_HEIGHT > WINDOW_HEIGHT) || (checkCollision(wall, collisonBox))) {
			posY -= velY;
			collisonBox.y = posY;
		}
	}

	bool checkCollision(SDL_Rect& recA, SDL_Rect& recB) {
		if (recA.x + recA.w <= recB.x) {
			return false;
		}
		if (recA.x >= recB.x + recB.w) {
			return false;
		}
		if (recA.y + recA.h <= recB.y) {
			return false;
		}
		if (recA.y >= recB.y + recB.h) {
			return false;
		}
		return true;
	}

	void render(SDL_Renderer* renderer, Texture* dotTexture) {
		dotTexture->render(renderer, posX, posY);
	}

private:
	int posX, posY;
	int velX, velY;
	SDL_Rect collisonBox;
};

struct TestMotion : public Window {
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
			window = SDL_CreateWindow("Test motion!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
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
		return success;
	}

	void run() override {
		bool quit = false;
		SDL_Event e;
		Dot dot;
		SDL_Rect wall;
		wall.x = 300;
		wall.y = 40;
		wall.w = 40;
		wall.h = 400;
		while (!quit) {
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				} else {
					dot.handleEvent(e);
				}
			}
			dot.move(wall);
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(renderer);
			SDL_SetRenderDrawColor(renderer, 0xC0, 0x00, 0x00, 0xFF);
			SDL_RenderDrawRect(renderer, &wall);
			dot.render(renderer, &dotTexture);
			SDL_RenderPresent(renderer);
		}
	}

	void close() override {
		dotTexture.free();
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
};


int main(int argc, char** argv) {
	TestMotion mainWindow;
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