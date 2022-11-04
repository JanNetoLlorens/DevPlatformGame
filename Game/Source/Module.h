#ifndef __MODULE_H__
#define __MODULE_H__

#include "SString.h"

#include "PugiXml/src/pugixml.hpp"

class App;
class PhysBody;

class Module
{
public:

	Module(bool startEnabled) : isEnabled(startEnabled)
	{}

	void Init()
	{
		isEnabled = true;
	}

	// Called before render is available
	virtual bool Awake(pugi::xml_node&)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	// L03: DONE 2: Create new virtual methods to LoadState / SaveState
	virtual bool LoadState(pugi::xml_node&)
	{
		return true;
	}

	virtual bool SaveState(pugi::xml_node&)
	{
		return true;
	}

	virtual void OnCollision(PhysBody* bodyA, PhysBody* bodyB)
	{

	}

	// Switches isEnabled and calls Start() method
	void Enable()
	{
		if (!isEnabled)
		{
			isEnabled = true;
			Start();
		}
	}

	// Switches isEnabled and calls CleanUp() method
	void Disable()
	{
		// TODO 0: Call CleanUp() for disabling a module
		if (isEnabled)
		{
			isEnabled = false;
			CleanUp();
		}
	}

	inline bool IsEnabled() const { return isEnabled; }

public:

	SString name;
	bool isEnabled;

};

#endif // __MODULE_H__