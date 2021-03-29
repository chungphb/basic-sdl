#include <core/Window.h>
#include <core/Texture.h>
#include <stdio.h>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

struct TestRenderingEx : public Window {
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
			window = SDL_CreateWindow("Kitty world!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
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

	bool loadMedia() override {
		bool success = true;

		if (!spriteSheetTexture.loadFromFile(renderer, "image/animated_character.png")) {
			printf("Failed to load \"animated_character\" texture image!\n");
			success = false;
		} else {
			const int WIDTH = 104;
			const int HEIGHT = 147;
			for (int i = 0; i < NUM_FRAMES; i++) {
				spriteClips[i].x = i * WIDTH;
				spriteClips[i].y = 0;
				spriteClips[i].w = 104;
				spriteClips[i].h = 147;
			}
		}

		if (!backgroundTexture.loadFromFile(renderer, "image/background.png")) {
			printf("Failed to load \"background\" texture image!\n");
			success = false;
		}

		if (!upBackgroundTexture.loadFromFile(renderer, "image/up_background.png")) {
			printf("Failed to load \"up_background\" texture image!\n");
			success = false;
		}

		if (!downBackgroundTexture.loadFromFile(renderer, "image/down_background.png")) {
			printf("Failed to load \"down_background\" texture image!\n");
			success = false;
		}

		if (!leftBackgroundTexture.loadFromFile(renderer, "image/left_background.png")) {
			printf("Failed to load \"left_background\" texture image!\n");
			success = false;
		}

		if (!rightBackgroundTexture.loadFromFile(renderer, "image/right_background.png")) {
			printf("Failed to load \"right_background\" texture image!\n");
			success = false;
		}

		return success;
	}

	void run() override {
		bool quit = false;
		SDL_Event e;
		int frame = 0;
		Texture* currentBackgroundTexture = nullptr;
		while (!quit) {
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				}
			}
			const Uint8* currentKeyStates = SDL_GetKeyboardState(nullptr);
			if (currentKeyStates[SDL_SCANCODE_UP]) {
				currentBackgroundTexture = &upBackgroundTexture;
			} else if (currentKeyStates[SDL_SCANCODE_DOWN]) {
				currentBackgroundTexture = &downBackgroundTexture;
			} else if (currentKeyStates[SDL_SCANCODE_LEFT]) {
				currentBackgroundTexture = &leftBackgroundTexture;
			} else if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
				currentBackgroundTexture = &rightBackgroundTexture;
			} else {
				currentBackgroundTexture = &backgroundTexture;
			}
			SDL_SetRenderDrawColor(renderer, 0x59, 0x59, 0x59, 0xFF);
			SDL_RenderClear(renderer);
			currentBackgroundTexture->render(renderer, 0, 0);
			spriteSheetTexture.render(renderer, 360, 280, &spriteClips[frame / 300]);
			frame++;
			if (frame / 300 >= NUM_FRAMES) {
				frame = 0;
			}
			SDL_RenderPresent(renderer);
		}
	}

	void close() override {
		spriteSheetTexture.free();
		backgroundTexture.free();
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
		SDL_DestroyWindow(window);
		window = nullptr;
		IMG_Quit();
		SDL_Quit();
	}

private:
	static constexpr int NUM_FRAMES = 4;
	SDL_Renderer* renderer = nullptr;
	SDL_Rect spriteClips[NUM_FRAMES];
	Texture spriteSheetTexture;
	Texture backgroundTexture;
	Texture upBackgroundTexture;
	Texture downBackgroundTexture;
	Texture leftBackgroundTexture;
	Texture rightBackgroundTexture;
};


int main(int argc, char** argv) {
	TestRenderingEx mainWindow;
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