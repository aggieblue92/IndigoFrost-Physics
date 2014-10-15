/*
This source file is part of the Indigo Frost physics engine

The MIT License (MIT)

Copyright (c) 2014 Kamaron Peterson (aggieblue92)

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#ifndef FROST_FORCES_ANCHORED_SPRING_FORCE
#define FROST_FORCES_ANCHORED_SPRING_FORCE

#include "IForce.h"

namespace Frost
{
	class AnchoredSpringForce : public IForce
	{
	public:
		AnchoredSpringForce(const Vect3& worldAnchor, const Vect3& localConnectionPoint,
			float springConstant, float restLength);
		AnchoredSpringForce(const AnchoredSpringForce& o);
		virtual void applyForce(IPhysicsObject* affected, float timeDuration);
		virtual IForce* getNewForcePtr() const;

	private:
		Vect3 _anchor_world;
		Vect3 _connection_local;
		float _springConstant;
		float _restLength;
	};
}

#endif