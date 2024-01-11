#pragma once
#include <map>

using std::map;

class SoundEffects {
private:
	map<string, Mix_Chunk*> SoundEffectsMap;
public:
	SoundEffects() 
	{
		SDL_Init(SDL_INIT_AUDIO);

		int audio_rate = 22050;
		Uint16 audio_format = AUDIO_S16SYS;
		int audio_channels = 2;
		int audio_buffers = 4096;

		if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0)
		{
			cout << "Couldn't init audio: " << Mix_GetError() << endl;
		}
	}

	void addSoundEffect(string name, const char* path) {
		Mix_Chunk* tmpChunk = Mix_LoadWAV(path);

		if (tmpChunk != nullptr) {
			SoundEffectsMap.insert(pair<std::string, Mix_Chunk*>(name, tmpChunk));
		}
	}

	void playSoundEffect(string name, int loop) {
		Mix_PlayChannel(-1, SoundEffectsMap.at(name), loop);
	}
};
