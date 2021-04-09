#include <util/TestBase.h>
#include <stdio.h>

std::string TestBase::name() {
	return "Test";
}

void TestBase::test() {
	if (!init()) {
		printf("Failed to initialize!\n");
	} else {
		if (!loadMedia()) {
			printf("Failed to load media!\n");
		} else {
			run();
		}
	}
	close();
}

bool BasicTestBase::init() {
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! Error: %s\n", SDL_GetError());
		success = false;
	} else {
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			printf("Warning: Linear texture filtering is not enabled!");
		}
		window = SDL_CreateWindow(name().c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
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

bool BasicTestBase::loadMedia() {
	bool success = true;
	return success;
}

void BasicTestBase::run() {
	bool quit = false;
	SDL_Event e;
	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			}
		}
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);
	}
}

void BasicTestBase::close() {
	SDL_DestroyRenderer(renderer);
	renderer = nullptr;
	SDL_DestroyWindow(window);
	window = nullptr;
	IMG_Quit();
	SDL_Quit();
}

std::string BasicTestBase::name() {
	return "Basic Test";
}

bool BasicTestBaseWithTTF::init() {
	bool success = BasicTestBase::init();
	if (TTF_Init() == -1) {
		printf("SDL_ttf could not initialize! Error: %s\n", TTF_GetError());
		success = false;
	}
	return success;
}

void BasicTestBaseWithTTF::close() {
	TTF_Quit();
	BasicTestBase::close();
}

std::string BasicTestBaseWithTTF::name() {
	return "Test TTF";
}

bool BasicTestBaseWithAudio::init() {
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		printf("SDL could not initialize! Error: %s\n", SDL_GetError());
		success = false;
	} else {
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			printf("Warning: Linear texture filtering is not enabled!");
		}
		window = SDL_CreateWindow(name().c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
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

std::string BasicTestBaseWithAudio::name() {
	return "Test Audio";
}

bool BasicTestBaseWithTTFAndAudio::init() {
	bool success = BasicTestBaseWithAudio::init();
	if (TTF_Init() == -1) {
		printf("SDL_ttf could not initialize! Error: %s\n", TTF_GetError());
		success = false;
	}
	return success;
}

void BasicTestBaseWithTTFAndAudio::close() {
	TTF_Quit();
	BasicTestBase::close();
}

std::string BasicTestBaseWithTTFAndAudio::name() {
	return "Test TTF and Audio";
}

bool BasicTestBaseWithController::init() {
	bool success = true;
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC) < 0) {
		printf("SDL could not initialize! Error: %s\n", SDL_GetError());
		success = false;
	} else {
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
			printf("Warning: Linear texture filtering is not enabled!");
		}

		if (SDL_NumJoysticks() < 1) {
			printf("Warning: No joystick is connected!");
		} else {
			gameController = SDL_JoystickOpen(0);
			if (!gameController) {
				printf("Warning: Unable to open game controller! Error: %s\n", SDL_GetError());
			} else {
				controllerHaptic = SDL_HapticOpenFromJoystick(gameController);
				if (!controllerHaptic) {
					printf("Warning: Controller does not support haptics! Error: %s\n", SDL_GetError());
				} else {
					if (!SDL_HapticRumbleInit(controllerHaptic)) {
						printf("Warning: Unable to initialize rumble! Error: %s\n", SDL_GetError());
					}
				}
			}
		}

		window = SDL_CreateWindow("Test controller!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
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

void BasicTestBaseWithController::close() {
	SDL_HapticClose(controllerHaptic);
	controllerHaptic = nullptr;
	SDL_JoystickClose(gameController);
	gameController = nullptr;
	BasicTestBase::close();
}

std::string BasicTestBaseWithController::name() {
	return "Test Controller";
}