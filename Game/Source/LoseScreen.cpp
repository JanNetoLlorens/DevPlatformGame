#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Map.h"
#include "LoseScreen.h"
#include "ModuleFadeToBlack.h"

#include "Defs.h"
#include "Log.h"

LoseScreen::LoseScreen(bool startEnabled) : Module(startEnabled)
{
	name.Create("introScreen");
}

// Destructor
LoseScreen::~LoseScreen()
{}

// Called before render is available
bool LoseScreen::Awake(pugi::xml_node& config)
{
	LOG("Loading LoseScreen");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool LoseScreen::Start()
{
	img = app->tex->Load(app->LoadConfigFile().child("LoseScreen").child("img").attribute("texturepath").as_string());
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");

	return true;
}

// Called each loop iteration
bool LoseScreen::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool LoseScreen::Update(float dt)
{
	app->render->DrawTexture(img, 0, 20); // Placeholder not needed any more

	return true;
}

// Called each loop iteration
bool LoseScreen::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool LoseScreen::CleanUp()
{
	LOG("Freeing LoseScreen");

	return true;
}