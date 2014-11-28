#include "PhysicsTestResult.h"

PhysicsTestResult::PhysicsTestResult(PhysicsWorldState is)
: _initState(is)
, _timeElapsed(0.f)
, _gameTime(0.f)
, _errMessage("")
, _passed(true)
{}

void PhysicsTestResult::failTest(PhysicsWorldState fs, std::string em)
{
	_failState = fs;
	_errMessage = em;
	_passed = false;
}

void PhysicsTestResult::tick(float gt, float at)
{
	_timeElapsed += at;
	_gameTime += gt;
	_frameTimeList.push_back(gt);
}

bool PhysicsTestResult::passed()
{
	return _passed;
}

std::string PhysicsTestResult::errMessage()
{
	return _errMessage;
}

std::vector<float> PhysicsTestResult::frameTimeList()
{
	return _frameTimeList;
}

float PhysicsTestResult::gameTime()
{
	return _gameTime;
}

float PhysicsTestResult::timeElapsed()
{
	return _timeElapsed;
}

PhysicsWorldState PhysicsTestResult::initState()
{
	return _initState;
}

PhysicsWorldState PhysicsTestResult::failState()
{
	return _failState;
}