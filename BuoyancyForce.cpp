#include "BuoyancyForce.h"
using namespace Frost;

BuoyancyForce::BuoyancyForce(float maxDepth, float objVolume,
	float liquidHeight, float liquidDensity) :
	m_maxDepth(maxDepth),
	m_objVolume(objVolume),
	m_liquidHeight(liquidHeight),
	m_liquidDensity(liquidDensity)
{}

void BuoyancyForce::updateForce(Particle* p, float timeElapsed) {
	// Calculate submersion depth
	float depth = p->GetPosition().y;

	// Check to see if out of the water...
	if (depth >= m_liquidHeight + m_maxDepth)
		return;

	Vect3 force(0.0f, 0.0f, 0.0f);

	// Check to see if we're fully submersed...
	if (depth <= m_liquidHeight - m_maxDepth)
		force.y = m_liquidDensity * m_objVolume;
	else
		force.y = m_liquidDensity * p->GetGravity() *
		m_objVolume *
		abs((depth - m_maxDepth - m_liquidHeight) / (m_maxDepth));

	// Force Y: m_objVolume * ((depth - liquidHeigh - maxDepth) / maxDepth) * density * g

	p->AugmentNetForce(force);
}