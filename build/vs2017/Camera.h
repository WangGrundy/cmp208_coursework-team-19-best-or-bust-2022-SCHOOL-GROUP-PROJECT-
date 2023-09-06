#pragma once
#include <Platform.h>
#include <maths/matrix44.h>
#include <graphics/renderer_3d.h>
#include "input/keyboard.h"
#include "input/input_manager.h"
#include <graphics/sprite_renderer.h>

class Camera
{
public:
	Camera(gef::Platform& platformInput, gef::Renderer3D* renderer3DInput, gef::InputManager* inputManagerInput, gef::SpriteRenderer* spriteRendererInput)
		: platform(platformInput),
		renderer3D(renderer3DInput),
		inputManager(inputManagerInput),
		spriteRenderer(spriteRendererInput)
	{
		Init();
	}

	void Update(float frameTime);
	void CameraControl(float frameTime);
	void DebugLog(gef::Font* font);
	void SetPlayerCoords(b2Vec2);
	void FollowPlayer();

	void SetCameraPosition(float, float, float);
	void SetCameraPositionX(float);
	void SetCameraPositionY(float);
	void SetCameraPositionZ(float);
	gef::Vector4 GetCameraPosition();

	void SetLookAt(float, float, float);
	void SetLookAtX(float);
	void SetLookAtY(float);
	void SetLookAtZ(float);
	gef::Vector4 GetLookAt();

	void SetUpVector(float, float, float);
	void SetUpVectorX(float);
	void SetUpVectorY(float);
	void SetUpVectorZ(float);
	gef::Vector4 GetUpVector();

	void SetPitch(float);
	void SetYaw(float);
	void SetRoll(float);
	float GetPitch();
	float GetYaw();
	float GetRoll();

	void moveForward(float dt);
	void moveBackwards(float dt);
	void moveLeft(float dt);
	void moveRight(float dt);
	void moveDown();
	void moveUp();

	void SetForward(float, float, float);
	gef::Vector4 GetForward();

	gef::Matrix44 GetProjection();
	gef::Matrix44 GetView();


protected:

private:
	void Init();

	gef::InputManager* inputManager;
	gef::Keyboard* keyboard;
	gef::TouchInputManager* touchManager;
	gef::SpriteRenderer* spriteRenderer;

	gef::Matrix44 projection;
	gef::Matrix44 view;
	gef::Platform& platform;
	gef::Renderer3D* renderer3D;

	float aspectRatio;
	float fov;
	float nearPlane;
	float farPlane;

	gef::Vector4 eye;
	gef::Vector4 lookAt;
	gef::Vector4 up;

	gef::Vector4 forward;
	gef::Vector4 right;

	float cosR, cosP, cosY;
	float sinR, sinP, sinY;
	float Pitch, Yaw, Roll;

	float speed = 10;

	b2Vec2 playerCoords;
};

