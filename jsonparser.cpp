#include "jsonparser.h"

JSONParser::JSONParser(QObject *parent) : QObject(parent)
{

}

ulong JSONParser::XferCRC16(QByteArray pu8Data, uchar u8LengthTelegramm){
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

QByteArray JSONParser::JsonDocumentTOByteArray(QJsonDocument inputDocument){
    QByteArray sendBytes = inputDocument.toJson(QJsonDocument::Compact);
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
    return ba;
}

QJsonObject JSONParser::ByteArrayTOJsonObject(QByteArray inputBytes){
    bool startHeader = false;
    bool startText = false;
    QByteArray checkBytes;
    QByteArray jsonBytes;
    QByteArray sumBytes;
    ulong u32Lenght;
    ulong u32CRC;
    ushort u32CRCHighByte;
    ushort u32CRCLowByte;
    for(int i=0;i<inputBytes.length();i++){
        switch(inputBytes[i]){
            case CTRL_SOH:
                startHeader = true;
            break;
            case CTRL_STX:
                startText = true;
                startHeader = false;
            break;
            case CTRL_ETX:
            case CTRL_EOT:
                startText = false;
                startHeader = false;
            break;
            default:
            if(startHeader == true){
                checkBytes.append(inputBytes[i]);
            }
            if(startText == true){
                jsonBytes.append(inputBytes[i]);
            }
            break;
        }
    }
    u32Lenght = jsonBytes.size();
    u32CRC = XferCRC16(jsonBytes, u32Lenght);
    u32CRCLowByte = (u32CRC & 0x00FF);
    u32CRCHighByte = (u32CRC & 0xFF00) >> 8;
    sumBytes.append(u32CRCLowByte);
    sumBytes.append(u32CRCHighByte);
    QJsonDocument returnDocument;
    QJsonObject returnObject;
    if(checkBytes == sumBytes){
        returnDocument = QJsonDocument::fromJson(jsonBytes);
        returnObject = returnDocument.object();
    } else {
        returnObject["Error"]="Checksum missmatch!";
    }
    return returnObject;
}
