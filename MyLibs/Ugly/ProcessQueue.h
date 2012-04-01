
#if !defined(__UGLY_PROCESS_QUEUE_H__)
#define __UGLY_PROCESS_QUEUE_H__

#include <QObject>
#include <QString>
#include <QStringList>
#include <boost/thread.hpp>
#include "../Utils/WaitHandle.h"
#include <deque>

namespace ugly {;

struct ProcessQueue : public QObject
{

	Q_OBJECT 
		;
public:
	ProcessQueue();
	~ProcessQueue();

	struct ProcessCommand
	{
		ProcessCommand(const QString& commandLine_ = QString(),
			const QString& name_ = QString())
			: commandLine(commandLine_),
			name(name_) {}
		QString name;
		QString commandLine;
	};

	void add_process(const ProcessCommand& cmd);

signals:
	// connections to this should use Qt::QueuedConnection
	void process_output(QString);

private:
	void process_output_callback(const ProcessCommand& cmd, const char* buff, size_t count);
	void run_processes_thread();
	//void read_output_thread();
	void run_command(const ProcessCommand& cmd);
private:
	WaitHandle _quitProcessThread, _commandsAvailable;
	boost::mutex _processCommandsMutex;
	std::deque<ProcessCommand> _processCommands;
	boost::thread _processCommandsThread;
};

}

#endif // !defined(__UGLY_PROCESS_QUEUE_H__)