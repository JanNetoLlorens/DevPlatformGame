#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "List.h"

#include "PugiXml/src/pugixml.hpp"

#define CONFIG_FILENAME		"config.xml"
#define SAVE_STATE_FILENAME "save_game.xml"

// Modules
class Window;
class Input;
class Render;
class Textures;
class Audio;
class LogoScreen;
class IntroScreen;
class Scene;
class Hud;
class LoseScreen;
class WinScreen;
class EntityManager;
class Map;
class ModuleFadeToBlack;
//L07 DONE 2: Add Physics module
class Physics;
class PathFinding;
class Debug;
class GuiManager;



class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	// L03: DONE 1: Create methods to control that the real Load and Save happens at the end of the frame
	void LoadGameRequest();
	void SaveGameRequest() ;
	bool LoadFromFile();
	bool SaveToFile() ;
	pugi::xml_node LoadConfigFile();

private:

	// Load config file
	bool LoadConfig();

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

public:

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	Audio* audio;
	LogoScreen* logoScreen;
	IntroScreen* introScreen;
	Scene* scene;
	Hud* hud;
	LoseScreen* lose;
	WinScreen* winScreen;
	EntityManager* entityManager;
	Map* map;
	ModuleFadeToBlack* fade;
	//L07 DONE 2: Add Physics module
	Physics* physics;
	PathFinding* pathfinding;
	Debug* debug;
	GuiManager* guiManager;

private:

	int argc;
	char** args;
	SString title;
	SString organization;

	List<Module*> modules;

	// L01: DONE 2: Create new variables from pugui namespace:
	// xml_document to store the config file and
	// xml_node(s) to read specific branches of the xml
	pugi::xml_document configFile;
	pugi::xml_node configNode;

	uint frames;
	float dt;

	// L03: DONE 1: Create control variables to control that the real Load and Save happens at the end of the frame
    bool saveGameRequested;
	bool loadGameRequested;
};

extern App* app;

#endif	// __APP_H__