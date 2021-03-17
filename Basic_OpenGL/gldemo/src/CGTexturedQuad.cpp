
#include "CGTexturedQuad.h"
#include "shader_setup.h"
#include "texture_loader.h"


using namespace std;
using namespace CoreStructures;


// Geometry data for textured quad (this is rendered directly as a triangle strip)

static float quadPositionArray[] = {

	0.0f, 0.5f, 0.0f, 1.0f, //0
	-0.5f, -0.5f, 0.0f, 1.0f, //1
	0.5f, -0.5f, 0.0f, 1.0f, //2
	0.0f, 0.0f, 1.0f, 1.0f //3

	/*-0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, 0.5f, 0.0f, 1.0f,
	0.5f, 0.5f, 0.0f, 1.0f*/
};

static float quadTextureCoordArray[] = {

	0.0f, 1.0f,
	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f
};

static GLubyte quadVertexIndices[] = { 0, 1, 2,
3, 0, 1 };

static GLuint quadVertexBuffer, quadTextureCoordBuffer, quadVertexIndicesVBO;

//
// Private API
//

void CGTexturedQuad::loadShader() {

	// setup shader for textured quad
	//quadShader = setupShaders(string("Shaders\\basic_texture.vs"), string("Shaders\\basic_texture.fs"));
}


void CGTexturedQuad::setupVAO() {

	// setup VAO for textured quad object
	glGenVertexArrays(1, &quadVertexArrayObj);
	glBindVertexArray(quadVertexArrayObj);


	// setup vbo for position attribute
	glGenBuffers(1, &quadVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quadVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadPositionArray), quadPositionArray, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);


	// setup vbo for texture coord attribute
	glGenBuffers(1, &quadTextureCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quadTextureCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadTextureCoordArray), quadTextureCoordArray, GL_STATIC_DRAW);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_TRUE, 0, (const GLvoid*)0);


	glGenBuffers(1, &quadVertexIndicesVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadVertexIndicesVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadVertexIndices), quadVertexIndices, GL_STATIC_DRAW);

	
	// enable vertex buffers for textured quad rendering (vertex positions and colour buffers)
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(3);

	// unbind textured quad VAO
	glBindVertexArray(0);
}


CGTexturedQuad::CGTexturedQuad(const char* texturePath) {

	loadShader();
	setupVAO();

	// Load texture
	texture = fiLoadTexture(texturePath, TextureProperties(true));
}


CGTexturedQuad::CGTexturedQuad(GLuint initTexture) {

	loadShader();
	setupVAO();

	texture = initTexture;
}


CGTexturedQuad::~CGTexturedQuad() {

	// unbind textured quad VAO
	glBindVertexArray(0);

	// unbind VBOs
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDeleteBuffers(1, &quadVertexBuffer);
	glDeleteBuffers(1, &quadTextureCoordBuffer);


	glDeleteVertexArrays(1, &quadVertexArrayObj);

	glDeleteShader(quadShader);
}


void CGTexturedQuad::render(const GUMatrix4& T) {

	static GLint mvpLocation = glGetUniformLocation(quadShader, "mvpMatrix");

	glUseProgram(quadShader);
	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, (const GLfloat*)&(T.M));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glBindVertexArray(quadVertexArrayObj);

	// draw quad directly - no indexing needed
	glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_BYTE, (void*)0);

	

	// unbind VAO for textured quad
	glBindVertexArray(0);
}


