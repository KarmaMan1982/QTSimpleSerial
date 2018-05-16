#include <QCoreApplication>
#include "serialadapter.h"
#include <QChar>
#include <QTextStream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QMap>
#include <QFile>
#include <QTimer>
#include <QDateTime>
#include <string.h>
#include <QTest>
#include <QSerialPort>
#include <QDebug>

#define CTRL_SOH	0x01
#define CTRL_STX	0x02
#define CTRL_ETX	0x03
#define CTRL_EOT	0x04
/* UART jeweils RX und TX Byte Buffer Größe */
#define UART_RXTX_BUFFSIZE		512

using namespace std;

ulong XferCRC16(QByteArray pu8Data, uchar u8LengthTelegramm)
{
    const ulong u32Poly16 = 0xA001;
    ulong u32LSB;
    ulong u8Cnt1;
    uchar u8Cnt2;

    ulong u32Crc = 0xFFFF;

    for (u8Cnt1 = 0; u8Cnt1 < u8LengthTelegramm; u8Cnt1++)
    {
        u32Crc = ((u32Crc^(pu8Data.at(u8Cnt1))) | 0xFF00) & (u32Crc | 0x00FF);

        for (u8Cnt2 = 0; u8Cnt2 < 8; u8Cnt2++)
        {
            u32LSB = (u32Crc & 0x0001);
            u32Crc = u32Crc >> 1;
            if (u32LSB)
            {
                u32Crc = u32Crc ^ u32Poly16;
            }
        }
        //pu8Data++;
    }

    return u32Crc;
}

QByteArray ledSet(QString ledColor){
    int messageId = 12345;
    QString type = "function";
    QString name = "ledSet";

    QJsonObject sendParametersField;
    sendParametersField["color"]=ledColor;
    QJsonObject sendJsonField;
    sendJsonField["messageId"] = messageId;
    sendJsonField["type"]=type;
    sendJsonField["name"]=name;
    sendJsonField["parameters"] = sendParametersField;
    QJsonDocument sendJson(sendJsonField);
    QByteArray sendBytes = sendJson.toJson(QJsonDocument::Compact);

    ulong u32Lenght;
    ulong u32CRC;
    //char jsonBuffer[UART_RXTX_BUFFSIZE-6];
    ushort u32CRCHighByte;
    ushort u32CRCLowByte;
    QByteArray ba;
    ba.resize(UART_RXTX_BUFFSIZE);
    ba[0] = CTRL_SOH;
    ba[1] = CTRL_SOH;
    ba[2] = CTRL_SOH;
    ba[3] = CTRL_STX;
    for (int s = 0; s < sendBytes.size(); s++){
        ba[s+4] = sendBytes.at(s);
    }
    u32Lenght = sendBytes.size();
    u32CRC = XferCRC16(sendBytes, u32Lenght);
    u32CRCLowByte = (u32CRC & 0x00FF);
    u32CRCHighByte = (u32CRC & 0xFF00) >> 8;
    ba[1] = u32CRCLowByte;
    ba[2] = u32CRCHighByte;
    /* Steuerzeichen Ende */
    int byteLength = sendBytes.size();
    byteLength = byteLength + 4;
    ba[byteLength] = CTRL_ETX;
    byteLength++;
    ba[byteLength] = CTRL_EOT;
    byteLength++;
    ba.resize(byteLength);
    //QTextStream(stdout) << ba << endl;
    QFile file("out.log");
    QFile IOfile("inout.log");
    QDateTime now = QDateTime::currentDateTime();

    if (file.open(QIODevice::WriteOnly | QIODevice::Append)){
    QTextStream out(&file);
    out << "Output: " + now.toString("dd.MM.yyyy HH:mm:ss:zzz") + " " + ba << endl;
    file.close();
    }

    if (IOfile.open(QIODevice::WriteOnly | QIODevice::Append)){
    QTextStream out(&IOfile);
    out << "Output: " + now.toString("dd.MM.yyyy HH:mm:ss:zzz") + " " + ba << endl;
    IOfile.close();
    }

    return ba;
    //iSerialPort.openSerialPort();


}

int main(int argc, char *argv[])
{
QCoreApplication a(argc, argv);
SerialAdapter adapter;
QByteArray baOutput;
QTimer timer;
int colorSize = 4;
QString colorField[colorSize]={"red","green","blue","yellow"};
int colorPosition = 0;
QObject::connect(&timer, &QTimer::timeout, [&adapter,&colorField,&colorPosition,&colorSize]() {
    auto baOutput = ledSet(colorField[colorPosition]);
    adapter.sendData(baOutput);
    if(colorPosition < colorSize - 1){
        colorPosition++;
    } else {
        colorPosition = 0;
    }

});
timer.start(850);

return a.exec();
} 

