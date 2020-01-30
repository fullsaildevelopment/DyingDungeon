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
		bool m_muted;
	private:
		RedAudio* FindAudio(const char* alias);
		void StopEvent(AudioStopEvent* asEvent);
		//static RedAudioManager* m_p_Instance;
		
		RedAudioManager();
	public:
		static RedAudioManager& Instance();
		~RedAudioManager();
		/// <summary>
		/// Plays the 'alias' audio file that is stored in the manager
		/// </summary>
		/// <param name="alias">Name the audio file is stored as in the manager</param>
		void Play(const char* alias);
		/// <summary>
		/// Stops the 'alias' audio file that is stored in the manager
		/// </summary>
		/// <param name="alias">Name the audio file is stored as in the manager</param>
		void Stop(const char* alias);
		/// <summary>
		/// Plays an instance of 'alias' audio file
		/// </summary>
		/// <param name="alias">Name the audio file is stored as in the manager</param>
		void PlaySFX(const char* alias);
		/// <summary>
		/// Plays the 'alias' audio file in a loop
		/// </summary>
		/// <param name="alias">Name the audio file is stored as in the manager</param>
		void Loop(const char* alias);
		/// <summary>
		/// Listenner function for a Audio Loop event
		/// </summary>
		/// <param name="alEvent"></param>
		void LoopEvent(AudioLoopEvent* alEvent);
		/// <summary>
		/// Plays segment of 'alias' audio file
		/// </summary>
		/// <param name="alias">Name the audio file is stored as in the manager</param>
		/// <param name="start">Time stump the audio track starts from</param>
		/// <param name="end">Time stump the audio track ends at</param>
		void Segment(const char* alias, unsigned int start, unsigned int end);
		/// <summary>
		/// Setter for the volume of 'alias' audio file
		/// </summary>
		/// <param name="alias">Name the audio file is stored as in the manager</param>
		/// <param name="volume">Volume value between 0 and 1000</param>
		bool SetMasterVolume(const char* alias, unsigned int volume);
		/// <summary>
		/// Setter for the volume of audio files in game
		/// </summary>
		/// <param name="volume"></param>
		/// <returns></returns>
		bool SetMasterVolume(unsigned int volume);
		/// <summary>
		/// Reduces all auido to 0
		/// </summary>
		void Mute();
		/// <summary>
		/// Restors all audio to master volume value
		/// </summary>
		void Unmute();
		/// <summary>
		/// Listenner function for a Audio Volume event
		/// </summary>
		/// <param name="avEvent"></param>
		void SetVolumeEvent(AudioVolumeEvent* avEvent);
		/// <summary>
		/// Tells weather audio is muted
		/// </summary>
		/// <returns>bool of wheatehr the audio is muted or not</returns>
		bool isMuted();
		/// <summary>
		/// Getter for the master volume
		/// </summary>
		/// <returns>The current value of the master volume</returns>
		unsigned int GetVolume();
		/// <summary>
		/// Update call made to check the state of playing audio tracks
		/// </summary>
		//void Update();
		/// <summary>
		/// Adds audio file to the manager
		/// </summary>
		/// <param name="path">Path to audio file in system</param>
		/// <param name="alias">Name the audio file is stored as in the manager</param>
		void AddAudio(const char* path, const char* alias);
		/// <summary>
		/// Adds audio file to the manager
		/// </summary>
		/// <param name="in_audio">Audio object to be added</param>
		void AddAudio(RedAudio in_audio);
		/// <summary>
		/// Getter for audio files stored in the manager
		/// </summary>
		/// <param name="i">index of audio object</param>
		/// <returns>Pointer to requested audio object if found</returns>
		RedAudio* GetAudio(int i);
		/// <summary>
		/// Getter for audio stored in the manager
		/// </summary>
		/// <param name="alias">Name the audio file is stored as in the manager</param>
		/// <returns>Pointer to requested audio object if found</returns>
		RedAudio* GetAudio(const char* alias);
		/// <summary>
		/// Sets the default audio used if an audio file is not found
		/// </summary>
		/// <param name="path">Path to audio file in system</param>
		void SetDefult(const char* path);
		/// <summary>
		/// Getter for the size for the container holding the audio files
		/// </summary>
		/// <returns>Size of containter holding the audio objects</returns>
		unsigned int AudioListSize();
};

