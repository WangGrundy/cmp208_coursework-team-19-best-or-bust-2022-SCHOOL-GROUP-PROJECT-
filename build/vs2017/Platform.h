#pragma once
#include "GameObject3D.h"

class Platform : public GameObject3D
{
public:
	Platform(gef::Platform& platformInput, gef::Renderer3D* renderer3DInput, gef::SpriteRenderer* spriteRendererInput, PrimitiveBuilder* primitiveBuilderInput, gef::InputManager* inputManagerInput, gef::AudioManager* audioManagerInput, b2World* worldInput, gef::Vector4 positionInput, gef::Vector4 sizeInput)
		: GameObject3D(platformInput, renderer3DInput, spriteRendererInput, primitiveBuilderInput, inputManagerInput, audioManagerInput, worldInput),
		position(positionInput)
	{ 
		sizeOfShape = sizeInput;
		Init();
	}

	~Platform() {
		
	}

	void Render(float frameTime) override;
	void Update(float frameTime) override;
	
	void DebugLog(gef::Font* font);
	void ChangeColour(bool);
	void CollisionResponse(GameObject3D* collidedObject) override;
	void SetWallColour(gef::Colour);
	void SetWall(bool);
	bool GetWall();

protected:

private:
	void Init() override; 
	void PhysicsSimulation();

	bool changeColour;
	gef::Material platMat;
	gef::Vector4 position;
	gef::Vector4 sizeOfShape;
	bool isWall;
};