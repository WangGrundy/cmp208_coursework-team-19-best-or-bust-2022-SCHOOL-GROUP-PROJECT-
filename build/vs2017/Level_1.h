#pragma once
#include "BaseScene.h"

class Level_1 : public BaseScene
{
public:
	Level_1(gef::Platform& platformInput, gef::Renderer3D* renderer3DInput, gef::SpriteRenderer* spriteRendererInput, PrimitiveBuilder* primitiveBuilderInput, gef::InputManager* inputManagerInput, gef::AudioManager* audioManagerInput, int& currentSceneInput) :
		BaseScene(platformInput, renderer3DInput, spriteRendererInput, primitiveBuilderInput, inputManagerInput, audioManagerInput),
		currentSceneNum(currentSceneInput)
	{
		Init();
	};

	~Level_1() {
		CleanUp();
	}

	void Init() override;
	void Update(float frameTime) override;
	void Render(float frameTime) override;
	void CleanUp() override;
	
protected:

private:
	void DrawHUD();
	void SetupLights();
	void UpdateWorld(float frameTime);
	void UpdateCollision();
	void InitObjects();
	void InitSprites();
	void InitModels();

	gef::Font* font;

	int& currentSceneNum;

	b2World* world;
	Player* player;

	std::vector<Platform*> floor;
	std::vector<Platform*> wall;
	Platform* buttonWall;

	std::vector<Coin*> coin;
	std::vector<Lava*> lava;
	Button* button;
	Button* button2;
	Goal* goal;

	Camera* camera1;

	bool musicPlaying;

	float winTimer;

	float fps;

	gef::Texture* buttonPressedTexture;
	gef::Texture* backgroundTexture;
	gef::Texture* winTextTexture;
	gef::Sprite backgroundSprite;
	gef::Sprite winBackgroundSprite;
	gef::Sprite winText;
};