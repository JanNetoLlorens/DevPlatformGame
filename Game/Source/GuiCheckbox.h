#ifndef __GUICHECKBOX_H__
#define __GUICHECKBOX_H__

#include "GuiControl.h"

#include "Point.h"
#include "SString.h"
#include "Animation.h"

class GuiCheckbox : public GuiControl
{
public:

	GuiCheckbox(uint32 id, SDL_Rect bounds, const char* text);
	virtual ~GuiCheckbox();

	bool Start();
	bool Update(float dt);
	bool Draw(Render* render);

	bool CleanUp();

	int mouseX, mouseY;
	unsigned int click;

	bool canClick = true;
	bool drawBasic = false;

	const char* texturePath;

	bool isClicked = false;
};

#endif // __GUICHECKBOX_H__
