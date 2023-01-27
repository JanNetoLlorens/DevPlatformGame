#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "WalkingEnemy.h"
#include "FlyingEnemy.h"
#include "Item.h"
#include "GuiButton.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene(bool startEnabled);

	// Destructor
	virtual ~Scene();

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

	//L02: DONE 3: Declare a Player attribute 
	Player* player;
	WalkingEnemy* walkingEn;
	FlyingEnemy* flyingEn;

	List<WalkingEnemy*> walkingEnList;
	List<FlyingEnemy*> flyingEnList;

	uint Susume_fx;

private:
	SDL_Texture* img;
	SDL_Texture* mouseTileTex = nullptr;
	SDL_Texture* originTex = nullptr;

	// L12: Debug pathfinding
	iPoint origin;
	bool originSelected = false;

	//Declare GUI
	GuiButton* button1;
	GuiButton* button2;

	//BUTTONS
	//menu buttons
	GuiButton* resume;
	GuiButton* settings;
	GuiButton* backToTitle;
	GuiButton* Exit;
	//settings
	//GuiSlider* musicVolumeSlider;
	GuiButton* musicVolume;
	GuiButton* fxVolume;
	//GuiSlider* fxVolumeSlider;
	GuiButton* fullscreen;
	//GuiCheckBox* fullscreenCheck;
	GuiButton* vsync;
	//GuiCheckBox* VsyncCheck;
	bool returnToIntro;
	bool exit;

};

#endif // __SCENE_H__