#include <core/Timer.h>

Timer::Timer() {
	startTicks = 0;
	pausedTicks = 0;
	started = false;
	paused = false;
}

void Timer::start() {
	startTicks = SDL_GetTicks();
	pausedTicks = 0;
	started = true;
	paused = false;
}

void Timer::stop() {
	startTicks = 0;
	pausedTicks = 0;
	started = false;
	paused = false;
}

void Timer::pause() {
	if (started && !paused) {
		startTicks = 0;
		pausedTicks = SDL_GetTicks() - startTicks;
		paused = true;
	}
}

void Timer::unpause() {
	if (started && paused) {
		startTicks = SDL_GetTicks() - pausedTicks;
		pausedTicks = 0;
		paused = false;
	}
}

Uint32 Timer::getTicks() {
	Uint32 time = 0;
	if (started) {
		if (paused) {
			time = pausedTicks;
		} else {
			time = SDL_GetTicks() - startTicks;
		}
	}
	return time;
}

bool Timer::isStarted() {
	return started;
}

bool Timer::isPaused() {
	return started && paused;
}