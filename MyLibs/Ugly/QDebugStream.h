//################
//# qdebugstream.h  #
//################

#ifndef Q_DEBUG_STREAM_H
#define Q_DEBUG_STREAM_H

#include <iostream>
#include <streambuf>
#include <string>

#include <QTextEdit>

#include <functional>

class QDebugStream : public QObject, std::basic_streambuf<char>
{
	Q_OBJECT
		;
public:
	QDebugStream(std::ostream &stream, QTextEdit* text_edit);
	~QDebugStream();

protected:
	virtual int_type overflow(int_type v);
	virtual std::streamsize xsputn(const char *p, std::streamsize n);

signals:
	void append_sig(QString);
private slots:
	void append_slot(QString);

private:
	std::function<void(QString)> appender;
	std::ostream &m_stream;
	std::streambuf *m_old_buf;
	std::string m_string;
	QTextEdit* log_window;
};

#endif