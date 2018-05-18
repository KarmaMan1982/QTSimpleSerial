#include <QCoreApplication>
#include "serialadapter.h"
#include <QTimer>

int main(int argc, char *argv[])
{
QCoreApplication a(argc, argv);
SerialAdapter adapter;
QTimer timer;
QObject::connect(&timer, &QTimer::timeout, [&adapter]() {
    adapter.intervallRunner();
});
timer.start(850);

return a.exec();
} 

