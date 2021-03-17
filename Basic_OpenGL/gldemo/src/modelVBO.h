#pragma once

#include <CoreStructures\GUObject.h>
#include <CoreStructures\GUMatrix4.h>
#include <glew\glew.h>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

class modelVBO : public CoreStructures::GUObject {

private:

	GLuint meshVAO;
	GLuint meshPosBuffer;
	GLuint meshTexCoordBuffer;
	GLuint normalsBuffer;
	GLuint meshFaceVBO;

	GLuint numFaces;

	//GLuint normalsVAO;
	//GLuint normalsBuffer;
	//GLuint lightSourceBuffer;
	//GLuint attenuationBuffer;
	//GLuint lightColourBuffer;

	
public:

	modelVBO(const aiScene* scene, int meshIndex);
	
	void render();

};