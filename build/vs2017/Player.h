#pragma once
#include "GameObject3D.h"
#include "Platform.h" //my object class platform
#include <graphics/scene.h>
#include <animation/skeleton.h>
#include <animation/animation.h>
#include <graphics/skinned_mesh_instance.h>
#include "motion_clip_player.h"



class Player : public GameObject3D
{
	enum PlayerState {
		walking, jumping, still
	};

public:
	Player(gef::Platform& platformInput, gef::Renderer3D* renderer3DInput, gef::SpriteRenderer* spriteRendererInput, PrimitiveBuilder* primitiveBuilderInput, gef::InputManager* inputManagerInput, gef::AudioManager* audioManagerInput, b2World* worldInput, gef::Vector4 positionInput, gef::Font* fontInput)  //need to use weird initialization cuz u get error if u don't intialize it
		: GameObject3D(platformInput, renderer3DInput, spriteRendererInput, primitiveBuilderInput, inputManagerInput, audioManagerInput, worldInput),
		font(fontInput)
	{ //Reference data members must be intialized when compiler enters constructor as references can't be just declared & initialized later. This is possible only with constructor initializer list.
		positionBody = positionInput;
		Init();
	}

	~Player() {
		plat.clear();
		audioManager->UnloadSample(jumpSampleID);
		audioManager->UnloadSample(coinSound);
		audioManager->UnloadSample(deathSampleID);
	}
	
	void Render(float frameTime) override;
	void Update(float frameTime) override;
	void CollisionResponse(GameObject3D* collidedObject) override;
	void TakeDamage(float damageNum);
	void PickUpCoin(float number);
	void SetRespawnPoint(float x, float y);
	void SetPlatformsAllowed(int num);
	void DebugLog(gef::Font* font);
	void SuperJump();

protected:

private:
	void Init() override; //treat this as constructor
	void PlatformCoolDownUI();
	void Respawn();
	void Control(float frameTime);
	void PhysicsSimulation(float frameTime);
	void InitUI();
	void HealthUI();
	void State();
	void SetAnimation();
	void UpdateAnimation();
	gef::Skeleton* Player::GetFirstSkeleton(gef::Scene* scene);
	gef::Mesh* Player::GetFirstMesh(gef::Scene* scene);
	gef::Animation* LoadAnimation(const char* anim_scene_filename, const char* anim_name);

	gef::Keyboard* keyboard;
	gef::TouchInputManager* touchManager;
	float movementSpeed;
	float jumpSpeed;
	int numberOfPlatformsAllowed;
	gef::Font* font;

	std::vector<Platform*> plat;
	float health, maxHealth;
	bool dead;
	int jumps;
	int coinsPickedUp;

	//timers
	float platformCooldownTimer;
	float platformCooldown;
	float dontResetJumpTimer;
	float dontResetJumpCooldown;
	float immunityFrameTimer;
	float immunityFrameCooldown;
	float superJumpTimer;
	float superJumpCooldown;

	gef::Vector2 respawnPoint;

	gef::Matrix44 rotationOriginal;

	gef::Sprite healthSprites[3];
	gef::Sprite coinsSprite;
	gef::Sprite platformSprite;

	int32 jumpSampleID;
	int32 deathSampleID;
	int32 coinSound;
	int32 musicID;

	//animation
	gef::Scene* modelScene;
	gef::SkinnedMeshInstance* skinnedMeshInstance;
	MotionClipPlayer motionClipplayer;
	gef::Animation* walkAnim;
	gef::Animation* jumpAnim;

	PlayerState state;
	float frameTimeLocal;
};