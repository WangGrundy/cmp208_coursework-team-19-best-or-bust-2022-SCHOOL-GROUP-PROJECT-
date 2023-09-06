#pragma once
#include <system/platform.h> //need this to pass in platform
#include <graphics/mesh_instance.h> //need this to inherit mesh instance
#include <graphics/sprite_renderer.h>
#include <graphics/renderer_3d.h>
#include "primitive_builder.h"
#include "input/keyboard.h"
#include "input/input_manager.h"

#include <box2d/Box2D.h>
#include <graphics/sprite.h>
#include <graphics/font.h>
#include <system/debug_log.h>
#include <maths/math_utils.h>
#include <memory>
#include <math.h>
#include <array>

class GameObject2D : public gef::Sprite 
{
public:

	GameObject2D(gef::Platform& platformInput, gef::SpriteRenderer* spriteRendererInput, PrimitiveBuilder* primitiveBuilderInput, gef::InputManager* inputManagerInput, b2World* worldInput)  //need to use weird initialization cuz u get error if u don't intialize it
		: platform(platformInput),
		spriteRenderer(spriteRendererInput),
		primitiveBuilder(primitiveBuilderInput),
		inputManager(inputManagerInput),
		world(worldInput)
	{ //Reference data members must be intialized when compiler enters constructor as references can't be just declared & initialized later. This is possible only with constructor initializer list.
		Init();
	}

	virtual ~GameObject2D() {
		world->DestroyBody(body);
		delete bodyDef;
	}

	virtual void Init() = 0;
	virtual void Render(float frameTime) = 0;
	virtual void Update(float frameTime) = 0;

	//positions
	gef::Vector4 GetSpritePosition();
	b2Vec2 GetBodyPosition();

	//physics
	b2Body* GetBody();
	b2BodyDef* GetBodyDef();

	//general
	void SetWorld(b2World* worldInput);
	b2World* GetWorld(b2World* worldInput); 

	//test
	void SetMatrixes(gef::Matrix44 viewInput, gef::Matrix44 projectionInput);

	gef::Matrix44 view;
	gef::Matrix44 projection;
	gef::Matrix44 mvp;
	//test ^^^^^^^^^^^^^^^^^^^^^^^

protected:
	gef::Sprite* sprite = new gef::Sprite();
	gef::Platform& platform;
	gef::SpriteRenderer* spriteRenderer;
	gef::InputManager* inputManager;
	PrimitiveBuilder* primitiveBuilder;
	b2World* world = nullptr;

	gef::Matrix44 rotationBody;
	gef::Vector4 positionBody; 
	gef::Matrix44 transformBody;

	gef::Vector4 positionSprite;
	gef::Matrix44 transformSprite; //test

	//physics
	b2Body* body = nullptr; //created by b2d
	b2BodyDef* bodyDef = new b2BodyDef;
	b2PolygonShape shape;
	b2FixtureDef fixtureDef;

private:
	
};