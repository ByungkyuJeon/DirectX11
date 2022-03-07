#pragma once
#include "Mesh.h"

class Model
{
public:
	void addMesh(const Mesh& mesh);
	void setDirectory(const std::string& directory);

	const std::string& getDirectory() const;
	const std::vector<Mesh>& getMeshes() const;
	Mesh& getMeshAt(size_t idx);

private:

	std::vector<Mesh> mMeshes;
	std::string mDirectory;

};