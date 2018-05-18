#include "mccontroller.h"

MCController::MCController(QObject *parent) : QObject(parent)
{
    Modus = "Timer";
    //Modus = "Single";
    Active = true;
    colorField << "red" << "green" << "blue" << "yellow";
}

QJsonDocument MCController::Starter(){
    QJsonDocument outputDocument;
    if(Active == true){
        outputDocument = ledSet(colorField.at(colorPosition));
        if(colorPosition < colorSize - 1){
            colorPosition++;
        } else {
            colorPosition = 0;
        }
    }
    if(Modus == "Single"){Active = false;}
    return outputDocument;
}

void MCController::ControlInputData(QJsonObject inputObject){
    QTextStream(stdout) << "Type: " + inputObject["type"].toString() << endl;
    QTextStream(stdout) << "Name: " + inputObject["name"].toString() << endl;
}

QJsonDocument MCController::ledSet(QString ledColor){
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
    return sendJson;
}
