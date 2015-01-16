#ifndef FROST_IRR_DEMO_INPUT_H
#define FROST_IRR_DEMO_INPUT_H

#include <irrlicht.h>

class DemoInput : public irr::IEventReceiver
{
public:
	DemoInput();
	virtual bool OnEvent(const irr::SEvent& evt);
	virtual bool IsKeyDown(irr::EKEY_CODE keyCode) const { return _keyIsDown[keyCode]; }

private:
	bool _keyIsDown[irr::KEY_KEY_CODES_COUNT];
};

#endif
