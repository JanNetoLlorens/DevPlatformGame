#ifndef __ENDINGSCREEN_H__
#define __ENDINGSCREEN_H__

#include "Module.h"
#include "Player.h"
#include "Item.h"

struct SDL_Texture;

class EndingScreen : public Module
{
public:

	EndingScreen(bool startEnabled);

	// Destructor
	virtual ~EndingScreen();

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

public:


private:
	SDL_Texture* img;

};

#endif // __ENDINGSCREEN_H__
