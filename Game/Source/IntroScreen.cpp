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
#include "GuiManager.h"

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
	app->audio->PlayMusic("One Piece Game Dev Assets/Music/Overtaken.wav");

	return ret;
}

// Called before the first frame
bool IntroScreen::Start()
{
	img = app->tex->Load(app->LoadConfigFile().child("introScreen").child("img").attribute("texturepath").as_string());
	berry = app->tex->Load(app->LoadConfigFile().child("introScreen").child("berry").attribute("texturepath").as_string());
	creditsTex = app->tex->Load(app->LoadConfigFile().child("introScreen").child("credits").attribute("texturepath").as_string());

	app->win->scale = 1;
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");
	
	app->render->camera.x = 0;
	app->render->camera.y = 0;

	//Declare a GUI
	uint w, h;
	app->win->GetWindowSize(w, h);

	playButton = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "PLAY", { 550,480,90,35 }, this);
	continueGame = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 2, "CONTINUE", { 550,480+45,90,35 }, this);
	settings = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 3, "SETTINGS", { 550,480+90,90,35 }, this);
	credits = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 4, "CREDITS", { 550,480+135,90,35 }, this);
	exit = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 5, "EXIT", { 550,480+180,90,35 }, this);

	fullscreenCB = (GuiCheckbox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 6, "FULLSCREEN", { 550,480 + 90,135,35 }, this);
	vsyncCB = (GuiCheckbox*)app->guiManager->CreateGuiControl(GuiControlType::CHECKBOX, 7, "VSYNC", { 550,480 + 135,135,35 }, this);
	goBack = (GuiButton*)app->guiManager->CreateGuiControl(GuiControlType::BUTTON, 8, "BACK", { 550,480 + 180,90,35 }, this);
	musicSLider = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 9, "MUSIC", { 633,480 - 16,35,35 }, this, { 550,480-8,100,21});
	SFXslider = (GuiSlider*)app->guiManager->CreateGuiControl(GuiControlType::SLIDER, 10, " SFX ", { 633,480 + 49,35,35 }, this, { 550,480 + 57,100,21 }); 

	//settings


	app->guiManager->Enable();

	//enable gui main menu

	playNow = false;
	continuePlaying = false;
	creditsActive = false;
	exitGame = false;

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

		//printedRight = false;

	/*if(!printedRight)
		app->render->DrawTexture(berry, 460, 590);
	else if (printedRight)
		app->render->DrawTexture(berry, 810, 590);*/

	if (playNow)
	{
		playNow = false;
		app->fade->FadeToBlack(this, (Module*)app->scene, 10);
	}

	if (continuePlaying)
	{
		continuePlaying = false;
		app->guiManager->Disable();
		app->fade->FadeToBlack(this, (Module*)app->scene, 10);
	}

	if (exitGame)
	{
		exitGame = false;
		return false;
	}

	if (app->win->fullscreenOn)
		SDL_SetWindowFullscreen(app->win->window, SDL_WINDOW_FULLSCREEN);
	else if(!app->win->fullscreenOn)
		SDL_SetWindowFullscreen(app->win->window, 0);

	if (!app->guiManager->isSettingsActive)
	{
		ListItem<GuiControl*>* controlMenu = app->guiManager->guiControlsList.start;

		while (controlMenu != nullptr)
		{
			for (int i = 1; i <= 5; ++i) {
				if (controlMenu->data->id == i) {
					controlMenu->data->showMenu = true;
				}
			}
			controlMenu = controlMenu->next;
		}

		ListItem<GuiControl*>* control = app->guiManager->guiControlsList.start;

		while (control != nullptr)
		{
			for (int i = 6; i <= 10; ++i) {
				if (control->data->id == i) {
					control->data->showMenu = false;
				}
			}
			control = control->next;
		}
	}
	if (app->guiManager->isSettingsActive)
	{
		ListItem<GuiControl*>* controlMenu = app->guiManager->guiControlsList.start;

		while (controlMenu != nullptr)
		{
			for (int i = 1; i <= 5; ++i) {
				if (controlMenu->data->id == i) {
					controlMenu->data->showMenu = false;
				}
			}
			controlMenu = controlMenu->next;
		}

		ListItem<GuiControl*>* control = app->guiManager->guiControlsList.start;

		while (control != nullptr)
		{
			for (int i = 6; i <= 10; ++i) {
				if (control->data->id == i) {
					control->data->showMenu = true;
				}
			}
			control = control->next;
		}
	}

	return true;
}

// Called each loop iteration
bool IntroScreen::PostUpdate()
{
	bool ret = true;

	app->render->DrawTexture(img, 0, 45);
	app->guiManager->Draw();

	if (creditsActive)
	{
		app->render->DrawTexture(creditsTex, 500, 500);
	}

	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		creditsActive = false;

	return ret;
}

// Called before quitting
bool IntroScreen::CleanUp()
{
	app->guiManager->Disable();
	app->tex->UnLoad(img);
	app->tex->UnLoad(berry);
	app->tex->UnLoad(creditsTex);
	LOG("Freeing LogoScreen");

	return true;
}

bool IntroScreen::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: TODO 5: Implement the OnGuiMouseClickEvent method
	LOG("Event by %d ", control->id);

	switch (control->id)
	{
	case 1:
		playNow = true;
		LOG("Button 1 clicked");
		break;
	case 2:
		continuePlaying = true;
		LOG("Button 2 clicked");
		break;
	case 3:
		app->guiManager->enableSettings();
		LOG("Button 3 clicked");
		break;
	case 4:
		creditsActive = true;
		LOG("Button 4 clicked");
		break;
	case 5:
		exitGame = true;
		LOG("Button 5 clicked");
		break;
	case 6:
		app->win->enableFullscreen();
		LOG("Button 6 clicked");
		break;
	case 7:
		LOG("Button 7 clicked");
		break;
	case 8:
		app->guiManager->enableSettings();
		LOG("Button 8 clicked");
		break;
	case 9:
		LOG("Button 9 clicked");
		break;
	case 10:
		LOG("Button 10 clicked");
		break;
	}
	return true;
}