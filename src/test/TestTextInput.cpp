#include <core/Window.h>
#include <core/Texture.h>
#include <stdio.h>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

struct TestTextInput : public Window {
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
			window = SDL_CreateWindow("Test text input!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
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
					if (TTF_Init() == -1) {
						printf("SDL_ttf could not initialize! Error: %s\n", TTF_GetError());
						success = false;
					}
				}
			}
		}
		return success;
	}

	bool loadMedia() override {
		bool success = true;
		font = TTF_OpenFont("font/prompt.ttf", 16);
		if (!font) {
			printf("Failed to load \"prompt\" font! Error: %s\n", TTF_GetError());
			success = false;
		} else {
			SDL_Color textColor{0xFF, 0xFF, 0xFF};
			if (!promptTextTexture.loadFromRenderedText(renderer, font, "[chungphb] ", textColor)) {
				printf("Failed to render prompt text texture!\n");
				success = false;
			}
		}
		return success;
	}

	void run() override {
		bool quit = false;
		SDL_Event e;
		SDL_Color textColor{0xFF, 0xFF, 0xFF};
		std::string inputText = " ";
		inputTextTexture.loadFromRenderedText(renderer, font, inputText, textColor);
		while (!quit) {
			bool renderText = false;
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				} else if (e.type == SDL_KEYDOWN) {
					if (e.key.keysym.sym == SDLK_BACKSPACE) {
						if (!inputText.empty()) {
							inputText.pop_back();
							renderText = true;
						}
					} else if (e.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL) {
						SDL_SetClipboardText(inputText.c_str());
					} else if (e.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL) {
						inputText = SDL_GetClipboardText();
						renderText = true;
					}
				} else if (e.type == SDL_TEXTINPUT) {
					if (!(SDL_GetModState() & KMOD_CTRL && (e.text.text[0] == 'c' || e.text.text[0] == 'C' || e.text.text[0] == 'v' || e.text.text[0] == 'V'))) {
						inputText += e.text.text;
						renderText = true;
					}
				}
			}
			if (renderText) {
				if (!inputText.empty()) {
					inputTextTexture.loadFromRenderedText(renderer, font, inputText.c_str(), textColor);
				} else {
					inputTextTexture.loadFromRenderedText(renderer, font, " ", textColor);
				}
			}
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
			SDL_RenderClear(renderer);
			promptTextTexture.render(renderer, 0, 0);
			inputTextTexture.render(renderer, 100, 0);
			SDL_RenderPresent(renderer);
		}
		SDL_StopTextInput();
	}

	void close() override {
		inputTextTexture.free();
		promptTextTexture.free();
		TTF_CloseFont(font);
		font = nullptr;
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
		SDL_DestroyWindow(window);
		window = nullptr;
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
	}

private:
	SDL_Renderer* renderer = nullptr;
	TTF_Font* font = nullptr;
	Texture inputTextTexture;
	Texture promptTextTexture;
};

int main(int argc, char** argv) {
	TestTextInput mainWindow;
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