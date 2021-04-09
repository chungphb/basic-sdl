#include <util/TestBase.h>
#include <stdio.h>
#include <string>

struct TestBasicRendering : public BasicTestBase {
public:
	bool loadMedia() override {
		bool success = true;
		texture = loadTexture("image/landscape.png");
		if (!texture) {
			printf("Failed to load texture image!\n");
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

			// IMAGE RENDERING

			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, texture, nullptr, nullptr);

			// GEOMETRY RENDERING

			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(renderer);

			SDL_Rect rect;
			rect.x = WINDOW_WIDTH / 4;
			rect.y = WINDOW_HEIGHT / 4;
			rect.w = WINDOW_WIDTH / 2;
			rect.h = WINDOW_HEIGHT / 2;
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 0xFF);
			SDL_RenderFillRect(renderer, &rect);

			SDL_Rect otherRect;
			otherRect.x = WINDOW_WIDTH / 6;
			otherRect.y = WINDOW_HEIGHT / 6;
			otherRect.w = 2 * WINDOW_WIDTH / 3;
			otherRect.h = 2 * WINDOW_HEIGHT / 3;
			SDL_SetRenderDrawColor(renderer, 0xFF, 0x69, 0xB4, 0xFF);
			SDL_RenderDrawRect(renderer, &otherRect);

			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderDrawLine(renderer, WINDOW_WIDTH / 3, WINDOW_HEIGHT / 2, 2 * WINDOW_WIDTH / 3, WINDOW_HEIGHT / 2);
			for (int i = WINDOW_HEIGHT / 3; i <= 2 * WINDOW_HEIGHT / 3; i += WINDOW_HEIGHT / 120) {
				SDL_RenderDrawPoint(renderer, WINDOW_WIDTH / 2, i);
			}

			// VIEWPORT

			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(renderer);

			SDL_Rect topLeftViewPort;
			topLeftViewPort.x = 0;
			topLeftViewPort.y = 0;
			topLeftViewPort.w = WINDOW_WIDTH / 2;
			topLeftViewPort.h = WINDOW_HEIGHT / 2;
			SDL_RenderSetViewport(renderer, &topLeftViewPort);
			SDL_RenderCopy(renderer, texture, nullptr, nullptr);

			SDL_Rect topRightViewPort;
			topRightViewPort.x = WINDOW_WIDTH / 2;
			topRightViewPort.y = 0;
			topRightViewPort.w = WINDOW_WIDTH / 2;
			topRightViewPort.h = WINDOW_HEIGHT / 2;
			SDL_RenderSetViewport(renderer, &topRightViewPort);
			SDL_RenderCopy(renderer, texture, nullptr, nullptr);

			SDL_Rect bottomViewPort;
			bottomViewPort.x = 0;
			bottomViewPort.y = WINDOW_HEIGHT / 2;
			bottomViewPort.w = WINDOW_WIDTH;
			bottomViewPort.h = WINDOW_HEIGHT / 2;
			SDL_RenderSetViewport(renderer, &bottomViewPort);
			SDL_RenderCopy(renderer, texture, nullptr, nullptr);

			SDL_RenderPresent(renderer);
		}
	}

	void close() override {
		SDL_DestroyTexture(texture);
		texture = nullptr;
		BasicTestBase::close();
	}

	SDL_Texture* loadTexture(std::string path) {
		SDL_Texture* newTexture = nullptr;
		SDL_Surface* loadedSurface = IMG_Load(path.c_str());
		if (!loadedSurface) {
			printf("Unable to load image %s! Error: %s\n", path.c_str(), IMG_GetError());
		} else {
			newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
			if (!newTexture) {
				printf("Unable to create texture from %s! Error: %s\n", path.c_str(), SDL_GetError());
			}
			SDL_FreeSurface(loadedSurface);
		}
		return newTexture;
	}

	std::string name() override {
		return "Test Basic Rendering";
	}

private:
	SDL_Texture* texture = nullptr;
};


int main(int argc, char** argv) {
	TestBasicRendering mainWindow;
	mainWindow.test();
	return 0;
}