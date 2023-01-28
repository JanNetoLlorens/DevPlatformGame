#include "GuiCheckbox.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Textures.h"
#include "Log.h"

GuiCheckbox::GuiCheckbox(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;
}

GuiCheckbox::~GuiCheckbox()
{

}

bool GuiCheckbox::Start()
{
	texturePath = "Assets/Textures/Button.png";
	texture = app->tex->Load(texturePath);
	texturePath = "Assets/Textures/Button_Hover.png";
	texture2 = app->tex->Load(texturePath);
	texturePath = "Assets/Textures/Button_Clicked.png";
	texture3 = app->tex->Load(texturePath);

	isClicked = false;

	return true;
}

bool GuiCheckbox::Update(float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		// L15: TODO 3: Update the state of the GUiButton according to the mouse position
		app->input->GetMousePosition(mouseX, mouseY);

		GuiControlState previousState = state;

		// I'm inside the limitis of the button
		if (mouseX >= bounds.x && mouseX <= bounds.x + bounds.w &&
			mouseY >= bounds.y && mouseY <= bounds.y + bounds.h) {

			state = GuiControlState::FOCUSED;

			if (previousState != state) {
				LOG("Change state from %d to %d", previousState, state);
				//app->audio->PlayFx(audioFxId);
			}

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT) {
				state = GuiControlState::PRESSED;
			}

			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN) {
				isClicked = !isClicked;
			}


			//
			if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP) {
				NotifyObserver();
			}
		}
		else {
			state = GuiControlState::NORMAL;
		}
	}

	return false;
}

bool GuiCheckbox::Draw(Render* render)
{
	//L15: TODO 4: Draw the button according the GuiControl State

	switch (state)
	{
	case GuiControlState::DISABLED:
		break;
	case GuiControlState::NORMAL:
		section = { bounds.x,bounds.y,36,36 };
		app->render->DrawRectangle(section, 150, 150, 150, 255, false);
		if (isClicked)
		{
			section = { bounds.x + 3,bounds.y + 3,30,30 };
			app->render->DrawRectangle(section, 0, 255, 0);
		}
		break;
	case GuiControlState::FOCUSED:
		section = { bounds.x,bounds.y,36,36 };
		app->render->DrawRectangle(section, 255, 255, 255, 255, false);
		if (isClicked)
		{
			section = { bounds.x + 3,bounds.y + 3,30,30 };
			app->render->DrawRectangle(section, 0, 255, 0);
		}
		break;
	case GuiControlState::PRESSED:
		section = { bounds.x,bounds.y,36,36 };
		app->render->DrawRectangle(section, 150, 150, 150, 255, false);
		if (isClicked)
		{
			section = { bounds.x + 3,bounds.y + 3,30,30 };
			app->render->DrawRectangle(section, 0, 255, 0);
		}
		break;
	}

	app->render->DrawText(text.GetString(), bounds.x + 40, bounds.y +10, bounds.w - 20, bounds.h - 20, { 255,255,255 });

	return true;
}

bool GuiCheckbox::CleanUp()
{
	app->tex->UnLoad(texture);
	app->tex->UnLoad(texture2);
	app->tex->UnLoad(texture3);
	return true;
}