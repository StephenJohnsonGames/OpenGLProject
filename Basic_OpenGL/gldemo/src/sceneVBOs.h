#pragma once

#include <CoreStructures\GUObject.h>
#include <CoreStructures\GUMatrix4.h>
#include <glew\glew.h>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include "modelVBO.h"

class sceneVBOs : public CoreStructures::GUObject {

	GLuint numModels;
	modelVBO** modelArray;

public:

	sceneVBOs(const aiScene* scene);

	void render();

};