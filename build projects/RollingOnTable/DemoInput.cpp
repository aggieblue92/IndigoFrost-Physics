#include "DemoInput.h"

bool DemoInput::OnEvent(const irr::SEvent& evt)
{
	// Remember whether each key is down or up...
	if (irr::EET_KEY_INPUT_EVENT == evt.EventType)
	{
		_keyIsDown[evt.KeyInput.Key] = evt.KeyInput.PressedDown;
	}

	return false;
}

DemoInput::DemoInput()
{
	for (unsigned int i = 0; i < irr::KEY_KEY_CODES_COUNT; ++i)
	{
		_keyIsDown[i] = false;
	}
}