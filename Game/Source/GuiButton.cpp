#include "GuiButton.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Textures.h"
#include "Log.h"

GuiButton::GuiButton(uint32 id, SDL_Rect bounds, const char* text) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;

	canClick = true;
	drawBasic = false;
}

GuiButton::~GuiButton()
{

}

bool GuiButton::Start()
{
	texturePath = "Assets/Textures/Button.png";
	texture = app->tex->Load(texturePath);
	texturePath = "Assets/Textures/Button_Hover.png";
	texture2 = app->tex->Load(texturePath);
	texturePath = "Assets/Textures/Button_Clicked.png";
	texture3 = app->tex->Load(texturePath);

	return true;
}

bool GuiButton::Update(float dt)
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

bool GuiButton::Draw(Render* render)
{
	//L15: TODO 4: Draw the button according the GuiControl State

	switch (state)
	{
	case GuiControlState::DISABLED:
		break;
	case GuiControlState::NORMAL:
		section = { 0,0,90,35 };
		render->DrawTexture(texture, bounds.x, bounds.y, &section, 0);
		break;
	case GuiControlState::FOCUSED:
		section = { 0,0,92,37 };
		render->DrawTexture(texture2, bounds.x-1, bounds.y-1, &section, 0);
		break;
	case GuiControlState::PRESSED:
		section = { 0,0,92,37 };
		render->DrawTexture(texture3, bounds.x-1, bounds.y-1, &section, 0);
		break;
	}

	app->render->DrawText(text.GetString(), bounds.x+10, bounds.y+10, bounds.w-20, bounds.h-20, { 255,255,255 });

	return true;
}

bool GuiButton::CleanUp()
{
	app->tex->UnLoad(texture);
	app->tex->UnLoad(texture2);
	app->tex->UnLoad(texture3);
	return true;
}