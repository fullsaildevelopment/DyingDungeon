#include "Converter.h"
 const wchar_t* Converter::ConvertCharToWChar(const char* p_char) {
	const size_t word_size = strlen(p_char) + 1;
	size_t zero = 0;
	wchar_t* output = new wchar_t[word_size];

	mbstowcs_s(&zero, output, word_size, p_char, _TRUNCATE);
	return output;
}

std::wstring Converter::ConvertCharToWStr(const char* p_char) {
	const size_t word_size = strlen(p_char) + 1;
	size_t zero = 0;
	wchar_t* output = new wchar_t[word_size];

	mbstowcs_s(&zero, output, word_size, p_char, _TRUNCATE);
	return std::wstring(output);
}

std::string Converter::ConvertWStrToStr(const wchar_t* p_char) {
	const size_t word_size = wcslen(p_char) + 1;
	size_t zero = 0;
	char* output = new char[word_size];

	wcstombs_s(&zero, output, word_size, p_char, _TRUNCATE);
	return std::string(output);
}

std::string Converter::FormatToPercentage(float number, unsigned int decimal_places = 2)
{
	unsigned int count = 1 + decimal_places;
	for (float i = number; i > 0; i--, count++)
	{
		i /= 10.0f;
	}

	return std::to_string(number).substr(0, count);
}

std::wstring Converter::FormatToPercentageW(float number, unsigned int decimal_places = 2)
{
	unsigned int count = 1 + decimal_places;
	for (float i = number; i > 0; i--, count++)
	{
		i /= 10.0f;
	}

	return std::to_wstring(number).substr(0, count);
}