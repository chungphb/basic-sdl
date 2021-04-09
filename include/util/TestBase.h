#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>

struct TestBase {
public:
	virtual bool init() = 0;
	
	virtual bool loadMedia() = 0;
	
	virtual void run() = 0;
	
	virtual void close() = 0;

	virtual std::string name();

	virtual void test();

protected:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
};