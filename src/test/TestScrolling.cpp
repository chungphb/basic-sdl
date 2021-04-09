#include <util/TestBase.h>
#include <core/Texture.h>
#include <stdio.h>
#include <vector>
#include <math.h>

const int LEVEL_WIDTH = 960;
const int LEVEL_HEIGHT = 720;

namespace test_scrolling {

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

struct TestScrolling : public BasicTestBase {
public:
	bool loadMedia() override {
		bool success = true;
		if (!dotTexture.loadFromFile(renderer, "image/red_dot.png")) {
			printf("Failed to load \"red_dot\" texture image!\n");
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
		BasicTestBase::close();
	}

	std::string name() override {
		return "Test Scrolling";
	}

private:
	Texture dotTexture;
	Texture backgroundTexture;
};

}

namespace test_scrolling_backgrounds {

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

	void render(SDL_Renderer* renderer, Texture* dotTexture) {
		dotTexture->render(renderer, posX, posY);
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

struct TestScrolling : public BasicTestBase {
public:
	bool loadMedia() override {
		bool success = true;
		if (!dotTexture.loadFromFile(renderer, "image/red_dot.png")) {
			printf("Failed to load \"red_dot\" texture image!\n");
			success = false;
		}
		if (!backgroundTexture.loadFromFile(renderer, "image/background3.png")) {
			printf("Failed to load \"background3\" texture image!\n");
			success = false;
		}
		return success;
	}

	void run() override {
		bool quit = false;
		SDL_Event e;
		Dot dot;
		int scrollingOffset = 0;
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
			scrollingOffset--;
			if (scrollingOffset < -WINDOW_WIDTH) {
				scrollingOffset = 0;
			}
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(renderer);
			SDL_SetRenderDrawColor(renderer, 0xC0, 0x00, 0x00, 0xFF);
			backgroundTexture.render(renderer, scrollingOffset, 0);
			backgroundTexture.render(renderer, scrollingOffset + WINDOW_WIDTH, 0);
			dot.render(renderer, &dotTexture);
			SDL_RenderPresent(renderer);
		}
	}

	void close() override {
		dotTexture.free();
		backgroundTexture.free();
		BasicTestBase::close();
	}

	std::string name() override {
		return "Test Scrolling Backgrounds";
	}

private:
	Texture dotTexture;
	Texture backgroundTexture;
};

}

int main(int argc, char** argv) {
	{
		test_scrolling::TestScrolling mainWindow;
		mainWindow.test();
	}
	{
		test_scrolling_backgrounds::TestScrolling mainWindow;
		mainWindow.test();
	}
	return 0;
}