#include "Platform.h"

void Platform::Init() {
	typeOfObject = "platform";
	isWall = false;

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

	platMat.set_colour(gef::Colour(0,1,0).GetABGR());
}

void Platform::Render(float frameTime) {
	if (changeColour) {
		renderer3D->set_override_material(&primitiveBuilder->red_material());
	}
	if (!changeColour) {
		renderer3D->set_override_material(&platMat);
	}

	renderer3D->DrawMesh(*this);
	renderer3D->set_override_material(nullptr);
}

void Platform::Update(float frameTime) {
	PhysicsSimulation();
	changeColour = false;
}

void Platform::CollisionResponse(GameObject3D* collidedObject)
{
	Platform* plat = reinterpret_cast<Platform*>(collidedObject); //change colour if collided with player
	if (plat) {
		ChangeColour(true);
	}
}

void Platform::SetWallColour(gef::Colour col) {
	platMat.set_colour(col.GetABGR());
}

void Platform::PhysicsSimulation() { 
	if (body)  // Update the transform of this object from a physics rigid body
	{
		rotationBody.RotationZ(body->GetAngle());
		positionBody.set_value(body->GetPosition().x, body->GetPosition().y, 0.0f);
		transformBody = rotationBody;
		transformBody.SetTranslation(positionBody); //actual movement here
		set_transform(transformBody);
	}
}

void Platform::DebugLog(gef::Font* font) {

}

void Platform::ChangeColour(bool change) {

	if (change) {
		changeColour = true;
	}
	else {
		changeColour = false;
	}
}

void Platform::SetWall(bool x) {
	isWall = x;
}

bool Platform::GetWall() {
	return isWall;
}