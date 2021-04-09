#include <util/TestBase.h>
#include <core/Texture.h>
#include <stdio.h>

struct TestFile : public BasicTestBaseWithTTF {
public:
	bool loadMedia() override {
		bool success = true;
		font = TTF_OpenFont("font/prompt.ttf", 16);
		if (!font) {
			printf("Failed to load \"prompt\" font! Error: %s\n", TTF_GetError());
			success = false;
		} else {
			SDL_Color textColor{0xFF, 0xFF, 0xFF};
			if (!promptTextTexture.loadFromRenderedText(renderer, font, "[chungphb] Data: ", textColor)) {
				printf("Failed to render prompt text texture!\n");
				success = false;
			}
			success = loadDataFromFile(FILE_PATH);
			for (int i = 0; i < NUM_ELEMENTS; i++) {
				if (!dataTextures[i].loadFromRenderedText(renderer, font, std::to_string(data[i]), textColor)) {
					printf("Failed to render data text texture!\n");
					success = false;
				}
			}
		}
		return success;
	}

	bool loadDataFromFile(std::string path) {
		bool success = true;
		SDL_RWops* file = SDL_RWFromFile(path.c_str(), "r+b");
		if (!file) {
			printf("Warning: Unable to open file \"%s\"! Error: %s\n", SDL_GetError(), path.c_str());
			file = SDL_RWFromFile(path.c_str(), "w+b");
			if (!file) {
				printf("Unable to create file \"%s\"! Error: %s\n", SDL_GetError(), path.c_str());
				success = false;
			} else {
				printf("New file \"%s\" created!", path.c_str());
				for (int i = 0; i < NUM_ELEMENTS; i++) {
					data[i] = 0;
					SDL_RWwrite(file, &data[i], sizeof(Sint16), 1);
				}
				SDL_RWclose(file);
			}
		} else {
			printf("Reading file \"%s\"!", path.c_str());
			for (int i = 0; i < NUM_ELEMENTS; i++) {
				SDL_RWread(file, &data[i], sizeof(Sint16), 1);
			}
			SDL_RWclose(file);
		}
		return success;
	}

	void run() override {
		bool quit = false;
		SDL_Event e;
		SDL_Color normalColor{0xFF, 0xFF, 0xFF};
		SDL_Color highlightColor{0x00, 0x00, 0xFF};
		int currentId = 0;
		while (!quit) {
			bool renderText = false;
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				} else if (e.type == SDL_KEYDOWN) {
					switch (e.key.keysym.sym) {
						case SDLK_UP: {
							data[currentId]++;
							dataTextures[currentId].loadFromRenderedText(renderer, font, std::to_string(data[currentId]), highlightColor);
							break;
						}
						case SDLK_DOWN: {
							data[currentId]--;
							dataTextures[currentId].loadFromRenderedText(renderer, font, std::to_string(data[currentId]), highlightColor);
							break;
						}
						case SDLK_LEFT: {
							dataTextures[currentId].loadFromRenderedText(renderer, font, std::to_string(data[currentId]), normalColor);
							currentId--;
							if (currentId < 0) {
								currentId = NUM_ELEMENTS - 1;
							}
							dataTextures[currentId].loadFromRenderedText(renderer, font, std::to_string(data[currentId]), highlightColor);
							break;
						}
						case SDLK_RIGHT: {
							dataTextures[currentId].loadFromRenderedText(renderer, font, std::to_string(data[currentId]), normalColor);
							currentId++;
							if (currentId >= NUM_ELEMENTS) {
								currentId = 0;
							}
							dataTextures[currentId].loadFromRenderedText(renderer, font, std::to_string(data[currentId]), highlightColor);
							break;
						}
						default: {
							break;
						}
					}
				}
			}
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
			SDL_RenderClear(renderer);
			promptTextTexture.render(renderer, 0, 0);
			for (int i = 0; i < NUM_ELEMENTS; i++) {
				dataTextures[i].render(renderer, 170 + 35 * i, 0);
			}
			SDL_RenderPresent(renderer);
		}
		SDL_StopTextInput();
	}

	void close() override {
		SDL_RWops* file = SDL_RWFromFile(FILE_PATH.c_str(), "r+b");
		if (!file) {
			printf("Unable to save file \"%s\"! Error: %s\n", SDL_GetError(), FILE_PATH.c_str());
		} else {
			printf("Saving file \"%s\"!", FILE_PATH.c_str());
			for (int i = 0; i < NUM_ELEMENTS; i++) {
				SDL_RWwrite(file, &data[i], sizeof(Sint16), 1);
			}
			SDL_RWclose(file);
		}
		for (int i = 0; i < NUM_ELEMENTS; i++) {
			dataTextures[i].free();
		}
		promptTextTexture.free();
		TTF_CloseFont(font);
		font = nullptr;
		BasicTestBaseWithTTF::close();
	}

	std::string name() override {
		return "Test File";
	}

private:
	static constexpr int NUM_ELEMENTS = 12;
	const std::string FILE_PATH = "data.bin";
	TTF_Font* font = nullptr;
	Texture promptTextTexture;
	Sint16 data[NUM_ELEMENTS] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
	Texture dataTextures[NUM_ELEMENTS];
};

int main(int argc, char** argv) {
	TestFile mainWindow;
	mainWindow.test();
	return 0;
}