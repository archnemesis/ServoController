
#ifndef _QEXTSERIALPORT_H_
#define _QEXTSERIALPORT_H_

#ifdef _TTY_POSIX_
#include "posix_qextserialport.h"
#define QExtBaseType Posix_QExtSerialPort
#endif

#ifdef _TTY_WIN_
#include "win_qextserialport.h"
#define QExtBaseType Win_QExtSerialPort
#endif

class QExtSerialPort: public QExtBaseType
{
	Q_OBJECT
	
	public:
        typedef QExtSerialBase::QueryMode QueryMode;

        QExtSerialPort();
        QExtSerialPort(const QString & name, QueryMode mode = QExtSerialPort::Polling);
        QExtSerialPort(PortSettings const& s, QueryMode mode = QExtSerialPort::Polling);
        QExtSerialPort(const QString & name, PortSettings const& s, QueryMode mode = QExtSerialPort::Polling);
        QExtSerialPort(const QExtSerialPort& s);
        QExtSerialPort& operator=(const QExtSerialPort&);
        virtual ~QExtSerialPort();

};

#endif
