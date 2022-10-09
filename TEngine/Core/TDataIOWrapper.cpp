#include "TDataIOWrapper.h"
#include <string>
#include <fstream>

TDataIOWrapper* DataIOWrapper;

bool TDataIOWrapper::Initialize()
{
	TPath filePath = GetDataDefXmlFilePath();

	if (false == std::filesystem::exists(filePath))
	{
		if (false == GenerateDefXmlFile())
			return false;
	}

	tinyxml2::XMLDocument dataDefDocument;
	dataDefDocument.Parse(GetDataDefXmlFileContentString(filePath).c_str());

	tinyxml2::XMLNode* rootNode = dataDefDocument.FirstChild();
	if (nullptr == rootNode)
		return false;

	tinyxml2::XMLElement* nodeItr = rootNode->FirstChildElement("DataDefinition");

	while (nodeItr)
	{
		const tinyxml2::XMLAttribute* nameAttr = nodeItr->FindAttribute("name");
		const tinyxml2::XMLAttribute* srcAttr = nodeItr->FindAttribute("src");

		if (nullptr == nameAttr || nullptr == srcAttr)
		{
			nodeItr = nodeItr->NextSiblingElement();
			continue;
		}

		std::string name = std::string(nameAttr->Value());
		std::string src = std::string(srcAttr->Value());

		m_XmlDocumentPathContainer.emplace(name, src);
		nodeItr = nodeItr->NextSiblingElement();
	}

	dataDefDocument.Clear();

	return true;
}

TPath TDataIOWrapper::GetDataDefXmlFilePath() const
{
	TPath dataDefXmlFilePath = m_GamePath;
	dataDefXmlFilePath += m_DataDefXmlFilePath;

	return dataDefXmlFilePath;
}

bool TDataIOWrapper::GenerateDefXmlFile() const
{
	TPath filePath = GetDataDefXmlFilePath();

	std::ofstream dataDefXmlFileStream{ filePath };
	dataDefXmlFileStream << std::string();
	dataDefXmlFileStream.close();

	return std::filesystem::exists(filePath);
}

std::string TDataIOWrapper::GetDataDefXmlFileContentString(const TPath& InFilePath) const
{
	std::string dataDefXmlFileContentString;

	std::ifstream xmlInterfaceContent(InFilePath);
	if (false == xmlInterfaceContent.is_open())
		return std::string();

	std::string contentLine;
	while (std::getline(xmlInterfaceContent, contentLine))
	{
		dataDefXmlFileContentString.append(contentLine);
	}
	xmlInterfaceContent.close();

	return dataDefXmlFileContentString;
}
