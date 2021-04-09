#include <util/TestBase.h>
#include <core/Texture.h>
#include <stdio.h>

struct TestTextInput : public BasicTestBaseWithTTF {
public:
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
		BasicTestBaseWithTTF::close();
	}

	std::string name() override {
		return "Test Text Input";
	}

private:
	TTF_Font* font = nullptr;
	Texture inputTextTexture;
	Texture promptTextTexture;
};

int main(int argc, char** argv) {
	TestTextInput mainWindow;
	mainWindow.test();
	return 0;
}