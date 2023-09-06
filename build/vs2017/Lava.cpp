#include "Lava.h"

void Lava::Init() {
	typeOfObject = "lava";

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

	mat.set_colour(gef::Colour(0.7, 0.05, 0.05).GetABGR());
	player = nullptr;
}

void Lava::Render(float frameTime) {
	
	renderer3D->set_override_material(&mat);
	renderer3D->DrawMesh(*this);
	renderer3D->set_override_material(nullptr);
}

void Lava::Update(float frameTime) {

	PhysicsSimulation();
}

void Lava::CollisionResponse(GameObject3D* collidedObject)
{
	if (collidedObject->typeOfObject == "player") {
		Player* x = reinterpret_cast<Player*>(collidedObject);
		x->TakeDamage(1.0f); //constantly tick 100 damage
	}
}

void Lava::PhysicsSimulation() {
	if (body)  // Update the transform of this object from a physics rigid body
	{
		rotationBody.RotationZ(body->GetAngle());
		positionBody.set_value(body->GetPosition().x, body->GetPosition().y, 0.0f);
		transformBody = rotationBody;
		transformBody.SetTranslation(positionBody); //actual movement here
		set_transform(transformBody);
	}
}

void Lava::SetPlayer(Player* playerInput) {
	player = playerInput;
}

void Lava::DebugLog(gef::Font* font) {

}