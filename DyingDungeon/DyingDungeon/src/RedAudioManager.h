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
		/// <summary>
		/// 
		/// </summary>
		/// <param name="alias"></param>
		void Play(const char* alias);
		/// <summary>
		/// 
		/// </summary>
		/// <param name="alias"></param>
		void PlaySFX(const char* alias);
		/// <summary>
		/// 
		/// </summary>
		/// <param name="alias"></param>
		void Loop(const char* alias);
		/// <summary>
		/// 
		/// </summary>
		/// <param name="alias"></param>
		/// <param name="start"></param>
		/// <param name="end"></param>
		void Segment(const char* alias, unsigned int start, unsigned int end);
		/// <summary>
		/// 
		/// </summary>
		/// <param name="alias"></param>
		/// <param name="volume"></param>
		void SetVolume(const char* alias, unsigned int volume);
		/// <summary>
		/// 
		/// </summary>
		void Update();
		/// <summary>
		/// 
		/// </summary>
		/// <param name="path"></param>
		/// <param name="alias"></param>
		void AddAudio(const char* path, const char* alias);
		/// <summary>
		/// 
		/// </summary>
		/// <param name="in_audio"></param>
		void AddAudio(RedAudio in_audio);
		/// <summary>
		/// 
		/// </summary>
		/// <param name="i"></param>
		/// <returns></returns>
		RedAudio* GetAudio(int i);
		/// <summary>
		/// 
		/// </summary>
		/// <param name="alias"></param>
		/// <returns></returns>
		RedAudio* GetAudio(const char* alias);
		/// <summary>
		/// 
		/// </summary>
		/// <param name="path"></param>
		void SetDefult(const char* path);
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		unsigned int AudioListSize();
};

