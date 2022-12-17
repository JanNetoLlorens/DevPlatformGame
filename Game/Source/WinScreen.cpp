#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Map.h"
#include "WinScreen.h"
#include "ModuleFadeToBlack.h"

#include "Defs.h"
#include "Log.h"

WinScreen::WinScreen(bool startEnabled) : Module(startEnabled)
{
	name.Create("WinScreen");
}

// Destructor
WinScreen::~WinScreen()
{}

// Called before render is available
bool WinScreen::Awake(pugi::xml_node& config)
{
	LOG("Loading EndingScreen");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool WinScreen::Start()
{
	img = app->tex->Load(app->LoadConfigFile().child("winScreen").child("img").attribute("texturepath").as_string());
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");
	
	app->render->camera.x = 0;
	app->render->camera.y = 0;

	app->win->scale = 1;

	return true;
}

// Called each loop iteration
bool WinScreen::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool WinScreen::Update(float dt)
{
	app->render->DrawTexture(img, 0, 20); // Placeholder not needed any more

	return true;
}

// Called each loop iteration
bool WinScreen::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool WinScreen::CleanUp()
{
	LOG("Freeing EndingScreen");

	return true;
}