#include "DebugLogger.h"

namespace Frost
{
	std::shared_ptr<DebugLogger> DebugLogger::_inst = std::shared_ptr<DebugLogger>(nullptr);

	DebugLogger::DebugLogger(DEBUG_LEVEL debugFlags, std::string logFileName)
	: _bRunning(true)
	, _mutex()
	, _cv()
	, _messageQueue()
	, _log()
	, _debugLevel(debugFlags)
	{
		if(logFileName != "")
		{
			_log.open(logFileName);
		}

		if(!_log.is_open())
		{
			if(_debugLevel & DEBUG_LEVEL_ERR_TO_FILE && _debugLevel & DEBUG_LEVEL_ERR_TO_CERR)
			{
				std::lock_guard<std::mutex> lock(_mutex);
				std::stringstream ss("");
				ss << "Error opening file " << logFileName << " - file logger not created.";
				_messageQueue.push(Message(ss.str(), &std::cerr));
			}
		}

		if(_debugLevel & DEBUG_LEVEL_LOG_TO_FILE && _log.is_open())
		{
			std::lock_guard<std::mutex> lock(_mutex);
			std::stringstream ss("");
			ss << "Opened debug file " << logFileName;
			_messageQueue.push(Message(ss.str(), &_log));
		}

		_loggerThread = std::thread(DebugLogger::run);
	}

	DebugLogger::~DebugLogger()
	{
		DebugLogger::debug("Closing debugger...");

		// Shutdown debugger thread...
		_bRunning = false;
		_cv.notify_one();
		if(_loggerThread.joinable())
		{
			_loggerThread.join();
		}

		_log.close();
	}

	void DebugLogger::createLogger(DEBUG_LEVEL debugFlags, std::string fName)
	{
		_inst = std::shared_ptr<DebugLogger>(new DebugLogger(debugFlags, fName));
	}

	void DebugLogger::setFlags(DEBUG_LEVEL flags)
	{
		if(_inst)
		{
			std::lock_guard<std::mutex> lock(_inst->_mutex);
			_inst->_debugLevel |= flags;
		}
	}

	void DebugLogger::removeFlags(DEBUG_LEVEL flags)
	{
		if(_inst)
		{
			std::lock_guard<std::mutex> lock(_inst->_mutex);
			_inst->_debugLevel &= ~flags;
		}
	}

	void DebugLogger::err(std::string m)
	{
		if(!_inst)
			return;

		std::lock_guard<std::mutex> lock(_inst->_mutex);
		std::stringstream ss("");

		ss << "(E) " << m;
		if(_inst->_debugLevel & DEBUG_LEVEL_ERR_TO_CERR)
		{
			_inst->_messageQueue.push(Message(ss.str(), &std::cerr));
			_inst->_cv.notify_one();
		}
		if(_inst->_debugLevel & DEBUG_LEVEL_ERR_TO_FILE && _inst->_log.is_open())
		{
			_inst->_messageQueue.push(Message(ss.str(), &_inst->_log));
			_inst->_cv.notify_one();
		}
	}

	void DebugLogger::log(std::string m)
	{
		if(!_inst)
			return;

		std::lock_guard<std::mutex> lock(_inst->_mutex);
		std::stringstream ss("");

		ss << "(L) " << m;
		if(_inst->_debugLevel & DEBUG_LEVEL_LOG_TO_CLOG)
		{
			_inst->_messageQueue.push(Message(ss.str(), &std::clog));
			_inst->_cv.notify_one();
		}

		if(_inst->_debugLevel & DEBUG_LEVEL_LOG_TO_FILE && _inst->_log.is_open())
		{
			_inst->_messageQueue.push(Message(ss.str(), &_inst->_log));
			_inst->_cv.notify_one();
		}
	}

	void DebugLogger::debug(std::string m)
	{
		if(!_inst)
			return;

		std::lock_guard<std::mutex> lock(_inst->_mutex);
		std::stringstream ss("");

		if(_inst->_debugLevel & DEBUG_LEVEL_DEBUG_TO_COUT)
		{
			ss << "(D) " << m;
			_inst->_messageQueue.push(Message(ss.str(), &std::cout));
			_inst->_cv.notify_one();
		}
		if(_inst->_debugLevel & DEBUG_LEVEL_DEBUG_TO_FILE && _inst->_log.is_open())
		{
			_inst->_messageQueue.push(Message(ss.str(), &_inst->_log));
			_inst->_cv.notify_one();
		}
	}

	bool DebugLogger::isFlagSet(DEBUG_LEVEL flag)
	{
		if(!_inst)
			return false;
		else
			return (_inst->_debugLevel & flag) > 0x00;
	}

	void DebugLogger::run()
	{
		if(!_inst)
			return;

		while(true)
		{
			// Is there a new message?
			std::unique_lock<std::mutex> ul(_inst->_mutex);
			_inst->_cv.wait(ul, [&]{ return _inst->_messageQueue.size() > 0u || _inst->_bRunning == false; });
			if(_inst->_bRunning == false) break;
			Message nextMessage = _inst->_messageQueue.front();
			(*nextMessage.destination) << nextMessage.contents;
			_inst->_messageQueue.pop();
			ul.unlock();
		}

		return;
	}
}