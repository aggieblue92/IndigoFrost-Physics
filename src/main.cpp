#include <iostream>
#include <iomanip>
#include <random>
#include <ctime>
#include <chrono>
#include "FrostWorldMan.h"

const float LO = -25.f;
const float HI = 25.f;
#define RAND_FLOAT LO + static_cast<float>(std::rand()) / (static_cast<float>(RAND_MAX/(HI-LO)))

std::ostream& operator<<(std::ostream& o, const Frost::Vect3& v)
{
	return o << "<" << v._x << ", " << v._y << ", " << v._z << ">";
}

std::ostream& operator<<(std::ostream& o, const Frost::Quaternion& q)
{
	return o << "[" << q._w << " + " << q._x << "i + " << q._y << "j + " << q._z << "k]";
}

std::ostream& operator<<(std::ostream& o, const Frost::Matrix& m)
{
	return o
		<< m.m[0][0] << ", " << m.m[0][1] << ", " << m.m[0][2] << ", " << m.m[0][3] << "\n"
		<< m.m[1][0] << ", " << m.m[1][1] << ", " << m.m[1][2] << ", " << m.m[1][3] << "\n"
		<< m.m[2][0] << ", " << m.m[2][1] << ", " << m.m[2][2] << ", " << m.m[2][3] << "\n"
		<< m.m[3][0] << ", " << m.m[3][1] << ", " << m.m[3][2] << ", " << m.m[3][3] << "\n";
}

std::ostream& operator<<(std::ostream& o, const Frost::BoundingSphere& b)
{
	return o << "-------BOUNDING SPHERE------------" << std::endl
		<< "Position: " << b.getPos() << std::endl
		<< "Radius: " << b.getRadius() << std::endl;
}

int main()
{
	Frost::Vect3 angularVelocity = Frost::Vect3Normal(1.f, 0.f, 0.f);
	Frost::Vect3 collisionArm = Frost::Vect3Normal(1.f, 0.f, 0.f);
	Frost::Vect3 instSpeedOfRotation = Frost::CrossProduct(angularVelocity, collisionArm);

	std::cout << "Angular Velocity: " << angularVelocity << " (" << angularVelocity.magnitude() << ")" << std::endl;
	std::cout << "Collision Arm: " << collisionArm << " (" << collisionArm.magnitude() << ")" << std::endl;
	std::cout << "Instantaneous Speed: " << instSpeedOfRotation << " (" << instSpeedOfRotation.magnitude() << ")" << std::endl;

	std::cin.ignore();
	return 0;
}