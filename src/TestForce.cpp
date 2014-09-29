#include "TestForce.h"
using namespace Frost;

void TestForce::ApplyForce(IPhysicsObject* o, float dt)
{
	/*Vect3 pt_w = o->GetPos() - MathConstants::VECTOR_UNIT_Y;
	o->addForceAtPoint(MathConstants::VECTOR_UNIT_Z, pt_w);*/
}

IForce* TestForce::getNewForcePtr() const
{
	return new TestForce(*this);
}