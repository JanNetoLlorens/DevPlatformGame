#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Map.h"
#include "EndingScreen.h"

#include "Defs.h"
#include "Log.h"

EndingScreen::EndingScreen(bool startEnabled) : Module(startEnabled)
{
	name.Create("introScreen");
}

// Destructor
EndingScreen::~EndingScreen()
{}

// Called before render is available
bool EndingScreen::Awake(pugi::xml_node& config)
{
	LOG("Loading EndingScreen");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool EndingScreen::Start()
{
	//img = app->tex->Load("Assets/Textures/test.png");
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");

	return true;
}

// Called each loop iteration
bool EndingScreen::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool EndingScreen::Update(float dt)
{
	//app->render->DrawTexture(img, 380, 100); // Placeholder not needed any more

	return true;
}

// Called each loop iteration
bool EndingScreen::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool EndingScreen::CleanUp()
{
	LOG("Freeing EndingScreen");

	return true;
}