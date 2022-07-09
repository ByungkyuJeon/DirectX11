#pragma once

#include <string>

class StringHelper
{
public:
	static std::wstring StringToWide(std::string str);
	static std::string GetdirectoryFromPath(const std::string& filePath);
	static std::string GetFileExtension(const std::string& fileName);
};