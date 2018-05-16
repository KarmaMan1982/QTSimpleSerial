#ifndef SERIALADAPTER_H
#define SERIALADAPTER_H
#include <QtSerialPort/QSerialPort>

class SerialAdapter : public QSerialPort
{
    Q_OBJECT
public:
    SerialAdapter();
    ~SerialAdapter();
    virtual void init();
    void sendData(QByteArray);
private:
    bool setWriteable;
public slots:
    void readData();
};

#endif // SERIALADAPTER_H
