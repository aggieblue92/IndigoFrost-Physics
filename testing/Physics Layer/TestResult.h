#ifndef INDIGOFROSTMATHTESTING_TESTRESULT_H_
#define INDIGOFROSTMATHTESTING_TESTRESULT_H_

#include <string>
#include <sstream>

class TestResult
{
public:
	template<class ExpectedType, class ObservedType>
	TestResult(bool wasSuccess, std::string message, ExpectedType expected, ObservedType observed)
		: _wasSuccess(wasSuccess)
	{
		std::stringstream ss("");
		if (message != "")
			ss << message;
		ss << "\n\tExpected: " << expected << "\n\tObserved: " << observed << std::endl;
		_message = ss.str();
	}

	TestResult(bool wasSuccess = true, std::string message = "")
		: _wasSuccess(wasSuccess)
		, _message(message)
	{}

	std::string toString()
	{
		return _message;
	}

	bool wasSuccess()
	{
		return _wasSuccess;
	}

private:
	bool _wasSuccess;
	std::string _message;
};

const TestResult TEST_PASSED(true, "");

#endif
