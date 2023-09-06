#include "Player.h"

void Player::Init() {

	typeOfObject = "player";
	state = PlayerState::still;

	// create a new scene object and read in the data from the file
	// no meshes or materials are created yet
	// we're not making any assumptions about what the data may be loaded in for
	modelScene = new gef::Scene();
	modelScene->ReadSceneFromFile(platform, "models/player/amy.scn");

	// we do want to render the data stored in the scene file so lets create the materials from the material data present in the scene file
	modelScene->CreateMaterials(platform);

	// if there is mesh data in the scene, create a mesh to draw from the first mesh
	mesh_ = GetFirstMesh(modelScene);

	// get the first skeleton in the scene
	gef::Skeleton* skeleton = GetFirstSkeleton(modelScene);

	if (skeleton)
	{
		skinnedMeshInstance = new gef::SkinnedMeshInstance(*skeleton);
		motionClipplayer.Init(skinnedMeshInstance->bind_pose());
		skinnedMeshInstance->set_mesh(mesh_);
	}


	// anims
	walkAnim = LoadAnimation("models/player/amy@walk.scn", "");
	jumpAnim = LoadAnimation("models/player/amy@jump.scn", "");

	if (walkAnim)
	{
		motionClipplayer.set_clip(walkAnim);
		motionClipplayer.set_looping(true);
		motionClipplayer.set_anim_time(0.5f);
	}

	// create a physics body for the player
	bodyDef->type = b2_dynamicBody;
	bodyDef->position = b2Vec2(positionBody.x(), positionBody.y());
	bodyDef->angularDamping = 10;
	bodyDef->userData.pointer = reinterpret_cast<uintptr_t>(this);
	bodyDef->allowSleep = false; //never sleep
	bodyDef->awake = true; //wake up
	body = world->CreateBody(bodyDef);
	body->SetSleepingAllowed(false); //never sleep
	body->SetAwake(true); //set awake

	// create the fixture and set it on the rigid body
	shape.SetAsBox(0.3f, 0.7f); //,b2Vec2(0,0), 0
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.6f;
	fixtureDef.friction = 0.4f;
	body->CreateFixture(&fixtureDef);
	
	//input
	keyboard = inputManager->keyboard();
	touchManager = inputManager->touch_manager();

	//basic set up
	rotationBody.RotationZ(body->GetAngle());
	body->SetFixedRotation(true); //body doesn't rotate
	movementSpeed = 230;
	jumpSpeed = 7;
	numberOfPlatformsAllowed = 2; 
	health = 3;
	maxHealth = 3;
	coinsPickedUp = 0;
	dead = false;
	jumps = 2; 
	respawnPoint.x = 0;
	respawnPoint.y = 0;

	//timers
	platformCooldownTimer = 3;
	dontResetJumpTimer = 1;
	dontResetJumpCooldown = 0.2;
	platformCooldown = 3;
	immunityFrameTimer = 1;
	immunityFrameCooldown = 1;
	superJumpTimer = -1;
	superJumpCooldown = 0.1;

	InitUI();

	//audio
	jumpSampleID = audioManager->LoadSample("sounds/jump.wav", platform);
	deathSampleID = audioManager->LoadSample("sounds/game_over.wav", platform);
	coinSound = audioManager->LoadSample("sounds/coin.wav", platform);
}

void Player::Render(float frameTime) {
	
	renderer3D->DrawSkinnedMesh(*this, skinnedMeshInstance->bone_matrices());

	//render all platforms spawned
	for (Platform* element : plat) { 
		element->Render(frameTime);
	}

	spriteRenderer->Begin(false);
	
	float SpriteSeperationX = platform.width() / 20;
	float SpriteSeperationY = platform.height() / 15;
	
	//health text
	font->RenderText(spriteRenderer, gef::Vector4(SpriteSeperationX / 4, SpriteSeperationY / 2, -1.0f), 1.5f, 0xffffffff, gef::TJ_LEFT, "Health: ");
	for (gef::Sprite sprite : healthSprites) {
		spriteRenderer->DrawSprite(sprite);
	}

	//coins
	std::string amountOfCoins = "Coins: ";
	amountOfCoins += std::to_string(coinsPickedUp);
	font->RenderText(spriteRenderer, gef::Vector4(SpriteSeperationX / 4, SpriteSeperationY * 1.5, -1.0f), 1.5f, 0xffffffff, gef::TJ_LEFT, amountOfCoins.c_str());
	spriteRenderer->DrawSprite(coinsSprite);

	//platform CD
	font->RenderText(spriteRenderer, gef::Vector4(SpriteSeperationX / 4, SpriteSeperationY * 3, -1.0f), 1.0f, 0xffffffff, gef::TJ_LEFT, "Platform CD %.2f",
		platformCooldownTimer
	);
	spriteRenderer->DrawSprite(platformSprite);

	spriteRenderer->End();
}

void Player::Update(float frameTime) {

	frameTimeLocal = frameTime;

	//volume and sound settings
	gef::VolumeInfo volumeInfo;
	volumeInfo.volume = 100;
	audioManager->SetSampleVoiceVolumeInfo(jumpSampleID, volumeInfo);

	//animation
	State();
	UpdateAnimation();

	//input
	inputManager->Update(); 
	Control(frameTime);

	//physics updates
	PhysicsSimulation(frameTime);

	//object spawned by this object
	for (Platform* element : plat) { //update all platforms spawned
		element->Update(frameTime);
	}

	HealthUI(); 
	PlatformCoolDownUI();

	//respawn
	Respawn();

	if (positionBody.y() < -10) {
		dead = true;
	}

	//timers
	dontResetJumpTimer -= frameTime;
	immunityFrameTimer -= frameTime;
	superJumpTimer -= frameTime;

	//jumpspeed if no super jump buff
	if (superJumpTimer < 0) {
		jumpSpeed = 7;
	}	
}

void Player::Control(float frameTime) {

	//walk left
	if (keyboard->IsKeyDown(keyboard->KC_A)) {
		rotationBody.RotationY(gef::DegToRad(270));
		body->SetLinearVelocity(b2Vec2(-movementSpeed * frameTime, body->GetLinearVelocity().y));
	}
	//future functionality for button down.
	else if (keyboard->IsKeyDown(keyboard->KC_S)) {
		rotationBody.RotationY(gef::DegToRad(0));
	}
	//walk right
	else if (keyboard->IsKeyDown(keyboard->KC_D)) {
		rotationBody.RotationY(gef::DegToRad(90));
		body->SetLinearVelocity(b2Vec2(movementSpeed * frameTime, body->GetLinearVelocity().y));
	}
	else {
		body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, body->GetLinearVelocity().y));
	}

	//jump
	if (keyboard->IsKeyPressed(keyboard->KC_SPACE) && (jumps > 0)) {
		jumps -= 1;
		body->SetLinearVelocity(b2Vec2(0, jumpSpeed));
		audioManager->PlaySample(jumpSampleID);
	}

	//place platform below you
	if (keyboard->IsKeyPressed(keyboard->KC_Q)) {
		
		if (numberOfPlatformsAllowed <= 0) { //are we allowed platforms?
			return;
		}

		if (platformCooldownTimer > 0) { //make sure you cannot use platform ability without waiting for the cooldown
			return;
		}

		if(plat.size() > numberOfPlatformsAllowed - 1){ //when platforms exceed number allowed in map, deconstruct it.
			delete plat[0];
			plat.erase(plat.begin());
		}

		//push back a new platform when pressing the button
		plat.push_back(new Platform(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, gef::Vector4(positionBody.x(), positionBody.y() - 2, positionBody.z()), gef::Vector4(1, 0.1, 1)));
		platformCooldownTimer = platformCooldown; //reset cooldown for placing platforms
	}

	//update platform cooldown
	platformCooldownTimer -= frameTime;
	
	//min value for cooldown
	if (platformCooldownTimer < 0) {
		platformCooldownTimer = 0;
	}

}

void Player::CollisionResponse(GameObject3D* collidedObject)
{
	//logic for resetting jumps
	if (collidedObject->typeOfObject == "platform") { //when player has contact with platform, reset jumps back to 2
		Platform* tempPlat = reinterpret_cast<Platform*>(collidedObject);

		//if hits a floor and its not a wall, reset jump
		//also, a little delay before resetting after jumping off platform so it doesn't reset right before jumping
		if (!tempPlat->GetWall() && dontResetJumpTimer < 0) {
			jumps = 2;
			dontResetJumpTimer = dontResetJumpCooldown;
		}
		
	}
	if (collidedObject->typeOfObject == "button") { //when player has contact with button, reset jumps back to 2
		if (dontResetJumpTimer < 0) {
			jumps = 2;
			dontResetJumpTimer = dontResetJumpCooldown;
		}
	}
}

void Player::PhysicsSimulation(float frameTime) {

	if (body)  // Update the transform of this object from a physics rigid body
	{
		positionBody.set_value(body->GetPosition().x, body->GetPosition().y, 0.0f);
		transformBody.Scale(gef::Vector4(0.015, 0.015, 0.015));
		transformBody = transformBody * rotationBody;
		transformBody.SetTranslation(positionBody); 

		//mesh not at y = 0 so, unique matrix is needed
		gef::Matrix44 uniqueMatrixPlayerMesh;
		uniqueMatrixPlayerMesh = transformBody;
		uniqueMatrixPlayerMesh.SetTranslation(gef::Vector4(uniqueMatrixPlayerMesh.GetTranslation().x(), uniqueMatrixPlayerMesh.GetTranslation().y() - 0.8, uniqueMatrixPlayerMesh.GetTranslation().z()));
		
		set_transform(uniqueMatrixPlayerMesh);
	}
}

void Player::DebugLog(gef::Font* font) { 

	/*font->RenderText(spriteRenderer, gef::Vector4(50, 300, -0.9f), 1.f, 0xffffffff, gef::TJ_LEFT, "Player Body X: %.1f, Y: %.1f",
		body->GetPosition().x,
		body->GetPosition().y
	);*/

	/*font->RenderText(spriteRenderer, gef::Vector4(50, 340, -0.9f), 1.f, 0xffffffff, gef::TJ_LEFT, "Velocity X: %.1f / Y: %.1f",
		body->GetLinearVelocity().x,
		body->GetLinearVelocity().y
	);*/

	//font->RenderText(spriteRenderer, gef::Vector4(50, 180, -0.9f), .5f, 0xffffffff, gef::TJ_LEFT, "Cooldown %.5f",
	//	platformCooldownTimer
	//);

	//font->RenderText(spriteRenderer, gef::Vector4(50, 200, -0.9f), .5f, 0xffffffff, gef::TJ_LEFT, "Health %.1f",
	//	health
	//);

	//font->RenderText(spriteRenderer, gef::Vector4(50, 220, -0.9f), .5f, 0xffffffff, gef::TJ_LEFT, "Coins %.1i",
	//	coinsPickedUp
	//);
}

void Player::TakeDamage(float damageNum) {

	if (immunityFrameTimer > 0) { //return if immunity frames are happening
		return;
	}
	else {
		immunityFrameTimer = immunityFrameCooldown; //reset immunity frames for next time
	}

	if (health > 0) { //decrease health
		health -= damageNum;

		if (health < 0) { //min value for health
			health = 0;
		}
	}

	if (health <= 0) { //set dead if health below 0
		dead = true;
	}
}

void Player::PickUpCoin(float number) {
	coinsPickedUp += number; //increment coin number

	if (coinSound != -1) {
		audioManager->PlaySample(coinSound); //play coin pick-up sound
	}
}

void Player::SetRespawnPoint(float x, float y) {
	respawnPoint.x = x;
	respawnPoint.y = y;
}

void Player::Respawn() {
	if (health == 0 || dead) {
		SetPosition(respawnPoint.x, respawnPoint.y, 0.1); //respawn point 
		health = maxHealth;
		audioManager->PlaySample(deathSampleID);
		dead = false;
	}
}

void Player::SetPlatformsAllowed(int num) {
	numberOfPlatformsAllowed = num;
}

void Player::HealthUI() {

	//colours Init
	UInt32 black = gef::Colour(0, 0, 0, 1).GetABGR();
	UInt32 red = gef::Colour(0.8, 0, 0, 0.8).GetABGR();

	//change colour of health UI if taken damage
	if (health == 3) { 
		healthSprites[0].set_colour(red);
		healthSprites[1].set_colour(red);
		healthSprites[2].set_colour(red);
	}
	else if (health == 2) {
		healthSprites[0].set_colour(red);
		healthSprites[1].set_colour(red);
		healthSprites[2].set_colour(black);
	}
	else if (health == 1) {
		healthSprites[0].set_colour(red);
		healthSprites[1].set_colour(black);
		healthSprites[2].set_colour(black);
	}
	else if (health <= 0) {
		healthSprites[0].set_colour(black);
		healthSprites[1].set_colour(black);
		healthSprites[2].set_colour(black);
	}
}

void Player::PlatformCoolDownUI() {

	//colours init
	UInt32 black = gef::Colour(0, 0, 0, 1).GetABGR();
	UInt32 blue = gef::Colour(0, 0, 1, 1).GetABGR();

	//if no platforms allowed, don't show plat available to press in UI.
	if (numberOfPlatformsAllowed <= 0) { 
		platformSprite.set_colour(black);
		return;
	}

	//set UI platforms available if cooldown is finished.
	if (platformCooldownTimer <= 0) { 
		platformSprite.set_colour(blue);
	}
	else {
		platformSprite.set_colour(black);
	}
}

void Player::InitUI() {

	//HEALTH UI
	gef::Sprite x;
	float healthSpriteSeperationX = platform.width() / 20;
	float healthSpriteSeperationY = platform.height() / 15;
	x.set_width(platform.width() / 35);
	x.set_height(platform.height() / 20);
	x.set_colour(gef::Colour(0.8, 0, 0, 0.8).GetABGR());
	x.set_position(healthSpriteSeperationX * 2, healthSpriteSeperationY, -1);
	healthSprites[0] = x;
	x.set_position(healthSpriteSeperationX * 3, healthSpriteSeperationY, -1);
	healthSprites[1] = x;
	x.set_position(healthSpriteSeperationX * 4, healthSpriteSeperationY, -1);
	healthSprites[2] = x;

	//COINS UI
	x.set_colour(gef::Colour(1, 1, 0, 1).GetABGR());
	x.set_position(healthSpriteSeperationX * 2, healthSpriteSeperationY * 2, -1);
	coinsSprite = x;

	//PLATFORM UI
	x.set_width(platform.width() / 20);
	x.set_colour(gef::Colour(0, 0, 1, 1).GetABGR());
	x.set_position(healthSpriteSeperationX * 3, healthSpriteSeperationY * 3, -1);
	platformSprite = x;
}

void Player::SuperJump() {
	jumpSpeed = 20; //jump boost
	jumps = 1; //dont double jump
	superJumpTimer = superJumpCooldown; //start timer that this power up is availible
}

gef::Skeleton* Player::GetFirstSkeleton(gef::Scene* scene)
{
	gef::Skeleton* skeleton = NULL;
	if (scene)
	{
		// check to see if there is a skeleton in the the scene file
		// if so, pull out the bind pose and create an array of matrices
		// that wil be used to store the bone transformations
		if (scene->skeletons.size() > 0)
			skeleton = scene->skeletons.front();
	}

	return skeleton;
}

gef::Mesh* Player::GetFirstMesh(gef::Scene* scene)
{
	gef::Mesh* mesh = NULL;

	if (scene)
	{
		// now check to see if there is any mesh data in the file, if so lets create a mesh from it
		if (scene->mesh_data.size() > 0)
			mesh = modelScene->CreateMesh(platform, scene->mesh_data.front());
	}

	return mesh;
}

gef::Animation* Player::LoadAnimation(const char* anim_scene_filename, const char* anim_name)
{
	gef::Animation* anim = NULL;

	gef::Scene anim_scene;
	if (anim_scene.ReadSceneFromFile(platform, anim_scene_filename))
	{
		// if the animation name is specified then try and find the named anim
		// otherwise return the first animation if there is one
		std::map<gef::StringId, gef::Animation*>::const_iterator anim_node_iter;
		if (anim_name)
			anim_node_iter = anim_scene.animations.find(gef::GetStringId(anim_name));
		else
			anim_node_iter = anim_scene.animations.begin();

		if (anim_node_iter != anim_scene.animations.end())
			anim = new gef::Animation(*anim_node_iter->second);
	}

	return anim;
}

void Player::State() {

	if (body->GetLinearVelocity().y != 0) {
		state = PlayerState::jumping;
	}
	else if (body->GetLinearVelocity().x != 0) {
		state = PlayerState::walking;
	}
	else if ((body->GetLinearVelocity().x == 0) && (body->GetLinearVelocity().y == 0)) {
		state = PlayerState::still;
	}

	SetAnimation();
}

void Player::SetAnimation() {

	switch (state) {
	case PlayerState::jumping:
		motionClipplayer.set_clip(jumpAnim);
		break;
	case PlayerState::walking:
		motionClipplayer.set_clip(walkAnim);
		break;
	case PlayerState::still:
		break;
	}
}

void Player::UpdateAnimation() {
	if (skinnedMeshInstance)
	{
		if (state == PlayerState::still) {
			
		}
		else {
			// update the pose in the anim player from the animation
			motionClipplayer.Update(frameTimeLocal, skinnedMeshInstance->bind_pose());
		}

		// update the bone matrices that are used for rendering the character
		// from the newly updated pose in the anim player
		skinnedMeshInstance->UpdateBoneMatrices(motionClipplayer.pose());
	}
}