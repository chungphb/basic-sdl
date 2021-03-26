#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

const int JOYSTICK_DEAD_ZONE = 8000;

struct MainWindow {
public:
	bool init() {
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

			window = SDL_CreateWindow("Hello world!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
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

	bool loadMedia() {
		bool success = true;
		if (!characterTexture.loadFromFile(renderer, "image/character.png")) {
			printf("Failed to load \"character\" texture image!\n");
			success = false;
		}
		if (!sunTexture.loadFromFile(renderer, "image/sun.png")) {
			printf("Failed to load \"sun\" texture image!\n");
			success = false;
		}
		if (!backgroundTexture.loadFromFile(renderer, "image/background.png")) {
			printf("Failed to load \"background\" texture image!\n");
			success = false;
		}
		return success;
	}

	void run() {
		bool quit = false;
		SDL_Event e;
		int xDir = 0, yDir = 0;
		while (!quit) {
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				} else if (e.type == SDL_JOYAXISMOTION) {
					if (e.jaxis.which == 0) {
						if (e.jaxis.axis == 0) {
							if (e.jaxis.value < -JOYSTICK_DEAD_ZONE) {
								xDir = -1;
							} else if (e.jaxis.value > JOYSTICK_DEAD_ZONE) {
								xDir = 1;
							} else {
								xDir = 0;
							}
						} else if (e.jaxis.axis == 1) {
							if (e.jaxis.value < -JOYSTICK_DEAD_ZONE) {
								yDir = -1;
							} else if (e.jaxis.value > JOYSTICK_DEAD_ZONE) {
								yDir = 1;
							} else {
								yDir = 0;
							}
						}
					}
				} else if (e.type == SDL_JOYBUTTONDOWN) {
					if (!SDL_HapticRumblePlay(controllerHaptic, 0.75, 500)) {
						printf("Warning: Unable to play rumble! Error: %s\n", SDL_GetError());
					}
				}
			}
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(renderer);
			double joystickAngle = atan2(static_cast<double>(yDir), static_cast<double>(yDir)) * (180.0 / M_PI);
			if (xDir == 0 && yDir == 0) {
				joystickAngle = 0;
			}
			backgroundTexture.render(renderer, 0, 0);
			sunTexture.render(renderer, WINDOW_WIDTH - 100, 10, nullptr, joystickAngle);
			characterTexture.render(renderer, WINDOW_WIDTH / 2 + 40, WINDOW_HEIGHT / 2 + 40);
			SDL_RenderPresent(renderer);
		}
	}

	void close() {
		characterTexture.free();
		sunTexture.free();
		backgroundTexture.free();
		SDL_HapticClose(controllerHaptic);
		controllerHaptic = nullptr;
		SDL_JoystickClose(gameController);
		gameController = nullptr;
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
		SDL_DestroyWindow(window);
		window = nullptr;
		IMG_Quit();
		SDL_Quit();
	}

private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	struct Texture {
	public:
		Texture() {
			texture = nullptr;
			width = 0;
			height = 0;
		}

		~Texture() {
			free();
		}

		bool loadFromFile(SDL_Renderer* renderer, std::string path) {
			free();
			SDL_Texture* newTexture = nullptr;
			SDL_Surface* loadedSurface = IMG_Load(path.c_str());
			if (!loadedSurface) {
				printf("Unable to load image %s! Error: %s\n", path.c_str(), IMG_GetError());
			} else {
				SDL_SetColorKey(loadedSurface, true, SDL_MapRGB(loadedSurface->format, 0x00, 0xFF, 0xFF));
				newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
				if (!newTexture) {
					printf("Unable to create texture from %s! Error: %s\n", path.c_str(), SDL_GetError());
				} else {
					width = loadedSurface->w;
					height = loadedSurface->h;
				}
				SDL_FreeSurface(loadedSurface);
			}
			texture = newTexture;
			return texture != nullptr;
		}

		void free() {
			if (texture) {
				SDL_DestroyTexture(texture);
				texture = nullptr;
				width = 0;
				height = 0;
			}
		}

		void render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip = nullptr, double angle = 0.0, SDL_Point* center = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE) {
			SDL_Rect renderQuad{x, y, width, height};
			if (clip) {
				renderQuad.w = clip->w;
				renderQuad.h = clip->h;
			}
			SDL_RenderCopyEx(renderer, texture, clip, &renderQuad, angle, center, flip);
		}

		int getWidth() {
			return width;
		}

		int getHeight() {
			return height;
		}

	private:
		SDL_Texture* texture;
		int width;
		int height;
	};
	Texture characterTexture;
	Texture sunTexture;
	Texture backgroundTexture;

	SDL_Joystick* gameController = nullptr;
	SDL_Haptic* controllerHaptic = nullptr;
};


int main(int argc, char** argv) {
	MainWindow mainWindow;
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