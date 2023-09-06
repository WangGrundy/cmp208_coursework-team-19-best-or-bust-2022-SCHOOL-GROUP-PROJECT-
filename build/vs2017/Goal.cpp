#include "Goal.h"

void Goal::Init() {
	typeOfObject = "goal";
	triggered = false;

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
	fixtureDef.isSensor = true;
	body->CreateFixture(&fixtureDef);

	goalSound = audioManager->LoadSample("sounds/level_complete.wav", platform);
}

void Goal::Render(float frameTime) {

	renderer3D->set_override_material(&mat);
	renderer3D->DrawMesh(*this);
	renderer3D->set_override_material(nullptr);
}

void Goal::Update(float frameTime) {

	PhysicsSimulation();

}

void Goal::CollisionResponse(GameObject3D* collidedObject)
{
	if (triggered) {
		return;
	}

	if (collidedObject->typeOfObject == "player") {
		if ((goalSound != -1)) { //play goal reached sound
			audioManager->PlaySample(goalSound);
		}
		triggered = true;
	}
}

void Goal::PhysicsSimulation() {
	if (body)  // Update the transform of this object from a physics rigid body
	{
		transformBody.SetIdentity();
		transformBody.Scale(gef::Vector4(0.3, 0.3, 0.3));
		rotationBody.SetIdentity();
		rotationBody.RotationY(gef::DegToRad(270));

		transformBody = transformBody * rotationBody;

		positionBody.set_value(body->GetPosition().x, body->GetPosition().y, 0.0f);
		
		transformBody.SetTranslation(positionBody); 
		set_transform(transformBody);
	}
}

bool Goal::IsTriggered() {
	return triggered;
}

void Goal::DebugLog(gef::Font* font) {

}