#pragma once

#include <SDL.h>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

struct Window {
public:
	Window();
	bool init();
	SDL_Renderer* createRenderer();
	void handleEvent(SDL_Event& event, SDL_Renderer* renderer);
	void free();
	int getWidth();
	int getHeight();
	bool hasMouseFocus();
	bool hasKeyboardFocus();
	bool isMinimized();

private:
	SDL_Window* window;
	int width;
	int height;
	bool mouseFocus;
	bool keyboardFocus;
	bool fullScreen;
	bool minimized;
};