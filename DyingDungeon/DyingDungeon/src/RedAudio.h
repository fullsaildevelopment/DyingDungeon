#pragma once

#include <iostream>
#include <Windows.h>
#include <MMSystem.h>
#include <vector>
#include <time.h>
#include <string>
#include <assert.h>

class RedAudio
{
	private:
		const char* m_path;
		const char* m_alias;
		unsigned int m_start;
		unsigned int m_end;
		unsigned long m_duration;
		bool m_playing;
		bool m_looping;
		bool m_segmented;
	public:
	private:
		const wchar_t* ConvertCharToWChar(const char* p_char);
		//unsigned int CountCharsInInt(int _value);
	public:
		RedAudio(const char* path, const char* alias);
		~RedAudio();
		void Clear();
		/// <summary>
		///		<para><c>Play</c> plays the audio file from it's beginning</para>
		///	</summary>
		void Play();
		/// <summary>
		///		<para><c>PlayLoop</c> plays the audio file from it's beginning, and at it's end it will play again from the begining</para>
		/// </summary>
		void PlayLoop();
		/// <summary>
		/// <para><c>PlaySegment</c> plays the audio file from 'start' to 'end'</para>
		/// </summary>
		/// <param name="start">unsigned intiger value of the start of the segment of the audio file. Has to be less than 'end'</param>
		/// <param name="end">unsigned intiger value of the end of the segment of the audio file. Has to be greater than 'start'</param>
		void PlaySegment(unsigned int start, unsigned int end);
		/// <summary>
		/// <para><c>PlaySegmentLoop</c> plays the audio file from 'start' to 'end', and at 'end' it will play from 'start' again</para>
		/// </summary>
		/// <param name="start"></param>
		/// <param name="end"></param>
		void PlaySegmentLoop(unsigned int start, unsigned int end);
		/// <summary>
		///		<para>PlayInstance</para> plays track under diffrent alias and closes track when done 
		/// </summary>
		void PlayInstance();
		/// <summary>
		///		<para><c>Pause</c> stops the audio file playing and holds it's current place</para>
		/// </summary>
		void Pause();
		/// <summary>
		///		<para><c>Stop</c> stops the audio file playing and sets it back to the beginning of the file</para>
		/// </summary>
		void Stop();
		/// <summary>
		///		<para><c>SeekBegin</c> sets it back to the beginning of the file</para>
		/// </summary>
		void SeekBegin();
		/// <summary>
		///		<para><c>SeekEnd</c> sets it back to the end of the file</para>
		/// </summary>
		void SeekEnd();
		/// <summary>
		/// <para><c>SetVolume</c> sets volume of the audio file</para>
		/// </summary>
		/// <param name="volume">unsigned intiger value of the audio level between 0 and 1000</param>
		void SetVolume(unsigned int volume);
		unsigned int GetVolume();
		/// <summary>
		///		<para><c>Update</c> checks how long the track has played and if not looping changes playing to false</para>
		/// </summary>
		void Update();
		/// <summary>
		///		<para><c>isPlaying</c> checks whether the audio file is playing or not</para>
		/// </summary>
		/// <returns>
		///		<para>&#160;&#160;&#160;&#160;&#160;&#160;Returns true if playing and false if not playing</para>
		///	</returns>
		bool isPlaying();
		/// <summary>
		///		<para><c>isLooping</c> checks weather the audio file is looping or not</para>
		/// </summary>
		/// <returns>
		///		<para>&#160;&#160;&#160;&#160;&#160;&#160;Returns true if playing and false if not playing</para>
		///	</returns>
		bool isLooping();
		/// <summary>
		///		<para><c>Duration</c> returns the duration of the entire audio file</para>
		/// </summary>
		/// <returns>unsigned long value representing the duration of the audio file</returns>
		unsigned long Duration();
		/// <summary>
		///		<para><c>Close</c> closes the audio file</para>
		/// </summary>
		void Close();
		/// <summary>
		///		<para><c>Open</c> opens the audio file for use by other RedAudio functions</para>
		/// </summary>
		void Open();

		/// <summary>
		/// <para><c>SetPath</c> changes the objects address of the audio file</para>
		/// </summary>
		/// <param name="path">quoats("ex") or char array containing the address of the audio file</param>
		void SetPath(const char* path);
		/// <summary>
		/// <para><c>GetPath</c> returns the address of the audio file</para>
		/// </summary>
		/// <returns>Returns a const char* to the address of the audio file</returns>
		const char* GetPath();
		/// <summary>
		/// <para><c>SetAlias</c> changes the objects audio file will be refered as</para>
		/// </summary>
		/// <param name="alias">quoats("ex") or char array containing the name the audio file will be refered by</param>
		void SetAlias(const char* alias);
		/// <summary>
		/// <para><c>GetAlias</c> returns the name by which the objects audio file is refered by</para>
		/// </summary>
		/// <returns>Returns a const char* to the name the audio file is refered by</returns>
		const char* GetAlias();
};

