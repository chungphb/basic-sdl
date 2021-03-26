#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <string>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

struct MainWindow {
public:
	bool init() {
		bool success = true;
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
			printf("SDL could not initialize! Error: %s\n", SDL_GetError());
			success = false;
		} else {
			if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
				printf("Warning: Linear texture filtering is not enabled!");
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

					if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
						printf("SDL_mixer could not initialize! Error: %s\n", Mix_GetError());
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
		if (!backgroundTexture.loadFromFile(renderer, "image/background.png")) {
			printf("Failed to load \"background\" texture image! Error: %s\n", Mix_GetError());
			success = false;
		}
		music = Mix_LoadMUS("sound/nico.wav");
		if (!music) {
			printf("Failed to load \"nico\" music! Error: %s\n", Mix_GetError());
			success = false;
		}
		soundEffect = Mix_LoadWAV("sound/kitty.wav");
		if (!soundEffect) {
			printf("Failed to load \"kitty\" sound effect! Error: %s\n", Mix_GetError());
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
				} else if (e.type == SDL_KEYDOWN) {
					switch (e.key.keysym.sym) {
						case SDLK_1: {
							Mix_PlayChannel(-1, soundEffect, 0);
							break;
						}
						case SDLK_9: {
							if (Mix_PlayingMusic() == 0) {
								Mix_PlayMusic(music, -1);
								break;
							} else {
								if (Mix_PausedMusic() == 1) {
									Mix_ResumeMusic();
								} else {
									Mix_PauseMusic();
								}
							}
							break;
						}
						case SDLK_0: {
							Mix_HaltMusic();
							break;
						}
						default: {
							break;
						}
					}
				}
			}
			SDL_RenderClear(renderer);
			backgroundTexture.render(renderer, 0, 0);
			characterTexture.render(renderer, WINDOW_WIDTH / 2 + 40, WINDOW_HEIGHT / 2 + 40);
			SDL_RenderPresent(renderer);
		}
	}

	void close() {
		characterTexture.free();
		backgroundTexture.free();
		Mix_FreeChunk(soundEffect);
		soundEffect = nullptr;
		Mix_FreeMusic(music);
		music = nullptr;
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

		void render(SDL_Renderer* renderer, int x, int y) {
			SDL_Rect renderQuad{x, y, width, height};
			SDL_RenderCopy(renderer, texture, nullptr, &renderQuad);
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
	Texture backgroundTexture;

	Mix_Music* music = nullptr;
	Mix_Chunk* soundEffect = nullptr;;
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