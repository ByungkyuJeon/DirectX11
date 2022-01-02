#include "StringHelper.h"
#include <algorithm>

std::wstring StringHelper::StringToWide(std::string str)
{
	std::wstring wide_string(str.begin(), str.end());
	return wide_string;
}

std::string StringHelper::GetdirectoryFromPath(const std::string& filePath)
{
	size_t offset_1 = filePath.find_last_of('\\');
	size_t offset_2 = filePath.find_last_of('/');
	if (offset_1 == std::string::npos && offset_2 == std::string::npos)
	{
		return "";
	}

	if (offset_1 == std::string::npos)
	{
		return filePath.substr(0, offset_2);
	}
	if (offset_2 == std::string::npos)
	{
		return filePath.substr(0, offset_1);
	}

	return filePath.substr(0, std::max(offset_1, offset_2));
}

std::string StringHelper::GetFileExtension(const std::string& fileName)
{
	size_t offset = fileName.find_last_of('.');
	if (offset == std::string::npos)
	{
		return "";
	}
	return std::string(fileName.substr(offset + 1));
}
