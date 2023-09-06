#pragma once
#include <system/application.h>
#include <maths/vector2.h>
#include "primitive_builder.h"
#include <graphics/mesh_instance.h>

#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/font.h>
#include <system/debug_log.h>
#include <graphics/renderer_3d.h>
#include <maths/math_utils.h>

#include "TitleScreen.h"
#include "Level_1.h"
#include "Level_2.h"
#include "BaseScene.h"

class SceneManager{
public:
	SceneManager(gef::Platform& platformInput, gef::Renderer3D* renderer3DInput, gef::SpriteRenderer* spriteRendererInput, PrimitiveBuilder* primitiveBuilderInput, gef::InputManager* inputManagerInput, gef::AudioManager* audioManagerInput)
		: platform(platformInput),
		renderer3D(renderer3DInput),
		spriteRenderer(spriteRendererInput),
		primitiveBuilder(primitiveBuilderInput),
		inputManager(inputManagerInput),
		audioManager(audioManagerInput)
	{
		SceneManagerInit();
	}

	void SceneManagerInit();
	void SceneManagerCleanUp();
	void UpdateFrameTime(float);
	
	void Init();
	void Update();
	void Render();
	void CleanUp();

	BaseScene* currentScene;
protected:	
	Level_1* level1;
	Level_2* level2;
	TitleScreen* titleScreen;
	float frameTime;

private:
	void PlayLevelCompleteSound();
	void GlobalControls();
	void ReCreateAllScenes();

	gef::Platform& platform;
	gef::Renderer3D* renderer3D;
	gef::SpriteRenderer* spriteRenderer;
	gef::InputManager* inputManager;
	gef::AudioManager* audioManager;
	PrimitiveBuilder* primitiveBuilder;

	std::vector<BaseScene*> sceneList;
	int currentSceneNum;
	int changeToScene;
	gef::Keyboard* keyboard;
};

