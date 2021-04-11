#include <core/Window.h>
#include <core/Texture.h>

struct TestWindow {
public:
	bool init() {
		bool success = true;
		if (!window.init()) {
			printf("Window could not be created! Error: %s\n", SDL_GetError());
			success = false;
		} else {
			renderer = window.createRenderer();
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
		return success;
	}

	bool loadMedia() {
		bool success = true;
		if (!texture.loadFromFile(renderer, "image/landscape.png")) {
			printf("Failed to load \"landscape\" texture image!\n");
			success = false;
		}
		return success;
	}

	void run() {
		bool quit = false;
		SDL_Event e;
		while (!quit) {
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				}
				window.handleEvent(e, renderer);
			}
			if (!window.isMinimized()) {
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(renderer);
				texture.render(renderer, (window.getWidth() - texture.getWidth()) / 2, (window.getHeight() - texture.getHeight()) / 2);
				SDL_RenderPresent(renderer);
			}
		}
	}

	void close() {
		texture.free();
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
		window.free();
		IMG_Quit();
		SDL_Quit();
	}

private:
	Window window;
	SDL_Renderer* renderer;
	Texture texture;
};

int main(int argc, char** argv) {
	TestWindow mainWindow;
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