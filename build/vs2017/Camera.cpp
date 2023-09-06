#include "Camera.h"

void Camera::Init() {

	//input
	keyboard = inputManager->keyboard();
	touchManager = inputManager->touch_manager();

	eye.set_value(0, 0, 8);
	lookAt.set_value(0, 2, 1);
	up.set_value(0, 1, 0);

	Pitch = 0, Yaw = 1, Roll = 0;
	sinR = 0, sinP = 0, sinY = 0;
	cosR = 0, cosP = 0, cosY = 0;

	aspectRatio = 1920 / 1080;
	fov = 90;
	nearPlane = 0.5f;
	farPlane = 10;

	projection = platform.PerspectiveProjectionFov(fov, aspectRatio, nearPlane, farPlane);
	renderer3D->set_projection_matrix(projection);
	renderer3D->set_view_matrix(view);
}

void Camera::Update(float frameTime) {

	FollowPlayer();

	// Only want to calculate these values once, when rotation changes, not every frame.
	cosY = cosf(Yaw * 3.1415 / 180);
	cosP = cosf(Pitch * 3.1415 / 180);
	cosR = cosf(Roll * 3.1415 / 180);
	sinY = sinf(Yaw * 3.1415 / 180);
	sinP = sinf(Pitch * 3.1415 / 180);
	sinR = sinf(Roll * 3.1415 / 180);

	// Direction camera is looking //CANT BE MORE OR LESS THAN +1/-1
	forward.set_x(sinY * cosP);
	forward.set_y(sinP);
	forward.set_z(cosP * -cosY);

	// Look At Point
	// To calculate add Forward Vector to Camera eye.
	lookAt = eye + forward;

	// Side Vector (right)
	// this is a cross product between the forward and up vector.
	right = forward.CrossProduct(up);

	// Up Vector
	up.set_x(-cosY * sinR - sinY * sinP * cosR);
	up.set_y(cosP * cosR);
	up.set_z(-sinY * sinR - sinP * cosR * -cosY);

	view.LookAt(eye, lookAt, up);

	view.LookAt(eye, lookAt, up);
	renderer3D->set_view_matrix(view);

	CameraControl(frameTime);
}

void Camera::CameraControl(float frameTime) {

	//position of camera
	if (keyboard->IsKeyDown(keyboard->KC_U)) {
		if (eye.z() > 5) { //zoom out in
			moveForward(1); //zoom in
		}
	}
	if (keyboard->IsKeyDown(keyboard->KC_H)) {
		moveLeft(1);
	}
	if (keyboard->IsKeyDown(keyboard->KC_J)) {

		if (eye.z() < 10) { //zoom out max
			moveBackwards(1); //zoom out
		}
	}

	//for debugging mostly (free camera)
	if (keyboard->IsKeyDown(keyboard->KC_K)) {
		moveRight(1);
	}

	if (keyboard->IsKeyDown(keyboard->KC_Y)) {
		moveUp();

	}
	if (keyboard->IsKeyDown(keyboard->KC_I)) {
		moveDown();
	}

}

void Camera::DebugLog(gef::Font* font) {
	font->RenderText(spriteRenderer, gef::Vector4(300, 140, -0.9f), .5f, 0xffffffff, gef::TJ_LEFT, "CAMERA EYE: X: %.1f, Y: %.1f, Z: %.1f",
		eye.x(),
		eye.y(),
		eye.z()

	);

	font->RenderText(spriteRenderer, gef::Vector4(300, 160, -0.9f), .5f, 0xffffffff, gef::TJ_LEFT, "CAMERA LOOK AT: X: %.1f, Y: %.1f, Z: %.1f",
		lookAt.x(),
		lookAt.y(),
		lookAt.z()

	);

	font->RenderText(spriteRenderer, gef::Vector4(300, 180, -0.9f), .5f, 0xffffffff, gef::TJ_LEFT, "CAMERA UP: X: %.1f, Y: %.1f, Z: %.1f",
		up.x(),
		up.y(),
		up.z()

	);
}

void Camera::SetCameraPosition(float x, float y, float z) {
	eye.set_value(x, y, z);
}

void Camera::SetCameraPositionX(float x) {
	eye.set_x(eye.x() + x);
}
void Camera::SetCameraPositionY(float y) {
	eye.set_y(eye.y() + y);
}
void Camera::SetCameraPositionZ(float z) {
	eye.set_z(eye.z() + z);
}

gef::Vector4 Camera::GetCameraPosition() {
	return eye;
}

//////////////////////////////////////////

void Camera::SetLookAt(float x, float y, float z) {
	forward.set_value(x, y, z);
}

void Camera::SetLookAtX(float x) {
	forward.set_x(forward.x() + x);
}
void Camera::SetLookAtY(float y) {
	forward.set_y(forward.y() + y);
}
void Camera::SetLookAtZ(float z) {
	forward.set_z(forward.z() + z);
}

gef::Vector4 Camera::GetLookAt() {
	return lookAt;
}

////////////////////////////////////////////////////////////////////

void Camera::SetUpVector(float x, float y, float z) {
	up.set_value(x, y, z);
}

void Camera::SetUpVectorX(float x) {
	up.set_x(up.x() + x);
}
void Camera::SetUpVectorY(float y) {
	up.set_y(up.y() + y);
}
void Camera::SetUpVectorZ(float z) {
	up.set_z(up.z() + z);
}

gef::Vector4 Camera::GetUpVector() {
	return up;
}

/////////////////////////////////////////////////////////////////

void Camera::SetPitch(float x) {

}

void Camera::SetYaw(float y) {
	Yaw += y;
}

void Camera::SetRoll(float z) {
	Roll += z;
}

float Camera::GetPitch() {
	return Pitch;
}

float Camera::GetYaw() {
	return Yaw;
}

float Camera::GetRoll() {
	return Roll;
}

void Camera::moveForward(float dt)
{
	eye = eye + (forward / speed);
}

void Camera::moveBackwards(float dt)
{
	eye = eye - (forward / speed);
}

void Camera::moveLeft(float dt) {
	eye = eye - (right / speed);
}

void Camera::moveRight(float dt) {
	eye = eye + (right / speed);
}

void Camera::moveUp() {

	eye.set_y(eye.y() + speed / 60);
}

void Camera::moveDown() {
	eye.set_y(eye.y() - speed / 60);
}

void Camera::SetForward(float x, float y, float z) {
	forward.set_x(x);
	forward.set_y(y);
	forward.set_z(z);
}

gef::Vector4 Camera::GetForward() {
	return forward;
}

gef::Matrix44 Camera::GetProjection() {
	return projection;
}

gef::Matrix44 Camera::GetView() {
	return view;
}

void Camera::SetPlayerCoords(b2Vec2 coords) {
	playerCoords = coords;
}

void Camera::FollowPlayer() {
	eye.set_x(playerCoords.x);
	eye.set_y(playerCoords.y + 2);
}