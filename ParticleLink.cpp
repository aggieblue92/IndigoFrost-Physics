#include "ParticleLink.h"
using namespace Frost;

float ParticleLink::currentLength() const {
	Vect3 relativePosition =
		m_particle[0]->GetPosition() -
		m_particle[1]->GetPosition();
	return relativePosition.Magnitude();
}