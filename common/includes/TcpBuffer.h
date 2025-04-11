// file : TcpBuffer.h
// name : eric garcia

#ifndef TCPBUFFER_H
#define TCPBUFFER_H

#include <QTcpSocket>
#include <QByteArray>
#include <QDebug>

class TcpBuffer
{
public:
    static const quint32 sizeBytes; // # of bytes before a message that
                                    // tell the size of the message
    TcpBuffer();
    ~TcpBuffer() = default;
    TcpBuffer(const TcpBuffer&) = default;
    TcpBuffer& operator=(const TcpBuffer&) = default;

    void append(const QByteArray& data);
    bool readyRead() const;
    QByteArray read();
private:
    QByteArray data_;
    quint32 expectedSize_;
    bool flag_; // has the expected been assigned for the current msg
};

void TcpSmartWrite(QTcpSocket * socket, const QByteArray& data);

#endif 
