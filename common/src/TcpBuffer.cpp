// file : TcpBuffer.cpp
// name : eric garcia

#include "TcpBuffer.h"

const quint32 TcpBuffer::sizeBytes(4);

TcpBuffer::TcpBuffer()
    : data_(QByteArray()),
      expectedSize_(0),
      flag_(false)
{}

void TcpBuffer::append(const QByteArray& data)
{
    data_.append(data);

    if (!flag_ && quint32(data_.size()) >= sizeBytes)
    {
        bool ok;
        expectedSize_=QString::fromUtf8(data_.left(sizeBytes)).toUInt(&ok,16);
        if (ok)
        {
            qDebug() << "read expected size: " << expectedSize_;
            data_ = data_.mid(sizeBytes); // only keep the message in data
            flag_ = true; // we have set the expected_ size
        }
        else
        {
            qDebug() << "Tcpbuffer: could not determine from"
                     << data_.left(sizeBytes);
        }
    }
}

bool TcpBuffer::readyRead() const
{
    return (flag_ && quint32(data_.size()) >= expectedSize_);
}

QByteArray TcpBuffer::read()
{
    QByteArray ret = data_.left(expectedSize_);
    if (ret.size() > expectedSize)
    {
        qDebug() << "Warning: Buffer is returing" << ret
                 << "but discarding" << data_.mid(expectedSize);
    }
     // clear the buffer
    data_ = QByteArray();
    // reset the expected size & flag
    expectedSize_ = 0; 
    flag_ = false; 
    return ret;
}

void TcpSmartWrite(QTcpSocket * socket, const QByteArray& data)
{
    quint32 size = data.size(); // assert that the size is 4bytes
    // make a string: size - data size, 4 - min chars, 16 - base16, '0' - fill
    QString sizeStr = QString("%1").arg(size, 4, 16, QChar('0'));
    // turn into byte array, and then add the data on beind it
    QByteArray block = sizeStr.toUtf8();
    block.append(data);
    socket->write(block);
    socket->flush(); // make sure everything sends
}
