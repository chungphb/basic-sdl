#include <core/Button.h>

Button::Button() {
	width = 0;
	height = 0;
	position.x = 0;
	position.y = 0;
	currentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

void Button::setSize(int w, int h) {
	width = w;
	height = h;
}

void Button::setPosition(int x, int y) {
	position.x = x;
	position.y = y;
}

void Button::handleEvent(SDL_Event* e) {
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP) {
		int x, y;
		SDL_GetMouseState(&x, &y);
		bool inside = true;
		if (x < position.x) {
			inside = false;
		} else if (x > position.x + width) {
			inside = false;
		} else if (y < position.y) {
			inside = false;
		} else if (y > position.y + height) {
			inside = false;
		}
		if (!inside) {
			currentSprite = BUTTON_SPRITE_MOUSE_OUT;
		} else {
			switch (e->type) {
				case SDL_MOUSEMOTION: {
					currentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
					break;
				}
				case SDL_MOUSEBUTTONDOWN: {
					currentSprite = BUTTON_SPRITE_MOUSE_DOWN;
					break;
				}
				case SDL_MOUSEBUTTONUP: {
					currentSprite = BUTTON_SPRITE_MOUSE_UP;
					break;
				}
				default: {
					break;
				}
			}
		}
	}
}

void Button::render(SDL_Renderer* renderer, Texture* buttonSpriteSheetTexture, SDL_Rect* buttonClips) {
	buttonSpriteSheetTexture->render(renderer, position.x, position.y, &buttonClips[currentSprite]);
}