#include "mainwindow.h"
#include "graphwidget.h"
#include <QApplication>
#include <QTime>
#include <QMainWindow>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    MainWindow w;
    w.show();

    return a.exec();
}
