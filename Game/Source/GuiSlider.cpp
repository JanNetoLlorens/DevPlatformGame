#include "GuiSlider.h"
#include "Render.h"
#include "App.h"
#include "Audio.h"
#include "Textures.h"
#include "Log.h"

#include "SDL_mixer/include/SDL_mixer.h"

GuiSlider::GuiSlider(uint32 id, SDL_Rect bounds, const char* text, SDL_Rect sliderBound_) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;
	sliderBound = sliderBound_;

	canClick = true;
	drawBasic = false;
}

GuiSlider::~GuiSlider()
{

}

bool GuiSlider::Start()
{

	return true;
}

bool GuiSlider::Update(float dt)
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

			if (state == GuiControlState::PRESSED)
			{
				bounds.x = mouseX - (bounds.w / 2);
				if (bounds.x > sliderBound.x + sliderBound.w) {
					bounds.x = sliderBound.x + sliderBound.w;
				}
				else if (bounds.x < sliderBound.x) {
					bounds.x = sliderBound.x;
				}
			}

			if (this->id == 5) {
				Mix_VolumeMusic(bounds.x - sliderBound.x);
			}
			if (this->id == 6) {
				Mix_Volume(-1, bounds.x - sliderBound.x);
			}

		}
		else {
			state = GuiControlState::NORMAL;
		}


	}

	return false;
}

bool GuiSlider::Draw(Render* render)
{
	//L15: TODO 4: Draw the button according the GuiControl State

	switch (state)
	{
	case GuiControlState::DISABLED:
		break;
	case GuiControlState::NORMAL:
		section = { sliderBound.x,sliderBound.y,100,21 };
		render->DrawRectangle(section, 200, 200, 255);
		section = { bounds.x, bounds.y, 35, 35 };
		render->DrawRectangle(section, 0, 150, 255);
		break;
	case GuiControlState::FOCUSED:
		section = { sliderBound.x,sliderBound.y,100,21 };
		render->DrawRectangle(section, 200, 200, 255);
		section = { bounds.x, bounds.y, 35, 35 };
		render->DrawRectangle(section, 0, 150, 255);
		break;
	case GuiControlState::PRESSED:
		section = { sliderBound.x,sliderBound.y,100,21 };
		render->DrawRectangle(section, 200, 200, 255);
		section = { bounds.x, bounds.y, 35, 35 };
		render->DrawRectangle(section, 0, 150, 255);
		break;
	}

	app->render->DrawText(text.GetString(), sliderBound.x, sliderBound.y - 25, sliderBound.w-50, sliderBound.h, { 255,255,255 });

	return true;
}

bool GuiSlider::CleanUp()
{
	return true;
}