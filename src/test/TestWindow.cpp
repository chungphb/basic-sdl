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

struct TestMultipleWindows {
public:
	bool init() {
		bool success = true;
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			printf("SDL could not initialize! Error: %s\n", SDL_GetError());
			success = false;
		} else {
			if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
				printf("Warning: Linear texture filtering is not enabled!");
			}
			if (!windows[0].init()) {
				printf("Window 0 could not be created! Error: %s\n", SDL_GetError());
				success = false;
			}
		}
		return success;
	}

	bool loadMedia() {
		bool success = true;
		return success;
	}

	void run() {
		for (int i = 1; i < NUM_WINDOWS; i++) {
			windows[i].init();
		}
		bool quit = false;
		SDL_Event e;
		while (!quit) {
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				}
				for (int i = 0; i < NUM_WINDOWS; i++) {
					windows[i].handleEvent(e);
				}
				if (e.type == SDL_KEYDOWN) {
					auto id = e.key.keysym.sym - SDLK_1;
					if (id >= 0 && id < NUM_WINDOWS) {
						windows[id].focus();
					}
				}
			}
			for (int i = 0; i < NUM_WINDOWS; i++) {
				windows[i].render();
			}
			bool allClosed = true;
			for (int i = 0; i < NUM_WINDOWS; i++) {
				if (windows[i].isShown()) {
					allClosed = false;
					break;
				}
			}
			if (allClosed) {
				quit = true;
			}
		}
	}

	void close() {
		for (int i = 0; i < NUM_WINDOWS; i++) {
			windows[i].free();
		}
		SDL_Quit();
	}

private:
	static constexpr int NUM_WINDOWS = 3;
	WindowEx windows[NUM_WINDOWS];
};

struct TestMultipleDisplays {
public:
	bool init() {
		bool success = true;
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			printf("SDL could not initialize! Error: %s\n", SDL_GetError());
			success = false;
		} else {
			if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
				printf("Warning: Linear texture filtering is not enabled!");
			}
			WindowEx::numDisplays = SDL_GetNumVideoDisplays();
			if (WindowEx::numDisplays < 2) {
				printf("Warning: Only one display connected!");
			}
			WindowEx::displayBounds = new SDL_Rect[WindowEx::numDisplays];
			for (int i = 0; i < WindowEx::numDisplays; i++) {
				SDL_GetDisplayBounds(i, &WindowEx::displayBounds[i]);
			}
			if (!window.init()) {
				printf("Window could not be created! Error: %s\n", SDL_GetError());
				success = false;
			}
		}
		return success;
	}

	bool loadMedia() {
		bool success = true;
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
				window.handleEvent(e);
			}
			window.render();
		}
	}

	void close() {
		window.free();
		SDL_Quit();
	}

private:
	WindowEx window;
};

#define TEST(name) \
{ \
	name mainWindow; \
	if (!mainWindow.init()) { \
		printf("Failed to initialize!\n"); \
	} \
	else { \
		if (!mainWindow.loadMedia()) { \
			printf("Failed to load media!\n"); \
		} else { \
			mainWindow.run(); \
		} \
	} \
	mainWindow.close(); \
}

int main(int argc, char** argv) {
	TEST(TestWindow)
	TEST(TestMultipleWindows)
	TEST(TestMultipleDisplays)
	return 0;
}