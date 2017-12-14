#ifndef SHOW1_H
#define SHOW1_H

#include <QWidget>

namespace Ui {
class Show1;
}

class Show1 : public QWidget
{
    Q_OBJECT

public:
    explicit Show1(QWidget *parent = 0);
    ~Show1();

private:
    Ui::Show1 *ui;
};

#endif // SHOW1_H
