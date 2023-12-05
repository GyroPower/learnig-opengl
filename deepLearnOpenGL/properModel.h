#pragma once
#include"properMesh.h"
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>
#include"stb_image.h"


unsigned int properTextureFromFile(const char* path, const std::string& directory, bool gamma=false);


class properModel {

public:
	std::vector<properMesh> meshes;
	std::string directory;
	std::vector<Texture> textures_loaded;
	bool gammaCorrection;


    properModel(std::string const& path, bool gamma = false);

    void Draw(properShader& shader);


private:


    void loadModel(std::string const& path);

    void processNode(aiNode* node, const aiScene* scene);
	//void processNode(aiMesh *mesh, const aiScene *scene);

	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

    properMesh processMesh(aiMesh* mesh, const aiScene* scene);
	
};
