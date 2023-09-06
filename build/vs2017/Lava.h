#pragma once
#include "GameObject3D.h"
#include "Player.h"

class Lava : public GameObject3D
{
public:
	Lava(gef::Platform& platformInput, gef::Renderer3D* renderer3DInput, gef::SpriteRenderer* spriteRendererInput, PrimitiveBuilder* primitiveBuilderInput, gef::InputManager* inputManagerInput, b2World* worldInput, gef::AudioManager* audioManagerInput, gef::Vector4 positionInput, gef::Vector4 sizeInput)
		: GameObject3D(platformInput, renderer3DInput, spriteRendererInput, primitiveBuilderInput, inputManagerInput, audioManagerInput, worldInput),
		position(positionInput)
	{
		sizeOfShape = sizeInput;
		Init();
	}

	~Lava() {

	}

	void Render(float frameTime) override;
	void Update(float frameTime) override;
	void CollisionResponse(GameObject3D* collidedObject) override;
	void DebugLog(gef::Font* font);
	void SetPlayer(Player*);

protected:

private:
	void Init() override;
	void PhysicsSimulation();

	bool changeColour;
	gef::Material mat;
	gef::Vector4 position;
	gef::Vector4 sizeOfShape;
	Player* player;
	
};