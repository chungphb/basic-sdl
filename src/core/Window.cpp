#include <core/Window.h>
#include <sstream>

Window::Window() {
	window = nullptr;
	width = 0;
	height = 0;
	mouseFocus = false;
	keyboardFocus = false;
	fullScreen = false;
	minimized = false;
}

bool Window::init() {
	window = SDL_CreateWindow("Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (window) {
		mouseFocus = true;
		keyboardFocus = true;
		width = WINDOW_WIDTH;
		height = WINDOW_HEIGHT;
		return true;
	}
	return false;
}

SDL_Renderer* Window::createRenderer() {
	return SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void Window::handleEvent(SDL_Event& event, SDL_Renderer* renderer) {
	if (event.type == SDL_WINDOWEVENT) {
		bool updateTitle = false;
		switch (event.window.event) {
			case SDL_WINDOWEVENT_SIZE_CHANGED: {
				width = event.window.data1;
				height = event.window.data2;
				SDL_RenderPresent(renderer);
				break;
			}
			case SDL_WINDOWEVENT_EXPOSED: {
				SDL_RenderPresent(renderer);
				break;
			}
			case SDL_WINDOWEVENT_ENTER: {
				mouseFocus = true;
				updateTitle = true;
				break;
			}
			case SDL_WINDOWEVENT_LEAVE: {
				mouseFocus = false;
				updateTitle = true;
				break;
			}
			case SDL_WINDOWEVENT_FOCUS_GAINED: {
				keyboardFocus = true;
				updateTitle = true;
				break;
			}
			case SDL_WINDOWEVENT_FOCUS_LOST: {
				keyboardFocus = false;
				updateTitle = true;
				break;
			}
			case SDL_WINDOWEVENT_MINIMIZED: {
				minimized = true;
				break;
			}
			case SDL_WINDOWEVENT_MAXIMIZED: {
				minimized = false;
				break;
			}
			case SDL_WINDOWEVENT_RESTORED: {
				minimized = false;
				break;
			}
			default: {
				break;
			}
		}
		if (updateTitle) {
			std::stringstream title;
			title << "Window - Mouse Focus: " << (mouseFocus ? "ON" : "OFF") << ", Keyboard Focus: " << (keyboardFocus ? "ON" : "OFF");
			SDL_SetWindowTitle(window, title.str().c_str());
		}
	} else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
		if (fullScreen) {
			SDL_SetWindowFullscreen(window, SDL_FALSE);
			fullScreen = false;
		} else {
			SDL_SetWindowFullscreen(window, SDL_TRUE);
			fullScreen = true;
			minimized = false;
		}
	}
}

void Window::free() {
	SDL_DestroyWindow(window);
	window = nullptr;
}

int Window::getWidth() {
	return width;
}

int Window::getHeight() {
	return height;
}

bool Window::hasMouseFocus() {
	return mouseFocus;
}

bool Window::hasKeyboardFocus() {
	return keyboardFocus;
}

bool Window::isMinimized() {
	return minimized;
}
