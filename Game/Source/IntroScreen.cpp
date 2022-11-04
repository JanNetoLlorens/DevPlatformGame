#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Map.h"
#include "IntroScreen.h"

#include "Defs.h"
#include "Log.h"

IntroScreen::IntroScreen() : Module()
{
	name.Create("introScreen");
}

// Destructor
IntroScreen::~IntroScreen()
{}

// Called before render is available
bool IntroScreen::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool IntroScreen::Start()
{
	//img = app->tex->Load("Assets/Textures/test.png");
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");

	return true;
}

// Called each loop iteration
bool IntroScreen::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool IntroScreen::Update(float dt)
{
	//app->render->DrawTexture(img, 380, 100); // Placeholder not needed any more

	return true;
}

// Called each loop iteration
bool IntroScreen::PostUpdate()
{
	bool ret = true;

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool IntroScreen::CleanUp()
{
	LOG("Freeing LogoScreen");

	return true;
}