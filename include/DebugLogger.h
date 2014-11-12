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

#ifndef FROST_DEBUG_LOGGER_H_
#define FROST_DEBUG_LOGGER_H_

#include <iostream>
#include <string>
#include <fstream>
#include <memory>
#include <thread>
#include <mutex>
#include <chrono>
#include <queue>
#include <sstream>
#include <condition_variable>

// TODO: Make outputs to the file atomic - use a mutex
//  to lock down the logger.

/////////////////////////////////////////
// DebugLogger: Class used for debugging
//  and logging. Singleton instance.
//  May or may not initialize.
/////////////////////////////////////////

namespace Frost
{
	class DebugLogger
	{
	public:
		enum DEBUG_LEVEL
		{
			DEBUG_LEVEL_NO_DEBUG = 0,
			// Log: Simple steps (created object, collisions generated, etc.)
			DEBUG_LEVEL_LOG_TO_FILE = 1 << 1,
			DEBUG_LEVEL_LOG_TO_CLOG = 1 << 2,
			// Err: Exceptions thrown, fatal errors detected
			DEBUG_LEVEL_ERR_TO_FILE = 1 << 3,
			DEBUG_LEVEL_ERR_TO_CERR = 1 << 4,
			// Debug: Verbose detail on things.
			DEBUG_LEVEL_DEBUG_TO_FILE = 1 << 5,
			DEBUG_LEVEL_DEBUG_TO_COUT = 1 << 6,
			DEBUG_LEVEL_ALL = ((1 << 7) - 1)
		};

		struct Message
		{
			std::string contents;
			std::ostream* destination;
			Message(std::string c, std::ostream* dest) : contents(c), destination(dest) {}
		};

		// TODO: Might have to overload operators for DEBUG_LEVEL

		static void createLogger(DEBUG_LEVEL debugFlags = DEBUG_LEVEL_NO_DEBUG, std::string logFileName = "");

		~DebugLogger();

		static void setFlags(DEBUG_LEVEL flags);
		static void removeFlags(DEBUG_LEVEL flags);

		static void err(std::string errorMessage);
		static void log(std::string logMessage);
		static void debug(std::string debugMessage);

		static bool isFlagSet(DEBUG_LEVEL flag);

	private:
		DebugLogger(DEBUG_LEVEL debugFlags, std::string logFileName);

		static void run();

		static std::shared_ptr<DebugLogger> _inst;

		bool _bRunning;
		std::thread _loggerThread;
		std::mutex _mutex;
		std::condition_variable _cv;
		std::queue<Message> _messageQueue;

		std::ofstream _log;
		DEBUG_LEVEL _debugLevel;
	};

	inline DebugLogger::DEBUG_LEVEL& operator|=(DebugLogger::DEBUG_LEVEL& a, DebugLogger::DEBUG_LEVEL b)
	{
		a |= b;
		return a = static_cast<DebugLogger::DEBUG_LEVEL>(a);
	}

	inline DebugLogger::DEBUG_LEVEL& operator&=(DebugLogger::DEBUG_LEVEL& a, DebugLogger::DEBUG_LEVEL b)
	{
		a &= b;
		return a = static_cast<DebugLogger::DEBUG_LEVEL>(a);
	}

	inline DebugLogger::DEBUG_LEVEL operator|(DebugLogger::DEBUG_LEVEL a, DebugLogger::DEBUG_LEVEL b)
	{
		return static_cast<DebugLogger::DEBUG_LEVEL>(static_cast<int>(a) | static_cast<int>(b));
	}

	inline DebugLogger::DEBUG_LEVEL operator&(DebugLogger::DEBUG_LEVEL a, DebugLogger::DEBUG_LEVEL b)
	{
		return static_cast<DebugLogger::DEBUG_LEVEL>(static_cast<int>(a) & static_cast<int>(b));
	}

	inline DebugLogger::DEBUG_LEVEL operator~(DebugLogger::DEBUG_LEVEL& a)
	{
		return a = static_cast<DebugLogger::DEBUG_LEVEL>(~static_cast<int>(a));
	}
}

#endif