#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "WalkingEnemy.h"
#include "FlyingEnemy.h"
#include "Item.h"
#include "GuiButton.h"
#include "GuiCheckbox.h"
#include "GuiSlider.h"
#include "CheckPoint.h"

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
	CheckPoint* checkpoint;

	List<WalkingEnemy*> walkingEnList;
	List<FlyingEnemy*> flyingEnList;
	List<Entity*> items;

	uint Susume_fx;

private:
	SDL_Texture* img = nullptr;
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
	GuiButton* exit;
	//settings
	GuiCheckbox* fullscreenCB_;
	GuiCheckbox* vsyncCB_;
	GuiButton* goBack_;
	GuiSlider* musicSLider_;
	GuiSlider* SFXslider_;

	bool goMainMenu;
	bool exitGame;

};

#endif // __SCENE_H__