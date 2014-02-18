#include "PlayerMotionForce.h"
using namespace Frost;

PlayerMotionForce::PlayerMotionForce(Vect3 maxVelocity, float accTime) :
	m_timeToMaxSpeed(accTime),
	m_active(false)
{
	// Direction - unit of maxVelcity
	// Speed - magnitude of maxVelocity
	m_direction = maxVelocity.GetNormal();
	m_speed = maxVelocity.Magnitude();
}

// Method behind updating force: derived with some great caution
//  is: acc = (maxVel(1-dampingConstant))/(duration)
//  to handle the damping alone, and then for reaching that
//  maximum velocity acc += (maxVel - curVel) / (timeToMax)
void PlayerMotionForce::updateForce(RigidBody* rb, float duration) {

	// We'll be using the inverse of duration, so make sure
	//  that it's greater than zero. If zero, nothing happened.
	
	if (0 == duration)
		return;

	if (!m_active)
		return;

	// To be 100% honest with you, this is all very haphazardly put together
	//  after this. It seems to work, but hasn't been extensively tested yet.
	// So if it breaks, I am truly sorry.
	// It does seem to check out though, so don't be afraid to use it.

	Vect3 maxVelocity = m_direction * m_speed;

	// This is possibly broken. I don't really know.
	Vect3 accountForDampingAtMaxVelocity =
		maxVelocity * (pow(rb->getDamping(), -duration) - 1.0f);
	accountForDampingAtMaxVelocity *= (1.0f / duration);
	
	// Yeah, not sure about this either.
	Vect3 speedUpIfSlowerThanMaxVelocity =
		(maxVelocity) * (10.0f / m_timeToMaxSpeed);

	Vect3 total = accountForDampingAtMaxVelocity + speedUpIfSlowerThanMaxVelocity;
	// Best idea so far: but WHY is that 10.0f there on the second line?!?
	total = (maxVelocity) * (1.0f / rb->getDamping());
	total *= (10.0f * rb->getDamping());

	rb->addForce(total);

	m_active = false;
}

void PlayerMotionForce::SetActive() {
	m_active = true;
}

void PlayerMotionForce::SetDirection(Vect3 newDir) {
	m_direction = newDir;
	newDir.Normalize();
}

void PlayerMotionForce::SetSpeed(float newSpeed) {
	m_speed = newSpeed;
	if (m_speed < 0)
		m_speed = -m_speed;
}

void PlayerMotionForce::SetTimeToMaxVelocity(float newTime) {
	m_timeToMaxSpeed = newTime;
	if (m_timeToMaxSpeed <= 0.0f)
		m_timeToMaxSpeed = 1.0f;
}
