#include <util/TestBase.h>
#include <core/Texture.h>
#include <stdio.h>
#include <vector>
#include <math.h>

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
		collider.w = DOT_HEIGHT;
		collider.h = DOT_WIDTH;
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
		collider.x = posX;
		if (posX < 0 || (posX + DOT_WIDTH > WINDOW_WIDTH) || checkCollision(wall, collider)) {
			posX -= velX;
			collider.x = posX;
		}
		posY += velY;
		collider.y = posY;
		if (posY < 0 || (posY + DOT_HEIGHT > WINDOW_HEIGHT) || checkCollision(wall, collider)) {
			posY -= velY;
			collider.y = posY;
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
	SDL_Rect collider;
};

struct TestMotion : public BasicTestBase {
public:
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
		BasicTestBase::close();
	}

	std::string name() override {
		return "Test Collision Detection";
	}

private:
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
		if (posX < 0 || (posX + DOT_WIDTH > WINDOW_WIDTH) || checkCollision(colliders, otherColliders)) {
			posX -= velX;
			shiftColliders();
		}
		posY += velY;
		shiftColliders();
		if (posY < 0 || (posY + DOT_HEIGHT > WINDOW_HEIGHT) || checkCollision(colliders, otherColliders)) {
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

struct TestMotion : public BasicTestBase {
public:
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
		BasicTestBase::close();
	}

	std::string name() override {
		return "Test Per-Pixel Collision Detection";
	}

private:
	Texture greenDotTexture;
	Texture redDotTexture;
};

}


namespace circular_collision_detection {

struct Circle {
	int x, y;
	int r;
};

struct Dot {
public:
	static constexpr int DOT_WIDTH = 20;
	static constexpr int DOT_HEIGHT = 20;
	static constexpr int DOT_VEL = 10;

	Dot(int x, int y) {
		posX = x;
		posY = y;
		velX = 0;
		velY = 0;
		collider.r = DOT_WIDTH / 2;
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

	void move(std::vector<SDL_Rect>& squares, Circle& circle) {
		posX += velX;
		shiftColliders();
		if (posX - collider.r < 0 || (posX + collider.r > WINDOW_WIDTH) || checkCollision(collider, squares) || checkCollision(collider, circle)) {
			posX -= velX;
			shiftColliders();
		}
		posY += velY;
		shiftColliders();
		if (posY - collider.r < 0 || (posY + collider.r > WINDOW_HEIGHT) || checkCollision(collider, squares) || checkCollision(collider, circle))  {
			posY -= velY;
			shiftColliders();
		}
	}

	bool checkCollision(Circle& circle, std::vector<SDL_Rect>& rects) {
		for (const auto& rect : rects) {
			SDL_Point closestPoint;
			if (circle.x < rect.x) {
				closestPoint.x = rect.x;
			} else if (circle.x > rect.x + rect.w) {
				closestPoint.x = rect.x + rect.w;
			} else {
				closestPoint.x = circle.x;
			}
			if (circle.y < rect.y) {
				closestPoint.y = rect.y;
			} else if (circle.y > rect.y + rect.h) {
				closestPoint.y = rect.y + rect.h;
			} else {
				closestPoint.y = circle.y;
			}
			if (distance(circle.x, closestPoint.x, circle.y, closestPoint.y) < circle.r * circle.r) {
				return true;
			}
		}
		return false;
	}

	bool checkCollision(Circle& circleA, Circle& circleB) {
		if (distance(circleA.x, circleB.x, circleA.y, circleB.y) < (circleA.r + circleB.r) * (circleA.r + circleB.r)) {
			return true;
		}
		return false;
	}

	void render(SDL_Renderer* renderer, Texture* dotTexture) {
		dotTexture->render(renderer, posX - collider.r, posY - collider.r);
	}

	Circle& getCollider() {
		return collider;
	}

	int distance(int x1, int x2, int y1, int y2) {
		return (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
	}

private:
	void shiftColliders() {
		collider.x = posX;
		collider.y = posY;
	}

private:
	int posX, posY;
	int velX, velY;
	Circle collider;
};

struct TestMotion : public BasicTestBase {
public:
	bool loadMedia() override {
		bool success = true;
		if (!greenDotTexture.loadFromFile(renderer, "image/green_dot.png")) {
			printf("Failed to load \"green_dot\" texture image!\n");
			success = false;
		}
		if (!redDotTexture.loadFromFile(renderer, "image/red_dot.png")) {
			printf("Failed to load \"red_dot\" texture image!\n");
			success = false;
		}
		return success;
	}

	void run() override {
		bool quit = false;
		SDL_Event e;
		Dot greenDot{Dot::DOT_WIDTH / 2, Dot::DOT_HEIGHT / 2};
		Dot redDot{240, WINDOW_HEIGHT / 2};
		std::vector<SDL_Rect> walls;
		walls.emplace_back(SDL_Rect{300, 40, 40, 400});
		walls.emplace_back(SDL_Rect{140, 40, 40, 200});
		walls.emplace_back(SDL_Rect{460, 240, 40, 200});
		while (!quit) {
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				} else {
					greenDot.handleEvent(e);
				}
			}
			greenDot.move(walls, redDot.getCollider());
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(renderer);
			SDL_SetRenderDrawColor(renderer, 0x00, 0x70, 0xC0, 0xFF);
			SDL_RenderFillRect(renderer, &walls[0]);
			SDL_RenderFillRect(renderer, &walls[1]);
			SDL_RenderFillRect(renderer, &walls[2]);
			greenDot.render(renderer, &greenDotTexture);
			redDot.render(renderer, &redDotTexture);
			SDL_RenderPresent(renderer);
		}
	}

	void close() override {
		greenDotTexture.free();
		redDotTexture.free();
		BasicTestBase::close();
	}

	std::string name() override {
		return "Test Circular Collision Detection";
	}

private:
	Texture greenDotTexture;
	Texture redDotTexture;
};

}

int main(int argc, char** argv) {
	{
		collision_detection::TestMotion mainWindow;
		mainWindow.test();
	}
	{
		per_pixel_collision_detection::TestMotion mainWindow;
		mainWindow.test();
	}
	{
		circular_collision_detection::TestMotion mainWindow;
		mainWindow.test();
	}
	return 0;
}