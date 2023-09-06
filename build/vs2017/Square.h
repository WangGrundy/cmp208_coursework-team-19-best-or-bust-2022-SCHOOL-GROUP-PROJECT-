#pragma once
#include "GameObject2D.h"
class Square : public GameObject2D //class is in test mode. 
{
public:
	Square(gef::Platform& platformInput, gef::SpriteRenderer* spriteRendererInput, PrimitiveBuilder* primitiveBuilderInput, gef::InputManager* inputManagerInput, b2World* worldInput)  //need to use weird initialization cuz u get error if u don't intialize it
		: GameObject2D(platformInput, spriteRendererInput, primitiveBuilderInput, inputManagerInput, worldInput)
	{ //Reference data members must be intialized when compiler enters constructor as references can't be just declared & initialized later. This is possible only with constructor initializer list.
		Init();
	}

	~Square() {
	}

	void Render(float frameTime) override;
	void Update(float frameTime) override;
	void PhysicsSimulation();
	void DebugLog(gef::Font*);

protected:

private:
	void Init() override; 
};