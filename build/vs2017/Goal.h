#pragma once
#include "GameObject3D.h"

class Goal : public GameObject3D
{
public:
	Goal(gef::Platform& platformInput, gef::Renderer3D* renderer3DInput, gef::SpriteRenderer* spriteRendererInput, PrimitiveBuilder* primitiveBuilderInput, gef::InputManager* inputManagerInput, gef::AudioManager* audioManagerInput, b2World* worldInput, gef::Vector4 positionInput, gef::Vector4 sizeInput)
		: GameObject3D(platformInput, renderer3DInput, spriteRendererInput, primitiveBuilderInput, inputManagerInput, audioManagerInput, worldInput),
		position(positionInput)
	{
		sizeOfShape = sizeInput;
		Init();
	}

	~Goal() {
		
	}

	void Render(float frameTime) override;
	void Update(float frameTime) override;
	void DebugLog(gef::Font* font);
	void CollisionResponse(GameObject3D* collidedObject) override;
	bool IsTriggered();

protected:

private:
	void Init() override;
	void PhysicsSimulation();

	gef::Material mat;
	gef::Vector4 position;
	gef::Vector4 sizeOfShape;
	bool triggered;

	int32 goalSound;
};