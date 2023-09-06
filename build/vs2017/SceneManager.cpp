#include "SceneManager.h"

void SceneManager::SceneManagerInit() {

	//next scene
	changeToScene = 0;

	//Instantiate scenes
	titleScreen = new TitleScreen(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, changeToScene);
	level1 = new Level_1(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, changeToScene);
	level2 = new Level_2(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, changeToScene);

	//Put scenes into list
	sceneList.push_back(titleScreen); 
	sceneList.push_back(level1);
	sceneList.push_back(level2);

	//current scene
	currentSceneNum = 0;
	currentScene = sceneList[currentSceneNum];

	//controls to get back here
	keyboard = inputManager->keyboard();
}

void SceneManager::Init() {
	currentScene->Init();
}

void SceneManager::Update() {
	
	GlobalControls();
	
	//keep checking if we changed scene
	if (currentSceneNum != changeToScene) { //when current scene changes
		PlayLevelCompleteSound(); //play complete lvl sound if needed

		CleanUp();
		currentSceneNum = changeToScene; //set current scene Number to the changed scene number, as changed scene is the scene we want to get to

		//if we run out of scenes
		if (currentSceneNum > 2) {
			ReCreateAllScenes();
			currentSceneNum = 0; //go to main menu
			changeToScene = 0;
		}

		//move to next Scene
		currentScene = sceneList[currentSceneNum];
	}

	currentScene->Update(frameTime);
}

void SceneManager::Render() {
	currentScene->Render(frameTime);
}

void SceneManager::CleanUp() {
	currentScene->CleanUp();
}

void SceneManager::UpdateFrameTime(float ft) {
	frameTime = ft;
}

void SceneManager::SceneManagerCleanUp() {

	for (BaseScene* ele : sceneList) {
		delete ele;
	}
	sceneList.clear();
	audioManager->UnloadAllSamples();
	audioManager->UnloadMusic();
}

void SceneManager::PlayLevelCompleteSound() {
	if (currentSceneNum == 0) { //make sure level complete sound doesn't play when menu play is activated
		return;
	}
}

void SceneManager::GlobalControls() {
	if (currentSceneNum == 0) { //update the keyboard when we are not in a level
		keyboard->Update();
	}

	//go to main title screen
	if (keyboard->IsKeyDown(keyboard->KC_ESCAPE) && currentSceneNum != 0) {

		ReCreateAllScenes(); //recreate all scenes

		//current scene
		currentSceneNum = 0; //go to main menu
		changeToScene = 0;
		currentScene = sceneList[currentSceneNum];
	}
}

void SceneManager::ReCreateAllScenes() {

	sceneList.erase(sceneList.begin() + 1, sceneList.end()); //erase all scenes except title screen (saved variables here)
	level1 = new Level_1(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, changeToScene);
	level2 = new Level_2(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, changeToScene);

	//Put scenes into list again
	sceneList.push_back(level1);
	sceneList.push_back(level2);
}