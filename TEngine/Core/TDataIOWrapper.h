#pragma once

#include <filesystem>
#include <unordered_map>
#include "../ThirdParty/TinyXML/tinyxml2.h"

typedef std::filesystem::path TPath;

class TDataIOWrapper
{

private:
	const std::string m_DataDefFIlePathString = "\\..\\DataDefinition.xml";

	const TPath m_GamePath = std::filesystem::current_path();
	const TPath m_DataDefXmlFilePath = TPath(m_DataDefFIlePathString);

	std::unordered_map<std::string, std::string> m_XmlDocumentPathContainer;
public:
	bool Initialize();


private:
	TPath GetDataDefXmlFilePath() const;

	bool GenerateDefXmlFile() const;
	std::string GetDataDefXmlFileContentString(const TPath& InFilePath) const;

};

extern TDataIOWrapper* DataIOWrapper;

static bool InitializeDataIOWrapper()
{
	DataIOWrapper = new TDataIOWrapper();

	return DataIOWrapper->Initialize();
}