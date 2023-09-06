#pragma once
#include <system/application.h>
#include <maths/vector2.h>
#include "primitive_builder.h"
#include <graphics/mesh_instance.h>
#include "graphics/scene.h"
#include "input/input_manager.h"

#include <Player.h>
#include <Square.h>
#include <Platform.h>
#include <Lava.h>
#include <Goal.h>
#include <Button.h>
#include <Camera.h>
#include <Coin.h>
#include <CreateTextureFunction.h>

#include <assets/png_loader.h>
#include <graphics/image_data.h>
#include <graphics/texture.h>

class BaseScene
{
public:
	BaseScene(gef::Platform& platformInput, gef::Renderer3D* renderer3DInput, gef::SpriteRenderer* spriteRendererInput, PrimitiveBuilder* primitiveBuilderInput, gef::InputManager* inputManagerInput, gef::AudioManager* audioManagerInput)
		: platform(platformInput),
		renderer3D(renderer3DInput),
		spriteRenderer(spriteRendererInput),
		primitiveBuilder(primitiveBuilderInput),
		audioManager(audioManagerInput),
		inputManager(inputManagerInput)
	{
		nextScene = 0;
	}

	gef::Scene* LoadSceneAssets(gef::Platform& platform, const char* filename);
	gef::Mesh* GetMeshFromSceneAssets(gef::Scene* scene);

	virtual void Init() = 0;
	virtual void Update(float) = 0;
	virtual void Render(float) = 0;
	virtual void CleanUp() = 0;

protected:
	gef::Platform& platform;
	gef::Renderer3D* renderer3D;
	gef::SpriteRenderer* spriteRenderer;
	gef::InputManager* inputManager;
	gef::AudioManager* audioManager;
	PrimitiveBuilder* primitiveBuilder;
	int nextScene;
};