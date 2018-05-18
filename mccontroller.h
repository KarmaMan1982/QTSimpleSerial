#ifndef MCCONTROLLER_H
#define MCCONTROLLER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonDocument>
#include <QTextStream>
#include <QStringList>

class MCController : public QObject
{
    Q_OBJECT
private:
    QString Modus;
    bool Active;
    int colorSize = 4;
    QStringList colorField;
    int colorPosition = 0;
    QJsonDocument ledSet(QString ledColor);
public:
    explicit MCController(QObject *parent = nullptr);
    QJsonDocument Starter();
    void ControlInputData(QJsonObject inputObject);
signals:

public slots:
};

#endif // MCCONTROLLER_H
