#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "helpbrowser.h"
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_action_7_triggered();

    void on_action_5_triggered();

    void on_action_6_triggered();

    void on_action_triggered();

    void on_action_2_triggered();

//    void on_action_11_triggered();

    void on_checkBox_clicked();

    void on_pushButton_clicked();

//    void on_pushButton_3_clicked();

//    void on_pushButton_2_clicked();

    void on_actionAbout_triggered();

private:
    Ui::MainWindow *ui;

private:
    void createUI(const QStringList &headers);
};

#endif // MAINWINDOW_H
