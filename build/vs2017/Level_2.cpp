#include "Level_2.h"

void Level_2::Init() {
	font = new gef::Font(platform);
	font->Load("comic_sans");

	world = new b2World(b2Vec2(0, -7.81f));
	world->SetAllowSleeping(false);
	world->SetContinuousPhysics(true);

	InitObjects();
	InitSprites();
	InitModels();

	musicPlaying = false;
	winTimer = 5;
}

void Level_2::Update(float frameTime) {

	UpdateWorld(frameTime);

	if (!musicPlaying) { //if no music is playing, start music.
		musicPlaying = true;
		audioManager->LoadMusic("sounds/level_2.wav", platform);
		audioManager->PlayMusic();
	}

	if (goal->IsTriggered()) {
		audioManager->StopMusic();
		winTimer -= frameTime;
		if (winTimer <= 0) { //wait for win screen to show
			currentSceneNum = 3; //back to title screen
			return; //go back, nothing left to do.
		}
	}

	//update camera
	camera1->Update(frameTime);
	camera1->SetPlayerCoords(player->GetPosition());

	//update objects in world
	for (Platform* element : floor) {
		element->Update(frameTime);
	}

	for (Platform* element : wall) {
		element->Update(frameTime);
	}

	button->Update(frameTime);
	button2->Update(frameTime);
	button3->Update(frameTime);
	goal->Update(frameTime);

	//update coins
	for (int i = 0; i < coin.size(); i++) {

		if (coin[i]->IsTriggered()) {
			coin[i]->~Coin();
			coin.erase(coin.begin() + i);
			continue; //don't update if coin is picked up
		}

		coin[i]->Update(frameTime);
	}

	UpdateCollision();
}

void Level_2::Render(float frameTime) {

	renderer3D->Begin(); //---------------------------------------

	//player
	player->Render(frameTime);

	//obstacles
	for (Platform* element : floor) {
		element->Render(frameTime);
	}

	for (Platform* element : wall) {
		element->Render(frameTime);
	}

	for (Coin* element : coin) {
		element->Render(frameTime);
	}

	button->Render(frameTime);
	button2->Render(frameTime);
	button3->Render(frameTime);
	goal->Render(frameTime);

	renderer3D->End(); //---------------------------------------

	spriteRenderer->Begin(false); //---------------------------------------

	spriteRenderer->DrawSprite(backgroundSprite);

	DrawHUD();

	if (goal->IsTriggered()) {
		spriteRenderer->DrawSprite(winBackgroundSprite);
		spriteRenderer->DrawSprite(winText);
	}

	spriteRenderer->End(); //---------------------------------------
}

void Level_2::DrawHUD()
{
	if (font)
	{
		player->DebugLog(font);
		for (Platform* element : floor) {
			element->DebugLog(font);
		}
	}
}

void Level_2::CleanUp() {
	delete camera1;
	delete goal;
	delete button3;
	delete button2;
	delete button;
	delete player;
	delete font;

	button = nullptr;
	button2 = nullptr;
	button3 = nullptr;
	goal = nullptr;
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

	floor.clear();
	wall.clear();
	
	audioManager->UnloadMusic();
	world->~b2World();
}

void Level_2::SetupLights()
{
	// grab the data for the default shader used for rendering 3D geometry
	gef::Default3DShaderData& default_shader_data = renderer3D->default_shader_data();

	// set the ambient light
	default_shader_data.set_ambient_light_colour(gef::Colour(0.25f, 0.25f, 0.25f, 1.0f));

	// add a point light that is almost white, but with a blue tinge
	// the position of the light is set far away so it acts light a directional light
	gef::PointLight default_point_light;
	default_point_light.set_colour(gef::Colour(0.7f, 0.7f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(-500.0f, 400.0f, 700.0f));
	default_shader_data.AddPointLight(default_point_light);
}

void Level_2::UpdateWorld(float frameTime) {
	//update physics world
	float timeStep = 1.0f / 60.0f;
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	float timeStep2 = 1.0f / 60.0f;
	int32 velocityIterations2 = 6;
	int32 positionIterations2 = 2;

	world->Step(timeStep, velocityIterations, positionIterations);
	player->Update(frameTime);
}

void Level_2::UpdateCollision() {
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

gef::Vector4 Level_2::OriginLeft(gef::Vector4 position, gef::Vector4 size) { // (I think gef draws platforms in the centre depending on how large the platform is)

	float temp = 0; 
	temp = size.x() / 2; 
	position.set_x(position.x() + temp); //add half the size to the position to spawn on the left hand side.

	temp = size.y() / 2;
	position.set_y(position.y() + temp);

	temp = size.z() / 2;
	position.set_z(position.z() + temp);

	return position;
}

void Level_2::InitObjects() {

	//camera
	camera1 = new Camera(platform, renderer3D, inputManager, spriteRenderer);

	//player
	player = new Player(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, gef::Vector4(5, 10, 1), font);
	player->SetRespawnPoint(5, 2);
	player->SetPlatformsAllowed(0);
	player->SetRespawnPoint(5, 10);

	//goal
	goal = new Goal(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, OriginLeft(gef::Vector4(115, 20, 0), gef::Vector4(0.5, 0.5, 0.2)), gef::Vector4(0.5, 0.5, 0.2));

	//floor
	floor.push_back(new Platform(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, OriginLeft(gef::Vector4(5, 0, 0), gef::Vector4(10, 0.1, 0.2)), gef::Vector4(10, 0.1, 0.2)));
	floor.push_back(new Platform(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, OriginLeft(gef::Vector4(25, 20, 0), gef::Vector4(10, 0.1, 0.2)), gef::Vector4(10, 0.1, 0.2)));
	floor.push_back(new Platform(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, OriginLeft(gef::Vector4(50, 40, 0), gef::Vector4(10, 0.1, 0.2)), gef::Vector4(10, 0.1, 0.2)));
	floor.push_back(new Platform(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, OriginLeft(gef::Vector4(70, 0, 0), gef::Vector4(20, 0.1, 0.2)), gef::Vector4(20, 0.1, 0.2)));
	floor.push_back(new Platform(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, OriginLeft(gef::Vector4(105, 20, 0), gef::Vector4(10, 0.1, 0.2)), gef::Vector4(10, 0.1, 0.2)));

	//wall
	wall.push_back(new Platform(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, OriginLeft(gef::Vector4(20, 5, 0), gef::Vector4(0.1, 10, 0.2)), gef::Vector4(0.1, 10, 0.2)));
	wall.push_back(new Platform(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, OriginLeft(gef::Vector4(0, 5, 0), gef::Vector4(0.1, 10, 0.2)), gef::Vector4(0.1, 10, 0.2)));
	wall.push_back(new Platform(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, OriginLeft(gef::Vector4(45, 25, 0), gef::Vector4(0.1, 10, 0.2)), gef::Vector4(0.1, 10, 0.2)));
	wall.push_back(new Platform(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, OriginLeft(gef::Vector4(65, 10, 0), gef::Vector4(0.1, 20, 0.2)), gef::Vector4(0.1, 20, 0.2)));
	wall.push_back(new Platform(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, OriginLeft(gef::Vector4(100, 0, 0), gef::Vector4(0.1, 20, 0.2)), gef::Vector4(0.1, 10, 0.2)));
	wall.push_back(new Platform(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, OriginLeft(gef::Vector4(120, 30, 0), gef::Vector4(0.1, 20, 0.2)), gef::Vector4(0.1, 20, 0.2)));

	//buttons
	buttonPressedTexture = CreateTextureWithPNG("Images/pattern.png", platform);
	button = new Button(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, buttonPressedTexture, OriginLeft(gef::Vector4(17, 0.2, 0), gef::Vector4(0.5, 0.2, 0.2)), gef::Vector4(0.5, 0.2, 0.2));
	button->InteractionType(Interaction::JUMP);
	button->SetPlayer(player);
	button2 = new Button(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, buttonPressedTexture, OriginLeft(gef::Vector4(42, 20.2, 0), gef::Vector4(0.5, 0.2, 0.2)), gef::Vector4(0.5, 0.2, 0.2));
	button2->InteractionType(Interaction::JUMP);
	button2->SetPlayer(player);
	button3 = new Button(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, buttonPressedTexture, OriginLeft(gef::Vector4(97, 0.2, 0), gef::Vector4(0.5, 0.2, 0.2)), gef::Vector4(0.5, 0.2, 0.2));
	button3->InteractionType(Interaction::JUMP);
	button3->SetPlayer(player);

	//coins
	coin.push_back(new Coin(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, OriginLeft(gef::Vector4(17, 2, 0), gef::Vector4(0.2, 0.2, 0.2)), gef::Vector4(0.2, 0.2, 0.2)));
	coin.push_back(new Coin(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, OriginLeft(gef::Vector4(17, 3, 0), gef::Vector4(0.2, 0.2, 0.2)), gef::Vector4(0.2, 0.2, 0.2)));
	coin.push_back(new Coin(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, OriginLeft(gef::Vector4(17, 4, 0), gef::Vector4(0.2, 0.2, 0.2)), gef::Vector4(0.2, 0.2, 0.2)));
	coin.push_back(new Coin(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, OriginLeft(gef::Vector4(17, 5, 0), gef::Vector4(0.2, 0.2, 0.2)), gef::Vector4(0.2, 0.2, 0.2)));
	coin.push_back(new Coin(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, OriginLeft(gef::Vector4(17, 6, 0), gef::Vector4(0.2, 0.2, 0.2)), gef::Vector4(0.2, 0.2, 0.2)));
	coin.push_back(new Coin(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, OriginLeft(gef::Vector4(17, 7, 0), gef::Vector4(0.2, 0.2, 0.2)), gef::Vector4(0.2, 0.2, 0.2)));
	coin.push_back(new Coin(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, OriginLeft(gef::Vector4(17, 8, 0), gef::Vector4(0.2, 0.2, 0.2)), gef::Vector4(0.2, 0.2, 0.2)));
	coin.push_back(new Coin(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, OriginLeft(gef::Vector4(17, 9, 0), gef::Vector4(0.2, 0.2, 0.2)), gef::Vector4(0.2, 0.2, 0.2)));
	coin.push_back(new Coin(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, OriginLeft(gef::Vector4(17, 10, 0), gef::Vector4(0.2, 0.2, 0.2)), gef::Vector4(0.2, 0.2, 0.2)));
	coin.push_back(new Coin(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, OriginLeft(gef::Vector4(17, 11, 0), gef::Vector4(0.2, 0.2, 0.2)), gef::Vector4(0.2, 0.2, 0.2)));
	coin.push_back(new Coin(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, OriginLeft(gef::Vector4(17, 12, 0), gef::Vector4(0.2, 0.2, 0.2)), gef::Vector4(0.2, 0.2, 0.2)));
	coin.push_back(new Coin(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, OriginLeft(gef::Vector4(25, 21, 0), gef::Vector4(0.2, 0.2, 0.2)), gef::Vector4(0.2, 0.2, 0.2)));
	coin.push_back(new Coin(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, OriginLeft(gef::Vector4(28, 21, 0), gef::Vector4(0.2, 0.2, 0.2)), gef::Vector4(0.2, 0.2, 0.2)));
	coin.push_back(new Coin(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, OriginLeft(gef::Vector4(31, 21, 0), gef::Vector4(0.2, 0.2, 0.2)), gef::Vector4(0.2, 0.2, 0.2)));
	coin.push_back(new Coin(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, OriginLeft(gef::Vector4(33, 21, 0), gef::Vector4(0.2, 0.2, 0.2)), gef::Vector4(0.2, 0.2, 0.2)));
	coin.push_back(new Coin(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, OriginLeft(gef::Vector4(35, 21, 0), gef::Vector4(0.2, 0.2, 0.2)), gef::Vector4(0.2, 0.2, 0.2)));
	coin.push_back(new Coin(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, OriginLeft(gef::Vector4(50, 41, 0), gef::Vector4(0.2, 0.2, 0.2)), gef::Vector4(0.2, 0.2, 0.2)));
	coin.push_back(new Coin(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, OriginLeft(gef::Vector4(53, 41, 0), gef::Vector4(0.2, 0.2, 0.2)), gef::Vector4(0.2, 0.2, 0.2)));
	coin.push_back(new Coin(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, OriginLeft(gef::Vector4(56, 41, 0), gef::Vector4(0.2, 0.2, 0.2)), gef::Vector4(0.2, 0.2, 0.2)));
	coin.push_back(new Coin(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, OriginLeft(gef::Vector4(70, 1, 0), gef::Vector4(0.2, 0.2, 0.2)), gef::Vector4(0.2, 0.2, 0.2)));
	coin.push_back(new Coin(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, OriginLeft(gef::Vector4(74, 1, 0), gef::Vector4(0.2, 0.2, 0.2)), gef::Vector4(0.2, 0.2, 0.2)));
	coin.push_back(new Coin(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, OriginLeft(gef::Vector4(78, 1, 0), gef::Vector4(0.2, 0.2, 0.2)), gef::Vector4(0.2, 0.2, 0.2)));
	coin.push_back(new Coin(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, OriginLeft(gef::Vector4(82, 1, 0), gef::Vector4(0.2, 0.2, 0.2)), gef::Vector4(0.2, 0.2, 0.2)));
	coin.push_back(new Coin(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, OriginLeft(gef::Vector4(105, 21, 0), gef::Vector4(0.2, 0.2, 0.2)), gef::Vector4(0.2, 0.2, 0.2)));
	coin.push_back(new Coin(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, OriginLeft(gef::Vector4(109, 21, 0), gef::Vector4(0.2, 0.2, 0.2)), gef::Vector4(0.2, 0.2, 0.2)));
	coin.push_back(new Coin(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, OriginLeft(gef::Vector4(113, 21, 0), gef::Vector4(0.2, 0.2, 0.2)), gef::Vector4(0.2, 0.2, 0.2)));

	//refs
	for (Platform* wall : wall) {
		wall->SetWall(true);
	}

	for (Coin* x : coin) {
		x->SetPlayer(player);
	}
}

void Level_2::InitSprites() {

	//background
	backgroundSprite.set_height(platform.height());
	backgroundSprite.set_width(platform.width());
	backgroundSprite.set_position(gef::Vector4(platform.width() / 2, platform.height() / 2, 1));
	backgroundSprite.set_colour(gef::Colour(0.5, 0.5, 0.5, 1).GetABGR());
	backgroundTexture = CreateTextureWithPNG("Images/forest.png", platform);
	backgroundSprite.set_texture(backgroundTexture);

	//win background
	winBackgroundSprite.set_height(platform.height());
	winBackgroundSprite.set_width(platform.width());
	winBackgroundSprite.set_position(gef::Vector4(platform.width() / 2, platform.height() / 2, -1));
	winBackgroundSprite.set_colour(gef::Colour(0.2, 0.7, 0.3, 1).GetABGR());

	//win text
	winText.set_height(platform.height() / 5);
	winText.set_width(platform.width() / 5);
	winText.set_position(gef::Vector4(platform.width() / 2, platform.height() / 2, -1));
	winTextTexture = CreateTextureWithPNG("Images/win.png", platform);
	winText.set_texture(winTextTexture);
}

void Level_2::InitModels() {

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