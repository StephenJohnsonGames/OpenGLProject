#pragma once

#include <CoreStructures\GUObject.h>
#include <CoreStructures\GUMatrix4.h>
#include <glew\glew.h>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags


class normalsVBO : public CoreStructures::GUObject {

private:

	GLuint normalsVAO;
	GLuint normalsBuffer;
	//GLuint lightSourceBuffer;
	//GLuint attenuationBuffer;
	//GLuint lightColourBuffer; 


public:

	normalsVBO(const aiScene* scene, int meshIndex);

};