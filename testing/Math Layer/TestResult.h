#ifndef INDIGOFROSTMATHTESTING_TESTRESULT_H_
#define INDIGOFROSTMATHTESTING_TESTRESULT_H_

#include <string>

class TestResult
{
public:
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

#endif
