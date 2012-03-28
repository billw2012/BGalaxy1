
#include "QDebugStream.h"
#include <QObject>

QDebugStream::QDebugStream( std::ostream &stream, QTextEdit* text_edit ) : m_stream(stream)
{
	log_window = text_edit;
	m_old_buf = stream.rdbuf();
	stream.rdbuf(this);
	QObject::connect(this, SIGNAL(append_sig(QString)), this, SLOT(append_slot(QString)), Qt::QueuedConnection);
	appender = std::bind(&QTextEdit::append, log_window, std::placeholders::_1);
}

QDebugStream::~QDebugStream()
{
	// output anything that is left
	if (!m_string.empty())
		append_slot(m_string.c_str());

	m_stream.rdbuf(m_old_buf);
}

QDebugStream::int_type QDebugStream::overflow( int_type v )
{
	if (v == '\n')
	{
		emit append_sig(m_string.c_str());
		m_string.erase(m_string.begin(), m_string.end());
	}
	else
		m_string += v;

	return v;
}

std::streamsize QDebugStream::xsputn( const char *p, std::streamsize n )
{
	m_string.append(p, p + n);

	size_t pos = 0;
	while (pos != std::string::npos)
	{
		pos = m_string.find('\n');
		if (pos != std::string::npos)
		{
			std::string tmp(m_string.begin(), m_string.begin() + pos);
			emit append_sig(tmp.c_str());
			m_string.erase(m_string.begin(), m_string.begin() + pos + 1);
		}
	}

	return n;
}

void QDebugStream::append_slot( QString line )
{
	appender(line);
}
