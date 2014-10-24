#ifndef DEMO_INPUT_IRR_FROST_H
#define DEMO_INPUT_IRR_FROST_H

#include <irrlicht.h>
using namespace irr;

class MyEventReceiver : public IEventReceiver
{
public:
	virtual bool OnEvent(const SEvent& event);
	virtual bool IsKeyDown(EKEY_CODE keyCode) const { return KeyIsDown[keyCode]; }
	MyEventReceiver();

private:
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
};

#endif
