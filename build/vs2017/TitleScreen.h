#pragma once
#include "BaseScene.h"
#include <assets/png_loader.h>
#include <graphics/image_data.h>
#include <graphics/texture.h>
#include <cstdlib>
#include <input/touch_input_manager.h>

class TitleScreen : public BaseScene
{
	struct ButtonInfo {
		float middleX;
		float middleY;
		float width;
		float height;
		float xMin;
		float xMax;
		float yMin;
		float yMax;
	};

	enum MenuState { MAIN_MENU, PLAY, OPTION, QUIT, VOLUME_UP, VOLUME_DOWN, LEVEL_1, LEVEL_2, DIFFICULTY_EASY, DIFFICULTY_NORMAL, DIFFICULTY_HARD };

	enum Difficulty { EASY, NORMAL, HARD };

public:
	TitleScreen(gef::Platform& platformInput, gef::Renderer3D* renderer3DInput, gef::SpriteRenderer* spriteRendererInput, PrimitiveBuilder* primitiveBuilderInput, gef::InputManager* inputManagerInput, gef::AudioManager* audioManagerInput, int& currentSceneInput) :
		BaseScene(platformInput, renderer3DInput, spriteRendererInput, primitiveBuilderInput, inputManagerInput, audioManagerInput),
		currentSceneNum(currentSceneInput)
	{
		Init();
	};

	~TitleScreen() {
	}

	void Init() override;
	void Update(float frameTime) override;
	void Render(float frameTime) override;
	void CleanUp() override;
	
protected:

private:
	void DrawHUD();
	void UpdateWorld(float frameTime);
	void ProcessTouchInput();
	void CheckForButtons();
	bool CheckButtonDimentions(ButtonInfo x);
	void ProcessMenuStateAction();
	void SplashCreation();
	void MainMenuButtonCreation();
	void OptionMenuButtonCreation();
	void LoadTextures();
	std::string DifficultyToString();

	void MainMenuHUD();
	void OptionsHUD();

	gef::Font* font;

	b2World* world;
	Square* square;
	Camera* camera1;
	gef::VolumeInfo gameVolumeInfo;

	int& currentSceneNum;
	int optionalLevelStart;
	Difficulty optionalLevelDifficulty;
	float optionalVolume;

	gef::Texture* splashScreenTexture;
	gef::Texture* splashScreenBGTexture;
	gef::Texture* mainMenuBGTexture;
	gef::Sprite splashScreenSprite;
	gef::Sprite splashScreenBGSprite;
	gef::Sprite mainMenuBGSprite;

	gef::Texture* titleTexture;
	gef::Sprite titleSprite;
	
	//main menu
	gef::Texture* buttonTexture;
	gef::Sprite playButtonSprite;
	gef::Sprite optionButtonSprite;
	gef::Sprite quitButtonSprite;
	
	//option menu
	gef::Sprite level1Sprite;
	gef::Sprite level2Sprite;
	gef::Sprite volumeUpSprite;
	gef::Sprite volumeDownSprite;
	gef::Sprite difficultyEasySprite;
	gef::Sprite difficultyNormalSprite;
	gef::Sprite difficultyHardSprite;
	gef::Sprite backSprite;
	
	//main menu
	ButtonInfo level1ButtonInfo;
	ButtonInfo level2ButtonInfo;
	ButtonInfo volumeUpButtonInfo;
	ButtonInfo volumeDownButtonInfo;
	ButtonInfo difficultyEasyButtonInfo;
	ButtonInfo difficultyNormalButtonInfo;
	ButtonInfo difficultyHardButtonInfo;
	ButtonInfo backButtonInfo;

	//main menu
	ButtonInfo playButtonInfo;
	ButtonInfo optionButtonInfo;
	ButtonInfo quitButtonInfo;

	gef::Vector2 touchPosition;

	float splashScreenFadeTime , splashScreenFadeCounter;
	bool musicPlaying;
	Int32 activeTouchID;

	MenuState currentState;
};