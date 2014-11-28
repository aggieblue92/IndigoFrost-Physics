#ifndef INDIGOFROST_PHYSICSTESTING_TESTPARAMS_H_
#define INDIGOFROST_PHYSICSTESTING_TESTPARAMS_H_

struct PhysicsTestingParams
{
	enum StressLevel
	{
		STRESS_LEVEL_LOW,
		STRESS_LEVEL_MEDIUM,
		STRESS_LEVEL_HIGH,
		STRESS_LEVEL_EXTREME
	};

	enum PrecisionLevel
	{
		PRECISION_LEVEL_LOW,
		PRECISION_LEVEL_MEDIUM,
		PRECISION_LEVEL_HIGH,
		PRECISION_LEVEL_EXTREME
	};

	enum ZMEHandling
	{
		ZME_IGNORE,
		ZME_DIE,
		ZME_WARNING
	};

	int frameRate;
	float frameRateMultiplier;
	StressLevel stressLevel;
	PrecisionLevel precisionLevel;
	ZMEHandling zmeAction;

	PhysicsTestingParams(int fps, float fps_multiplier, StressLevel stress, PrecisionLevel precision, ZMEHandling zme_action)
		: frameRate(fps)
		, frameRateMultiplier(fps_multiplier)
		, stressLevel(stress)
		, precisionLevel(precision)
		, zmeAction(zme_action)
	{}
};

#endif