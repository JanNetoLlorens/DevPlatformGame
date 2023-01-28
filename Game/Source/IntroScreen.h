#ifndef __INTROSCREEN_H__
#define __INTROSCREEN_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"
#include "GUICheckBox.h"

struct SDL_Texture;

class IntroScreen : public Module
{
public:

	IntroScreen(bool startEnabled);

	// Destructor
	virtual ~IntroScreen();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Define multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

public:
	GuiButton* playButton;
	GuiButton* continueGame;
	GuiButton* settings;
	GuiButton* credits;
	GuiButton* exit;

	GuiCheckbox* fullscreenCB;
	GuiCheckbox* vsyncCB;
	GuiButton* goBack;

private:
	SDL_Texture* img;
	SDL_Texture* berry;

	bool playNow = false;
	bool continuePlaying = false;
	bool exitGame = false;
	bool creditsActive = false;

};

#endif // __INTROSCREEN_H__
