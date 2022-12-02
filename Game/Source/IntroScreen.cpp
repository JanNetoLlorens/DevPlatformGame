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
#include "ModuleFadeToBlack.h"

#include "Defs.h"
#include "Log.h"

IntroScreen::IntroScreen(bool startEnabled) : Module(startEnabled)
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
	img = app->tex->Load(app->LoadConfigFile().child("introScreen").child("img").attribute("texturepath").as_string());
	berry = app->tex->Load(app->LoadConfigFile().child("introScreen").child("berry").attribute("texturepath").as_string());
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");

	//launch_fx = app->audio->LoadFx("Wahssets/Audio/Waluigi_Time.wav");

	printedRight = false;

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
	app->render->DrawTexture(img, 0, 45); // Placeholder not needed any more

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_DOWN)
		printedRight = true;
	
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_DOWN)
		printedRight = false;

	if(!printedRight)
		app->render->DrawTexture(berry, 460, 590);
	else if (printedRight)
		app->render->DrawTexture(berry, 810, 590);

	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		if (!printedRight)
			app->fade->FadeToBlack(this, (Module*)app->scene, 0);
		else if (printedRight)
			return false;
	}

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