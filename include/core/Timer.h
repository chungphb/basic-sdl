#pragma once

#include <SDL.h>

struct Timer {
public:
	Timer();
	void start();
	void stop();
	void pause();
	void unpause();
	Uint32 getTicks();
	bool isStarted();
	bool isPaused();

private:
	Uint32 startTicks;
	Uint32 pausedTicks;
	bool started;
	bool paused;
};