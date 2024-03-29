#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "EntityManager.h"
#include "Map.h"
#include "ModuleFadeToBlack.h"
#include "Pathfinding.h"
#include "GuiManager.h"
#include "IntroScreen.h"
#include "Hud.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene(bool startEnabled) : Module(startEnabled)
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake(pugi::xml_node& config)
{
	LOG("Loading Scene");
	bool ret = true;

	// iterate all objects in the scene
	// Check https://pugixml.org/docs/quickstart.html#access

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	app->win->scale = 1;

	for (pugi::xml_node itemNode = app->LoadConfigFile().child("scene").child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::ITEM);
		item->parameters = itemNode;
		items.Add(item);
	}
	for (pugi::xml_node itemNode = app->LoadConfigFile().child("scene").child("heart"); itemNode; itemNode = itemNode.next_sibling("heart"))
	{
		Item* item = (Item*)app->entityManager->CreateEntity(EntityType::HEART);
		item->parameters = itemNode;
		items.Add(item);
	}

	checkpoint = (CheckPoint*)app->entityManager->CreateEntity(EntityType::CHECKPOINT);
	checkpoint->parameters = app->LoadConfigFile().child("scene").child("checkpoint");

	//L02: DONE 3: Instantiate the player using the entity manager
	player = (Player*)app->entityManager->CreateEntity(EntityType::PLAYER);
	player->parameters = app->LoadConfigFile().child("scene").child("player");

	walkingEn = (WalkingEnemy*)app->entityManager->CreateEntity(EntityType::WALKING_ENEMY);
	walkingEn->parameters = app->LoadConfigFile().child("scene").child("walkingEnemy");
	walkingEnList.Add(walkingEn);


	flyingEn = (FlyingEnemy*)app->entityManager->CreateEntity(EntityType::FLYING_ENEMY);
	flyingEn->parameters = app->LoadConfigFile().child("scene").child("flyingEnemy");
	flyingEnList.Add(flyingEn);

	app->audio->PlayMusic("One Piece Game Dev Assets/Music/Bazooka.wav");

	
	// L03: DONE: Load map
	bool retLoad = app->map->Load();

	//Create walkability map
	if (retLoad) {
		int w, h;
		uchar* data = NULL;
		
		bool retWalkMap = app->map->CreateWalkabilityMap(w, h, &data);
		if (retWalkMap) app->pathfinding->SetMap(w, h, data);

		RELEASE_ARRAY(data);
	}

	// L04: DONE 7: Set the window title with map/tileset info
	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
		app->map->mapData.width,
		app->map->mapData.height,
		app->map->mapData.tileWidth,
		app->map->mapData.tileHeight,
		app->map->mapData.tilesets.Count());

	app->win->SetTitle(title.GetString());

	

	// Texture to highligh mouse position 
	mouseTileTex = app->tex->Load("Assets/Textures/path_square.png");

	// Texture to show path origin 
	originTex = app->tex->Load("Assets/Textures/x_square.png");
	
	app->guiManager->isMenuActive = false;
	app->guiManager->isSettingsActive = false;

	//Declare a GUI
	uint w, h;
	app->win->GetWindowSize(w, h);

	//menu
	resume = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "RESUME", { (int)w / 2 - 50,(int)h / 2 - 90,90,35 }, this);
	settings = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "SETTINGS", { (int)w / 2 - 50,(int)h / 2 -45,90,35 }, this);
	backToTitle = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "BACK to TITLE", { (int)w / 2 - 50,(int)h / 2,90,35 }, this);
	exit = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "EXIT", { (int)w / 2 - 50,(int)h / 2 + 45,90,35 }, this);

	fullscreenCB_ = (GuiCheckbox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 5, "FULLSCREEN", { (int)w / 2 - 50,(int)h / 2 + 90,135,35 }, this);
	vsyncCB_ = (GuiCheckbox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 6, "VSYNC", { (int)w / 2 - 50,(int)h / 2 + 135,135,35 }, this);
	goBack_ = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 7, "BACK", { (int)w / 2 - 50 + 83,(int)h / 2 + 180,90,35 }, this);
	musicSLider_ = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 8, "MUSIC", { (int)w / 2 - 50 + 83,(int)h / 2 - 16,35,35 }, this, { (int)w / 2 - 50,(int)h / 2 - 8,100,21 });
	SFXslider_ = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 9, " SFX ", { (int)w / 2 - 50 + 83,(int)h / 2 + 49,35,35 }, this, { (int)w / 2 - 50,(int)h / 2 + 57,100,21 });

	goMainMenu = false;
	exitGame = false;

	app->entityManager->Enable();
	app->pathfinding->Enable();
	app->guiManager->Enable();
	app->hud->Enable();

	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	// L03: DONE 3: Request App to Load / Save when pressing the keys F5 (save) / F6 (load)
	if (app->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		app->SaveGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		app->LoadGameRequest();

	if (app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += 3;

	if (app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= 3;

	if (app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += 3;

	if (app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= 3;

	if (player->position.x < 512/app->win->GetScale())
	{
		app->render->camera.x = app->render->camera.x;
	}
	else {
		app->render->camera.x = -player->position.x * app->win->GetScale() + 512;
	}

	if (player->position.y < 384/app->win->GetScale())
	{
		app->render->camera.y = app->render->camera.y;
	}
	else {
		app->render->camera.y = -player->position.y * app->win->GetScale() + 384;
	}

	//menu enable
	if (app->guiManager->isMenuActive)
	{
		ListItem<GuiControl*>* buttonToActivate = app->guiManager->guiControlsList.start;
		while (buttonToActivate != NULL)
		{
			for (uint i = 1; i <= 4; ++i)
			{
				if (buttonToActivate->data->id == i)
					buttonToActivate->data->showMenu = true;
			}
			buttonToActivate = buttonToActivate->next;
		}
	}

	if (!app->guiManager->isMenuActive)
	{
		ListItem<GuiControl*>* buttonToActivate = app->guiManager->guiControlsList.start;
		while (buttonToActivate != NULL)
		{
			for (uint i = 1; i <= 4; ++i)
			{
				if (buttonToActivate->data->id == i)
					buttonToActivate->data->showMenu = false;
			}
			buttonToActivate = buttonToActivate->next;
		}
	}

	//enable settings
	if (app->guiManager->isSettingsActive)
	{
		ListItem<GuiControl*>* buttonToActivate = app->guiManager->guiControlsList.start;
		while (buttonToActivate != NULL)
		{
			for (uint i = 5; i <= 9; ++i)
			{
				if (buttonToActivate->data->id == i)
					buttonToActivate->data->showMenu = true;
			}
			buttonToActivate = buttonToActivate->next;
		}
	}

	if (!app->guiManager->isSettingsActive)
	{
		ListItem<GuiControl*>* buttonToActivate = app->guiManager->guiControlsList.start;
		while (buttonToActivate != NULL)
		{
			for (uint i = 5; i <= 9; ++i)
			{
				if (buttonToActivate->data->id == i)
					buttonToActivate->data->showMenu = false;
			}
			buttonToActivate = buttonToActivate->next;
		}
	}

	// Draw map
	app->map->Draw();

	//World to map test
	int mouseX, mouseY;
	app->input->GetMousePosition(mouseX, mouseY);

	iPoint mouseTile = iPoint(0, 0);

	
	mouseTile = app->map->WorldToMap((mouseX - app->render->camera.x), (mouseY - app->render->camera.y));
	


	app->guiManager->Draw();

	SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:[%d,%d]",
		app->map->mapData.width,
		app->map->mapData.height,
		app->map->mapData.tileWidth,
		app->map->mapData.tileHeight,
		app->map->mapData.tilesets.Count(),
		mouseTile.x,
		mouseTile.y);

	app->win->SetTitle(title.GetString());

	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;	

	if (goMainMenu)
	{
		goMainMenu = false;
		app->fade->FadeToBlack(this, app->introScreen, 15);
		app->entityManager->Disable();
		app->pathfinding->Disable();
		app->guiManager->Disable();
		app->map->Disable();
		app->physics->PauseGame();
		app->SaveGameRequest();
	}

	if (exitGame)
	{
		ret = false;
		app->guiManager->isMenuActive = false;
		app->physics->PauseGame();
	}
	
	if (app->win->fullscreenOn)
		SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN);
	else if (!app->win->fullscreenOn)
		SDL_SetWindowFullscreen(app->win->window, 0);

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN && !app->guiManager->isSettingsActive)
	{
		app->guiManager->enableMenu();
		app->physics->PauseGame();
	}

	return ret;
}

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: TODO 5: Implement the OnGuiMouseClickEvent method
	LOG("Event by %d ", control->id);

	switch (control->id)
	{
	case 1:
		app->guiManager->enableMenu();
		app->physics->PauseGame();
		LOG("Button 1 clicked");
		break;
	case 2:
		app->guiManager->enableMenu();
		app->guiManager->enableSettings();
		LOG("Button 2 clicked");
		break;
	case 3:
		goMainMenu = true;
		LOG("Button 3 clicked");
		break;
	case 4:
		exitGame = true;
		LOG("Button 4 clicked");
		break;
	case 5:
		app->win->enableFullscreen();
		LOG("Button 5 clicked");
		break;
	case 6:
		LOG("Button 6 clicked");
		break;
	case 7:
		app->guiManager->enableSettings();
		app->guiManager->enableMenu();
		LOG("Button 7 clicked");
		break;
	case 8:
		LOG("Button 8 clicked");
		break;
	case 9:
		LOG("Button 9 clicked");
		break;
	}
	return true;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");

	ListItem<Entity*>* item;
	item = items.start;

	while (item != NULL)
	{
		app->entityManager->DestroyEntity(item->data);
		RELEASE(item->data);
		item = item->next;
	}
	items.Clear();

	app->entityManager->Disable();
	app->pathfinding->Disable();
	app->hud->Disable();
	app->guiManager->Disable();

	return true;
}
