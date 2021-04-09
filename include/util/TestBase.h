#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

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
	
};

struct BasicTestBase : public TestBase {
public:
	virtual bool init();

	virtual bool loadMedia();

	virtual void run();

	virtual void close();

	virtual std::string name();

protected:
	SDL_Renderer* renderer = nullptr;
};

struct BasicTestBaseWithTTF : public BasicTestBase {
public:
	virtual bool init();

	virtual void close();

	virtual std::string name();
};

struct BasicTestBaseWithAudio : public BasicTestBase {
public:
	virtual bool init();

	virtual std::string name();
};


struct BasicTestBaseWithTTFAndAudio : public BasicTestBaseWithAudio {
public:
	virtual bool init();

	virtual void close();

	virtual std::string name();
};

struct BasicTestBaseWithController : public BasicTestBase {
public:
	virtual bool init();

	virtual void close();

	virtual std::string name();

protected:
	SDL_Joystick* gameController = nullptr;
	SDL_Haptic* controllerHaptic = nullptr;
};