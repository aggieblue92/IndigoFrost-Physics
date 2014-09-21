#ifndef FROST_EXCEPTIONS_H
#define FROST_EXCEPTIONS_H

/////////////////////////////////////////
// Contains generic exception types used
//  throughout the Indigo Frost engine
/////////////////////////////////////////

namespace Frost
{
	////////////// GENERAL EXCEPTIONS ///////////
	class NotImplementedException {};

	////////////// MATH EXCEPTIONS //////////////
	class DivByZeroException {};
	class IndexOutOfBoundsException
	{
	public:
		int indexGiven;
		IndexOutOfBoundsException(int i) : indexGiven(i) {}
	};

	// Zero magnitude exception - a normalized vector object
	//  cannot have zero magnitude (this would cause a DivByZero)
	class ZeroMagnitudeException : public DivByZeroException {};

	// Singular matrix exception - thrown on attempting to
	//  invert a singular matrix.
	class SingularMatrixException {};

	////////////// PHYSICS EXCEPTIONS ///////////////
	class DuplicateActionException {};
}

#endif