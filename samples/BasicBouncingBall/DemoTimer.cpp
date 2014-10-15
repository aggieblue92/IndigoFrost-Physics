#include "DemoTimer.h"

DemoTimer::DemoTimer()
: _lastFrame(std::chrono::system_clock::now())
, _frameTime(0u)
{}

void DemoTimer::tick()
{
	auto now = std::chrono::system_clock::now();
	_frameTime = static_cast<float>(std::chrono::duration_cast<std::chrono::milliseconds>(now - _lastFrame).count()) / 1000.f;
	_lastFrame = now;
}

float DemoTimer::getFrameTime() const
{
	return _frameTime;
}