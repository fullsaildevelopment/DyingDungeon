#include "RedAudioManager.h"

std::vector<RedAudio> RedAudioManager::m_audioFiles;

RedAudio* RedAudioManager::FindAudio(const char* alias)
{
	for (int i = 0; i < m_audioFiles.size(); i++) 
	{
		if (strcmp(m_audioFiles[i].GetAlias(), alias) == 0) 
		{
			return &m_audioFiles[i];
		}
	}
	return m_default_audio;
}

RedAudioManager& RedAudioManager::Instance()
{
	static RedAudioManager instance_audio_manager;
	return instance_audio_manager;
}

RedAudioManager::RedAudioManager()
{
	//default_audio = nullptr;
	Odyssey::EventManager::getInstance().subscribe(this, &RedAudioManager::StopEvent);
	Odyssey::EventManager::getInstance().subscribe(this, &RedAudioManager::SetVolumeEvent);
	Odyssey::EventManager::getInstance().subscribe(this, &RedAudioManager::LoopEvent);
	m_volume = 500;
	m_default_audio = new RedAudio("assets/audio/wheres_the_lamb_sauce.mp3", "DEFAULT");
	m_default_audio->Open();
}

//RedAudioManager::RedAudioManager(const char* defult_audio)
//{
//	default_audio->SetPath(defult_audio);
//	default_audio->SetAlias("defualt");
//}

RedAudioManager::~RedAudioManager()
{
	//if (default_audio) {
	//default_audio->Clear();
	delete m_default_audio;
	//}
	/*for (int i = 0; i < m_audioFiles.size(); i++) {
		m_audioFiles[i].Clear();
	}*/
	m_audioFiles.clear();
}

void RedAudioManager::Play(const char* alias)
{
	FindAudio(alias)->Play();
}

void RedAudioManager::StopEvent(AudioStopEvent* asEvent)
{
	//Stop(asEvent->alias.c_str());
	FindAudio(asEvent->alias.c_str())->Stop();
}

void RedAudioManager::Stop(const char* alias) 
{
	Odyssey::EventManager::getInstance().publish(new AudioStopEvent(alias));
}

void RedAudioManager::PlaySFX(const char* alias)
{
	FindAudio(alias)->PlayInstance();
}

void RedAudioManager::Loop(const char* alias)
{
	Odyssey::EventManager::getInstance().publish(new AudioLoopEvent(alias));
}

void RedAudioManager::LoopEvent(AudioLoopEvent* alEvent)
{
	FindAudio(alEvent->alias.c_str())->PlayLoop();
}

void RedAudioManager::Segment(const char* alias, unsigned int start, unsigned int end)
{
	FindAudio(alias)->PlaySegment(start, end);
}

bool RedAudioManager::SetMasterVolume(const char* alias, unsigned int volume)
{
	if (volume > 1000)
	{
		return false;
	}
	else 
	{
		FindAudio(alias)->SetVolume(volume);
		return true;
	}
}

bool RedAudioManager::SetMasterVolume(unsigned int volume)
{
	if (volume > 1000) 
	{
		return false;
	}
	m_volume = volume;
	Odyssey::EventManager::getInstance().publish(new AudioVolumeEvent(volume));
	return true;
}

void RedAudioManager::SetVolumeEvent(AudioVolumeEvent* avEvent)
{
	m_volume = avEvent->volumeLevel;
	
	for (int i = 0; i < m_audioFiles.size(); i++)
	{
		m_audioFiles[i].SetVolume(m_volume);
	}
}

unsigned int RedAudioManager::GetVolume()
{
	return m_volume;
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
	m_audioFiles.emplace_back(RedAudio(path, alias));
	m_audioFiles[m_audioFiles.size() - 1].SetVolume(m_volume);
}

void RedAudioManager::AddAudio(RedAudio in_audio)
{
	m_audioFiles.emplace_back(in_audio);
	m_audioFiles[m_audioFiles.size() - 1].SetVolume(m_volume);
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
	m_default_audio->SetPath(path);
	m_default_audio->SetPath("DEFAULT");
	m_default_audio->Open();
	m_default_audio->SetVolume(m_volume);
}

unsigned int RedAudioManager::AudioListSize()
{
	return m_audioFiles.size();
}
