#include <core/Window.h>
#include <core/Texture.h>
#include <stdio.h>
#include <sstream>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

struct TestAudioRecording : public Window {
public:
	bool init() override {
		bool success = true;
		if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
			printf("SDL could not initialize! Error: %s\n", SDL_GetError());
			success = false;
		} else {
			if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
				printf("Warning: Linear texture filtering is not enabled!");
			}
			window = SDL_CreateWindow("Test audio recording!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
			if (!window) {
				printf("Window could not be created! Error: %s\n", SDL_GetError());
				success = false;
			} else {
				renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
				if (!renderer) {
					printf("Renderer could not be created! Error: %s\n", SDL_GetError());
					success = false;
				} else {
					SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
					int imgFlags = IMG_INIT_PNG;
					if (!(IMG_Init(imgFlags) & imgFlags)) {
						printf("SDL2_image could not initialize! Error: %s\n", IMG_GetError());
						success = false;
					}
					if (TTF_Init() == -1) {
						printf("SDL_ttf could not initialize! Error: %s\n", TTF_GetError());
						success = false;
					}
				}
			}
		}
		return success;
	}

	bool loadMedia() override {
		bool success = true;
		font = TTF_OpenFont("font/prompt.ttf", 16);
		if (!font) {
			printf("Failed to load \"prompt\" font! Error: %s\n", TTF_GetError());
			success = false;
		} else {
			if (!promptTexture.loadFromRenderedText(renderer, font, "Select your recording device: ", textColor)) {
				printf("Failed to render prompt text texture!\n");
				success = false;
			}
			numRecordingDevices = SDL_GetNumAudioDevices(SDL_TRUE);
			if (numRecordingDevices < 1) {
				printf("Unable to get audio capture device! Error: %s\n", SDL_GetError());
				success = false;
			} else {
				numRecordingDevices = std::min(numRecordingDevices, MAX_RECORDING_DEVICES);
				std::stringstream text;
				for (int i = 0; i < numRecordingDevices; i++) {
					text.str("");
					text << i << ": " << SDL_GetAudioDeviceName(i, SDL_TRUE);
					deviceTextures[i].loadFromRenderedText(renderer, font, text.str().c_str(), textColor);
				}
			}
		}
		return success;
	}

	void run() override {
		bool quit = false;
		SDL_Event e;
		RecordingState recordingState = RecordingState::SELECTING_DEVICE;
		SDL_AudioDeviceID recordingDeviceId = 0;
		SDL_AudioDeviceID playbackDeviceId = 0;
		while (!quit) {
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
				} else {
					switch (recordingState) {
						case RecordingState::SELECTING_DEVICE: {
							if (e.type == SDL_KEYDOWN) {
								if (e.key.keysym.sym >= SDLK_0 && e.key.keysym.sym <= SDLK_9) {
									int id = e.key.keysym.sym - SDLK_0;
									if (id < numRecordingDevices) {
										SDL_AudioSpec desiredRecordingSpec;
										SDL_zero(desiredRecordingSpec);
										desiredRecordingSpec.freq = 44100;
										desiredRecordingSpec.format = AUDIO_F32;
										desiredRecordingSpec.channels = 2;
										desiredRecordingSpec.samples = 4096;
										desiredRecordingSpec.callback = audioRecordingCallback;
										const char* recordingDeviceName = SDL_GetAudioDeviceName(id, SDL_TRUE);

										recordingDeviceId = SDL_OpenAudioDevice(recordingDeviceName, SDL_TRUE, &desiredRecordingSpec, &obtainedRecordingSpec, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
										
										if (recordingDeviceId == 0) {
											printf("Unable to open recording device! Error: %s\n", SDL_GetError());
											promptTexture.loadFromRenderedText(renderer, font, "Failed to open recording device!", textColor);
											recordingState = RecordingState::ERROR;
										} else {
											SDL_AudioSpec desiredPlaybackSpec;
											SDL_zero(desiredPlaybackSpec);
											desiredPlaybackSpec.freq = 44100;
											desiredPlaybackSpec.format = AUDIO_F32;
											desiredPlaybackSpec.channels = 2;
											desiredPlaybackSpec.samples = 4096;
											desiredPlaybackSpec.callback = audioPlaybackCallback;
											
											playbackDeviceId = SDL_OpenAudioDevice(NULL, SDL_FALSE, &desiredPlaybackSpec, &obtainedPlaybackSpec, SDL_AUDIO_ALLOW_FORMAT_CHANGE);
											
											if (playbackDeviceId == 0) {
												printf("Unable to open playback device! Error: %s\n", SDL_GetError());
												promptTexture.loadFromRenderedText(renderer, font, "Failed to open playback device!", textColor);
												recordingState = RecordingState::ERROR;
											} else {
												int bytesPerSample = obtainedRecordingSpec.channels * (SDL_AUDIO_BITSIZE(obtainedRecordingSpec.format) / 8);
												int bytesPerSecond = bytesPerSample * obtainedRecordingSpec.freq;
												
												bufferByteSize = bytesPerSecond * RECORDING_BUFFER_SECONDS;
												bufferByteMaxPosition = bytesPerSecond * MAX_RECORDING_SECONDS;
												recordingBuffer = new Uint8[bufferByteSize];
												memset(recordingBuffer, 0, bufferByteSize);
												
												promptTexture.loadFromRenderedText(renderer, font, "Press 1 to record.", textColor);
												recordingState = RecordingState::STOPPED;
											}
										}
									}
								}
							}
							break;
						}
						case RecordingState::STOPPED: {
							if (e.type == SDL_KEYDOWN) {
								if (e.key.keysym.sym == SDLK_1) {
									bufferBytePosition = 0;
									SDL_PauseAudioDevice(recordingDeviceId, SDL_FALSE);
									promptTexture.loadFromRenderedText(renderer, font, "Recording...", textColor);
									recordingState = RecordingState::RECORDING;
								}
							}
							break;
						}
						case RecordingState::RECORDED: {
							if (e.type == SDL_KEYDOWN) {
								if (e.key.keysym.sym == SDLK_1) {
									bufferBytePosition = 0;
									SDL_PauseAudioDevice(playbackDeviceId, SDL_FALSE);
									promptTexture.loadFromRenderedText(renderer, font, "Playing...", textColor);
									recordingState = RecordingState::PLAYBACK;
								} else if (e.key.keysym.sym == SDLK_2) {
									bufferBytePosition = 0;
									memset(recordingBuffer, 0, bufferByteSize);
									SDL_PauseAudioDevice(recordingDeviceId, SDL_FALSE);
									promptTexture.loadFromRenderedText(renderer, font, "Recording...", textColor);
									recordingState = RecordingState::RECORDING;
								}
							}
							break;
						}
						default: {
							break;
						}
					}

					if (recordingState == RecordingState::RECORDING) {
						SDL_LockAudioDevice(recordingDeviceId);
						if (bufferBytePosition > bufferByteMaxPosition) {
							SDL_PauseAudioDevice(recordingDeviceId, SDL_TRUE);
							promptTexture.loadFromRenderedText(renderer, font, "Press 1 to play back. Press 2 to re-record.", textColor);
							recordingState = RecordingState::RECORDED;
						}
						SDL_UnlockAudioDevice(recordingDeviceId);
					} else if (recordingState == RecordingState::PLAYBACK) {
						SDL_LockAudioDevice(playbackDeviceId);
						if (bufferBytePosition > bufferByteMaxPosition) {
							SDL_PauseAudioDevice(playbackDeviceId, SDL_TRUE);
							promptTexture.loadFromRenderedText(renderer, font, "Press 1 to play back. Press 2 to re-record.", textColor);
							recordingState = RecordingState::RECORDED;
						}
						SDL_UnlockAudioDevice(playbackDeviceId);
					}
				}
			}
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
			SDL_RenderClear(renderer);
			promptTexture.render(renderer, 0, 0);
			if (recordingState == RecordingState::SELECTING_DEVICE) {
				int heightOffset = promptTexture.getHeight() * 2;
				for (int i = 0; i < numRecordingDevices; i++) {
					deviceTextures[i].render(renderer, 0, heightOffset);
					heightOffset += deviceTextures[i].getHeight() + 1;
				}
			}
			SDL_RenderPresent(renderer);
		}
	}

	void close() override {
		if (!recordingBuffer) {
			delete[] recordingBuffer;
			recordingBuffer = nullptr;
		}
		promptTexture.free();
		for (int i = 0; i < numRecordingDevices; i++) {
			deviceTextures[i].free();
		}
		TTF_CloseFont(font);
		font = nullptr;
		SDL_DestroyRenderer(renderer);
		renderer = nullptr;
		SDL_DestroyWindow(window);
		window = nullptr;
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();
	}

private:
	static void audioRecordingCallback(void* userData, Uint8* stream, int len) {
		memcpy(&recordingBuffer[bufferBytePosition], stream, len);
		bufferBytePosition += len;
	}

	static void audioPlaybackCallback(void* userData, Uint8* stream, int len) {
		memcpy(stream, &recordingBuffer[bufferBytePosition], len);
		bufferBytePosition += len;
	}

private:
	static constexpr int MAX_RECORDING_DEVICES = 4;
	static constexpr int MAX_RECORDING_SECONDS = 4;
	static constexpr int RECORDING_BUFFER_SECONDS = MAX_RECORDING_SECONDS + 1;
	enum struct RecordingState : Uint8 {
		SELECTING_DEVICE = 0,
		STOPPED = 1,
		RECORDING = 2,
		RECORDED = 3,
		PLAYBACK = 4,
		ERROR = 5
	};

	SDL_Renderer* renderer = nullptr;
	TTF_Font* font = nullptr;
	SDL_Color textColor{0xFF, 0xFF, 0xFF};

	Texture promptTexture;
	Texture deviceTextures[MAX_RECORDING_DEVICES];

	int numRecordingDevices;

	SDL_AudioSpec obtainedRecordingSpec;
	SDL_AudioSpec obtainedPlaybackSpec;

	static Uint8* recordingBuffer;
	static Uint32 bufferByteSize;
	static Uint32 bufferBytePosition;
	static Uint32 bufferByteMaxPosition;
};

Uint8* TestAudioRecording::recordingBuffer = nullptr;

Uint32 TestAudioRecording::bufferByteSize = 0;

Uint32 TestAudioRecording::bufferBytePosition = 0;

Uint32 TestAudioRecording::bufferByteMaxPosition = 0;


int main(int argc, char** argv) {
	TestAudioRecording mainWindow;
	if (!mainWindow.init()) {
		printf("Failed to initialize!\n");
	} else {
		if (!mainWindow.loadMedia()) {
			printf("Failed to load media!\n");
		} else {
			mainWindow.run();
		}
	}
	mainWindow.close();
	return 0;
}