#include "modelVBO.h"
#include "aiWrapper.h"

//static float lightSourceArray[] = {
//
//	2.0f, 0.0f, 0.0f, //L[0]
//	-2.0f, 2.0f, 1.0f //L[1]
//
//};
//
//static float attenuationArray[] = {
//
//	1.0f, 0.1f, 0.0f, //lK[0]
//	1.0f, 0.1f, 0.0f //lK[1]
//
//};
//
//static float lightColourArray[] = {
//
//	0.0f, 1.0f, 0.0f, //Light 0 colour (Green)
//	1.0f, 0.0f, 0.0f //Light 1 colour (Red)
//
//};



modelVBO::modelVBO(const aiScene *scene, int meshIndex) {

	glGenVertexArrays(1, &meshVAO);
	glBindVertexArray(meshVAO);


	// setup vbo for position attribute
	glGenBuffers(1, &meshPosBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, meshPosBuffer);
	glBufferData(GL_ARRAY_BUFFER, scene->mMeshes[meshIndex]->mNumVertices * sizeof(aiVector3D), scene->mMeshes[meshIndex]->mVertices, GL_STATIC_DRAW);

	// setup vertex shader attribute bindings (connecting current <position> buffer to associated 'in' variable in vertex shader)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);


	// setup vao for tex co ord attribute
	glGenBuffers(1, &meshTexCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, meshTexCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, scene->mMeshes[meshIndex]->mNumVertices * sizeof(aiVector3D), scene->mMeshes[meshIndex]->mTextureCoords[0], GL_STATIC_DRAW);

	// setup vertex shader attribute bindings 
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, (const GLvoid*)0);


	// setup vbo for position attribute
	glGenBuffers(1, &normalsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
	glBufferData(GL_ARRAY_BUFFER, scene->mMeshes[meshIndex]->mNumVertices * sizeof(aiVector3D), scene->mMeshes[meshIndex]->mNormals, GL_STATIC_DRAW);

	// setup vertex shader attribute bindings (connecting current <position> buffer to associated 'in' variable in vertex shader)
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);



	// setup vbo for principle axis (pa) index buffer
	glGenBuffers(1, &meshFaceVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshFaceVBO);

	//setup contiguous array
	unsigned int numBytes = scene->mMeshes[meshIndex]->mNumFaces * 3 * sizeof(unsigned int);
	unsigned int *faceIndexArray = (unsigned int*)malloc(numBytes);


	for (int f = 0, dstIndex = 0; f < scene->mMeshes[meshIndex]->mNumFaces; f++) {

		unsigned int* I = scene->mMeshes[meshIndex]->mFaces[f].mIndices;

		faceIndexArray[dstIndex++] = I[0];
		faceIndexArray[dstIndex++] = I[1];
		faceIndexArray[dstIndex++] = I[2];

	}

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numBytes, faceIndexArray, GL_STATIC_DRAW);

	// enable vertex buffers for principle axes rendering (vertex positions and colour buffers)
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(4);

	// unbind principle axes VAO

	glBindVertexArray(0);

	/*glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDeleteBuffers(1, &meshPosBuffer);
	glDeleteBuffers(1, &meshTexCoordBuffer);
	glDeleteBuffers(1, &meshFaceVBO);*/

	numFaces = scene->mMeshes[meshIndex]->mNumFaces;

	//glGenVertexArrays(1, &normalsVAO);
	//glBindVertexArray(normalsVAO);


	//// setup vbo for position attribute
	//glGenBuffers(1, &normalsBuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
	//glBufferData(GL_ARRAY_BUFFER, scene->mMeshes[meshIndex]->mNumVertices * sizeof(aiVector3D), scene->mMeshes[meshIndex]->mNormals, GL_STATIC_DRAW);

	//// setup vertex shader attribute bindings (connecting current <position> buffer to associated 'in' variable in vertex shader)
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);


	//// setup vbo for position attribute
	//glGenBuffers(1, &lightSourceBuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, lightSourceBuffer);
	//glBufferData(GL_ARRAY_BUFFER, 2, lightSourceArray, GL_STATIC_DRAW);

	//// setup vertex shader attribute bindings (connecting current <position> buffer to associated 'in' variable in vertex shader)
	//glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);


	//glGenBuffers(1, &lightColourBuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, lightColourBuffer);
	//glBufferData(GL_ARRAY_BUFFER, 2, lightColourArray, GL_STATIC_DRAW);

	//// setup vertex shader attribute bindings (connecting current <position> buffer to associated 'in' variable in vertex shader)
	//glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);


	//// setup vbo for position attribute
	//glGenBuffers(1, &attenuationBuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, attenuationBuffer);
	//glBufferData(GL_ARRAY_BUFFER, 2, attenuationArray, GL_STATIC_DRAW);

	//// setup vertex shader attribute bindings (connecting current <position> buffer to associated 'in' variable in vertex shader)
	//glVertexAttribPointer(8, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);

}

void modelVBO::render() {

	glBindVertexArray(meshVAO);

	glDrawElements(GL_TRIANGLES, numFaces * 3, GL_UNSIGNED_INT, (GLvoid*)0);

}