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

/////////////////////////////////////////
// Movable: Represents an object that can
//  be moved (has a position and orientation)
/////////////////////////////////////////

#ifndef FROST_MOVABLE_H
#define FROST_MOVABLE_H

#include "FrostMath.h"

namespace Frost
{
	class Movable
	{
	public:
		/////////////////// CTORS //////////////////////

		// Default ctor - origin with no orientation
		Movable();

		// Copy constructor
		Movable(const Movable& toCopy);

		// Initialize with a position, no orientation
		Movable(const FLOAT3& pos);

		// Initialize with a position and orientation
		Movable(const FLOAT3& pos, const Quaternion& orientation);

		// TODO: Add a Movable(Matrix) ctor?

		//////////////// GETTERS/SETTERS /////////////////
		virtual Vect3 getPos() const;
		virtual Quaternion getOrientation() const;
		virtual void setPos(const Vect3& position);
		virtual void setOrientation(const Quaternion& orientation);
		virtual void setTransformMatrix(const Matrix& transformMatrix);
		virtual Matrix getTransformMatrix();
		virtual Matrix getTransformMatrix() const;

		//////////////// TRANSFORM OPS ////////////////////
		virtual void move(const FLOAT3& moveAmount_world);
		virtual void rotate(const Quaternion& rotateAmount_world);

	private:
		Vect3 _pos;
		Quaternion _orientation;

		bool _isDirty;
		Matrix _transformMatrix;
	};
}

#endif