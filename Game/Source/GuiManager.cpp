#include "GuiManager.h"
#include "App.h"
#include "Textures.h"

#include "GuiButton.h"
#include "GuiCheckbox.h"
#include "GuiSlider.h"
#include "Audio.h"

GuiManager::GuiManager(bool startEnabled) : Module(startEnabled)
{
	name.Create("guiManager");
}

GuiManager::~GuiManager() {}

bool GuiManager::Start()
{
	isMenuActive = false;
	isSettingsActive = false;

	ListItem<GuiControl*>* control = guiControlsList.start;

	while (control != nullptr)
	{
		control->data->Start();
		control = control->next;
	}

	return true;

}

GuiControl* GuiManager::CreateGuiControl(GuiControlType type, int id, const char* text, SDL_Rect bounds, Module* observer, SDL_Rect sliderBounds)
{
	// L15: TODO1: Create a GUI control and add it to the list of controls

	GuiControl* guiControl = nullptr;

	//Call the constructor according to the GuiControlType

	switch (type)
	{
	case GuiControlType::BUTTON:
		guiControl = new GuiButton(id, bounds, text);
		break;
	case GuiControlType::TOGGLE:
		break;
	case GuiControlType::CHECKBOX:
		guiControl = new GuiCheckbox(id, bounds, text);
		break;
	case GuiControlType::SLIDER:
		guiControl = new GuiSlider(id, bounds, text, sliderBounds);
		break;
	case GuiControlType::SLIDERBAR:
		break;
	case GuiControlType::COMBOBOX:
		break;
	case GuiControlType::DROPDOWNBOX:
		break;
	case GuiControlType::INPUTBOX:
		break;
	case GuiControlType::VALUEBOX:
		break;
	case GuiControlType::SPINNER:
		break;
	default:
		break;		
	}
	//Set the observer
	guiControl->SetObserver(observer);

	// Created GuiControls are add it to the list of controls
	guiControlsList.Add(guiControl);

	return guiControl;
}

bool GuiManager::Update(float dt)
{	
	accumulatedTime += dt;
	if (accumulatedTime >= updateMsCycle) doLogic = true;

	// We control how often the GUI is updated to optimize the performance
	if (doLogic == true)
	{
		ListItem<GuiControl*>* control = guiControlsList.start;

		while (control != nullptr)
		{
			if (control->data->showMenu)
				control->data->Update(dt);
			control = control->next;
		}

		accumulatedTime = 0.0f;
		doLogic = false;
	}
	
	return true;
}

bool GuiManager::Draw() {

	ListItem<GuiControl*>* control = guiControlsList.start;

	while (control != nullptr)
	{
		if(control->data->showMenu)
			control->data->Draw(app->render);
		control = control->next;
	}

	return true;

}

void GuiManager::ClearGuiControl(GuiControl* guiControl)
{
	ListItem<GuiControl*>* item;

	for (item = guiControlsList.start; item != NULL; item = item->next)
	{
		if (item->data == guiControl) guiControlsList.Del(item);
	}
}

bool GuiManager::CleanUp()
{
	bool ret = true;
	ListItem<GuiControl*>* item;
	item = guiControlsList.end;

	while (item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		ClearGuiControl(item->data);
		item = item->prev;
	}
	guiControlsList.Clear();

	return true;
}

void GuiManager::enableMenu()
{
	isMenuActive = !isMenuActive;
}

void GuiManager::enableSettings()
{
	isSettingsActive = !isSettingsActive;
}



