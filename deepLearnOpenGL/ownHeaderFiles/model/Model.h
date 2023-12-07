#pragma once
#include "../mesh/Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "../../stb_image.h"

unsigned int properTextureFromFile(const char* path, const std::string& directory, bool gamma = false);

class Model {
public:
	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;
	std::string directory;
	bool gamma;

	Model(std::string const& path, bool gamma = false);

	void Draw(Shader& shader);

private:

	void processNode(aiNode* node, const aiScene* scene);
	void loadModel(std::string const &path);

	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};