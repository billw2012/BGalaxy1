
#include "ProcessQueue.h"
#include "ProcessRedirect.h"
#include <boost/bind.hpp>

namespace ugly {;

ProcessQueue::ProcessQueue()
{
	_processCommandsThread = boost::thread(boost::bind(&ProcessQueue::run_processes_thread, this));
}

ProcessQueue::~ProcessQueue()
{
	_quitProcessThread.set();
	_processCommandsThread.join();
}

void ProcessQueue::add_process(const ProcessCommand& cmd)
{
	boost::mutex::scoped_lock scopedLock(_processCommandsMutex);
	_processCommands.push_back(cmd);
	_commandsAvailable.set();
}

void ProcessQueue::run_processes_thread()
{
	std::vector<WaitHandle*> waitHandles;
	waitHandles.push_back(&_quitProcessThread);
	waitHandles.push_back(&_commandsAvailable);

	while(1)
	{
		DWORD waitResult = WaitHandle::wait(waitHandles);
		if(waitResult == WAIT_OBJECT_0)
			break;
		ProcessCommand nextCommand;
		{
			boost::mutex::scoped_lock scopedLock(_processCommandsMutex);
			if(_processCommands.empty())
				continue;
			nextCommand = _processCommands.front();
			_processCommands.pop_front();
			if(_processCommands.empty())
				_commandsAvailable.reset();
		}
		run_command(nextCommand);
	}
}

void ProcessQueue::process_output_callback(const ProcessCommand& cmd, const char* buff, size_t count)
{
	emit process_output(QString::fromAscii(buff, count));
}

void ProcessQueue::run_command( const ProcessCommand& cmd )
{
	emit process_output(QString("\n-------------------------------------------"
		"\n-------------------------------------------"
		"\n-------------------------------------------\nExecuting: ") + cmd.commandLine + QString("\n\n"));
	run_redirected_process(cmd.commandLine.toAscii().data(), boost::bind(&ProcessQueue::process_output_callback, this, boost::ref(cmd), _1, _2));
}

}

