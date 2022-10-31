#include "TDataIOWrapper.h"
#include "TErrorLogger.h"
#include "TEngineDefines.h"
#include <string>
#include <fstream>

TDataIOWrapper* DataIOWrapper;

bool InitializeDataIOWrapper()
{
	DataIOWrapper = new TDataIOWrapper();

	return DataIOWrapper->Initialize();
}

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

	tinyxml2::XMLElement* nodeItr = rootNode->FirstChildElement("File");

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

TPath TDataIOWrapper::GetDataFilePath(const std::string& InFilePath) const
{
	if (InFilePath.length() == 0)
	{
		PRINT_ERROR("path param is empty");
		return TPath();
	}

	auto itr = m_XmlDocumentPathContainer.find(InFilePath);
	if (itr == m_XmlDocumentPathContainer.end())
	{
		PRINT_ERROR("can't find data file path" + InFilePath);
		return TPath();
	}

	TPath path = m_GamePath;
	path += itr->second;

	return path;
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

TDataElement GetRootDataNode(const std::string& InDataName)
{
	return TDataElement(InDataName);
}

TDataElement::TDataElement(const std::string InFileName)
{
	if (nullptr == DataIOWrapper)
		return;

	TPath path = DataIOWrapper->GetDataFilePath(InFileName);

	if (false == std::filesystem::exists(path))
	{
		PRINT_ERROR("file doesn't exists.");
		return;
	}

	std::ifstream fileStream(path);
	if (false == fileStream.is_open())
	{
		PRINT_ERROR("file stream open error.");
		return;
	}

	std::string fileContent, fileLine;
	while (std::getline(fileStream, fileLine))
	{
		fileContent.append(fileLine);
	}

	fileStream.close();

	// data node
	m_DataDocument.Parse(fileContent.c_str());

	tinyxml2::XMLNode* rootNode = m_DataDocument.FirstChild();
	if (nullptr == rootNode)
	{
		PRINT_ERROR("no root found.");
		return;
	}
	
	m_FilePath = InFileName;
	m_RootNode = rootNode->ToElement();
	m_CurrentNode = rootNode->ToElement();

	return;
}

TDataElement::TDataElement(tinyxml2::XMLElement* InRootNodePtr, tinyxml2::XMLElement* InCurrentNodePtr)
{
	if (nullptr == InRootNodePtr || nullptr == InCurrentNodePtr)
	{
		PRINT_ERROR("can't find node.");
		return;
	}

	m_RootNode = InRootNodePtr;
	m_CurrentNode = InCurrentNodePtr;
}

TDataElement TDataElement::FirstChildElement(const std::string& InNodeName)
{
	if (nullptr == m_RootNode || nullptr == m_CurrentNode)
	{
		PRINT_ERROR("node reference is broken.");
		return TDataElement();
	}

	return TDataElement(m_RootNode, m_CurrentNode->FirstChildElement(InNodeName.c_str()));
}

TDataElement TDataElement::NextSiblingElement()
{
	if (nullptr == m_RootNode || nullptr == m_CurrentNode)
	{
		PRINT_ERROR("node reference is broken.");
		return TDataElement();
	}

	return TDataElement(m_RootNode, m_CurrentNode->NextSiblingElement());
}

bool TDataElement::FindBoolAttribute(const std::string& InAttrName)
{
	if (nullptr == m_RootNode || nullptr == m_CurrentNode)
	{
		PRINT_ERROR("node reference is broken.");
		return false;
	}

	const tinyxml2::XMLAttribute* pAttr = m_CurrentNode->FindAttribute(InAttrName.c_str());
	if (nullptr == pAttr)
		return false;

	return pAttr->BoolValue();
}

int TDataElement::FindIntAttribute(const std::string& InAttrName)
{
	if (nullptr == m_RootNode || nullptr == m_CurrentNode)
	{
		PRINT_ERROR("node reference is broken.");
		return 0;
	}

	const tinyxml2::XMLAttribute* pAttr = m_CurrentNode->FindAttribute(InAttrName.c_str());
	if (nullptr == pAttr)
		return false;

	return pAttr->IntValue();
}

long long TDataElement::FindInt64Attribute(const std::string& InAttrName)
{
	if (nullptr == m_RootNode || nullptr == m_CurrentNode)
	{
		PRINT_ERROR("node reference is broken.");
		return 0;
	}

	const tinyxml2::XMLAttribute* pAttr = m_CurrentNode->FindAttribute(InAttrName.c_str());
	if (nullptr == pAttr)
		return false;

	return pAttr->Int64Value();
}

double TDataElement::FindDoubleAttribute(const std::string& InAttrName)
{
	if (nullptr == m_RootNode || nullptr == m_CurrentNode)
	{
		PRINT_ERROR("node reference is broken.");
		return 0.0;
	}

	const tinyxml2::XMLAttribute* pAttr = m_CurrentNode->FindAttribute(InAttrName.c_str());
	if (nullptr == pAttr)
		return false;

	return pAttr->DoubleValue();
}

float TDataElement::FindFloatAttribute(const std::string& InAttrName)
{
	if (nullptr == m_RootNode || nullptr == m_CurrentNode)
	{
		PRINT_ERROR("node reference is broken.");
		return 0.0f;
	}

	const tinyxml2::XMLAttribute* pAttr = m_CurrentNode->FindAttribute(InAttrName.c_str());
	if (nullptr == pAttr)
		return false;

	return pAttr->FloatValue();
}

std::string TDataElement::FindStringAttribute(const std::string& InAttrName)
{
	if (nullptr == m_RootNode || nullptr == m_CurrentNode)
	{
		PRINT_ERROR("node reference is broken.");
		return "";
	}

	const tinyxml2::XMLAttribute* pAttr = m_CurrentNode->FindAttribute(InAttrName.c_str());
	if (nullptr == pAttr)
		return std::string();

	return std::string(pAttr->Value());
}


TDataElement::~TDataElement()
{

}