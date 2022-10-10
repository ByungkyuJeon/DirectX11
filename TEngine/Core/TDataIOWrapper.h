#pragma once

#include "../ThirdParty/TinyXML/tinyxml2.h"
#include <functional>
#include <filesystem>
#include <unordered_map>

typedef std::filesystem::path TPath;

class TDataElement
{
private:
	std::string m_FilePath;
	tinyxml2::XMLDocument m_DataDocument;
	tinyxml2::XMLElement* m_RootNode = nullptr;
	tinyxml2::XMLElement* m_CurrentNode = nullptr;

public:
	TDataElement() = default;
	TDataElement(const std::string InFileName);
	TDataElement(tinyxml2::XMLElement* InRootNodePtr, tinyxml2::XMLElement* InCurrentNodePtr);

	const std::string& GetFilePath() const { return m_FilePath; }
	bool IsRootNode() const { return m_RootNode != nullptr && m_RootNode == m_CurrentNode; }
	TDataElement FirstChildElement(const std::string& InNodeName);
	TDataElement NextSiblingElement();
	bool FindBoolAttribute(const std::string& InAttrName);
	int FindIntAttribute(const std::string& InAttrName);
	long long FindInt64Attribute(const std::string& InAttrName);
	double FindDoubleAttribute(const std::string& InAttrName);
	float FindFloatAttribute(const std::string& InAttrName);

private:
	~TDataElement();
};

class TDataIOWrapper
{
private:
	const std::string m_DataDefFIlePathString = "\\..\\DataDefinition.xml";

	const TPath m_GamePath = std::filesystem::current_path();
	const TPath m_DataDefXmlFilePath = TPath(m_DataDefFIlePathString);

	std::unordered_map<std::string, std::string> m_XmlDocumentPathContainer;
public:
	bool Initialize();

	TPath GetDataFilePath(const std::string& InFilePath) const;

private:
	~TDataIOWrapper();
	TPath GetDataDefXmlFilePath() const;

	bool GenerateDefXmlFile() const;
	std::string GetDataDefXmlFileContentString(const TPath& InFilePath) const;
};

extern TDataIOWrapper* DataIOWrapper;

extern bool InitializeDataIOWrapper();

extern TDataElement GetRootDataNode(const std::string& InDataName);