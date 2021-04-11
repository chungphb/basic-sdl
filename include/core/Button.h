#pragma once

#include <core/Texture.h>
#include <SDL.h>

enum ButtonSprite {
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_TOTAL = 4
};

struct Button {
public:
	Button();
	void setSize(int w, int h);
	void setPosition(int x, int y);
	void handleEvent(SDL_Event* e);
	void render(SDL_Renderer* renderer, Texture* buttonSpriteSheetTexture, SDL_Rect* buttonClips);

private:
	int width;
	int height;
	SDL_Point position;
	ButtonSprite currentSprite;
};