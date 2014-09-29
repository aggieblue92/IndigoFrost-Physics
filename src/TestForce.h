#ifndef tf
#define tf

#include "IForce.h"

namespace Frost
{
	class TestForce : public IForce
	{
	public:
		virtual void ApplyForce(IPhysicsObject* affectedObject, float dt);
		virtual IForce* getNewForcePtr() const;
	};
}

#endif