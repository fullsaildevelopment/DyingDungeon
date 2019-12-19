#include "RedAudioManager.h"

RedAudioManager* RedAudioManager::m_p_Instance = nullptr;

RedAudio* RedAudioManager::FindAudio(const char* alias)
{
	for (int i = 0; i < m_audioFiles.size(); i++) 
	{
		if (strcmp(m_audioFiles[i].GetAlias(), alias) == 0) 
		{
			return &m_audioFiles[i];
		}
	}
	return default_audio;
}

RedAudioManager* RedAudioManager::Instance()
{
	if (!m_p_Instance) {
		m_p_Instance = new RedAudioManager;
	}
	return m_p_Instance;
}

RedAudioManager::RedAudioManager()
{
	//default_audio = new RedAudio("assets/audio/default_audio.mp3", "DEFAULT");
	//default_audio->Open();
}

//RedAudioManager::RedAudioManager(const char* defult_audio)
//{
//	default_audio->SetPath(defult_audio);
//	default_audio->SetAlias("defualt");
//}

RedAudioManager::~RedAudioManager()
{
	delete default_audio;
	delete m_p_Instance;
	m_audioFiles.clear();
}

void RedAudioManager::Play(const char* alias)
{
	FindAudio(alias)->Play();
}

void RedAudioManager::PlaySFX(const char* alias)
{
	FindAudio(alias)->PlayInstance();
}

void RedAudioManager::Loop(const char* alias)
{
	FindAudio(alias)->PlayLoop();
}

void RedAudioManager::Segment(const char* alias, unsigned int start, unsigned int end)
{
	FindAudio(alias)->PlaySegment(start, end);
}

void RedAudioManager::SetVolume(const char* alias, unsigned int volume)
{
	FindAudio(alias)->SetVolume(volume);
}

void RedAudioManager::Update()
{
	for (int i = 0; i < m_audioFiles.size(); i++) 
	{
		if (m_audioFiles[i].isPlaying() && !m_audioFiles[i].isLooping()) 
		{
			m_audioFiles[i].Update();
		}
	}
}

void RedAudioManager::AddAudio(const char* path, const char* alias)
{
	RedAudio temp = RedAudio(path, alias);
	m_audioFiles.push_back(temp);
	//m_audioFiles[m_audioFiles.size() - 1].Open();
}

void RedAudioManager::AddAudio(RedAudio in_audio)
{
	m_audioFiles.push_back(in_audio);
	//m_audioFiles[m_audioFiles.size() - 1].Open();
}

RedAudio* RedAudioManager::GetAudio(int i)
{
	return &m_audioFiles[i];
}

RedAudio* RedAudioManager::GetAudio(const char* alias)
{
	return FindAudio(alias);
}

void RedAudioManager::SetDefult(const char* path)
{
	default_audio->SetPath(path);
}

unsigned int RedAudioManager::AudioListSize()
{
	return m_audioFiles.size();
}
