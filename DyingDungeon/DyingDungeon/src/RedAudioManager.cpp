#include "RedAudioManager.h"

std::vector<std::pair<RedAudio, bool>> RedAudioManager::m_audioFiles;

RedAudio* RedAudioManager::FindAudio(const char* alias)
{
	for (int i = 0; i < m_audioFiles.size(); i++) 
	{
		if (strcmp(m_audioFiles[i].first.GetAlias(), alias) == 0) 
		{
			return &m_audioFiles[i].first;
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
	m_volume[0] = 500;
	m_volume[1] = 1000;
	m_volume[2] = 1000;
	m_volume[3] = 1000;
	m_muted = false;
	m_default_audio = new RedAudio("assets/audio/SFX/wheres_the_lamb_sauce.mp3", "DEFAULT");
	m_default_audio->Open();
}

bool RedAudioManager::isMuted()
{
	return m_muted;
}

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

void RedAudioManager::StopGroup(std::string group)
{
	std::vector<std::string> _temp = GetGroup(group);
	for(int i = 0; i < _temp.size(); i++)
	{
		Stop(GetGroup(group)[i].c_str());
	}
}

void RedAudioManager::PlaySFX(const char* alias)
{
	FindAudio(alias)->PlayInstance();
}

void RedAudioManager::PlaySFX(const char* alias, unsigned int volume, bool ignore_master_volume)
{
	FindAudio(alias)->PlayInstance(((ignore_master_volume) ? static_cast<unsigned int>(volume * static_cast<float>(m_volume[0] / 1000.0f)) : volume));
}

void RedAudioManager::Loop(const char* alias)
{
	Odyssey::EventManager::getInstance().publish(new AudioLoopEvent(alias));
}

void RedAudioManager::LoopRandom(std::string group)
{
	std::vector<std::string> _temp = GetGroup(group);
	srand(time(NULL));
	unsigned int index = rand() % (_temp.size());
	Odyssey::EventManager::getInstance().publish(new AudioLoopEvent(_temp[index]));
}

void RedAudioManager::LoopEvent(AudioLoopEvent* alEvent)
{
	FindAudio(alEvent->alias.c_str())->PlayLoop();
}

void RedAudioManager::Segment(const char* alias, unsigned int start, unsigned int end)
{
	FindAudio(alias)->PlaySegment(start, end);
}

bool RedAudioManager::SetVolume(const char* alias, unsigned int volume)
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

bool RedAudioManager::SetMasterVolume(unsigned int volume, AudioType audio_type)
{
	if (volume > 1000) 
	{
		return false;
	}
	if (!m_muted) {
		m_volume[int(audio_type) + 1] = volume;
	}
	Odyssey::EventManager::getInstance().publish(new AudioVolumeEvent(volume, int(audio_type)));
	return true;
}

void RedAudioManager::Mute()
{
	m_muted = true;
	Odyssey::EventManager::getInstance().publish(new AudioVolumeEvent(0));
}

void RedAudioManager::Unmute()
{
	m_muted = false;

	Odyssey::EventManager::getInstance().publish(new AudioVolumeEvent(m_volume[1], int(AudioType::Background)));
	Odyssey::EventManager::getInstance().publish(new AudioVolumeEvent(m_volume[2], int(AudioType::SFX)));
	Odyssey::EventManager::getInstance().publish(new AudioVolumeEvent(m_volume[3], int(AudioType::Dialog)));
}

void RedAudioManager::SetVolumeEvent(AudioVolumeEvent* avEvent)
{
	if (!m_muted) {
		m_volume[avEvent->audioType + 1] = avEvent->volumeLevel;
	}
	
	for (int i = 0; i < m_audioFiles.size(); i++)
	{
		if (m_audioIdentifiers[i].first == AudioType(avEvent->audioType)) 
		{
			m_audioFiles[i].first.SetVolume(static_cast<unsigned int>(avEvent->volumeLevel * static_cast<float>(m_volume[0]/1000.0f)));
		}
		else if (AudioType(avEvent->audioType) == AudioType::None)
		{
			m_audioFiles[i].first.SetVolume(static_cast<unsigned int>(avEvent->volumeLevel * static_cast<float>(m_volume[0] / 1000.0f)));
		}
	}
}

unsigned int RedAudioManager::GetVolume(AudioType audio_type)
{
	return m_volume[int(audio_type) + 1];
}

void RedAudioManager::AddAudio(const char* path, const char* alias, AudioType audio_type, std::string group)
{
	m_audioFiles.emplace_back(std::make_pair(RedAudio(path, alias), false));
	m_audioIdentifiers.emplace_back(std::make_pair(audio_type, group));
	m_audioFiles[m_audioFiles.size() - 1].first.Open();
	m_audioFiles[m_audioFiles.size() - 1].first.SetVolume(static_cast<unsigned int>(m_volume[int(audio_type) + 1] * static_cast<float>(m_volume[0]/1000.0f)));
}

void RedAudioManager::AddAudio(RedAudio in_audio, AudioType audio_type, std::string group)
{
	m_audioFiles.emplace_back(std::make_pair(in_audio, false));
	m_audioIdentifiers.emplace_back(std::make_pair(audio_type, group));
	m_audioFiles[m_audioFiles.size() - 1].first.Open();
	m_audioFiles[m_audioFiles.size() - 1].first.SetVolume(static_cast<unsigned int>(m_volume[int(audio_type) + 1] * static_cast<float>(m_volume[0] / 1000.0f)));
}

RedAudio* RedAudioManager::GetAudio(int i)
{
	return &m_audioFiles[i].first;
}

RedAudio* RedAudioManager::GetAudio(const char* alias)
{
	return FindAudio(alias);
}

std::vector<std::string> RedAudioManager::GetGroup(std::string group)
{
	std::vector<std::string> clips;
	for (int i = 0; i < m_audioIdentifiers.size(); i++)
	{
		if (m_audioIdentifiers[i].second == group) {
			clips.push_back(m_audioFiles[i].first.GetAlias());
		}
	}
	return clips;
}

size_t RedAudioManager::GetGroupCount(std::string group)
{
	return GetGroup(group).size();
}

void RedAudioManager::SetDefult(const char* path)
{
	m_default_audio->SetPath(path);
	m_default_audio->SetPath("DEFAULT");
	m_default_audio->Open();
	m_default_audio->SetVolume(m_volume[0]);
}

unsigned int RedAudioManager::AudioListSize()
{
	return m_audioFiles.size();
}