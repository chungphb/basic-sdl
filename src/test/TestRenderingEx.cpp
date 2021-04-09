#include <util/TestBase.h>
#include <core/Texture.h>
#include <stdio.h>

struct TestRenderingEx : public BasicTestBase {
public:
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
			spriteSheetTexture.render(renderer, 360, 280, &spriteClips[frame / 12]);
			frame++;
			if (frame / 12 >= NUM_FRAMES) {
				frame = 0;
			}
			SDL_RenderPresent(renderer);
		}
	}

	void close() override {
		spriteSheetTexture.free();
		backgroundTexture.free();
		BasicTestBase::close();
	}

private:
	static constexpr int NUM_FRAMES = 4;
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
	mainWindow.test();
	return 0;
}