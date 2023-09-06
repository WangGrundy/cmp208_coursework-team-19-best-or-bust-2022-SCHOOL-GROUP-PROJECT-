#pragma once
#include "BaseScene.h"

class Level_2 : public BaseScene
{
public:
	Level_2(gef::Platform& platformInput, gef::Renderer3D* renderer3DInput, gef::SpriteRenderer* spriteRendererInput, PrimitiveBuilder* primitiveBuilderInput, gef::InputManager* inputManagerInput, gef::AudioManager* audioManagerInput, int& currentSceneInput) :
		BaseScene(platformInput, renderer3DInput, spriteRendererInput, primitiveBuilderInput, inputManagerInput, audioManagerInput),
		currentSceneNum(currentSceneInput)
	{
		Init();
	};

	~Level_2() {
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

	gef::Vector4 OriginLeft(gef::Vector4 position, gef::Vector4 size); //helper

	gef::Font* font;
	int& currentSceneNum;

	b2World* world;
	gef::Texture* buttonPressedTexture;
	Player* player;

	std::vector<Platform*> floor;
	std::vector<Platform*> wall;
	std::vector<Coin*> coin;
	Button* button;
	Button* button2;
	Button* button3;
	Goal* goal;
	Camera* camera1;

	bool musicPlaying;
	float winTimer;
	gef::Sprite winBackgroundSprite;
	gef::Texture* winBackgroundTexture;
	gef::Sprite winText;
	gef::Texture* winTextTexture;
	gef::Sprite backgroundSprite;
	gef::Texture* backgroundTexture;
};