#include <core/Texture.h>
#include <stdio.h>

Texture::Texture() {
	texture = nullptr;
	width = 0;
	height = 0;
}

Texture::~Texture() {
	free();
}

bool Texture::loadFromFile(SDL_Renderer* renderer, std::string path) {
	free();
	SDL_Texture* newTexture = nullptr;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (!loadedSurface) {
		printf("Unable to load image %s! Error: %s\n", path.c_str(), IMG_GetError());
	} else {
		SDL_SetColorKey(loadedSurface, true, SDL_MapRGB(loadedSurface->format, 0x00, 0xFF, 0xFF));
		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (!newTexture) {
			printf("Unable to create texture from %s! Error: %s\n", path.c_str(), SDL_GetError());
		} else {
			width = loadedSurface->w;
			height = loadedSurface->h;
		}
		SDL_FreeSurface(loadedSurface);
	}
	texture = newTexture;
	return texture != nullptr;
}

bool Texture::loadFromRenderedText(SDL_Renderer* renderer, TTF_Font* font, std::string textureText, SDL_Color textColor) {
	free();
	SDL_Texture* newTexture = nullptr;
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
	if (!textSurface) {
		printf("Unable to render text surface! Error: %s\n", TTF_GetError());
	} else {
		newTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (!newTexture) {
			printf("Unable to create texture from rendered text! Error: %s\n", SDL_GetError());
		} else {
			width = textSurface->w;
			height = textSurface->h;
		}
		SDL_FreeSurface(textSurface);
	}
	texture = newTexture;
	return texture != nullptr;
}

void Texture::setColor(Uint8 r, Uint8 g, Uint8 b) {
	SDL_SetTextureColorMod(texture, r, g, b);
}

void Texture::setBlendMode(SDL_BlendMode blendMode) {
	SDL_SetTextureBlendMode(texture, blendMode);
}

void Texture::setAlpha(Uint8 alpha) {
	SDL_SetTextureAlphaMod(texture, alpha);
}

void Texture::free() {
	if (texture) {
		SDL_DestroyTexture(texture);
		texture = nullptr;
		width = 0;
		height = 0;
	}
}

void Texture::render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip) {
	SDL_Rect renderQuad{x, y, width, height};
	if (clip) {
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}
	SDL_RenderCopyEx(renderer, texture, clip, &renderQuad, angle, center, flip);
}

int Texture::getWidth() {
	return width;
}

int Texture::getHeight() {
	return height;
}