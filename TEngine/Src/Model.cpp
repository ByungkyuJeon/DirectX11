#include "Model.h"

void Model::addMesh(const Mesh& mesh)
{
	this->mMeshes.emplace_back(mesh);
}

void Model::setDirectory(const std::string& directory)
{
	this->mDirectory = directory;
}

const std::string& Model::getDirectory() const
{
	return this->mDirectory;
}

const std::vector<Mesh>& Model::getMeshes() const
{
	return this->mMeshes;
}

Mesh& Model::getMeshAt(size_t idx) 
{
	return this->mMeshes[idx];
}
