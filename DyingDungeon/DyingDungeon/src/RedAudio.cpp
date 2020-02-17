#include "RedAudio.h"

//#define DEBUG_AUDIO_CONSOLE_OUT

RedAudio::RedAudio(const char* path, const char* alias)
{
	m_path = path;
	m_alias = alias;
	m_playing = false;
	m_looping = false;
	m_segmented = false;
	//m_personal_volume = false;
	m_start = 0;
	m_volume = 500;

	Open();
	SetVolume(m_volume);
	std::string cmd = "status ";
	int curr_count = 8;

//	LPTSTR out_string = LPTSTR(new char[15]);
//
//	cmd.append(m_alias);
//	cmd.append(" length");
//
//	const wchar_t* in_cmd = ConvertCharToWChar(cmd.c_str());
//	mciSendString(in_cmd, out_string, 15, NULL);
//	if (strcmp((const char*)out_string, "") != 0) {
//		m_end = m_duration = std::stoul(out_string, nullptr, 10);
//	}
//#ifdef DEBUG_AUDIO_CONSOLE_OUT
//	else {
//		std::cout << "ERROR: RedAudio-RedAudio()-Could not get duration/end of song-file:" << m_path << "\n";
//	}
//#endif // DEBUG_AUDIO_CONSOLE_OUT
//	cmd.clear();
//	delete[] in_cmd;
//	delete[] out_string;
}

RedAudio::~RedAudio()
{
	Stop();
	Close();
	//Clear();
	/*if (m_path) delete m_path;
	if (m_alias) delete m_alias;*/

}

const wchar_t* RedAudio::ConvertCharToWChar(const char* p_char)
{
	const size_t word_size = strlen(p_char) + 1;
	size_t zero = 0;
	wchar_t* output = new wchar_t[word_size];

	mbstowcs_s(&zero, output, word_size, p_char, _TRUNCATE);
	return output;
}

void RedAudio::Clear()
{
	delete m_path;
	delete m_alias;
}

void RedAudio::Close() {
	LPTSTR out_string = LPTSTR(new char[60]);
	std::string cmd = "close ";
	cmd.append(m_alias);
	const wchar_t* in_cmd = ConvertCharToWChar(cmd.c_str());
	mciSendString(in_cmd, out_string, sizeof(out_string) * 2, NULL);
#ifdef DEBUG_AUDIO_CONSOLE_OUT
	if (strcmp((const char*)out_string, "") != 0) {
		LPTSTR out_error = LPTSTR(new char[60]);
		mciGetErrorString(std::stoul(out_string), out_error, sizeof(out_error) * 2);
		std::cout << "ERROR: RedAudio-Close()-"; std::wcout << out_error; std::cout << "-file:" << m_path << "\n";
		delete out_error;
	}
#endif
	m_playing = false;
	m_segmented = false;
	m_looping = false;
	cmd.clear();
	delete[] in_cmd;
	delete[] out_string;
}

void RedAudio::Open()
{
	LPTSTR out_string = LPTSTR(new char[60]);
	std::string cmd = "open ";
	cmd.append(m_path);
	cmd.append(" alias ");
	cmd.append(m_alias);
	const wchar_t* in_cmd = ConvertCharToWChar(cmd.c_str());
	mciSendString(in_cmd, out_string, sizeof(out_string) * 2, NULL);
#ifdef DEBUG_AUDIO_CONSOLE_OUT
	if (strcmp((const char*)out_string, "") != 0) {
		LPTSTR out_error = LPTSTR(new char[60]);
		mciGetErrorString(std::stoul(out_string), out_error, sizeof(out_error) * 2);
		std::cout << "ERROR: RedAudio-Open()-";  std::wcout << out_error; std::cout << "-file:" << m_path << "\n";
		delete out_error;
	}
#endif
	SetVolume(m_volume);
	cmd.clear();
	delete[] in_cmd;
	delete[] out_string;
}

void RedAudio::Play()
{
	Open();
	LPTSTR out_string = LPTSTR(new char[60]);
	std::string cmd = "play ";
	cmd.append(m_alias);
	const wchar_t* in_cmd = ConvertCharToWChar(cmd.c_str());
	mciSendString(in_cmd, out_string, sizeof(out_string) * 2, NULL);
#ifdef DEBUG_AUDIO_CONSOLE_OUT
	if (strcmp((const char*)out_string, "") != 0) {
		LPTSTR out_error = LPTSTR(new char[45]);
		mciGetErrorString(std::stoul(out_string), out_error, sizeof(out_error) * 2);
		std::cout << "ERROR: RedAudio-Play()-"; std::wcout << out_error; std::cout << "-file:" << m_path << "\n";
		delete out_error;
	}
#endif
	m_playing = true;
	cmd.clear();
	delete[] in_cmd;
	delete[] out_string;
}

void RedAudio::PlayLoop()
{
	Open();
	LPTSTR out_string = LPTSTR(new char[60]);
	std::string cmd = "play ";
	cmd.append(m_alias);
	cmd.append(" repeat notify");
	const wchar_t* in_cmd = ConvertCharToWChar(cmd.c_str());
	mciSendString(in_cmd, out_string, sizeof(out_string) * 2, NULL);
#ifdef DEBUG_AUDIO_CONSOLE_OUT
	if (strcmp((const char*)out_string, "") != 0) {
		LPTSTR out_error = LPTSTR(new char[45]);
		mciGetErrorString(std::stoul(out_string), out_error, sizeof(out_error) * 2);
		std::cout << "ERROR: RedAudio-PlayLoop()-"; std::wcout << out_error; std::cout << "-file:" << m_path << "\n";
		delete out_error;
	}
#endif
	m_playing = true;
	m_looping = true;
	cmd.clear();
	delete[] in_cmd;
	delete[] out_string;
}

void RedAudio::PlayLoop(unsigned int volume)
{
	Open();
	SetVolume(volume);
	//m_personal_volume = true;
	LPTSTR out_string = LPTSTR(new char[60]);
	std::string cmd = "play ";
	cmd.append(m_alias);
	cmd.append(" repeat notify");
	const wchar_t* in_cmd = ConvertCharToWChar(cmd.c_str());
	mciSendString(in_cmd, out_string, sizeof(out_string) * 2, NULL);
#ifdef DEBUG_AUDIO_CONSOLE_OUT
	if (strcmp((const char*)out_string, "") != 0) {
		LPTSTR out_error = LPTSTR(new char[45]);
		mciGetErrorString(std::stoul(out_string), out_error, sizeof(out_error) * 2);
		std::cout << "ERROR: RedAudio-PlayLoop()-"; std::wcout << out_error; std::cout << "-file:" << m_path << "\n";
		delete out_error;
	}
#endif
	m_playing = true;
	m_looping = true;
	cmd.clear();
	delete[] in_cmd;
	delete[] out_string;
}

void RedAudio::PlaySegment(unsigned int start, unsigned int end)
{
	assert(start < end);
	Open();
	LPTSTR out_string = LPTSTR(new char[60]);
	std::string first = std::to_string(start);
	std::string last = std::to_string(end);
	std::string cmd = "play ";
	cmd.append(m_alias);
	cmd.append(" from ");
	cmd.append(first);
	cmd.append(" to ");
	cmd.append(last);
	//cmd[static_cast<int>(strlen(m_alias)) + static_cast<int>(strlen(first)) + static_cast<int>(strlen(last)) + 15] = '\0';
	const wchar_t* in_cmd = ConvertCharToWChar(cmd.c_str());
	mciSendString(in_cmd, out_string, sizeof(out_string) * 2, NULL);
#ifdef DEBUG_AUDIO_CONSOLE_OUT
	if (strcmp((const char*)out_string, "") != 0) {
		LPTSTR out_error = LPTSTR(new char[45]);
		mciGetErrorString(std::stoul(out_string), out_error, sizeof(out_error) * 2);
		std::cout << "ERROR: RedAudio-PlaySegment()-"; std::wcout << out_error; std::cout << "-file:" << m_path << "\n";
		delete out_error;
	}
#endif
	m_playing = true;
	m_segmented = true;
	cmd.clear();
	delete[] in_cmd;
	delete[] out_string;
}

void RedAudio::PlaySegmentLoop(unsigned int start, unsigned int end)
{
	if (start > end) {
		std::cout << "ERROR: RedAudio-PlaySegment()-The start " << start << " is greater than end " << end << "-file:" << m_path << "\n";
		return;
	}
	Open();
	LPTSTR out_string = LPTSTR(new char[60]);
	std::string str = std::to_string(start);
	std::string first = std::to_string(start);
	std::string last = std::to_string(end);
	std::string cmd = "play ";
	cmd.append(m_alias);
	cmd.append(" from ");
	cmd.append(first);
	cmd.append(" to ");
	cmd.append(last);
	cmd.append(" repeat notify");
	//cmd[static_cast<int>(strlen(m_alias)) + static_cast<int>(strlen(first)) + static_cast<int>(strlen(last)) + 29] = '\0';
	const wchar_t* in_cmd = ConvertCharToWChar(cmd.c_str());
	mciSendString(in_cmd, out_string, sizeof(out_string) * 2, NULL);
#ifdef DEBUG_AUDIO_CONSOLE_OUT
	if (strcmp((const char*)out_string, "") != 0) {
		LPTSTR out_error = LPTSTR(new char[45]);
		mciGetErrorString(std::stoul(out_string), out_error, sizeof(out_error) * 2);
		std::cout << "ERROR: RedAudio-PlaySegment()-"; std::wcout << out_error; std::cout << "-file:" << m_path << "\n";
		delete out_error;
	}
#endif
	m_playing = true;
	m_segmented = true;
	m_looping = true;
	cmd.clear();
	delete[] in_cmd;
	delete[] out_string;
}

void RedAudio::PlayInstance() {
	//if (m_volume > 0) {
		srand(static_cast<unsigned int>(time(NULL)));
		std::string cmd = "open ";
		cmd.append(m_path);
		cmd.append(" alias ");
		std::string newAlias = m_alias;
		newAlias.append(std::to_string((rand() % (999 - 100 + 1) + 100)));
		cmd.append(newAlias);
		const wchar_t* in_cmd = ConvertCharToWChar(cmd.c_str());
		mciSendString(in_cmd, NULL, 0, NULL);

		cmd = "setaudio ";
		cmd.append(newAlias);
		cmd.append(" volume to ");
		cmd.append(std::to_string(m_volume));
		delete[] in_cmd;
		in_cmd = ConvertCharToWChar(cmd.c_str());
		mciSendString(in_cmd, NULL, 0, NULL);

		//delete cmd;
		cmd.clear();
		cmd.append("play ");
		cmd.append(newAlias);
		//cmd.append(" wait");
		delete[] in_cmd;
		in_cmd = ConvertCharToWChar(cmd.c_str());
		mciSendString(in_cmd, NULL, 0, NULL);

		/*cmd.clear();
		cmd.append("close ");
		cmd.append(newAlias);
		delete[] in_cmd;
		in_cmd = ConvertCharToWChar(cmd.c_str());
		mciSendString(in_cmd, NULL, 0, NULL);*/

		cmd.clear();
		newAlias.clear();
		delete[] in_cmd;
	//}
}

void RedAudio::PlayInstance(unsigned int volume) {
	//if (m_volume > 0) {
	srand(static_cast<unsigned int>(time(NULL)));
	std::string cmd = "open ";
	cmd.append(m_path);
	cmd.append(" alias ");
	std::string newAlias = m_alias;
	newAlias.append(std::to_string((rand() % (999 - 100 + 1) + 100)));
	cmd.append(newAlias);
	const wchar_t* in_cmd = ConvertCharToWChar(cmd.c_str());
	mciSendString(in_cmd, NULL, 0, NULL);

	cmd = "setaudio ";
	cmd.append(newAlias);
	cmd.append(" volume to ");
	cmd.append(std::to_string(volume));
	delete[] in_cmd;
	in_cmd = ConvertCharToWChar(cmd.c_str());
	mciSendString(in_cmd, NULL, 0, NULL);

	//delete cmd;
	cmd.clear();
	cmd.append("play ");
	cmd.append(newAlias);
	//cmd.append(" wait");
	delete[] in_cmd;
	in_cmd = ConvertCharToWChar(cmd.c_str());
	mciSendString(in_cmd, NULL, 0, NULL);

	/*cmd.clear();
	cmd.append("close ");
	cmd.append(newAlias);
	delete[] in_cmd;
	in_cmd = ConvertCharToWChar(cmd.c_str());
	mciSendString(in_cmd, NULL, 0, NULL);*/

	cmd.clear();
	newAlias.clear();
	delete[] in_cmd;
	//}
}

void RedAudio::Pause()
{
	LPTSTR out_string = LPTSTR(new char[60]);
	std::string cmd = "pause ";
	cmd.append(m_alias);
	const wchar_t* in_cmd = ConvertCharToWChar(cmd.c_str());
	mciSendString(in_cmd, out_string, sizeof(out_string) * 2, NULL);
#ifdef DEBUG_AUDIO_CONSOLE_OUT
	if (strcmp((const char*)out_string, "") != 0) {
		LPTSTR out_error = LPTSTR(new char[45]);
		mciGetErrorString(std::stoul(out_string), out_error, sizeof(out_error) * 2);
		std::cout << "ERROR: RedAudio-PlaySegment()-"; std::wcout << out_error; std::cout << "-file:" << m_path << "\n";
		delete out_error;
	}
#endif
	m_playing = false;
	//cmd.c_str();
	delete[] in_cmd;
	delete[] out_string;
}

void RedAudio::Stop()
{
	Open();
	LPTSTR out_string = LPTSTR(new char[60]);
	std::string cmd = "stop ";
	cmd.append(m_alias);
	const wchar_t* in_cmd = ConvertCharToWChar(cmd.c_str());
	mciSendString(in_cmd, out_string, sizeof(out_string) * 2, NULL);
#ifdef DEBUG_AUDIO_CONSOLE_OUT
	if (strcmp((const char*)out_string, "") != 0) {
		LPTSTR out_error = LPTSTR(new char[45]);
		mciGetErrorString(std::stoul(out_string), out_error, sizeof(out_error) * 2);
		std::cout << "ERROR: RedAudio-PlaySegment()-"; std::wcout << out_error; std::cout << "-file:" << m_path << "\n";
		delete out_error;
	}
#endif
	m_playing = false;
	cmd.clear();
	delete[] in_cmd;
	delete[] out_string;
	SeekBegin();
}

void RedAudio::SeekBegin()
{
	LPTSTR out_string = LPTSTR(new char[60]);
	std::string cmd = "seek ";
	cmd.append(m_alias);
	cmd.append(" to start");
	const wchar_t* in_cmd = ConvertCharToWChar(cmd.c_str());
	mciSendString(in_cmd, out_string, sizeof(out_string) * 2, NULL);
#ifdef DEBUG_AUDIO_CONSOLE_OUT
	if (strcmp((const char*)out_string, "") != 0) {
		LPTSTR out_error = LPTSTR(new char[45]);
		mciGetErrorString(std::stoul(out_string), out_error, sizeof(out_error) * 2);
		std::cout << "ERROR: RedAudio-PlaySegment()-"; std::wcout << out_error; std::cout << "-file:" << m_path << "\n";
		delete out_error;
	}
#endif
	if (m_playing && m_looping) {
		PlayLoop();
	}
	else if (m_playing) {
		Play();
	}
	cmd.clear();
	delete[] in_cmd;
	delete[] out_string;
}

void RedAudio::SeekEnd()
{
	LPTSTR out_string = LPTSTR(new char[60]);
	std::string cmd = "seek ";
	cmd.append(m_alias);
	cmd.append(" to end");
	const wchar_t* in_cmd = ConvertCharToWChar(cmd.c_str());
	mciSendString(in_cmd, out_string, sizeof(out_string) * 2, NULL);
#ifdef DEBUG_AUDIO_CONSOLE_OUT
	if (strcmp((const char*)out_string, "") != 0) {
		LPTSTR out_error = LPTSTR(new char[45]);
		mciGetErrorString(std::stoul(out_string), out_error, sizeof(out_error) * 2);
		std::cout << "ERROR: RedAudio-PlaySegment()-"; std::wcout << out_error; std::cout << "-file:" << m_path << "\n";
		delete out_error;
	}
#endif
	if (m_looping) {
		PlayLoop();
	}
	cmd.clear();
	delete[] in_cmd;
	delete[] out_string;
}

void RedAudio::SetVolume(unsigned int volume)
{
	//Open();
	m_volume = volume;
	LPTSTR out_string = LPTSTR(new char[60]);

	std::string cmd = "setaudio ";
	cmd.append(m_alias);
	cmd.append(" volume to ");
	cmd.append(std::to_string(volume));
	cmd.append(" wait");
	const wchar_t* in_cmd = ConvertCharToWChar(cmd.c_str());
	mciSendString(in_cmd, out_string, sizeof(out_string) * 2, NULL);
#ifdef DEBUG_AUDIO_CONSOLE_OUT
	if (strcmp((const char*)out_string, "") != 0) {
		LPTSTR out_error = LPTSTR(new char[45]);
		mciGetErrorString(std::stoul(out_string), out_error, sizeof(out_error) * 2);
		std::cout << "ERROR: RedAudio-PlaySegment()-"; std::wcout << out_error; std::cout << "-file:" << m_path << "\n";
	}
#endif
	if (m_alias == "BackgroundBattle") 
	{
		int i = 0;
	}
	cmd.clear();
	delete[] in_cmd;
	delete[] out_string;
}

unsigned int RedAudio::GetVolume()
{
	LPTSTR out_string = LPTSTR(new char[60]);

	std::string cmd = "status ";
	cmd.append(m_alias);
	cmd.append(" volume");
	const wchar_t* in_cmd = ConvertCharToWChar(cmd.c_str());
	mciSendString(in_cmd, out_string, sizeof(out_string) * 2, NULL);
	if (strcmp((const char*)out_string, "") != 0) {
		unsigned long curr_pos = std::stoul(out_string);
		return static_cast<unsigned int>(curr_pos);
	}
	else {
		return static_cast<unsigned int>(1001);
#ifdef DEBUG_AUDIO_CONSOLE_OUT
		std::cout << "ERROR: RedAudio-Update()-No volume recived" << "-file:" << m_path << "\n";
#endif
	}
	cmd.clear();
	delete[] in_cmd;
	delete[] out_string;
}

void RedAudio::Update()
{
	if (m_playing) {
		LPTSTR out_string = LPTSTR(new char[60]);

		std::string cmd = "status ";
		cmd.append(m_alias);
		cmd.append(" position");
		const wchar_t* in_cmd = ConvertCharToWChar(cmd.c_str());
		mciSendString(in_cmd, out_string, sizeof(out_string) * 2, NULL);
		if (strcmp((const char*)out_string, "") != 0) {
			unsigned long curr_pos = std::stoul(out_string);
			if (!m_looping && (curr_pos >= m_duration)) {
				m_playing = false;
			}
		}
#ifdef DEBUG_AUDIO_CONSOLE_OUT
		else {
			std::cout << "ERROR: RedAudio-Update()-No position recived" << "-file:" << m_path << "\n";
		}
#endif
		cmd.clear();
		delete[] in_cmd;
		delete[] out_string;
	}
}

bool RedAudio::isPlaying()
{
	return m_playing;
}

bool RedAudio::isLooping()
{
	return m_looping;
}

unsigned long RedAudio::Duration()
{
	return m_duration;
}

void RedAudio::SetPath(const char* path)
{
	m_path = path;
}

const char* RedAudio::GetPath()
{
	return m_path;
}

void RedAudio::SetAlias(const char* alias)
{
	m_alias = alias;
}

const char* RedAudio::GetAlias()
{
	return m_alias;
}