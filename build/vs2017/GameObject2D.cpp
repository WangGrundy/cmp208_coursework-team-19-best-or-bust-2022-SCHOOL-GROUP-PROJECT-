#include "GameObject2D.h"

void GameObject2D::Init() {

}

b2Body* GameObject2D::GetBody() {
	return body;
}

b2BodyDef* GameObject2D::GetBodyDef() {
	return bodyDef;
}

void GameObject2D::SetWorld(b2World* worldInput) {
	world = worldInput;
}

b2World* GameObject2D::GetWorld(b2World* worldInput) {
	return world;
}

gef::Vector4 GameObject2D::GetSpritePosition() {
	return sprite->position();
}

b2Vec2 GameObject2D::GetBodyPosition() {
	return body->GetPosition();
}

void GameObject2D::SetMatrixes(gef::Matrix44 viewInput, gef::Matrix44 projectionInput) {
	view = viewInput;
	projection = projectionInput;
}