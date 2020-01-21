#pragma once
#include "RedAudio.h"
#include <vector>
#include "Event.h"
#include "EventManager.h"

class AudioStopEvent : public Odyssey::Event
{
public:
	std::string alias;
	AudioStopEvent(std::string audio_alias)
	{
		alias = audio_alias;
		priority = Odyssey::EventPriority::Deferred;
	}
};

class AudioVolumeEvent : public Odyssey::Event
{
public:
	unsigned int volumeLevel;
	AudioVolumeEvent(unsigned int volume)
	{
		volumeLevel = volume;
		priority = Odyssey::EventPriority::Deferred;
	}
};

class AudioLoopEvent : public Odyssey::Event
{
public:
	std::string alias;
	AudioLoopEvent(std::string audio_alias)
	{
		alias = audio_alias;
		priority = Odyssey::EventPriority::Deferred;
	}

};

class RedAudioManager
{
	public:
		enum class AudioType { Background = 0, SFX };
	private:
		static std::vector<RedAudio> m_audioFiles;
		RedAudio* m_default_audio;
		unsigned int m_volume;
	private:
		RedAudio* FindAudio(const char* alias);
		void StopEvent(AudioStopEvent* asEvent);
		//static RedAudioManager* m_p_Instance;
		
		RedAudioManager();
	public:
		static RedAudioManager& Instance();
		~RedAudioManager();
		/// <summary>
		/// 
		/// </summary>
		/// <param name="alias"></param>
		void Play(const char* alias);
		void Stop(const char* alias);
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
		void LoopEvent(AudioLoopEvent* alEvent);
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
		bool SetMasterVolume(const char* alias, unsigned int volume);
		bool SetMasterVolume(unsigned int volume);
		void SetVolumeEvent(AudioVolumeEvent* avEvent);

		unsigned int GetVolume();
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

