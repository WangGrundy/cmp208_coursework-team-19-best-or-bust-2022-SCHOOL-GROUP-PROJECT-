#include "TitleScreen.h"

void TitleScreen::Init() {

	//init font and world
	font = new gef::Font(platform);
	font->Load("comic_sans");
	world = new b2World(b2Vec2(0, 0));

	//custom level start, could add to it later with params
	gameVolumeInfo.volume = 0.5;
	optionalLevelStart = 1;
	optionalLevelDifficulty = Difficulty::EASY;
	optionalVolume = 100;
	currentState = MAIN_MENU;
	activeTouchID = -1;
	splashScreenFadeTime = 3.0f;
	splashScreenFadeCounter = 0.0f;
	splashScreenFadeCounter = splashScreenFadeTime;
	musicPlaying = false;

	LoadTextures(); //load all of the textures before creating UI
	SplashCreation();
	MainMenuButtonCreation();
	OptionMenuButtonCreation();

	audioManager->LoadMusic("sounds/menu_music.wav", platform);

	// make sure if there is a panel to detect touch input, then activate it
	if (inputManager && inputManager->touch_manager() && (inputManager->touch_manager()->max_num_panels() > 0)) {
		inputManager->touch_manager()->EnablePanel(0);
	}
	audioManager->PlayMusic();
}

void TitleScreen::Update(float frameTime) {
	
	gameVolumeInfo.volume = optionalVolume; //set volume to optional volume
	audioManager->SetMusicVolumeInfo(gameVolumeInfo); //set music volume info

	if (!musicPlaying) { //if no music is playing, start music.
		audioManager->LoadMusic("sounds/menu_music.wav", platform);
		musicPlaying = true;
		audioManager->PlayMusic();
	}

	UpdateWorld(frameTime);
	inputManager->Update();
	ProcessTouchInput();
}

void TitleScreen::Render(float frameTime) {
	renderer3D->Begin();
	renderer3D->End();

	spriteRenderer->Begin(false);

	if (splashScreenFadeCounter >= 0) {
		spriteRenderer->DrawSprite(splashScreenBGSprite);
		spriteRenderer->DrawSprite(splashScreenSprite);

		splashScreenFadeCounter -= frameTime;
	}
	
	DrawHUD();
	spriteRenderer->End();
}

void TitleScreen::DrawHUD()
{	
	if (currentState == MenuState::MAIN_MENU) {
		MainMenuHUD();
	}
	else if (currentState == MenuState::OPTION) {
		OptionsHUD();
	}
	else if (currentState == MenuState::QUIT) {

	}
}

void TitleScreen::MainMenuHUD() {

	//draw sprites
	if (splashScreenFadeCounter <= 0) { 
		spriteRenderer->DrawSprite(mainMenuBGSprite);
		spriteRenderer->DrawSprite(titleSprite);
		spriteRenderer->DrawSprite(playButtonSprite);
		spriteRenderer->DrawSprite(optionButtonSprite);
		spriteRenderer->DrawSprite(quitButtonSprite);
	}

	if (font && splashScreenFadeCounter <= 0)
	{
		//local variables to help adjust placement of text
		float tempWidthRatio = platform.width() / 40;
		float tempHeightRatio = platform.height() / 20;

		//button text
		font->RenderText(spriteRenderer, gef::Vector4(playButtonInfo.middleX - tempWidthRatio, playButtonInfo.middleY - tempHeightRatio * 3, 1.0f), 2.5f, 0xffffffff, gef::TJ_LEFT, "Play");
		font->RenderText(spriteRenderer, gef::Vector4(optionButtonInfo.middleX - tempWidthRatio * 2, optionButtonInfo.middleY - tempHeightRatio * 3, -0.9f), 2.5f, 0xffffffff, gef::TJ_LEFT, "Options");
		font->RenderText(spriteRenderer, gef::Vector4(quitButtonInfo.middleX - tempWidthRatio, quitButtonInfo.middleY - tempHeightRatio * 3, -0.9f), 2.5f, 0xffffffff, gef::TJ_LEFT, "Quit");
	}
}

void TitleScreen::OptionsHUD() {

	//draw sprites
	if (splashScreenFadeCounter <= 0) {
		spriteRenderer->DrawSprite(mainMenuBGSprite);
		spriteRenderer->DrawSprite(level1Sprite);
		spriteRenderer->DrawSprite(level2Sprite);
		spriteRenderer->DrawSprite(volumeUpSprite);
		spriteRenderer->DrawSprite(volumeDownSprite);
		spriteRenderer->DrawSprite(difficultyEasySprite);
		spriteRenderer->DrawSprite(difficultyNormalSprite);
		spriteRenderer->DrawSprite(difficultyHardSprite);
		spriteRenderer->DrawSprite(backSprite);
	}

	if (font && splashScreenFadeCounter <= 0)
	{
		//local variables to help adjust placement of text
		float buttonSeperationHeight = platform.height() / 10;
		float levelHeight = buttonSeperationHeight * 2;
		float volumeHeight = buttonSeperationHeight * 4;
		float difficultyHeight = buttonSeperationHeight * 6;
		float backHeight = buttonSeperationHeight * 8;

		float tempWidthRatio = platform.width() / 100;
		float tempHeightRatio = platform.height() / 100;
		float quaterScreenX = platform.width() / 4;
		float middleScreenX = platform.width() / 2;
		float threeForthsScreenX = quaterScreenX * 3;
		float oneSixthScreenX = platform.width() / 6;

		//button text -----

		//level 
		std::string tempLevel = "Current Level Selected: ";
		tempLevel += std::to_string(optionalLevelStart);
		font->RenderText(spriteRenderer, gef::Vector4(quaterScreenX, levelHeight - tempHeightRatio * 5, 1.0f), 1.8f, 0xffffffff, gef::TJ_CENTRE, tempLevel.c_str());
		font->RenderText(spriteRenderer, gef::Vector4(level1ButtonInfo.middleX , level1ButtonInfo.middleY - tempHeightRatio * 5, 1.0f), 2.5f, 0xffffffff, gef::TJ_CENTRE, "Level 1");
		font->RenderText(spriteRenderer, gef::Vector4(level2ButtonInfo.middleX, level2ButtonInfo.middleY - tempHeightRatio * 5, 1.0f), 2.5f, 0xffffffff, gef::TJ_CENTRE, "Level 2");

		//volume
		std::string tempVolume = "Music Volume: ";
		tempVolume += std::to_string(optionalVolume);
		tempVolume.resize(17); //resizes to 17 bytes
		font->RenderText(spriteRenderer, gef::Vector4(quaterScreenX, volumeHeight - tempHeightRatio * 5, 1.0f), 1.8f, 0xffffffff, gef::TJ_CENTRE, tempVolume.c_str());
		font->RenderText(spriteRenderer, gef::Vector4(volumeUpButtonInfo.middleX, volumeUpButtonInfo.middleY - tempHeightRatio * 5, 1.0f), 2.5f, 0xffffffff, gef::TJ_CENTRE, "+ 0.1");
		font->RenderText(spriteRenderer, gef::Vector4(volumeDownButtonInfo.middleX, volumeDownButtonInfo.middleY - tempHeightRatio * 5, 1.0f), 2.5f, 0xffffffff, gef::TJ_CENTRE, "- 0.1");

		//difficulty
		std::string tempDifficulty = "Current Difficulty: ";
		tempDifficulty += DifficultyToString();
		font->RenderText(spriteRenderer, gef::Vector4(quaterScreenX, difficultyHeight - tempHeightRatio * 5, 1.0f), 1.8f, 0xffffffff, gef::TJ_CENTRE, tempDifficulty.c_str());
		font->RenderText(spriteRenderer, gef::Vector4(difficultyEasyButtonInfo.middleX, difficultyEasyButtonInfo.middleY - tempHeightRatio * 5, 1.0f), 2.5f, 0xffffffff, gef::TJ_CENTRE, "Easy");
		font->RenderText(spriteRenderer, gef::Vector4(difficultyNormalButtonInfo.middleX, difficultyNormalButtonInfo.middleY - tempHeightRatio * 5, 1.0f), 2.5f, 0xffffffff, gef::TJ_CENTRE, "Normal");
		font->RenderText(spriteRenderer, gef::Vector4(difficultyHardButtonInfo.middleX, difficultyHardButtonInfo.middleY - tempHeightRatio * 5, 1.0f), 2.5f, 0xffffffff, gef::TJ_CENTRE, "Hard");

		//back
		font->RenderText(spriteRenderer, gef::Vector4(backButtonInfo.middleX, backButtonInfo.middleY - tempHeightRatio * 5, 1.0f), 2.5f, 0xffffffff, gef::TJ_CENTRE, "Back");
	}
}

void TitleScreen::CleanUp() {
	
}

void TitleScreen::UpdateWorld(float frameTime) {
	//update physics world
	float timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	world->Step(timeStep, velocityIterations, positionIterations);
}

void TitleScreen::ProcessTouchInput()
{
	const gef::TouchInputManager* touch_input = inputManager->touch_manager();
	if (touch_input && (touch_input->max_num_panels() > 0))
	{
		// get the active touches for this panel
		const gef::TouchContainer& panel_touches = touch_input->touches(0);

		// go through the touches
		for (gef::ConstTouchIterator touch = panel_touches.begin(); touch != panel_touches.end(); ++touch)
		{
			// if active touch id is -1, then we are not currently processing a touch
			if (activeTouchID == -1)
			{
				// check for the start of a new touch
				if (touch->type == gef::TT_NEW)
				{
					activeTouchID = touch->id;

					// do any processing for a new touch here
					// we're just going to record the position of the touch
					touchPosition = touch->position;

					CheckForButtons();
					ProcessMenuStateAction();
				}
			}
			else if (activeTouchID == touch->id)
			{
				// we are processing touch data with a matching id to the one we are looking for
				if (touch->type == gef::TT_ACTIVE)
				{
					// update an active touch here
					// we're just going to record the position of the touch
					touchPosition = touch->position;
				}
				else if (touch->type == gef::TT_RELEASED)
				{
					// the touch we are tracking has been released
					// perform any actions that need to happen when a touch is released here
					// we're not doing anything here apart from resetting the active touch id
					activeTouchID = -1;

				}
			}
		}
	}
}

void TitleScreen::CheckForButtons() {
	if (currentState == MenuState::MAIN_MENU) { //if we are currently in main menu state
		if (CheckButtonDimentions(playButtonInfo)) {
			currentState = MenuState::PLAY;
		}
		else if (CheckButtonDimentions(optionButtonInfo)) {
			currentState = MenuState::OPTION;
		}
		else if (CheckButtonDimentions(quitButtonInfo)) {
			currentState = MenuState::QUIT;
		}
	}
	else if (currentState == MenuState::OPTION) { //if we are currently in option menu state
		if (CheckButtonDimentions(level1ButtonInfo)) {
			currentState = MenuState::LEVEL_1;
		}
		else if (CheckButtonDimentions(level2ButtonInfo)) {
			currentState = MenuState::LEVEL_2;
		}
		else if (CheckButtonDimentions(volumeUpButtonInfo)) {
			currentState = MenuState::VOLUME_UP;
		}
		else if (CheckButtonDimentions(volumeDownButtonInfo)) {
			currentState = MenuState::VOLUME_DOWN;
		}
		else if (CheckButtonDimentions(difficultyEasyButtonInfo)) {
			currentState = MenuState::DIFFICULTY_EASY;
		}
		else if (CheckButtonDimentions(difficultyNormalButtonInfo)) {
			currentState = MenuState::DIFFICULTY_NORMAL;
		}
		else if (CheckButtonDimentions(difficultyHardButtonInfo)) {
			currentState = MenuState::DIFFICULTY_HARD;
		}
		else if (CheckButtonDimentions(backButtonInfo)) {
			currentState = MenuState::MAIN_MENU;
		}
	}
}

bool TitleScreen::CheckButtonDimentions(ButtonInfo button) {
	float mouseX = inputManager->touch_manager()->mouse_position().x;
	float mouseY = inputManager->touch_manager()->mouse_position().y;

	if (mouseX > button.xMin && mouseX < button.xMax) {
		if (mouseY > button.yMin && mouseY < button.yMax) {
			return true;
		}
	}
	
	return false;
}

void TitleScreen::ProcessMenuStateAction() {
	
	switch (currentState) {
	case MenuState::MAIN_MENU:
		break;
	case MenuState::PLAY:
		currentSceneNum = optionalLevelStart; //next scene
		musicPlaying = false;
		currentState = MenuState::MAIN_MENU;
		break;
	case MenuState::OPTION:
		break;
	case MenuState::QUIT:
		std::exit(true); //quit program
		break;
	case MenuState::VOLUME_UP:
		if (optionalVolume < 100) {
			optionalVolume += 10;
		}
		currentState = MenuState::OPTION;
		break;
	case MenuState::VOLUME_DOWN:
		if (optionalVolume > 0) {
			optionalVolume -= 10;
		}
		currentState = MenuState::OPTION;
		break;
	case MenuState::LEVEL_1:
		optionalLevelStart = 1;
		currentState = MenuState::OPTION;
		break;
	case MenuState::LEVEL_2:
		optionalLevelStart = 2;
		currentState = MenuState::OPTION;
		break;
	case MenuState::DIFFICULTY_EASY:
		optionalLevelDifficulty = Difficulty::EASY;
		currentState = MenuState::OPTION;
		break;
	case MenuState::DIFFICULTY_NORMAL:
		optionalLevelDifficulty = Difficulty::NORMAL;
		currentState = MenuState::OPTION;
		break;
	case MenuState::DIFFICULTY_HARD:
		optionalLevelDifficulty = Difficulty::HARD;
		currentState = MenuState::OPTION;
		break;
	}
}

void TitleScreen::SplashCreation() {

	//splash screen image
	splashScreenSprite.set_texture(splashScreenTexture);
	splashScreenSprite.set_height(1920 / 4);
	splashScreenSprite.set_width(1920 / 4);
	splashScreenSprite.set_position(gef::Vector4(platform.width() / 2, platform.height() / 2, 1));

	//splash screen background
	splashScreenBGSprite.set_texture(splashScreenBGTexture);
	splashScreenBGSprite.set_height(platform.height());
	splashScreenBGSprite.set_width(platform.width());
	splashScreenBGSprite.set_position(gef::Vector4(platform.width() / 2, platform.height() / 2, 1));

	//menu backgrounds
	mainMenuBGSprite.set_height(platform.height());
	mainMenuBGSprite.set_width(platform.width());
	mainMenuBGSprite.set_position(gef::Vector4(platform.width() / 2, platform.height() / 2, 1));
	mainMenuBGSprite.set_colour(gef::Colour(0, 0.7, 0.7, 0.7).GetABGR());
}

void TitleScreen::MainMenuButtonCreation() {
	
	//title name
	titleSprite.set_texture(titleTexture);
	titleSprite.set_height(170);
	titleSprite.set_width(903);
	titleSprite.set_position(gef::Vector4(platform.width() / 2, platform.height() / 6, 1));

	//set textures
	playButtonSprite.set_texture(buttonTexture);
	optionButtonSprite.set_texture(buttonTexture);
	quitButtonSprite.set_texture(buttonTexture);

	//set button sizes (specific)
	playButtonInfo.height = platform.height() / 5.5;
	playButtonInfo.width = platform.width() / 4.8;
	optionButtonInfo.height = platform.height() / 5.5;
	optionButtonInfo.width = platform.width() / 4.8;
	quitButtonInfo.height = platform.height() / 5.5;
	quitButtonInfo.width = platform.width() / 4.8;

	//set sprite sizes
	playButtonSprite.set_width(playButtonInfo.width);
	playButtonSprite.set_height(playButtonInfo.height);
	optionButtonSprite.set_width(optionButtonInfo.width);
	optionButtonSprite.set_height(optionButtonInfo.height);
	quitButtonSprite.set_width(quitButtonInfo.width);
	quitButtonSprite.set_height(quitButtonInfo.height);

	//local variables to help with adjusting spacing of buttons
	float buttonSeperationHeight = platform.height() / 10;
	float playHeight = buttonSeperationHeight * 4;
	float optionHeight = buttonSeperationHeight * 6;
	float quitHeight = buttonSeperationHeight * 8;

	float middleScreenX = platform.width() / 2;

	//button information for activate zone on screen (specific)
	playButtonInfo.xMin = middleScreenX - (playButtonInfo.width / 2);
	playButtonInfo.xMax = middleScreenX + (playButtonInfo.width / 2);
	playButtonInfo.yMin = playHeight - (playButtonInfo.height / 2);
	playButtonInfo.yMax = playHeight + (playButtonInfo.height / 4);
	playButtonInfo.middleX = middleScreenX;
	playButtonInfo.middleY = playHeight + (playButtonInfo.height / 2);

	optionButtonInfo.xMin = middleScreenX - (optionButtonInfo.width / 2);
	optionButtonInfo.xMax = middleScreenX + (optionButtonInfo.width / 2);
	optionButtonInfo.yMin = optionHeight - (optionButtonInfo.height / 2);
	optionButtonInfo.yMax = optionHeight + (optionButtonInfo.height / 4);
	optionButtonInfo.middleX = middleScreenX;
	optionButtonInfo.middleY = optionHeight + (playButtonInfo.height / 2);

	quitButtonInfo.xMin = middleScreenX - (quitButtonInfo.width / 2);
	quitButtonInfo.xMax = middleScreenX + (quitButtonInfo.width / 2);
	quitButtonInfo.yMin = quitHeight - (quitButtonInfo.height / 2);
	quitButtonInfo.yMax = quitHeight + (quitButtonInfo.height / 4);
	quitButtonInfo.middleX = middleScreenX;
	quitButtonInfo.middleY = quitHeight + (playButtonInfo.height / 2);

	//setting position of sprite on screen
	playButtonSprite.set_position(gef::Vector4(middleScreenX, playHeight, 1));
	optionButtonSprite.set_position(gef::Vector4(middleScreenX, optionHeight, 1));
	quitButtonSprite.set_position(gef::Vector4(middleScreenX, quitHeight, 1));
}

void TitleScreen::OptionMenuButtonCreation() {

	//set texture for each button
	level1Sprite.set_texture(buttonTexture);
	level2Sprite.set_texture(buttonTexture);
	volumeUpSprite.set_texture(buttonTexture);
	volumeDownSprite.set_texture(buttonTexture);
	difficultyEasySprite.set_texture(buttonTexture);
	difficultyNormalSprite.set_texture(buttonTexture);
	difficultyHardSprite.set_texture(buttonTexture);
	backSprite.set_texture(buttonTexture);

	//each individual button size (specific)
	level1ButtonInfo.height = platform.height() / 5.5;
	level1ButtonInfo.width = playButtonInfo.width = platform.width() / 4.8;
	level2ButtonInfo.height = platform.height() / 5.5;
	level2ButtonInfo.width = playButtonInfo.width = platform.width() / 4.8;
	volumeUpButtonInfo.height = platform.height() / 5.5;
	volumeUpButtonInfo.width = playButtonInfo.width = platform.width() / 4.8;
	volumeDownButtonInfo.height = platform.height() / 5.5;
	volumeDownButtonInfo.width = playButtonInfo.width = platform.width() / 4.8;
	difficultyEasyButtonInfo.height = platform.height() / 7.2;
	difficultyEasyButtonInfo.width = playButtonInfo.width = platform.width() / 6.4;
	difficultyNormalButtonInfo.height = platform.height() / 7.2;
	difficultyNormalButtonInfo.width = playButtonInfo.width = platform.width() / 6.4;
	difficultyHardButtonInfo.height = platform.height() / 7.2;
	difficultyHardButtonInfo.width = playButtonInfo.width = platform.width() / 6.4;
	backButtonInfo.height = platform.height() / 5.5;
	backButtonInfo.width = platform.height() / 5.5;

	//setting sprite to button width and height
	level1Sprite.set_width(level1ButtonInfo.width);
	level1Sprite.set_height(level1ButtonInfo.height);
	level2Sprite.set_width(level2ButtonInfo.width);
	level2Sprite.set_height(level2ButtonInfo.height);
	volumeUpSprite.set_width(volumeUpButtonInfo.width);
	volumeUpSprite.set_height(volumeUpButtonInfo.height);
	volumeDownSprite.set_width(volumeDownButtonInfo.width);
	volumeDownSprite.set_height(volumeDownButtonInfo.height);
	difficultyEasySprite.set_width(difficultyEasyButtonInfo.width);
	difficultyEasySprite.set_height(difficultyEasyButtonInfo.height);
	difficultyNormalSprite.set_width(difficultyNormalButtonInfo.width);
	difficultyNormalSprite.set_height(difficultyNormalButtonInfo.height);
	difficultyHardSprite.set_width(difficultyHardButtonInfo.width);
	difficultyHardSprite.set_height(difficultyHardButtonInfo.height);
	backSprite.set_width(backButtonInfo.width);
	backSprite.set_height(backButtonInfo.height);

	//local variables to help with adjusting spacing of buttons
	float buttonSeperationHeight = platform.height() / 10;
	float levelHeight = buttonSeperationHeight * 2;
	float volumeHeight = buttonSeperationHeight * 4;
	float difficultyHeight = buttonSeperationHeight * 6;
	float backHeight = buttonSeperationHeight * 8;

	float quaterScreenX = platform.width() / 4;
	float middleScreenX = platform.width() / 2;
	float threeForthsScreenX = quaterScreenX * 3;
	float oneSixthScreenX = platform.width() / 6;

	//button information for activate zone on screen (specific for each button)
	level1ButtonInfo.xMin = middleScreenX - (level1ButtonInfo.width / 2);
	level1ButtonInfo.xMax = middleScreenX + (level1ButtonInfo.width / 2);
	level1ButtonInfo.yMin = levelHeight - (level1ButtonInfo.height / 2);
	level1ButtonInfo.yMax = levelHeight + (level1ButtonInfo.height / 4);
	level1ButtonInfo.middleX = level1ButtonInfo.xMin + (level1ButtonInfo.width / 2);
	level1ButtonInfo.middleY = level1ButtonInfo.yMin + (level1ButtonInfo.height / 2);
	level1Sprite.set_position(gef::Vector4(middleScreenX, levelHeight, 1));

	level2ButtonInfo.xMin = threeForthsScreenX - (level2ButtonInfo.width / 2);
	level2ButtonInfo.xMax = threeForthsScreenX + (level2ButtonInfo.width / 2);
	level2ButtonInfo.yMin = levelHeight - (level2ButtonInfo.height / 2);
	level2ButtonInfo.yMax = levelHeight + (level2ButtonInfo.height / 4);
	level2ButtonInfo.middleX = level2ButtonInfo.xMin + (level2ButtonInfo.width / 2);
	level2ButtonInfo.middleY = level2ButtonInfo.yMin + (level2ButtonInfo.height / 2);
	level2Sprite.set_position(gef::Vector4(threeForthsScreenX, levelHeight, 1));

	volumeUpButtonInfo.xMin = middleScreenX - (volumeUpButtonInfo.width / 2);
	volumeUpButtonInfo.xMax = middleScreenX + (volumeUpButtonInfo.width / 2);
	volumeUpButtonInfo.yMin = volumeHeight - (volumeUpButtonInfo.height / 2);
	volumeUpButtonInfo.yMax = volumeHeight + (volumeUpButtonInfo.height / 4);
	volumeUpButtonInfo.middleX = volumeUpButtonInfo.xMin + (volumeUpButtonInfo.width / 2);
	volumeUpButtonInfo.middleY = volumeUpButtonInfo.yMin + (volumeUpButtonInfo.height / 2);
	volumeUpSprite.set_position(gef::Vector4(middleScreenX, volumeHeight, 1));

	volumeDownButtonInfo.xMin = threeForthsScreenX - (volumeDownButtonInfo.width / 2);
	volumeDownButtonInfo.xMax = threeForthsScreenX + (playButtonInfo.width / 2);
	volumeDownButtonInfo.yMin = volumeHeight - (volumeDownButtonInfo.height / 2);
	volumeDownButtonInfo.yMax = volumeHeight + (volumeDownButtonInfo.height / 4);
	volumeDownButtonInfo.middleX = volumeDownButtonInfo.xMin + (volumeDownButtonInfo.width / 2);
	volumeDownButtonInfo.middleY = volumeDownButtonInfo.yMin + (volumeDownButtonInfo.height / 2);
	volumeDownSprite.set_position(gef::Vector4(threeForthsScreenX, volumeHeight, 1));

	difficultyEasyButtonInfo.xMin = middleScreenX - (difficultyEasyButtonInfo.width / 2);
	difficultyEasyButtonInfo.xMax = middleScreenX + (difficultyEasyButtonInfo.width / 2);
	difficultyEasyButtonInfo.yMin = difficultyHeight - (difficultyEasyButtonInfo.height / 2);
	difficultyEasyButtonInfo.yMax = difficultyHeight + (difficultyEasyButtonInfo.height / 4);
	difficultyEasyButtonInfo.middleX = difficultyEasyButtonInfo.xMin + (difficultyEasyButtonInfo.width / 2);
	difficultyEasyButtonInfo.middleY = difficultyEasyButtonInfo.yMin + (difficultyEasyButtonInfo.height / 2);
	difficultyEasySprite.set_position(gef::Vector4(middleScreenX, difficultyHeight, 1));

	difficultyNormalButtonInfo.xMin = (oneSixthScreenX * 4) - (difficultyNormalButtonInfo.width / 2);
	difficultyNormalButtonInfo.xMax = (oneSixthScreenX * 4) + (difficultyNormalButtonInfo.width / 2);
	difficultyNormalButtonInfo.yMin = difficultyHeight - (difficultyNormalButtonInfo.height / 2);
	difficultyNormalButtonInfo.yMax = difficultyHeight + (difficultyNormalButtonInfo.height / 4);
	difficultyNormalButtonInfo.middleX = difficultyNormalButtonInfo.xMin + (difficultyNormalButtonInfo.width / 2);
	difficultyNormalButtonInfo.middleY = difficultyNormalButtonInfo.yMin + (difficultyNormalButtonInfo.height / 2);
	difficultyNormalSprite.set_position(gef::Vector4(oneSixthScreenX * 4, difficultyHeight, 1));

	difficultyHardButtonInfo.xMin = (oneSixthScreenX * 5) - (difficultyHardButtonInfo.width / 2);
	difficultyHardButtonInfo.xMax = (oneSixthScreenX * 5) + (difficultyHardButtonInfo.width / 2);
	difficultyHardButtonInfo.yMin = difficultyHeight - (difficultyHardButtonInfo.height / 2);
	difficultyHardButtonInfo.yMax = difficultyHeight + (difficultyHardButtonInfo.height / 4);
	difficultyHardButtonInfo.middleX = difficultyHardButtonInfo.xMin + (difficultyHardButtonInfo.width / 2);
	difficultyHardButtonInfo.middleY = difficultyHardButtonInfo.yMin + (difficultyHardButtonInfo.height / 2);
	difficultyHardSprite.set_position(gef::Vector4(oneSixthScreenX * 5, difficultyHeight, 1));

	backButtonInfo.xMin = quaterScreenX - (backButtonInfo.width / 2);
	backButtonInfo.xMax = quaterScreenX + (backButtonInfo.width / 2);
	backButtonInfo.yMin = backHeight - (backButtonInfo.height / 2);
	backButtonInfo.yMax = backHeight + (backButtonInfo.height / 4);
	backButtonInfo.middleX = backButtonInfo.xMin + (backButtonInfo.width / 2);
	backButtonInfo.middleY = backButtonInfo.yMin + (backButtonInfo.height / 2);
	backSprite.set_position(gef::Vector4(quaterScreenX, backHeight, 1));
}

void TitleScreen::LoadTextures() {
	splashScreenTexture = CreateTextureWithPNG("Images/cat.png", platform);
	splashScreenBGTexture = CreateTextureWithPNG("Images/black.png", platform);
	buttonTexture = CreateTextureWithPNG("Images/button.png", platform);
	titleTexture = CreateTextureWithPNG("Images/title.png", platform);
	mainMenuBGTexture = CreateTextureWithPNG("Images/white.png", platform);
}

std::string TitleScreen::DifficultyToString() {

	//convert difficulty enum to string
	if (optionalLevelDifficulty == Difficulty::EASY) {
		return "Easy";
	}
	else if(optionalLevelDifficulty == Difficulty::NORMAL){
		return "Normal";
	}
	else if (optionalLevelDifficulty == Difficulty::HARD) {
		return "Hard";
	}
	else { //should never get here
		return "ERROR";
	}
}