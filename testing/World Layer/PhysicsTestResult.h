#ifndef INDIGOFROST_PHYSICSTESTS_TESTRESULT_H_
#define INDIGOFROST_PHYSICSTESTS_TESTRESULT_H_

#include <vector>
#include <chrono>
#include <string>
#include "PhysicsWorldState.h"

class PhysicsTestResult
{
public:
	PhysicsTestResult(PhysicsWorldState initialState);
	void failTest(PhysicsWorldState failState, std::string errMessage);
	void tick(float gameTime, float actualTime);

	bool passed();
	std::string errMessage();
	std::vector<float> frameTimeList();
	float gameTime();
	float timeElapsed();
	PhysicsWorldState initState();
	PhysicsWorldState failState();

private:
	PhysicsWorldState _initState;
	PhysicsWorldState _failState;
	float _timeElapsed;
	float _gameTime;
	std::vector<float> _frameTimeList;
	std::string _errMessage;
	bool _passed;
};

#endif