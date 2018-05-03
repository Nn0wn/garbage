#include "show1.h"
#include "ui_show1.h"

Show1::Show1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Show1)
{
    ui->setupUi(this);

}

Show1::~Show1()
{
    delete ui;
}
