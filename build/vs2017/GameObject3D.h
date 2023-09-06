#pragma once
#include <system/platform.h> //need this to pass in platform
#include <graphics/mesh_instance.h> //need this to inherit mesh instance
#include <graphics/sprite_renderer.h>
#include <graphics/renderer_3d.h>
#include "primitive_builder.h"
#include "input/keyboard.h"
#include "input/input_manager.h"
#include <audio/audio_manager.h>

#include <box2d/Box2D.h>
#include <graphics/sprite.h>
#include <graphics/font.h>
#include <system/debug_log.h>
#include <maths/math_utils.h>
#include <memory>
#include <math.h>
#include <array>

class GameObject3D : public gef::MeshInstance
{
public:

	GameObject3D(gef::Platform& platformInput, gef::Renderer3D* renderer3DInput, gef::SpriteRenderer* spriteRendererInput, PrimitiveBuilder* primitiveBuilderInput, gef::InputManager* inputManagerInput, gef::AudioManager* audioManagerInput, b2World* worldInput)  //need to use weird initialization cuz u get error if u don't intialize it
		: platform(platformInput),
		renderer3D(renderer3DInput),
		spriteRenderer(spriteRendererInput),
		primitiveBuilder(primitiveBuilderInput),
		inputManager(inputManagerInput),
		audioManager(audioManagerInput),
		world(worldInput)

	{ 
		Init();
	}

	virtual ~GameObject3D() {
		world->DestroyBody(body);
		delete bodyDef;
	}

	virtual void Init() = 0;
	virtual void Render(float frameTime) = 0;
	virtual void Update(float frameTime) = 0;

	//position
	b2Vec2 GetPosition();
	void SetPosition(float x, float y, float z);

	//physics
	b2Body* GetBody();
	b2BodyDef* GetBodyDef();

	//mesh
	void SetMesh(gef::Mesh*); //only need to set mesh using mesh instance.
	gef::MeshInstance GetMeshInstance();

	//general
	void SetWorld(b2World* worldInput);
	b2World* GetWorld(b2World* worldInput); //might be nullptr

	virtual void CollisionResponse(GameObject3D* objectCollided);

	std::string typeOfObject; //public string
protected:

	gef::Platform& platform;
	gef::Renderer3D* renderer3D;
	gef::SpriteRenderer* spriteRenderer;
	gef::InputManager* inputManager;
	PrimitiveBuilder* primitiveBuilder;
	gef::AudioManager* audioManager;
	b2World* world = nullptr;

	b2Body* body = nullptr; //created by b2d
	b2BodyDef* bodyDef = new b2BodyDef;
	b2PolygonShape shape;
	b2FixtureDef fixtureDef;

	gef::Matrix44 rotationBody;
	gef::Vector4 positionBody;
	gef::Matrix44 transformBody;

	gef::MeshInstance meshInstance;
	gef::Mesh* mesh;

private:

};

