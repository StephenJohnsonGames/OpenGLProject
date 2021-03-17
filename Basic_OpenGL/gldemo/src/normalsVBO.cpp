#include "normalsVBO.h"
#include "modelVBO.h"


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





//vec3 Lcolour[2];
//Lcol[0] = vec3(0.0, 1.0, 0.0); // Light 0 colour (Green)
//Lcol[1] = vec3(1.0, 0.0, 0.0); // Light 1 colour (Red)
//
//
//vec3 L[2];
//L[0] = vec3(2.0, 0.0, 0.0); // Light source position 0
//L[1] = vec3(-2.0, 2.0, 1.0); // Light source position 1
//
//vec3 Lk[2];
//Lk[0] = vec3(1.0f, 0.1, 0.0); // attenuation properties for light 0
//Lk[1] = vec3(1.0f, 0.1, 0.0); // attenuation properties for light 1
//
//							  // Calculate brightness
//brightness = 0.0;
//
//for (int i = 0; i < 2; ++i) {
//
//	brightness += calcBrightnessForPointLight(vertexPos, vertexNormal, L[i], Lk[i]);


normalsVBO::normalsVBO(const aiScene *scene, int meshIndex) {

	glGenVertexArrays(1, &normalsVAO);
	glBindVertexArray(normalsVAO);


	// setup vbo for position attribute
	glGenBuffers(1, &normalsBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
	glBufferData(GL_ARRAY_BUFFER, scene->mMeshes[meshIndex]->mNumVertices * sizeof(aiVector3D), scene->mMeshes[meshIndex]->mNormals, GL_STATIC_DRAW);

	// setup vertex shader attribute bindings (connecting current <position> buffer to associated 'in' variable in vertex shader)
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);


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
