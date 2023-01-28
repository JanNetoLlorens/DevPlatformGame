#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Map.h"
#include "Hud.h"
#include "ModuleFadeToBlack.h"
#include "Timer.h"

#include "Defs.h"
#include "Log.h"

Hud::Hud(bool startEnabled) : Module(startEnabled)
{
	name.Create("hud");
}

// Destructor
Hud::~Hud()
{}

// Called before render is available
bool Hud::Awake(pugi::xml_node& config)
{
	LOG("Loading LoseScreen");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Hud::Start()
{
	coin = app->tex->Load(app->LoadConfigFile().child("hud").child("coin").attribute("texturepath").as_string());
	heart = app->tex->Load(app->LoadConfigFile().child("hud").child("heart").attribute("texturepath").as_string());
	app->render->camera.x = 0;
	app->render->camera.y = 0;
	
	app->win->scale = 1;

	timeCount.Start();
	timeCount_ = 0.0f;
	heartsCount = 2;
	coinsCount = 0;

	return true;
}

// Called each loop iteration
bool Hud::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Hud::Update(float dt)
{
	return true;
}

// Called each loop iteration
bool Hud::PostUpdate()
{
	bool ret = true;

	timeCount_ = timeCount.ReadSec();

	sprintf_s(totalTime, 4, "%.0f", timeCount_);
	sprintf_s(totalHearts, 4, "%d", heartsCount);
	sprintf_s(totalCoins, 4, "%d", coinsCount);

	SDL_Rect hudRect = { 0,0,350,70 };
	app->render->DrawRectangle(hudRect, 0, 0, 200, 0,true, true);

	app->render->DrawText("Time: ", 50, 10, 50, 30, { 255, 255, 255 });
	app->render->DrawText(totalTime, 100, 10, 50, 30, { 255, 255, 255 });

	SDL_Rect rect1 = { 0,0,34, 30 };
	app->render->DrawTexture(heart, 168, 10, &rect1, 0);
	app->render->DrawText(totalHearts, 220, 10, 50, 30, { 255, 255, 255 });

	SDL_Rect rect2 = { 7,7,18, 18 };
	app->render->DrawTexture(coin, 282, 16, &rect2, 0);
	app->render->DrawText(totalCoins, 310, 10, 50, 30, { 255, 255, 255 });



	return ret;
}

// Called before quitting
bool Hud::CleanUp()
{
	app->tex->UnLoad(coin);
	app->tex->UnLoad(heart);
	LOG("Freeing Hud");

	return true;
}