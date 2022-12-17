#pragma once

#include "Module.h"
#include <chrono>
using namespace std::chrono;

#define DEBUG_BOX 225

class ModuleDebug : public Module
{
public:

	ModuleDebug(bool startEnabled);
	virtual ~ModuleDebug();

	bool Start();
	bool Update(float dt);	// Switch debug on/off
	bool PostUpdate();		// Draw if debug true

	void DrawDebug();
	void DrawPhysics();

public:
	//flags
	bool drawDebug = false;
	bool drawPhysics = false;
	bool godMode = false;
	bool variables = false;
	bool lightsON = false;
	bool musicON = false;
	bool sfxON = true;
	bool changeUI = false;

	//Time calculation
	microseconds timePerCycle;
	microseconds elapsedFrame;
	int desiredFPSmic;
	int desiredFPS = 60;

};