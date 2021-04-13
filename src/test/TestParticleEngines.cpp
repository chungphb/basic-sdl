#include <util/TestBase.h>
#include <core/Texture.h>
#include <stdio.h>
#include <vector>
#include <math.h>

struct TestParticleEngines;

struct Particle {
public:
	Particle(TestParticleEngines* window, int x, int y);
	void render(SDL_Renderer* renderer);
	bool isDead();

private:
	TestParticleEngines* mainWindow;
	int posX, posY;
	int frame;
	Texture* texture = nullptr;
};

struct Dot {
public:
	Dot(TestParticleEngines* window);
	~Dot();
	void handleEvent(SDL_Event& e);
	void move(SDL_Rect& wall);
	void render(SDL_Renderer* renderer, Texture* dotTexture);

private:
	static constexpr int DOT_WIDTH = 20;
	static constexpr int DOT_HEIGHT = 20;
	static constexpr int DOT_VEL = 10;
	static constexpr int NUM_PARTICLES = 16;
	TestParticleEngines* mainWindow;
	int posX, posY;
	int velX, velY;
	Particle* particles[NUM_PARTICLES];

};

struct TestParticleEngines : public BasicTestBase {
public:
	bool loadMedia() override {
		bool success = true;

		if (!dotTexture.loadFromFile(renderer, "image/red_dot.png")) {
			printf("Failed to load \"red_dot\" texture image!\n");
			success = false;
		}

		for (int i = 0; i < NUM_PARTICLE_TYPES; i++) {
			std::string path = "image/particle_" + std::to_string(i) + ".png";
			if (!particleTextures[i].loadFromFile(renderer, path)) {
				printf("Failed to load \"particle_%d\" texture image!\n", i);
				success = false;
			} else {
				particleTextures[i].setAlpha(192);
			}
		}

		if (!shimmerTexture.loadFromFile(renderer, "image/shimmer.png")) {
			printf("Failed to load \"shimmer\" texture image!\n");
			success = false;
		} else {
			shimmerTexture.setAlpha(192);
		}

		return success;
	}

	void run() override {
		bool quit = false;
		SDL_Event e;
		Dot dot{this};
		SDL_Rect wall;
		while (!quit) {
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				} else {
					dot.handleEvent(e);
				}
			}
			dot.move(wall);
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
			SDL_RenderClear(renderer);
			dot.render(renderer, &dotTexture);
			SDL_RenderPresent(renderer);
		}
	}

	void close() override {
		dotTexture.free();
		for (int i = 0; i < NUM_PARTICLE_TYPES; i++) {
			particleTextures[i].free();
		}
		shimmerTexture.free();
		BasicTestBase::close();
	}

	std::string name() override {
		return "Test Particle Engines";
	}

public:
	static constexpr int NUM_PARTICLE_TYPES = 3;
	Texture particleTextures[NUM_PARTICLE_TYPES];
	Texture shimmerTexture;

private:
	Texture dotTexture;
};

Particle::Particle(TestParticleEngines* window, int x, int y) {
	mainWindow = window;
	posX = (x - 5) + (rand() % 25);
	posY = (y - 5) + (rand() % 25);
	frame = rand() % 5;
	texture = &mainWindow->particleTextures[rand() % mainWindow->NUM_PARTICLE_TYPES];
}

void Particle::render(SDL_Renderer* renderer) {
	texture->render(renderer, posX, posY);
	if (frame % 2 == 0) {
		mainWindow->shimmerTexture.render(renderer, posX, posY);
	}
	frame++;
}

bool Particle::isDead() {
	return frame > 10;
}

Dot::Dot(TestParticleEngines* window) {
	mainWindow = window;
	posX = 0;
	posY = 0;
	velX = 0;
	velY = 0;
	for (int i = 0; i < NUM_PARTICLES; i++) {
		particles[i] = new Particle(mainWindow, posX, posY);
	}
}

Dot::~Dot() {
	for (int i = 0; i < NUM_PARTICLES; i++) {
		delete particles[i];
	}
}

void Dot::handleEvent(SDL_Event& e) {
	if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {
		switch (e.key.keysym.sym) {
			case SDLK_UP: {
				velY -= DOT_VEL; break;
			}
			case SDLK_DOWN: {
				velY += DOT_VEL; break;
			}
			case SDLK_LEFT: {
				velX -= DOT_VEL; break;
			}
			case SDLK_RIGHT: {
				velX += DOT_VEL; break;
			}
			default: {
				break;
			}
		}
	} else if (e.type == SDL_KEYUP && e.key.repeat == 0) {
		switch (e.key.keysym.sym) {
			case SDLK_UP: {
				velY += DOT_VEL; break;
			}
			case SDLK_DOWN: {
				velY -= DOT_VEL; break;
			}
			case SDLK_LEFT: {
				velX += DOT_VEL; break;
			}
			case SDLK_RIGHT: {
				velX -= DOT_VEL; break;
			}
			default: {
				break;
			}
		}
	}
}

void Dot::move(SDL_Rect& wall) {
	posX += velX;
	if (posX < 0 || posX + DOT_WIDTH > WINDOW_WIDTH) {
		posX -= velX;
	}
	posY += velY;
	if (posY < 0 || posY + DOT_HEIGHT > WINDOW_HEIGHT) {
		posY -= velY;
	}
}

void Dot::render(SDL_Renderer* renderer, Texture* dotTexture) {
	dotTexture->render(renderer, posX, posY);
	for (int i = 0; i < NUM_PARTICLES; i++) {
		if (particles[i]->isDead()) {
			delete particles[i];
			particles[i] = new Particle(mainWindow, posX, posY);
		}
	}
	for (int i = 0; i < NUM_PARTICLES; i++) {
		particles[i]->render(renderer);
	}
}

int main(int argc, char** argv) {
	TestParticleEngines mainWindow;
	mainWindow.test();
	return 0;
}