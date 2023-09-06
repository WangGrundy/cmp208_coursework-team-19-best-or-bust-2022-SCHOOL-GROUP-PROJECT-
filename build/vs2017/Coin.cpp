#include "coin.h"

void Coin::Init() {
	typeOfObject = "coin";
	triggered = false;
	rotationAmount = 100;

	positionBody = position;
	gef::Mesh* m = primitiveBuilder->CreateSphereMesh(sizeOfShape.x(), 100, 100, gef::Vector4(0.0, 0.0, 0.0));
	set_mesh(m);

	// create a physics body for the player
	transformBody.SetTranslation(positionBody);
	set_transform(transformBody);

	bodyDef->type = b2_staticBody;
	bodyDef->position = b2Vec2(positionBody.x(), positionBody.y());
	bodyDef->awake = true;
	bodyDef->allowSleep = false;
	GetBodyDef()->userData.pointer = reinterpret_cast<uintptr_t>(this);
	body = world->CreateBody(bodyDef);
	body->SetAwake(true);
	body->SetSleepingAllowed(false);

	// create the fixture and set it on the rigid body
	shape.SetAsBox(sizeOfShape.x(), sizeOfShape.y());
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.5;
	fixtureDef.isSensor = true;
	body->CreateFixture(&fixtureDef);

	mat.set_colour(gef::Colour(1, 1, 0.0, 1).GetABGR()); //set yellow colour
	coinRotation = 0; //set original rotation to be 0 degrees
}

void Coin::Render(float frameTime) {
	renderer3D->set_override_material(&mat);
	renderer3D->DrawMesh(*this);
	renderer3D->set_override_material(nullptr);
}

void Coin::Update(float frameTime) {
	PhysicsSimulation();
	coinRotation += frameTime * rotationAmount; //add to rotation every frame.
}

void Coin::CollisionResponse(GameObject3D* collidedObject)
{
	//if player is collided with, add coin to player
	if (collidedObject->typeOfObject == "player") {  
		player->PickUpCoin(1);
		triggered = true;
	}
}

void Coin::PhysicsSimulation() {
	
	if (body)  // Update the transform of this object from a physics rigid body
	{	
		transformBody.SetIdentity(); //saving stuff here as the final matrix.
		transformBody.Scale(gef::Vector4(0.1, 0.1, 0.1)); //scale the model
		rotationBody.SetIdentity();
		rotationBody.RotationX(gef::DegToRad(90));
		gef::Matrix44 rotZ;
		rotZ.SetIdentity();
		rotZ.RotationZ(gef::DegToRad(coinRotation));
		rotationBody = rotZ * rotationBody; //for some reason u need to do Z * X

		transformBody = transformBody * rotationBody; //set matrices to be rotation and scale.

		positionBody.set_value(body->GetPosition().x, body->GetPosition().y, 0.0f); //Get position of model placement
		transformBody.SetTranslation(positionBody); //set the actual transform in another matrix

		set_transform(transformBody); //set the actual transform for the model with the final matrix
	}
}

bool Coin::IsTriggered() {
	return triggered;
}

void Coin::SetPlayer(Player* playerInput) {
	player = playerInput;
}

void Coin::DebugLog(gef::Font* font) {
	
}

void Coin::CleanUp() {
	
}