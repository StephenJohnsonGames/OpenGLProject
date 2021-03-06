
#include <glew/glew.h>
#include <glew/wglew.h>
#include <GL\freeglut.h>
#include <CoreStructures\CoreStructures.h>
#include <iostream>
#include <string>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

//#include <al.h>					//used for sound
//#include <alc.h>
//#include <efx.h>
//#include <efx-creative.h>
//#include <EFX-Util.h>
//#include <xram.h>
#include "CGPrincipleAxes.h"
#include "CGTexturedQuad.h"
#include "texture_loader.h"
#include "aiWrapper.h"
#include "sceneVBOs.h"
#include "shader_setup.h"
#include "GURiffModel.h"
#include <mmreg.h>




using namespace std;
using namespace CoreStructures;

#pragma region Scene variables and resources

// Variables needed to track where the mouse pointer is so we can determine which direction it's moving in
int								mouse_x, mouse_y;
bool							mDown = false;

GUClock							*mainClock = nullptr;

//
// Main scene resources
//
GUPivotCamera					*mainCamera = nullptr;
CGPrincipleAxes					*principleAxes = nullptr;
CGTexturedQuad					*texturedQuad = nullptr;
const aiScene					*aiBeast;
const aiScene					*aiLevel;
const aiScene					*aiShip1;
const aiScene					*aiShip2;
const aiScene					*aiStartline;
GLuint							beastTexture;
GLuint							levelTexture;
GLuint							ship1Texture;
GLuint							ship2Texture;
GLuint							startlineTexture;

sceneVBOs						*beastScene;
sceneVBOs						*levelScene;
sceneVBOs						*ship1Scene;
sceneVBOs						*ship2Scene;
sceneVBOs						*startlineScene;

GLuint							meshShader;
GLuint							waterShader;
GLuint							ship1Shader;
GLuint							ship2Shader;

GLuint							LocLcolour;
GLuint							LocL;
GLuint							LocLk;
GLuint							LocLdirect;
GLuint							LocLdisp;

//GLuint							LocModelMat;
//GLuint							LocViewMat;
//GLuint							LocProjMat;


GLuint							LocLtype;

GUVector4						camFocalPoint = GUVector4(0.0f, 60.0f, -120.0f, 1.0f);
bool							forwardDown = false;
bool							backwardDown = false;
bool							leftDown = false;
bool							rightDown = false;
bool							upDown = false;
bool							downDown = false;

float							beastX = 0.0f;
float							beastY = 0.0f;
float							beastZ = 0.0f;

float							playerX = 0.0f;
float							playerY = 0.0f;
float							playerZ = 0.0f;

float							enemyX = 0.0f;
float							enemyY = 0.0f;
float							enemyZ = 0.0f;
//float							enemyStartmark = -500.0f;

#pragma endregion


#pragma region Function Prototypes

void init(int argc, char* argv[]); // Main scene initialisation function
void update(void); // Main scene update function
void display(void); // Main scene render function

// Event handling functions
void mouseButtonDown(int button_id, int state, int x, int y);
void mouseMove(int x, int y);
void mouseWheel(int wheel, int direction, int x, int y);
void keyDown(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void closeWindow(void);
void reportContextVersion(void);
void reportExtensions(void);

#pragma endregion

//void setDemoVBOs(const aiScene *scene ){
//
//	// setup VAO for principle axes object
//
//}

int main(int argc, char* argv[])
{
	init(argc, argv);
	glutMainLoop();
	
	// Stop clock and report final timing data
	if (mainClock) {

		mainClock->stop();
		mainClock->reportTimingData();


		mainClock->release();
	}

    return 0;
}


void init(int argc, char* argv[]) {

	// Initialise FreeGLUT
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE/* | GLUT_MULTISAMPLE*/);
	glutSetOption(GLUT_MULTISAMPLE, 4);

	// Setup window
	int windowWidth = 1920;
	int windowHeight = 1080;
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(64, 64);
	glutCreateWindow("Basic_3D");
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	// Register callback functions
	glutIdleFunc(update); // Main scene update function
	glutDisplayFunc(display); // Main render function
	glutKeyboardFunc(keyDown); // Key down handler
	glutKeyboardUpFunc(keyUp);
	glutMouseFunc(mouseButtonDown); // Mouse button handler
	glutMotionFunc(mouseMove); // Mouse move handler
	glutMouseWheelFunc(mouseWheel); // Mouse wheel event handler
	glutCloseFunc(closeWindow); // Main resource cleanup handler


	// Initialise glew
	glewInit();

	// Initialise OpenGL...

	wglSwapIntervalEXT(0);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glFrontFace(GL_CCW); // Default anyway

	// Setup colour to clear the window
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Setup main camera
	float viewportAspect = (float)windowWidth / (float)windowHeight;
	mainCamera = new GUPivotCamera(-20.0f, 0.0f, 150.0f, 55.0f, viewportAspect, 0.1f);

	principleAxes = new CGPrincipleAxes();

	glDisable(GL_CULL_FACE);
	texturedQuad = new CGTexturedQuad("..\\..\\Common2\\Resources\\Textures\\bumblebee.png");
	
	/*aiBeast = aiImportModel(string("Beast.obj"),
		aiProcess_GenSmoothNormals |
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);
    
	beastScene = new sceneVBOs(aiBeast);

	beastTexture = fiLoadTexture("..\\..\\Common2\\Resources\\Textures\\beast_texture.bmp", TextureProperties(false));*/

	aiLevel = aiImportModel(string("Level.obj"),
		aiProcess_GenSmoothNormals |
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	levelScene = new sceneVBOs(aiLevel);
	
	levelTexture = fiLoadTexture("..\\..\\Common2\\Resources\\Textures\\LevelTexture.png", TextureProperties(false));

	aiShip1 = aiImportModel(string("Ship1.obj"),
		aiProcess_GenSmoothNormals |
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	ship1Scene = new sceneVBOs(aiShip1);

	ship1Texture = fiLoadTexture("..\\..\\Common2\\Resources\\Textures\\Metal.png", TextureProperties(false));

	aiShip2 = aiImportModel(string("Ship2.obj"),
		aiProcess_GenSmoothNormals |
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	ship2Scene = new sceneVBOs(aiShip2);

	ship2Texture = fiLoadTexture("..\\..\\Common2\\Resources\\Textures\\WaterAlpha.png", TextureProperties(false));

	aiStartline = aiImportModel(string("Startline.obj"),
		aiProcess_GenSmoothNormals |
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	startlineScene = new sceneVBOs(aiStartline);

	startlineTexture = fiLoadTexture("..\\..\\Common2\\Resources\\Textures\\Startline.png", TextureProperties(false));

	
	/*aiMaterial* mat = aiBeast->mMaterials[aiBeast->mMeshes[0]->mMaterialIndex];
	aiColor3D color(0.0f, 0.0f, 0.0f);
	mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);*/

	meshShader = setupShaders(string("shaders\\mesh_shader.vs"), string("shaders\\mesh_shader.fs"));
	
	waterShader = setupShaders(string("shaders\\water_shader.vs"), string("shaders\\water_shader.fs"));
	ship1Shader = setupShaders(string("shaders\\ship1_shader.vs"), string("shaders\\ship1_shader.fs"));
	ship2Shader = setupShaders(string("shaders\\ship2_shader.vs"), string("shaders\\ship2_shader.fs"));

	// Setup and start the main clock
	mainClock = new GUClock();

	LocLcolour = glGetUniformLocation(meshShader, "Lcolour");
	LocL = glGetUniformLocation(meshShader, "L");
	LocLk = glGetUniformLocation(meshShader, "Lk");
	LocLdirect = glGetUniformLocation(meshShader, "Ldirect");
	LocLdisp = glGetUniformLocation(meshShader, "Ldisp");
	
	LocLtype = glGetUniformLocation(meshShader, "Ltype");


	float L[] = { 200.0f, -500.0f, 0.0f, 
				0.0f, -500.0f, 0.0f, 
				-1500.0f, 200.0f, 0.0f, 
				0.0f, 50.0f, -200.0f, 
				0.0f, 50.0f, 0.0f, 
				0.0f, 50.0f, 200.0f };

	glUseProgram(meshShader);
	glUniform3fv(LocL, 6, (GLfloat*)&L);

	float Lk[] = { 1.0f, 0.15f, 0.0f, 
				1.0f, 0.15f, 0.0f, 
				1.0f, 0.0005f, 0.0f, 
				1.0f, 0.0005f, 0.0f, 
				1.0f, 0.0005f, 0.0f, 
				1.0f, 0.0005f, 0.0f };

	glUseProgram(meshShader);
	glUniform3fv(LocLk, 6, (GLfloat*)&Lk);

	float Lcolour[] = { 0.0f, 0.0f, 1.0f, 
				0.0f, 1.0f, 0.0f, 
				0.7f, 0.7f, 0.7f, 
				0.0f, 0.7f, 0.7f, 
				0.3f, 0.0f, 0.3f, 
				1.0f, 1.0f, 1.0f };

	glUseProgram(meshShader);
	glUniform3fv(LocLcolour, 6, (GLfloat*)&Lcolour);

	float Ldirect[] = { 0.0f, 1.0f, 0.0f,
				0.0f, 1.0f, 0.0f,
				0.0f, -1.0f, 0.0f,
				0.0f, -1.0f, 0.0f,
				0.0f, -1.0f, 0.0f, 
				0.0f, -1.0f, 0.0f };

	glUseProgram(meshShader);
	glUniform3fv(LocLdirect, 6, (GLfloat*)&Ldirect);

	float Ldisp[] = { 3.142 / 180.0 * 10.0, 
					3.142 / 180.0 * 10.0, 
					3.142 / 180.0 * 20.0, 
					3.142 / 180.0 * 10.0, 
					3.142 / 180.0 * 50.0, 
					3.142 / 180.0 * 30.0 };

	glUseProgram(meshShader);
	glUniform1fv(LocLdisp, 6, (GLfloat*)&Ldisp);

	// 0 - Point Light
	// 1 - Directional
	// 2 - Spotlight

	int Ltype[] = {0, 0, 0, 1, 1, 2 };

	glUseProgram(meshShader);
	glUniform1iv(LocLtype, 6, (GLint*)Ltype);


	/*LocModelMat = glGetUniformLocation(meshShader, "modelMatrix");
	glUseProgram(meshShader);
	glUniformMatrix4fv(LocModelMat, 1, GL_FALSE, (const GLfloat*)&(modelMatrix));
	glUniformMatrix2x4fv(

	static GLint mvpLocation = glGetUniformLocation(meshShader, "viewMatrix");
	glUseProgram(meshShader);
	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, (const GLfloat*)&(T.M));

	static GLint mvpLocation = glGetUniformLocation(meshShader, "projMatrix");
	glUseProgram(meshShader);
	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, (const GLfloat*)&(T.M));*/

	/////////////////////////////////////////////		SOUND		//////////////////////////////////////////////////////////////////////////

	//ALCdevice* alcDevice = alcOpenDevice(NULL);
	//ALCcontext* alcContext = nullptr;

	//alcContext = alcCreateContext(alcDevice, NULL);
	//alcMakeContextCurrent(alcContext);

	//ALuint buffer1;
	//alGenBuffers(1, &buffer1);

	//// GURiffModel encapsulates the contents of a RIFF file intro addressable ‘chunks’.  
	////The main chunks we’re interested in are the format (‘fmt ‘) and data (‘data’) chunks.  

	//auto mySoundData = new GURiffModel("Ambient_Race_Track_FINAL.wav");

	//RiffChunk formatChunk = mySoundData->riffChunkForKey(' tmf');
	//RiffChunk dataChunk = mySoundData-> riffChunkForKey('atad');

	//// Copy to wave format structure (included in the Windows SDL mmreg.h header).  This represents the format information of the audio data.
	//WAVEFORMATEXTENSIBLE wv;
	//memcpy_s(&wv, sizeof(WAVEFORMATEXTENSIBLE), formatChunk.data, formatChunk.chunkSize);

	//alBufferData(
	//	buffer1,
	//	AL_FORMAT_MONO16,
	//	(ALvoid*)dataChunk.data,
	//	(ALsizei)dataChunk.chunkSize,
	//	(ALsizei)wv.Format.nSamplesPerSec
	//);

	//ALuint source1;

	//alGenSources(1, &source1);

	////Attach buffer1 to source1
	//alSourcei(source1, AL_BUFFER, buffer1);

	//alSourcei(source1, AL_LOOPING, AL_TRUE);
	//alSourcef(source1, AL_GAIN, 10.0f);

	////setup source1 location, velocity and direction
	//alSource3f(source1, AL_POSITION, 10.0f, 0.0f, 0.0f);
	//alSource3f(source1, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
	//alSource3f(source1, AL_DIRECTION, 0.0f, 0.0f, 0.0f);

	//auto cameraLocation = mainCamera->cameraLocation();

	//ALfloat listenerVel[] = { 0.0f, 0.0f, 0.0f };
	//ALfloat listenerOri[] = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f };

	//alListenerfv(AL_POSITION, (ALfloat*)(&cameraLocation));
	//alListenerfv(AL_VELOCITY, listenerVel);
	//alListenerfv(AL_ORIENTATION, listenerOri);

	//alSourcePlay(source1);


}

// Main scene update function (called by FreeGLUT's main event loop every frame) 
void update(void) {

	if (forwardDown == true) {
		playerZ = playerZ - 0.1f;
		camFocalPoint.z = playerZ + 120.0f;
		//camFocalPoint.x = 3.142 / 180.0 * 10.0f;
	

	}
	
	if (backwardDown == true) {
		playerZ = playerZ + 0.1f;
		camFocalPoint.z = playerZ + 120.0f;
	}

	if (leftDown == true) {
		playerX = playerX - 0.1f;
		camFocalPoint.x = playerX;

	}
	if (rightDown == true) {
		playerX = playerX + 0.1f;
		camFocalPoint.x = playerX;
	}

	if (upDown == true) {
		playerY = playerY + 0.1f;
		camFocalPoint.y = playerY;
	}
	if (downDown == true) {
		playerY = playerY - 0.1f;
		camFocalPoint.y = playerY;
	}
	// Update clock
	mainClock->tick();

	// Redraw the screen
	display();

	// Update the window title to show current frames-per-second and seconds-per-frame data
	char timingString[256];
	sprintf_s(timingString, 256, "SJ_OpenGL. Average fps: %.0f; Average spf: %f", mainClock->averageFPS(), mainClock->averageSPF() / 1000.0f);
	glutSetWindowTitle(timingString);

	if (enemyZ < 2300.0f) {
		enemyZ++;
	}
	else {
		enemyZ = -499.0f;

	}
	
	}


void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set viewport to the client area of the current window
	glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

	// Get view-projection transform as a GUMatrix4
	GUMatrix4 T = mainCamera->projectionTransform() * mainCamera->viewTransform() * GUMatrix4::translationMatrix(-playerX, -playerY, -playerZ);


	if (principleAxes)
		principleAxes->render(T);

	glBindTexture(GL_TEXTURE_2D, ship1Texture);
	glEnable(GL_TEXTURE_2D);

	static GLint mvpLocation2 = glGetUniformLocation(meshShader, "mvpMatrix");
	glUseProgram(meshShader);
	//comes in as floats so i can manipulate with keybinds
	GUMatrix4 modelMat = GUMatrix4::translationMatrix(playerX, playerY, playerZ) * GUMatrix4::scaleMatrix(1.0f, 1.0f, 1.0f) * GUMatrix4::rotationMatrix(0.0f, 3.142 / 180.0 * 90.0f, 0.0f);
	GUMatrix4 mvpMatrix = T * modelMat;
	glUniformMatrix4fv(mvpLocation2, 1, GL_FALSE, (const GLfloat*)&(mvpMatrix.M));

	ship1Scene->render();


	// Render example model loaded from obj file
	glBindTexture(GL_TEXTURE_2D, levelTexture);
	glEnable(GL_TEXTURE_2D);


	static GLint mvpLocation4 = glGetUniformLocation(meshShader, "mvpMatrix");
	glUseProgram(meshShader);
	modelMat = GUMatrix4::translationMatrix(0.0f, -20.0f, 0.0f) * GUMatrix4::scaleMatrix(4.0f, 4.0f, 4.0f);
	mvpMatrix = T * modelMat;
	glUniformMatrix4fv(mvpLocation4, 1, GL_FALSE, (const GLfloat*)&(mvpMatrix.M));


	levelScene->render();
	

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_FALSE);

	//glColor4f(0.0f, 0.0f, 0.0f, 0.2f);

	glBindTexture(GL_TEXTURE_2D, ship2Texture);
	glEnable(GL_TEXTURE_2D);

	static GLint mvpLocation3 = glGetUniformLocation(meshShader, "mvpMatrix");
	glUseProgram(meshShader);
	//comes in as floats so i can manipulate with keybinds
	modelMat = GUMatrix4::translationMatrix(enemyX, enemyY, enemyZ - 1000.0f) * GUMatrix4::scaleMatrix(1.0f, 1.0f, 1.0f) * GUMatrix4::rotationMatrix(0.0f, 0.0f, 0.0f);
	mvpMatrix = T * modelMat;
	glUniformMatrix4fv(mvpLocation3, 1, GL_FALSE, (const GLfloat*)&(mvpMatrix.M));

	ship2Scene->render();

	glBindTexture(GL_TEXTURE_2D, startlineTexture);
	glEnable(GL_TEXTURE_2D);

	static GLint mvpLocation5 = glGetUniformLocation(meshShader, "mvpMatrix");
	glUseProgram(meshShader);
	//comes in as floats so i can manipulate with keybinds
	modelMat = GUMatrix4::translationMatrix(0.0f, -10.0f, -40.0f) * GUMatrix4::scaleMatrix(1.0f, 1.0f, 1.0f) * GUMatrix4::rotationMatrix(0.0f, 0.0f, 0.0f);
	mvpMatrix = T * modelMat;
	glUniformMatrix4fv(mvpLocation5, 1, GL_FALSE, (const GLfloat*)&(mvpMatrix.M));

	startlineScene->render();

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);

	/*if (texturedQuad)
	texturedQuad->render(T);*/

	//glBindTexture(GL_TEXTURE_2D, beastTexture);
	//glEnable(GL_TEXTURE_2D);

	//static GLint mvpLocation = glGetUniformLocation(meshShader, "mvpMatrix");
	//glUseProgram(meshShader);
	////comes in as floats so i can manipulate with keybinds
	//GUMatrix4 modelMat = GUMatrix4::translationMatrix(beastX, beastY, beastZ) * GUMatrix4::scaleMatrix(0.0f, 0.0f, 0.0f);
	//GUMatrix4 mvpMatrix = T * modelMat;
	//glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, (const GLfloat*)&(mvpMatrix.M));

	//beastScene->render();


	glutSwapBuffers();
}



#pragma region Event handling functions

void mouseButtonDown(int button_id, int state, int x, int y) {

	if (button_id == GLUT_LEFT_BUTTON) {

		if (state == GLUT_DOWN) {

			mouse_x = x;
			mouse_y = y;

			mDown = true;

		}
		else if (state == GLUT_UP) {

			mDown = false;
		}
	}
}


void mouseMove(int x, int y) {

	int dx = x - mouse_x;
	int dy = y - mouse_y;

	if (mainCamera) {

		mainCamera->transformCamera((float)-dy, (float)-dx, 0.0f);

		auto cameraLocation = mainCamera->cameraLocation();
		//alListenerfv(AL_POSITION, (ALfloat*)(&cameraLocation));
		cameraLocation.normalise();
		//ALfloat orientation[] = { -cameraLocation.x, -cameraLocation.y, -cameraLocation.z, 0.0f, 1.0f, 0.0f };
		//alListenerfv(AL_ORIENTATION, orientation);

	}

		mouse_x = x;
		mouse_y = y;

	
}


void mouseWheel(int wheel, int direction, int x, int y) {

	if (mainCamera) {

		if (direction<0)
			mainCamera->scaleCameraRadius(1.1f);
		else if (direction>0)
			mainCamera->scaleCameraRadius(0.9f);

		auto cameraLocation = mainCamera->cameraLocation();
		//alListenerfv(AL_POSITION, (ALfloat*)(&cameraLocation));

		cameraLocation.normalise();
		//ALfloat orientation[] = { -cameraLocation.x, -cameraLocation.y, -cameraLocation.z, 0.0f, 1.0f, 0.0f };
		//alListenerfv(AL_ORIENTATION, orientation);

	}
}


void keyDown(unsigned char key, int x, int y) {

	// Toggle fullscreen (This does not adjust the display mode however!)
	if (key == 'f')
		glutFullScreenToggle();

	if (key == 'w') {

		forwardDown = true;
	}
	if (key == 's') {

		backwardDown = true;
	}
	if (key == 'a') {

		leftDown = true;
	}
	if (key == 'd') {

		rightDown = true;
	}
	if (key == 'q') {

		upDown = true;
	}
	if (key == 'e') {

		downDown = true;
	}

}

void keyUp(unsigned char key, int x, int y) {

	if (key == 'w') {
		forwardDown = false;
	}
	if (key == 's') {
		backwardDown = false;

	}
	if (key == 'a') {

		leftDown = false;
	}
	if (key == 'd') {

		rightDown = false;
	}
	if (key == 'q') {

		upDown = false;
	}
	if (key == 'e') {

		downDown = false;
	}
}

void closeWindow(void) {

	// Clean-up scene resources

	if (mainCamera)
		mainCamera->release();

	if (principleAxes)
		principleAxes->release();

	if (texturedQuad)
		texturedQuad->release();



	//if (exampleModel)
	//	exampleModel->release();
}


#pragma region Helper Functions

void reportContextVersion(void) {

	int majorVersion, minorVersion;

	glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

	cout << "OpenGL version " << majorVersion << "." << minorVersion << "\n\n";
}

void reportExtensions(void) {

	cout << "Extensions supported...\n\n";

	const char *glExtensionString = (const char *)glGetString(GL_EXTENSIONS);

	char *strEnd = (char*)glExtensionString + strlen(glExtensionString);
	char *sptr = (char*)glExtensionString;

	while (sptr < strEnd) {

		int slen = (int)strcspn(sptr, " ");
		printf("%.*s\n", slen, sptr);
		sptr += slen + 1;
	}
}

#pragma endregion


#pragma endregion

