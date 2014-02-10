#ifndef _BUOYANCY_FORCE_KAM_H_
#define _BUOYANCY_FORCE_KAM_H__

#include "ForceGenerator.h"

namespace Frost {
	class BuoyancyForce : public ParticleForceGenerator {
	public:
		BuoyancyForce(float maxDepth, float objVolume,
			float liquidHeight, float liquidDensity = 1000.0f);
		virtual void updateForce(Particle* p, float timeElapsed);

	private:
		float m_maxDepth;
		float m_objVolume;
		float m_liquidHeight;
		float m_liquidDensity;
	};
}
#endif
