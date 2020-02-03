#pragma once
#include <string>
class Converter
{
public:
	static const wchar_t* ConvertCharToWChar(const char* p_char);
	static std::wstring ConvertCharToWStr(const char* p_char);
	static std::string ConvertWStrToStr(const wchar_t* p_char);
	static std::string FormatToPercentage(float number, unsigned int decimal_places);
	static std::wstring FormatToPercentageW(float number, unsigned int decimal_places);
};

