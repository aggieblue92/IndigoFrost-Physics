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
#ifndef FROST_SPRING_FORCE_H
#define FROST_SPRING_FORCE_H

/////////////////////////////////////////
// SpringForce: Connects two objects
//  together via a spring
/////////////////////////////////////////

#include "IPhysicsObject.h"
#include "IForce.h"

namespace Frost
{
	class SpringForce : public IForce
	{
	public:
		SpringForce(const Vect3& localConnectionPoint,
			std::shared_ptr<IPhysicsObject> otherObject,
			const Vect3& otherConnectionPoint,
			float springConstant,
			float restLength);
		SpringForce(const SpringForce& o);

		virtual void applyForce(std::shared_ptr<IPhysicsObject> affectedPhysicsObject, float duration);

	private:
		std::shared_ptr<IPhysicsObject> _otherObjectInvolved;
		float _springConstant;
		float _restLength;
		Vect3 _localConnectionPoint;
		Vect3 _otherLocalConnectionPoint;
	};
}

#endif