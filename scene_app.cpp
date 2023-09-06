#include "scene_app.h"

SceneApp::SceneApp(gef::Platform& platform) :
	Application(platform),
	sprite_renderer_(NULL),
	renderer_3d_(NULL),
	primitive_builder_(NULL),
	font_(NULL)
{
}

void SceneApp::Init()
{
	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);
	renderer_3d_ = gef::Renderer3D::Create(platform_);
	primitive_builder_ = new PrimitiveBuilder(platform_);
	input_manager_ = gef::InputManager::Create(platform_);
	audioManager_ = gef::AudioManager::Create();
	sceneManager = new SceneManager(platform_, renderer_3d_, sprite_renderer_, primitive_builder_, input_manager_, audioManager_);
	InitFont();
	SetupLights();
}

void SceneApp::CleanUp()
{
	CleanUpFont();

	delete primitive_builder_;
	primitive_builder_ = NULL;
	delete renderer_3d_;
	renderer_3d_ = NULL;
	delete sprite_renderer_;
	sprite_renderer_ = NULL;
	delete sceneManager;
	sceneManager = NULL;
	delete audioManager_;
	audioManager_ = NULL;
}

bool SceneApp::Update(float frame_time)
{
	sceneManager->UpdateFrameTime(frame_time);
	sceneManager->Update();
	return true;
}

void SceneApp::Render()
{
	sceneManager->Render();
}

void SceneApp::InitFont()
{
	font_ = new gef::Font(platform_);
	font_->Load("comic_sans");
}

void SceneApp::CleanUpFont()
{
	sceneManager->SceneManagerCleanUp();
	delete font_;
	font_ = NULL;
}

void SceneApp::DrawHUD()
{

}

void SceneApp::SetupLights()
{
	// grab the data for the default shader used for rendering 3D geometry
	gef::Default3DShaderData& default_shader_data = renderer_3d_->default_shader_data();

	// set the ambient light
	default_shader_data.set_ambient_light_colour(gef::Colour(0.25f, 0.25f, 0.25f, 1.0f));

	// add a point light that is almost white, but with a blue tinge
	// the position of the light is set far away so it acts light a directional light
	gef::PointLight default_point_light;
	default_point_light.set_colour(gef::Colour(0.7f, 0.7f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(-500.0f, 400.0f, 700.0f));
	default_shader_data.AddPointLight(default_point_light);
}