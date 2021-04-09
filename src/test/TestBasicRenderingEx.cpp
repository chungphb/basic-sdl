#include <util/TestBase.h>
#include <core/Texture.h>
#include <stdio.h>

struct TestBasicRenderingEx : public BasicTestBase {
public:
	bool loadMedia() override {
		bool success = true;
		if (!characterTexture.loadFromFile(renderer, "image/character.png")) {
			printf("Failed to load \"character\" texture image!\n");
			success = false;
		}
		if (!backgroundTexture.loadFromFile(renderer, "image/background.png")) {
			printf("Failed to load \"background\" texture image!\n");
			success = false;
		}
		return success;
	}

	void run() override {
		bool quit = false;
		SDL_Event e;
		while (!quit) {
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				}
			}
			SDL_RenderClear(renderer);
			backgroundTexture.render(renderer, 0, 0);
			characterTexture.render(renderer, WINDOW_WIDTH / 2 + 40, WINDOW_HEIGHT / 2 + 40);
			SDL_RenderPresent(renderer);
		}
	}

	void close() override {
		characterTexture.free();
		backgroundTexture.free();
		BasicTestBase::close();
	}

private:
	Texture characterTexture;
	Texture backgroundTexture;
};


int main(int argc, char** argv) {
	TestBasicRenderingEx mainWindow;
	mainWindow.test();
	return 0;
}