#include "DemoInput.h"

bool MyEventReceiver::OnEvent(const SEvent& evt)
{
	// Remember whether each key is down or up.
	if (evt.EventType == irr::EET_KEY_INPUT_EVENT)
	{
		KeyIsDown[evt.KeyInput.Key] = evt.KeyInput.PressedDown;
	}

	return false;
}

MyEventReceiver::MyEventReceiver()
{
	for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
	{
		KeyIsDown[i] = false;
	}
}