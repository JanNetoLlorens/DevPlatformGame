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
		section = { 124,10,90,35 };
		break;
	case GuiControlState::FOCUSED:
		section = { 12,10,90,35 };
		break;
	case GuiControlState::PRESSED:
		section = { 12,74,90,35 };
		break;
	}

	render->DrawTexture(texture, bounds.x, bounds.y, &section, 0);
	app->render->DrawText(text.GetString(), bounds.x, bounds.y, bounds.w, bounds.h, { 255,255,255 });

	return false;
}

bool GuiButton::CleanUp()
{
	app->tex->UnLoad(texture);
	return true;
}