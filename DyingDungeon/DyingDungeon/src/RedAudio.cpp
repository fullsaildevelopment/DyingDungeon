#include "RedAudio.h"

RedAudio::RedAudio(const char* path, const char* alias)
{
	m_path = path;
	m_alias = alias;
	m_playing = false;
	m_looping = false;
	m_segmented = false;
	m_start = 0;

	Open();

	char* cmd = new char[static_cast<int>(strlen(m_alias)) + 15];
	int curr_count = 8;

	LPTSTR out_string = LPTSTR(new char[15]);

	strcpy_s(cmd, curr_count, "status ");
	curr_count += static_cast<int>(strlen(m_alias)) + 1;
	strcat_s(cmd, curr_count, m_alias);
	curr_count += 8;
	strcat_s(cmd, curr_count, " length");
	//cmd[static_cast<int>(strlen(m_alias)) + 14] = '\0';
	mciSendString(ConvertCharToWChar(cmd), out_string, 15, NULL);
	if (strcmp((const char*)out_string, "") != 0) {
		m_end = m_duration = std::stoul(out_string, nullptr, 10);
	}
	else {
		std::cout << "ERROR: RedAudio-RedAudio()-Could not get duration/end song-file:" << m_path << "\n";
	}
}

RedAudio::~RedAudio()
{
	Stop();
	Close();
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
	LPTSTR out_error = LPTSTR(new char[60]);
	char* cmd = new char[static_cast<int>(strlen(m_alias)) + 15];
	int curr_count = 7;
	strcpy_s(cmd, curr_count, "close ");
	curr_count += static_cast<int>(static_cast<int>(strlen(m_alias))) + 1;
	strcat_s(cmd, curr_count, m_alias);
	mciSendString(ConvertCharToWChar(cmd), out_string, sizeof(out_string) * 2, NULL);
	if (strcmp((const char*)out_string, "") != 0) {
		mciGetErrorString(std::stoul(out_string), out_error, sizeof(out_error) * 2);
		std::cout << "ERROR: RedAudio-Close()-"; std::wcout << out_error; std::cout << "-file:" << m_path << "\n";
	}
}

void RedAudio::Open()
{
	LPTSTR out_string = LPTSTR(new char[60]);
	LPTSTR out_error = LPTSTR(new char[60]);
	char* cmd = new char[static_cast<int>(strlen(m_path)) + static_cast<int>(strlen(m_alias)) + 17];
	int curr_count = 6;
	strcpy_s(cmd, curr_count, "open ");
	curr_count += static_cast<int>(strlen(m_path)) + 1;
	strcat_s(cmd, curr_count, m_path);
	curr_count += 8;
	strcat_s(cmd, curr_count, " alias ");
	curr_count += static_cast<int>(strlen(m_alias)) + 1;
	strcat_s(cmd, curr_count, m_alias);
	//cmd[strlen(path) + strlen(alias) + 16] = '\0';
	mciSendString(ConvertCharToWChar(cmd), out_string, sizeof(out_string) * 2, NULL);
	if (strcmp((const char*)out_string, "") != 0) {
		mciGetErrorString(std::stoul(out_string), out_error, sizeof(out_error) * 2);
		std::cout << "ERROR: RedAudio-Open()-";  std::wcout << out_error; std::cout << "-file:" << m_path <<"\n";
	}
}

void RedAudio::Play()
{
	LPTSTR out_string = LPTSTR(new char[60]);
	LPTSTR out_error = LPTSTR(new char[45]);
	char* cmd = new char[static_cast<int>(strlen(m_alias)) + 6];
	int curr_count = 6;
	strcpy_s(cmd, curr_count, "play ");
	curr_count += static_cast<int>(strlen(m_alias)) + 1;
	strcat_s(cmd, curr_count, m_alias);
	//cmd[static_cast<int>(strlen(m_alias)) + 5] = '\0';
	mciSendString(ConvertCharToWChar(cmd), NULL, 0, NULL);
	if (strcmp((const char*)out_string, "") != 0) {
		mciGetErrorString(std::stoul(out_string), out_error, sizeof(out_error) * 2);
		std::cout << "ERROR: RedAudio-Play()-"; std::wcout << out_error; std::cout << "-file:" << m_path << "\n";
	}
	m_playing = true;
}

void RedAudio::PlayLoop()
{
	LPTSTR out_string = LPTSTR(new char[60]);
	LPTSTR out_error = LPTSTR(new char[45]);
	char* cmd = new char[static_cast<int>(strlen(m_alias)) + 20];
	int curr_count = 6;
	strcpy_s(cmd, curr_count, "play ");
	curr_count += static_cast<int>(strlen(m_alias)) + 1;
	strcat_s(cmd, curr_count, m_alias);
	curr_count += 15;
	strcat_s(cmd, curr_count, " repeat notify");
	//cmd[static_cast<int>(strlen(m_alias)) + 19] = '\0';
	mciSendString(ConvertCharToWChar(cmd), out_string, sizeof(out_string) * 2, NULL);
	if (strcmp((const char*)out_string, "") != 0) {
		mciGetErrorString(std::stoul(out_string), out_error, sizeof(out_error) * 2);
		std::cout << "ERROR: RedAudio-PlayLoop()-"; std::wcout << out_error; std::cout << "-file:" << m_path << "\n";
	}
	m_playing = true;
	m_looping = true;
}

void RedAudio::PlaySegment(unsigned int start, unsigned int end)
{
	assert(start < end);
	LPTSTR out_string = LPTSTR(new char[60]);
	LPTSTR out_error = LPTSTR(new char[45]);
	std::string str = std::to_string(start);
	const char* first = str.c_str();
	std::string str2 = std::to_string(start);
	str2 = std::to_string(end);
	const char* last = str2.c_str();
	char* cmd = new char[static_cast<int>(strlen(m_alias)) + static_cast<int>(strlen(first)) + static_cast<int>(strlen(last)) + 15];
	int curr_count = 6;
	strcpy_s(cmd, curr_count, "play ");
	curr_count += static_cast<int>(strlen(m_alias)) + 1;
	strcat_s(cmd, curr_count, m_alias);
	curr_count += 7;
	strcat_s(cmd, curr_count, " from ");
	curr_count += static_cast<int>(strlen(first)) + 1;
	strcat_s(cmd, curr_count, first);
	curr_count += 5;
	strcat_s(cmd, curr_count, " to ");
	curr_count += static_cast<int>(strlen(last)) + 1;
	strcat_s(cmd, curr_count, last);
	//cmd[static_cast<int>(strlen(m_alias)) + static_cast<int>(strlen(first)) + static_cast<int>(strlen(last)) + 15] = '\0';
	mciSendString(ConvertCharToWChar(cmd), out_string, sizeof(out_string) * 2, NULL);
	if (strcmp((const char*)out_string, "") != 0) {
		mciGetErrorString(std::stoul(out_string), out_error, sizeof(out_error) * 2);
		std::cout << "ERROR: RedAudio-PlaySegment()-"; std::wcout << out_error; std::cout << "-file:" << m_path << "\n";
	}
	m_playing = true;
	m_segmented = true;

}

void RedAudio::PlaySegmentLoop(unsigned int start, unsigned int end)
{
	if (start > end) {
		std::cout << "ERROR: RedAudio-PlaySegment()-The start " << start << " is greater than end " << end << "-file:" << m_path << "\n";
		return;
	}
	LPTSTR out_string = LPTSTR(new char[60]);
	LPTSTR out_error = LPTSTR(new char[45]);
	std::string str = std::to_string(start);
	const char* first = str.c_str();
	str = std::to_string(end);
	const char* last = str.c_str();
	char* cmd = new char[static_cast<int>(strlen(m_alias)) + static_cast<int>(strlen(first)) + static_cast<int>(strlen(last)) + 29];
	int curr_count = 6;
	strcpy_s(cmd, curr_count, "play ");
	curr_count += static_cast<int>(strlen(m_alias)) + 1;
	strcat_s(cmd, curr_count, m_alias);
	curr_count += 7;
	strcat_s(cmd, curr_count, " from ");
	curr_count += static_cast<int>(strlen(first)) + 1;
	strcat_s(cmd, curr_count, first);
	curr_count += 5;
	strcat_s(cmd, curr_count, " to ");
	curr_count += static_cast<int>(strlen(last)) + 1;
	strcat_s(cmd, curr_count, last);
	curr_count += 15;
	strcat_s(cmd, curr_count, " repeat notify");
	//cmd[static_cast<int>(strlen(m_alias)) + static_cast<int>(strlen(first)) + static_cast<int>(strlen(last)) + 29] = '\0';
	mciSendString(ConvertCharToWChar(cmd), out_string, sizeof(out_string) * 2, NULL);
	if (strcmp((const char*)out_string, "") != 0) {
		mciGetErrorString(std::stoul(out_string), out_error, sizeof(out_error) * 2);
		std::cout << "ERROR: RedAudio-PlaySegment()-"; std::wcout << out_error; std::cout << "-file:" << m_path << "\n";
	}
	m_playing = true;
	m_segmented = true;
	m_looping = true;

}

void RedAudio::Pause()
{
	LPTSTR out_string = LPTSTR(new char[60]);
	LPTSTR out_error = LPTSTR(new char[45]);
	char* cmd = new char[static_cast<int>(strlen(m_alias)) + 7];
	int curr_count = 7;
	strcpy_s(cmd, curr_count, "pause ");
	curr_count += static_cast<int>(strlen(m_alias)) + 1;
	strcat_s(cmd, curr_count, m_alias);
	//cmd[static_cast<int>(strlen(m_alias)) + 6] = '\0';
	mciSendString(ConvertCharToWChar(cmd), out_string, sizeof(out_string) * 2, NULL);
	if (strcmp((const char*)out_string, "") != 0) {
		mciGetErrorString(std::stoul(out_string), out_error, sizeof(out_error) * 2);
		std::cout << "ERROR: RedAudio-PlaySegment()-"; std::wcout << out_error; std::cout << "-file:" << m_path << "\n";
	}
	m_playing = false;
}

void RedAudio::Stop()
{
	LPTSTR out_string = LPTSTR(new char[60]);
	LPTSTR out_error = LPTSTR(new char[45]);
	char* cmd = new char[static_cast<int>(strlen(m_alias)) + 6];
	int curr_count = 6;
	strcpy_s(cmd, curr_count, "stop ");
	curr_count += static_cast<int>(strlen(m_alias)) + 1;
	strcat_s(cmd, curr_count, m_alias);
	//cmd[static_cast<int>(strlen(m_alias)) + 5] = '\0';
	mciSendString(ConvertCharToWChar(cmd), out_string, sizeof(out_string) * 2, NULL);
	if (strcmp((const char*)out_string, "") != 0) {
		mciGetErrorString(std::stoul(out_string), out_error, sizeof(out_error) * 2);
		std::cout << "ERROR: RedAudio-PlaySegment()-"; std::wcout << out_error; std::cout << "-file:" << m_path << "\n";
	}
}

void RedAudio::SeekBegin()
{
	LPTSTR out_string = LPTSTR(new char[60]);
	LPTSTR out_error = LPTSTR(new char[45]);
	char* cmd = new char[static_cast<int>(strlen(m_alias)) + 15];
	int curr_count = 6;
	strcpy_s(cmd, curr_count, "seek ");
	curr_count += static_cast<int>(strlen(m_alias)) + 1;
	strcat_s(cmd, curr_count, m_alias);
	curr_count += 10;
	strcat_s(cmd, curr_count, " to start");
	//cmd[static_cast<int>(strlen(m_alias)) + 14] = '\0';
	mciSendString(ConvertCharToWChar(cmd), out_string, sizeof(out_string) * 2, NULL);
	if (strcmp((const char*)out_string, "") != 0) {
		mciGetErrorString(std::stoul(out_string), out_error, sizeof(out_error) * 2);
		std::cout << "ERROR: RedAudio-PlaySegment()-"; std::wcout << out_error; std::cout << "-file:" << m_path << "\n";
	}
	if (m_playing) {
		Play();
	}
}

void RedAudio::SeekEnd()
{
	LPTSTR out_string = LPTSTR(new char[60]);
	LPTSTR out_error = LPTSTR(new char[45]);
	char* cmd = new char[static_cast<int>(strlen(m_alias)) + 15];
	int curr_count = 6;
	strcpy_s(cmd, curr_count, "seek ");
	curr_count += static_cast<int>(strlen(m_alias)) + 1;
	strcat_s(cmd, curr_count, m_alias);
	curr_count += 8;
	strcat_s(cmd, curr_count, " to end");
	//cmd[static_cast<int>(strlen(m_alias)) + 14] = '\0';
	mciSendString(ConvertCharToWChar(cmd), out_string, sizeof(out_string) * 2, NULL);
	if (strcmp((const char*)out_string, "") != 0) {
		mciGetErrorString(std::stoul(out_string), out_error, sizeof(out_error) * 2);
		std::cout << "ERROR: RedAudio-PlaySegment()-"; std::wcout << out_error; std::cout << "-file:" << m_path << "\n";
	}
	if (m_looping) {
		PlayLoop();
	}
}

void RedAudio::SetVolume(unsigned int volume)
{
	LPTSTR out_string = LPTSTR(new char[60]);
	LPTSTR out_error = LPTSTR(new char[45]);
	std::string str = std::to_string(volume);
	const char* vol = str.c_str();
	char* cmd = new char[static_cast<int>(strlen(m_alias)) + strlen(vol) + 21];
	int curr_count = 10;
	strcpy_s(cmd, curr_count, "setaudio ");
	curr_count += static_cast<int>(strlen(m_alias)) + 1;
	strcat_s(cmd, curr_count, m_alias);
	curr_count += 12;
	strcat_s(cmd, curr_count, " volume to ");
	curr_count += static_cast<int>(strlen(vol)) + 1;
	strcat_s(cmd, curr_count, vol);
	//cmd[static_cast<int>(strlen(m_alias)) + strlen(vol) + 20] = '\0';
	mciSendString(ConvertCharToWChar(cmd), out_string, sizeof(out_string) * 2, NULL);
	if (strcmp((const char*)out_string, "") != 0) {
		mciGetErrorString(std::stoul(out_string), out_error, sizeof(out_error) * 2);
		std::cout << "ERROR: RedAudio-PlaySegment()-"; std::wcout << out_error; std::cout << "-file:" << m_path << "\n";
	}
}

void RedAudio::Update()
{
	if (m_playing) {
		char* cmd = new char[static_cast<int>(strlen(m_alias)) + 19];
		int curr_count = 8;
		LPTSTR out_string = LPTSTR(new char[60]);
		strcpy_s(cmd, curr_count, "status ");
		curr_count += static_cast<int>(strlen(m_alias)) + 1;
		strcat_s(cmd, curr_count, m_alias);
		curr_count += 10;
		strcat_s(cmd, curr_count, " position");
		mciSendString(ConvertCharToWChar(cmd), out_string, sizeof(out_string) * 2, NULL);
		if (strcmp((const char*)out_string, "") != 0) {
			unsigned long curr_pos = std::stoul(out_string);
			if (!m_looping && (curr_pos >= m_duration)) {
				m_playing = false;
			}
		}
		else {
			std::cout << "ERROR: RedAudio-Update()-No position recived" << "-file:" << m_path << "\n";
		}
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
