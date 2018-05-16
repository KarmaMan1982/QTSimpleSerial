#include "serialadapter.h"
#include <QTextStream>
#include <QDebug>
#include <QFile>
#include <QDateTime>

void SerialAdapter::init(){
   QTextStream(stdout) << "Initialize SerialAdapter!" << endl;
   setPortName("/dev/ttyUSB0");
   //setPortName("/dev/Winsock");
   setBaudRate(QSerialPort::Baud9600);
   setStopBits(QSerialPort::OneStop);
   setDataBits(QSerialPort::Data8);
   setParity(QSerialPort::NoParity);
   setFlowControl(QSerialPort::SoftwareControl );//xonxoff*/#
   //connect(this, &SerialAdapter::readyRead, this, &SerialAdapter::readData);
   //connect(this, SIGNAL( readyRead() ), this, SLOT( readData()) );
}
SerialAdapter::SerialAdapter()
{
    init();
    bool success = false;
    if (open(QIODevice::ReadWrite)){
        setWriteable = true;

        //connect(this, &QSerialPort::readyRead, this, &SerialAdapter::readData);
        connect(this, &SerialAdapter::readyRead, this, &SerialAdapter::readData);
        //connect(this, SIGNAL( readyRead() ), this, SLOT( readData()) );
        //setDataTerminalReady(false);
        success = true;
    } else {
        qDebug() << errorString();
        setWriteable = false;
    }

    qDebug() << "Connected to usb device: " << (success ? "OK" : "FAIL");
}
SerialAdapter::~SerialAdapter(){
    QTextStream(stdout) << "Close SerialAdapter!" << endl;
    close();
}
void SerialAdapter::sendData(QByteArray inputData){
    if (setWriteable == true){
        write(inputData);
        if(bytesAvailable()){
            readData();
        }
    }
    flush();
    //close();
}

void SerialAdapter::readData()
{
   //QTextStream(stdout) << "readData" << endl;
   //qDebug() << readAll();
   QByteArray data = readAll();
   QFile file("in.log");
   QFile IOfile("inout.log");
   QDateTime now = QDateTime::currentDateTime();

   if (file.open(QIODevice::WriteOnly | QIODevice::Append)){
   QTextStream out(&file);
   out << "Input: " + now.toString("dd.MM.yyyy HH:mm:ss:zzz") + " " + data << endl;
   file.close();
   }

   if (IOfile.open(QIODevice::WriteOnly | QIODevice::Append)){
   QTextStream out(&IOfile);
   out << "Input: " + now.toString("dd.MM.yyyy HH:mm:ss:zzz") + " " + data << endl;
   IOfile.close();
   }
   QTextStream(stdout) << "Input: " + data << endl;

}
