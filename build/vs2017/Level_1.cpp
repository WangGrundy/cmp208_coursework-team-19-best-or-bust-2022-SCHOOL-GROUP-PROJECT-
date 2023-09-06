#include "Level_1.h"

void Level_1::Init() {

	font = new gef::Font(platform);
	font->Load("comic_sans");
	world = new b2World(b2Vec2(0, -7.81f));
	world->SetAllowSleeping(false); // SET THIS TO FALSE, WORLD SHOULD NEVER SLEEP.
	world->SetContinuousPhysics(true);
	
	InitObjects();
	InitSprites();
	InitModels();

	musicPlaying = false;
	winTimer = 3;
}

void Level_1::Update(float frameTime) {

	player->Update(frameTime);

	fps = 1.0f / frameTime;

	if (!musicPlaying) { //if no music is playing, start music.
		musicPlaying = true;
		audioManager->LoadMusic("sounds/level_1.wav", platform);
		audioManager->PlayMusic();
	}
	
	if (goal->IsTriggered()) {
		audioManager->StopMusic();
		winTimer -= frameTime;

		if (winTimer <= 0) { //wait for win screen to show
			currentSceneNum = 2; //next scene
			return; //go back, nothing left to do.
		}
	}

	UpdateWorld(frameTime);

	//camera
	camera1->Update(frameTime);
	camera1->SetPlayerCoords(player->GetPosition());
	
	//objects
	button->Update(frameTime);
	button2->Update(frameTime);
	goal->Update(frameTime);

	if (!button2->GetTriggered()) { // if button is triggerd, don't render it
		buttonWall->Update(frameTime);
	}

	for (Lava* element : lava) {
		element->Update(frameTime);
	}

	for (Platform* element : floor) {
		element->Update(frameTime);
	}

	for (Platform* element : wall) {
		element->Update(frameTime);
	}

	for (int i = 0; i < coin.size(); i++) {

		if (coin[i]->IsTriggered()) {
			coin[i]->~Coin();
			coin.erase(coin.begin() + i);
			continue; //don't update if coin is picked up
		}

		coin[i]->Update(frameTime);
	}

	//collision
	UpdateCollision();
}

void Level_1::Render(float frameTime) {

	renderer3D->Begin();// -----------------------------------------------------------------------
	
	//player
	player->Render(frameTime);

	//objects
	for (Platform* element : floor) {
		element->Render(frameTime);
	}

	for (Platform* element : wall) {
		element->Render(frameTime);
	}

	for (Coin* element : coin) {
		element->Render(frameTime);
	}

	for (Lava* element : lava) {
		element->Render(frameTime);
	}

	button->Render(frameTime);
	button2->Render(frameTime);

	if (!button2->GetTriggered()) { // if button is triggerd, don't render it
		buttonWall->Render(frameTime);
	}

	goal->Render(frameTime);

	renderer3D->End();// -----------------------------------------------------------------------

	spriteRenderer->Begin(false);// -----------------------------------------------------------------------
	spriteRenderer->DrawSprite(backgroundSprite);
	DrawHUD();

	if (goal->IsTriggered()) {
		spriteRenderer->DrawSprite(winBackgroundSprite);
		spriteRenderer->DrawSprite(winText);
	}
	spriteRenderer->End(); // -----------------------------------------------------------------------
}

void Level_1::DrawHUD()
{
	font->RenderText(spriteRenderer, gef::Vector4(platform.width() - 200, 100, -0.9f), 1.f, 0xffffffff, gef::TJ_LEFT, "FPS: %.f",
		fps
	);

	if (font)
	{
		player->DebugLog(font);
		for (Platform* element : floor) {
			element->DebugLog(font);
		}
	}
}

void Level_1::CleanUp() {

	delete camera1;
	delete player;
	delete button;
	delete button2;
	delete font;

	camera1 = nullptr;
	player = nullptr;

	for (Platform* element : floor) {
		delete element;
	}

	for (Platform* element : wall) {
		delete element;
	}

	for (Coin* element : coin) {
		delete element;
	}

	for (Lava* element : lava) {
		delete element;
	}

	lava.clear();
	floor.clear();
	coin.clear();
	wall.clear();
	
	audioManager->UnloadMusic();
	world->~b2World();
}

void Level_1::SetupLights()
{
	// grab the data for the default shader used for rendering 3D geometry
	gef::Default3DShaderData& default_shader_data = renderer3D->default_shader_data();

	// set the ambient light
	default_shader_data.set_ambient_light_colour(gef::Colour(0.25f, 0.25f, 0.25f, 1.0f));

	// add a point light that is almost white, but with a blue tinge
	// the position of the light is set far away so it acts light a directional light
	gef::PointLight default_point_light;
	default_point_light.set_colour(gef::Colour(0.7f, 0.07f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(-500.0f, 400.0f, 700.0f));
	default_shader_data.AddPointLight(default_point_light);
}

void Level_1::UpdateWorld(float frameTime) {
	//update physics world
	float timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	world->Step(timeStep, velocityIterations, positionIterations);
}

void Level_1::UpdateCollision() {
	//collision detection 
	 //get the head of the contact list
	b2Contact* contact = world->GetContactList();
	//get contact count
	int contact_count = world->GetContactCount();

	for (int contact_num = 0; contact_num < contact_count; ++contact_num)
	{
		if (contact->IsTouching())
		{
			// get the colliding bodies
			b2Body* bodyA = contact->GetFixtureA()->GetBody();
			b2Body* bodyB = contact->GetFixtureB()->GetBody();

			// DO COLLISION RESPONSE HERE 
			GameObject3D* game_objectA = (GameObject3D*)bodyA->GetUserData().pointer;
			GameObject3D* game_objectB = (GameObject3D*)bodyB->GetUserData().pointer;

			if (game_objectA) {
				game_objectA->CollisionResponse(game_objectB);
			}
			if (game_objectB) {
				game_objectB->CollisionResponse(game_objectA);
			}
		}

		// Get next contact point
		contact = contact->GetNext();
	}
}

void Level_1::InitObjects() {
	camera1 = new Camera(platform, renderer3D, inputManager, spriteRenderer);

	//player
	player = new Player(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, gef::Vector4(5, 4, 1), font);
	player->SetRespawnPoint(5, 2);
	player->SetPlatformsAllowed(1);
	goal = new Goal(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, gef::Vector4(60, 19, 0), gef::Vector4(0.5, 0.5, 0.2));

	//lava
	lava.push_back(new Lava(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, world, audioManager, gef::Vector4(15, -1, 0), gef::Vector4(3, 0.3, 1)));

	lava.push_back(new Lava(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, world, audioManager, gef::Vector4(25, 20, 0), gef::Vector4(0.2, 1.5, 1)));
	lava.push_back(new Lava(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, world, audioManager, gef::Vector4(30, 20, 0), gef::Vector4(0.2, 1.5, 1)));
	lava.push_back(new Lava(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, world, audioManager, gef::Vector4(35, 20, 0), gef::Vector4(0.2, 1.5, 1)));
	lava.push_back(new Lava(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, world, audioManager, gef::Vector4(40, 20, 0), gef::Vector4(0.2, 1.5, 1)));
	lava.push_back(new Lava(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, world, audioManager, gef::Vector4(45, 20, 0), gef::Vector4(0.2, 1.5, 1)));

	//floor
	floor.push_back(new Platform(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, gef::Vector4(30, -1, 0), gef::Vector4(30, 0.1, 0.2)));
	floor.push_back(new Platform(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, gef::Vector4(5, 3, 0), gef::Vector4(1.5, 0.1, 0.2)));
	floor.push_back(new Platform(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, gef::Vector4(10, 7, 0), gef::Vector4(1.5, 0.1, 0.2)));
	floor.push_back(new Platform(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, gef::Vector4(5, 11, 0), gef::Vector4(1.5, 0.1, 0.2)));
	floor.push_back(new Platform(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, gef::Vector4(10, 15, 0), gef::Vector4(1.5, 0.1, 0.2)));

	floor.push_back(new Platform(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, gef::Vector4(45, 19, 0), gef::Vector4(20, 0.1, 0.2)));
	floor.push_back(new Platform(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, gef::Vector4(35, 28, 0), gef::Vector4(1.5, 0.1, 0.2)));

	//walls
	wall.push_back(new Platform(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, gef::Vector4(0, 9, 0), gef::Vector4(0.1, 50, 0.2)));
	wall.push_back(new Platform(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, gef::Vector4(25, 9, 0), gef::Vector4(0.1, 10, 0.2)));
	wall.push_back(new Platform(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, gef::Vector4(65, 30, 0), gef::Vector4(0.1, 50, 0.2)));
	wall.push_back(new Platform(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, gef::Vector4(65, 30, 0), gef::Vector4(0.1, 50, 0.2)));

	for (Platform* x : wall) {
		x->SetWall(true);
	}

	//walls that need to be destroyed with button
	buttonWall = new Platform(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, gef::Vector4(55, 30, 0), gef::Vector4(0.1, 50, 0.2));
	buttonWall->SetWallColour(gef::Colour(0, 0.5, 0.5));
	buttonWall->SetWall(true);

	//buttons
	buttonPressedTexture = CreateTextureWithPNG("Images/pattern.png", platform);
	button = new Button(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, buttonPressedTexture, gef::Vector4(24, -0.8, 0), gef::Vector4(0.5, 0.2, 0.2));
	button->InteractionType(Interaction::PLACE);
	button->SetPlaceLocation(gef::Vector4(16, 16, 1));
	button2 = new Button(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, buttonPressedTexture, gef::Vector4(35, 28.2, 0), gef::Vector4(0.5, 0.2, 0.2));
	button2->InteractionType(Interaction::BREAK);
	button2->SetBreakPlatform(buttonWall);

	//coins
	coin.push_back(new Coin(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, gef::Vector4(5, 7, 0), gef::Vector4(0.2, 0.2, 0.2)));
	coin.push_back(new Coin(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, gef::Vector4(5, 12, 0), gef::Vector4(0.2, 0.2, 0.2)));
	coin.push_back(new Coin(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, gef::Vector4(10, 8, 0), gef::Vector4(0.2, 0.2, 0.2)));
	coin.push_back(new Coin(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, gef::Vector4(10, 16, 0), gef::Vector4(0.2, 0.2, 0.2)));
	coin.push_back(new Coin(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, gef::Vector4(16, 18, 0), gef::Vector4(0.2, 0.2, 0.2)));
	coin.push_back(new Coin(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, gef::Vector4(20.5, 21.4, 0), gef::Vector4(0.2, 0.2, 0.2)));

	//refs
	for (Lava* x : lava) {
		x->SetPlayer(player);
	}

	for (Coin* x : coin) {
		x->SetPlayer(player);
	}
}

void Level_1::InitSprites() {
	//background
	backgroundSprite.set_height(platform.height());
	backgroundSprite.set_width(platform.width());
	backgroundSprite.set_position(gef::Vector4(platform.width() / 2, platform.height() / 2, 1));
	backgroundSprite.set_colour(gef::Colour(0.2, 0.2, 0.2, 1).GetABGR());
	backgroundTexture = CreateTextureWithPNG("Images/grass.png", platform);
	backgroundSprite.set_texture(backgroundTexture);

	//win background
	winBackgroundSprite.set_height(platform.height());
	winBackgroundSprite.set_width(platform.width());
	winBackgroundSprite.set_position(gef::Vector4(platform.width() / 2, platform.height() / 2, -1));
	winBackgroundSprite.set_colour(gef::Colour(1, 1, 1, 1).GetABGR());

	//win text
	winText.set_height(platform.height() / 5);
	winText.set_width(platform.width() / 2);
	winText.set_position(gef::Vector4(platform.width() / 2, platform.height() / 2, -1));
	winTextTexture = CreateTextureWithPNG("Images/LEVEL_COMPLETE_TEXT.png", platform);
	winText.set_texture(winTextTexture);
}

void Level_1::InitModels() {
	//load the assets in from the .scn
	//const char* assetfile1 = "models/player/sekiro.scn";
	const char* assetfile2 = "models/coin/coin.scn";
	const char* assetfile3 = "models/portal/Portal.scn";
	//gef::Scene* sceneasset1 = LoadSceneAssets(platform, assetfile1);
	gef::Scene* sceneasset2 = LoadSceneAssets(platform, assetfile2);
	gef::Scene* sceneasset3 = LoadSceneAssets(platform, assetfile3);
	//player->SetMesh(GetMeshFromSceneAssets(sceneasset1));

	goal->SetMesh(GetMeshFromSceneAssets(sceneasset3));

	for (Coin* x : coin) {
		x->SetMesh(GetMeshFromSceneAssets(sceneasset2));
	}

}