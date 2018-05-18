#ifndef SERIALADAPTER_H
#define SERIALADAPTER_H
#include <QtSerialPort/QSerialPort>
#include "jsonparser.h"
#include "mccontroller.h"

#define CTRL_SOH	0x01
#define CTRL_STX	0x02
#define CTRL_ETX	0x03
#define CTRL_EOT	0x04
/* UART jeweils RX und TX Byte Buffer Größe */
#define UART_RXTX_BUFFSIZE		512

class SerialAdapter : public QSerialPort
{
    Q_OBJECT
public:
    SerialAdapter();
    ~SerialAdapter();
    virtual void init();
    void sendData(QByteArray);
    void intervallRunner();
private:
    bool setWriteable;
    QByteArray inputPackage;
    JSONParser jsonParser;
    MCController mcController;
public slots:
    void readData();
};

#endif // SERIALADAPTER_H
