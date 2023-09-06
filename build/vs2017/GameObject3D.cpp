#include "GameObject3D.h"

void GameObject3D::Init() {
	typeOfObject = "";
}

b2Body* GameObject3D::GetBody() {
	return body;
}

b2BodyDef* GameObject3D::GetBodyDef() {
	return bodyDef;
}

void GameObject3D::SetWorld(b2World* worldInput) {
	world = worldInput;
}

b2World* GameObject3D::GetWorld(b2World* worldInput) {
	return world;
}

void GameObject3D::CollisionResponse(GameObject3D* objectCollided)
{

}

void GameObject3D::SetMesh(gef::Mesh* meshInput) {
	meshInstance.set_mesh(meshInput);
	set_mesh(meshInput);
}

gef::MeshInstance GameObject3D::GetMeshInstance() {
	return meshInstance;
}

b2Vec2 GameObject3D::GetPosition() {
	return body->GetPosition();
}

void GameObject3D::SetPosition(float xInput, float yInput, float zInput) { 

	positionBody.set_value(xInput, yInput, 0.0f);
	transformBody = rotationBody;
	transformBody.SetTranslation(positionBody); 
	set_transform(transformBody);
	body->SetTransform(b2Vec2(xInput, yInput), 0);
}