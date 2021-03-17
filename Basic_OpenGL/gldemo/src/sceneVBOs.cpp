#include "sceneVBOs.h"

sceneVBOs::sceneVBOs(const aiScene* scene) {

	numModels = scene->mNumMeshes;

	modelArray = (modelVBO**)malloc(numModels * sizeof(modelVBO*));

	for (int i = 0; i < numModels; i++) {

		modelArray[i] = new modelVBO(scene, i);
	}

}

void sceneVBOs::render() {

	for (int i = 0; i < numModels; i++) {

		modelArray[i]->render();

	}

}