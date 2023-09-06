#include "Square.h"

void Square::Init() {

	//default sprite stuff - 2D
	sprite->set_width(50.0f);
	sprite->set_height(50.0f);
	sprite->set_position(250, 250, 1); 
	sprite->set_colour(gef::Colour(1, 1, 1, 1).GetABGR());

	//physics
	//create a physics body for the player
	bodyDef->type = b2_dynamicBody;
	bodyDef->position = b2Vec2(250, 250);
	body = world->CreateBody(bodyDef);

	// create the fixture and set it on the rigid body
	shape.SetAsBox(0.5f, 0.5f);
	fixtureDef.shape = &shape;
	fixtureDef.density = 1.0f;
	body->CreateFixture(&fixtureDef);

	GetBodyDef()->userData.pointer = reinterpret_cast<uintptr_t>(this); //idk if this part works as intended
	positionSprite.set_value(0, 0, 0, 0);
}

void Square::Render(float frameTime) {
	
	spriteRenderer->DrawSprite(*sprite);
}

void Square::Update(float frameTime) {

	PhysicsSimulation();
}

void Square::PhysicsSimulation() {

	//test
	///For answering this you would first have to understand screenand world space along with view matrices.We use matrices to transformand place objects in our scene.
	///So if an object is in world space, to get it into screen space we multiply the object transformation Matrix with the world matrix(0, 0, 0) then the view matrixand 
	///then finally the projection matrix in this order specifically.These matrix calculations convert an object from world to screen space.

	transformSprite.SetTranslation(positionSprite);
	mvp = (transformSprite * view * projection);
	///These values are not normalised, you would then need to divide the x, y and z values with the W in the transformation matrix.This will give you the position in screen space between
	///- 1 and 1 which you can then translate into actual screen coordinates by multiplying the value with screen width and height.
	float XS = (mvp.GetTranslation().x()) / mvp.GetScale().w();
	float YS = (mvp.GetTranslation().y()) / mvp.GetScale().w();
	float ZS = (mvp.GetTranslation().z()) / mvp.GetScale().w();

	//test ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

	if (body)  // Update the transform of this object from a physics rigid body
	{
		rotationBody.RotationZ(body->GetAngle());
		positionBody.set_value(body->GetPosition().x, body->GetPosition().y, 0.0f);
		transformBody = rotationBody;
		transformBody.SetTranslation(positionBody);

		//test
		//SetSpritePosition(XS, YS, ZS); //set actual pos of sprite
		//test
		//^^^^^^^^^^^^^^^^^^^^ DOES NOT WORK
	}
}
	
void Square::DebugLog(gef::Font* font) {
	font->RenderText(spriteRenderer, gef::Vector4(50, 50, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "square, sprite X: %.1f, Y: %.1f, Z: %.1f",
		sprite->position().x(),
		sprite->position().y(),
		sprite->position().z()

	);
	font->RenderText(spriteRenderer, gef::Vector4(50, 80, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "square, Body X: %.1f, Y: %.1f",
		body->GetPosition().x,
		body->GetPosition().y
	);
}