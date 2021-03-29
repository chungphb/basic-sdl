#pragma once

#include <SDL.h>

struct Window {
public:
	virtual bool init() = 0;
	
	virtual bool loadMedia() = 0;
	
	virtual void run() = 0;
	
	virtual void close() = 0;

protected:
	SDL_Window* window = nullptr;
};