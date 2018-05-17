#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <QObject>
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

#define CTRL_SOH	0x01
#define CTRL_STX	0x02
#define CTRL_ETX	0x03
#define CTRL_EOT	0x04
/* UART jeweils RX und TX Byte Buffer Größe */
#define UART_RXTX_BUFFSIZE		512

class JSONParser : public QObject
{
    Q_OBJECT
public:
    explicit JSONParser(QObject *parent = nullptr);
    QByteArray JsonDocumentTOByteArray(QJsonDocument inputDocument);
    QJsonObject ByteArrayTOJsonObject(QByteArray inputBytes);
private:
    ulong XferCRC16(QByteArray pu8Data, uchar u8LengthTelegramm);

signals:

public slots:
};

#endif // JSONPARSER_H
