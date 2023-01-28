#ifndef __GUISLIDER_H__
#define __GUISLIDER_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"
#include "Animation.h"

class GuiSlider : public GuiControl
{
public:

	GuiSlider(uint32 id, SDL_Rect bounds, const char* text, SDL_Rect sliderBound_);
	virtual ~GuiSlider();

	bool Start();
	bool Update(float dt);
	bool Draw(Render* render);

	bool CleanUp();

	SDL_Rect sliderBound;

	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;

	const char* texturePath;
};

#endif // __GUISLIDER_H__
