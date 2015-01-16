#ifndef FROST_IRR_DEMO_TIMER_H
#define FROST_IRR_DEMO_TIMER_H

#include <chrono>

class DemoTimer
{
public:
	DemoTimer();

	void tick();
	float getFrameTime() const;

private:
	std::chrono::system_clock::time_point _lastFrame;
	float _frameTime;
};

#endif
