#ifndef _PHYSICS_CLASS_KAM_H_
#define _PHYSICS_CLASS_KAM_H_

#include <assert.h>
#include <math.h>

namespace Frost {
	struct Vect3 {
		float x, y, z;
		Vect3() : x(0.0f), y(0.0f), z(0.0f) {}
		Vect3(float xPos, float yPos, float zPos) :
			x(xPos), y(yPos), z(zPos) {}
		Vect3& operator+(const Vect3 right) {
			return Vect3(x + right.x, y + right.y, z + right.z);
		}
		Vect3& operator-(const Vect3 right) {
			return Vect3(x - right.x, y - right.y, z - right.z);
		}
		Vect3& operator+=(const Vect3 right) {
			return Vect3(x += right.x, y += right.y, z += right.z);
		}
		Vect3& operator-=(const Vect3 right) {
			return Vect3(x -= right.x, y -= right.y, z -= right.z);
		}
		float operator*(const Vect3 right) const {
			return (x * right.x + y * right.y + z * right.z);
		}
		Vect3& operator*(const float right) {
			return Vect3(x * right, y * right, z * right);
		}
		Vect3 operator*=(const float right) {
			return Vect3(x *= right, y *= right, z *= right);
		}
		Vect3 CrossProduct(const Vect3 left, const Vect3 right) {
			return Vect3(left.y * right.z - left.z * right.y,
				left.z * right.x - left.x * right.z,
				left.x * right.y - left.y * right.x);
		}
		void Normalize() {
			float divBy = sqrt((x * x) + (y * y) + (z * z));
			// If divBy is zero, then x,y,z are all zero.
			if (0 == divBy)
				divBy = 1.0f;
			x /= divBy;
			y /= divBy;
			z /= divBy;
		}
		float Magnitude() {
			return sqrt((x * x) + (y * y) + (z * z));
		}
		Vect3 GetNormal() {
			Vect3 toReturn(*this);
			toReturn.Normalize();
			return toReturn;
		}
	};

	class Particle {
	public:
		Particle();
		Particle(Vect3 pos, Vect3 vel, Vect3 acc);
		Particle(Vect3 pos, Vect3 vel, Vect3 acc,
			float mass, float gravity, float damping);
		Particle(const Particle& right);
		~Particle() {}

		Vect3 GetPosition();
		Vect3 GetVelocity();
		Vect3 GetAcceleration();

		float GetMass();
		float GetInverseMass();
		float GetDrag();
		float GetGravity();

		bool IsFiniteMass();

		void SetMass(float mass);
		void SetInverseMass(float inverseMass);
		void SetPosition(Vect3 newPos);
		void SetVelocity(Vect3 newVel);
		void SetAcceleration(Vect3 newAcc);
		void SetGravity(float newG);
		void SetDamping(float newDampingConstant);

		void AugmentNetForce(Vect3 toAdd);

		void Integrate(float timeElapsed);

		// For the future, you will have force plugins here.
		// void Frame(); // Go through all force plugins, get forces
		// For now, it's just gravity.

	protected:
		Vect3 m_pos, m_vel, m_acc;
		float m_inverseMass;
		float m_g, m_damp;

		Vect3 m_netForces;
	};

}

#endif
