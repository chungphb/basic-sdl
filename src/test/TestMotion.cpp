#include <core/Window.h>
#include <core/Texture.h>
#include <stdio.h>
#include <vector>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

namespace collision_detection {

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

}

namespace per_pixel_collision_detection {

struct Dot {
public:
	static constexpr int DOT_WIDTH = 20;
	static constexpr int DOT_HEIGHT = 20;
	static constexpr int DOT_VEL = 1;

	Dot(int x, int y) {
		posX = x;
		posY = y;
		velX = 0;
		velY = 0;
		colliders.resize(11);
		colliders[0].w = 6;
		colliders[0].h = 1;
		colliders[1].w = 10;
		colliders[1].h = 1;
		colliders[2].w = 14;
		colliders[2].h = 1;
		colliders[3].w = 16;
		colliders[3].h = 2;
		colliders[4].w = 18;
		colliders[4].h = 2;
		colliders[5].w = 20;
		colliders[5].h = 6;
		colliders[6].w = 18;
		colliders[6].h = 2;
		colliders[7].w = 16;
		colliders[7].h = 2;
		colliders[8].w = 14;
		colliders[8].h = 1;
		colliders[9].w = 10;
		colliders[9].h = 1;
		colliders[10].w = 6;
		colliders[10].h = 1;
		shiftColliders();
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

	void move(std::vector<SDL_Rect>& otherColliders) {
		posX += velX;
		shiftColliders();
		if (posX < 0 || (posX + DOT_WIDTH > WINDOW_WIDTH) || (checkCollision(colliders, otherColliders))) {
			posX -= velX;
			shiftColliders();
		}
		posY += velY;
		shiftColliders();
		if (posY < 0 || (posY + DOT_HEIGHT > WINDOW_HEIGHT) || (checkCollision(colliders, otherColliders))) {
			posY -= velY;
			shiftColliders();
		}
	}

	bool checkCollision(std::vector<SDL_Rect>& collidersA, std::vector<SDL_Rect>& collidersB) {
		for (const auto& colliderA : collidersA) {
			for (const auto& colliderB : collidersB) {
				if (colliderA.x + colliderA.w > colliderB.x && 
					colliderA.x < colliderB.x + colliderB.w &&
					colliderA.y + colliderA.h > colliderB.y &&
					colliderA.y < colliderB.y + colliderB.h) {
					return true;
				}
			}
		}
		return false;
	}

	void render(SDL_Renderer* renderer, Texture* dotTexture) {
		dotTexture->render(renderer, posX, posY);
	}

	std::vector<SDL_Rect>& getColliders() {
		return colliders;
	}

private:
	void shiftColliders() {
		int h = 0;
		for (int i = 0; i < colliders.size(); i++) {
			colliders[i].x = posX + (DOT_WIDTH - colliders[i].w) / 2;
			colliders[i].y = posY + h;
			h += colliders[i].h;
		}
	}

private:
	int posX, posY;
	int velX, velY;
	std::vector<SDL_Rect> colliders;
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
		if (!greenDotTexture.loadFromFile(renderer, "image/green_dot.bmp")) {
			printf("Failed to load \"green_dot\" texture image!\n");
			success = false;
		}
		if (!redDotTexture.loadFromFile(renderer, "image/red_dot.bmp")) {
			printf("Failed to load \"red_dot\" texture image!\n");
			success = false;
		}
		return success;
	}

	void run() override {
		bool quit = false;
		SDL_Event e;
		Dot greenDot{0, 0};
		Dot redDot{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};
		while (!quit) {
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				} else {
					greenDot.handleEvent(e);
				}
			}
			greenDot.move(redDot.getColliders());
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(renderer);
			greenDot.render(renderer, &greenDotTexture);
			redDot.render(renderer, &redDotTexture);
			SDL_RenderPresent(renderer);
		}
	}

	void close() override {
		greenDotTexture.free();
		redDotTexture.free();
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
		SDL_DestroyWindow(window);
		window = nullptr;
		IMG_Quit();
		SDL_Quit();
	}

private:
	SDL_Renderer* renderer = nullptr;
	Texture greenDotTexture;
	Texture redDotTexture;
};

}


int main(int argc, char** argv) {
	per_pixel_collision_detection::TestMotion mainWindow;
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