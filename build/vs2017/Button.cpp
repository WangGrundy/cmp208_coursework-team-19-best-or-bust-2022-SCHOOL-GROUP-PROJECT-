#include "Button.h"

void Button::Init() {
	typeOfObject = "button";
	typeOfInteraction = Interaction::BREAK;

	positionBody = position;
	gef::Mesh* m = primitiveBuilder->CreateBoxMesh(sizeOfShape);
	set_mesh(m);

	// create a physics body for the player
	transformBody.SetTranslation(positionBody);
	set_transform(transformBody);

	bodyDef->type = b2_staticBody;
	bodyDef->position = b2Vec2(positionBody.x(), positionBody.y());
	GetBodyDef()->userData.pointer = reinterpret_cast<uintptr_t>(this);
	body = world->CreateBody(bodyDef);

	// create the fixture and set it on the rigid body
	shape.SetAsBox(sizeOfShape.x(), sizeOfShape.y());
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.5;
	body->CreateFixture(&fixtureDef);

	mat.set_colour(gef::Colour(0.1, 0.1, 0.9).GetABGR());
	//triggerdMat.set_colour(gef::Colour(0.0, 1, 0.0, 0.5).GetABGR());
	triggerdMat.set_texture(texture);
	triggered = false;

	player = nullptr;
	tempPlat = nullptr;
	buttonSound = audioManager->LoadSample("sounds/button.wav", platform);
}

void Button::Render(float frameTime) {

	renderer3D->set_override_material(&mat);
	renderer3D->DrawMesh(*this);
	renderer3D->set_override_material(nullptr);

	if (tempPlat != nullptr) {
		tempPlat->Render(frameTime);
	}
}

void Button::Update(float frameTime) {
	PhysicsSimulation();
	if (tempPlat != nullptr) {
		tempPlat->Update(frameTime);
	}
}

void Button::CollisionResponse(GameObject3D* collidedObject)
{
	if (triggered) {
		return;
	}

	if (collidedObject->typeOfObject == "player") { //change material when triggerd and spawn platform

		if (typeOfInteraction == Interaction::PLACE) {
			tempPlat = new Platform(platform, renderer3D, spriteRenderer, primitiveBuilder, inputManager, audioManager, world, locationOfPlatform, gef::Vector4(1, 1, 1));
			
		}
		else if(typeOfInteraction == Interaction::BREAK){
			if (givenPlat) {
				delete givenPlat;
			}
		}
		
		triggered = true;
		mat = triggerdMat;
		audioManager->PlaySample(buttonSound);

		if (typeOfInteraction == Interaction::JUMP) {
			triggered = false; //not a one time action
			player->SuperJump();
		}

		if (buttonSound != -1) { //play button sound when pressed
			audioManager->PlaySample(buttonSound);
		}
	}
}

void Button::PhysicsSimulation() {
	if (body)  // Update the transform of this object from a physics rigid body
	{
		rotationBody.RotationZ(body->GetAngle());
		positionBody.set_value(body->GetPosition().x, body->GetPosition().y, 0.0f);
		transformBody = rotationBody;
		transformBody.SetTranslation(positionBody); 
		set_transform(transformBody);
	}
}

void Button::SetPlayer(Player* playerInput) {
	player = playerInput;
}

void Button::DebugLog(gef::Font* font) {

}

void Button::SetPlaceLocation(gef::Vector4 x) {
	locationOfPlatform = x;
}

void Button::SetBreakPlatform(Platform* plat) {
	givenPlat = plat;
}

void Button::InteractionType(Interaction x) {
	typeOfInteraction = x;
}

bool Button::GetTriggered() {
	return triggered;
}