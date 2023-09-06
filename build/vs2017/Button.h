#pragma once
#include "GameObject3D.h"
#include "Player.h"
#include "Platform.h"
#include <graphics/texture.h>

enum Interaction
{
	PLACE,
	BREAK,
	JUMP
};

class Button : public GameObject3D
{
	

public:
	Button(gef::Platform& platformInput, gef::Renderer3D* renderer3DInput, gef::SpriteRenderer* spriteRendererInput, PrimitiveBuilder* primitiveBuilderInput, gef::InputManager* inputManagerInput, gef::AudioManager* audioManagerInput, b2World* worldInput, gef::Texture* textureInput, gef::Vector4 positionInput, gef::Vector4 sizeInput)
		: GameObject3D(platformInput, renderer3DInput, spriteRendererInput, primitiveBuilderInput, inputManagerInput, audioManagerInput, worldInput),
		position(positionInput),
		texture(textureInput)
	{
		sizeOfShape = sizeInput;
		Init();
	}

	~Button() {
		delete tempPlat;
	}

	void Render(float frameTime) override;
	void Update(float frameTime) override;
	void PhysicsSimulation();
	void DebugLog(gef::Font* font);

	void SetPlayer(Player*);
	void InteractionType(Interaction);
	void SetPlaceLocation(gef::Vector4);
	void SetBreakPlatform(Platform* plat);
	bool GetTriggered();

protected:

private:
	void Init() override;
	void CollisionResponse(GameObject3D* collidedObject) override;
	bool changeColour;
	gef::Material mat;
	gef::Material triggerdMat;
	gef::Vector4 position;
	gef::Vector4 sizeOfShape;
	Player* player;
	bool triggered;
	Platform* tempPlat;
	Platform* givenPlat;
	gef::Texture* texture;
	int32 buttonSound;
	gef::Vector4 locationOfPlatform;
	Interaction typeOfInteraction;

};

