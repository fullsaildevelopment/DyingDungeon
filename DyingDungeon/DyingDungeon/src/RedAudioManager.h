#pragma once
#include "RedAudio.h"
#include <vector>
class RedAudioManager
{
	private:
		std::vector<RedAudio> m_audioFiles;
		RedAudio* default_audio;
	public:
	private:
		RedAudio* FindAudio(const char* alias);
		static RedAudioManager* m_p_Instance;
		//RedAudioManager(const char* defult_audio);
		RedAudioManager();
	public:
		static RedAudioManager* Instance();
		~RedAudioManager();
		void Play(const char* alias);
		void Loop(const char* alias);
		void Segment(const char* alias, unsigned int start, unsigned int end);
		void SetVolume(const char* alias, unsigned int volume);
		void Update();
		void AddAudio(const char* path, const char* alias);
		void AddAudio(RedAudio in_audio);
		RedAudio* GetAudio(int i);
		RedAudio* GetAudio(const char* alias);
		void SetDefult(const char* path);
		unsigned int AudioListSize();
};

