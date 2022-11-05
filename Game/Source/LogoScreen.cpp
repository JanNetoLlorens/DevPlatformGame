#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Map.h"
#include "LogoScreen.h"
#include "ModuleFadeToBlack.h"

#include "Defs.h"
#include "Log.h"

LogoScreen::LogoScreen(bool startEnabled) : Module(startEnabled)
{
	name.Create("logoScreen");
}

// Destructor
LogoScreen::~LogoScreen()
{}

// Called before render is available
bool LogoScreen::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool LogoScreen::Start()
{
	//img = app->tex->Load("Assets/Textures/Logo.png");
	img = app->tex->Load(app->LoadConfigFile().child("logoScreen").child("img").attribute("texturepath").as_string());
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");

	return true;
}

// Called each loop iteration
bool LogoScreen::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool LogoScreen::Update(float dt)
{
	app->render->DrawTexture(img, 0, 20); // Placeholder not needed any more

	app->fade->FadeToBlack(this, (Module*)app->introScreen, 120);

	return true;
}

// Called each loop iteration
bool LogoScreen::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool LogoScreen::CleanUp()
{
	LOG("Freeing LogoScreen");

	return true;
}