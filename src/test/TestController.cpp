#include <util/TestBase.h>
#include <core/Texture.h>
#include <stdio.h>

const int JOYSTICK_DEAD_ZONE = 8000;

struct TestController : public BasicTestBaseWithController {
public:
	bool loadMedia() override {
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

	void run() override {
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

	void close() override {
		characterTexture.free();
		sunTexture.free();
		backgroundTexture.free();
		BasicTestBaseWithController::close();
	}

private:
	Texture characterTexture;
	Texture sunTexture;
	Texture backgroundTexture;
};


int main(int argc, char** argv) {
	TestController mainWindow;
	mainWindow.test();
	return 0;
}