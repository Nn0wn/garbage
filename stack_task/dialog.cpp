#include "dialog.h"
#include "ui_dialog.h"
#include <QPushButton>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    model=new QFileSystemModel(this);
    model->setFilter(QDir::AllEntries|QDir::NoDotAndDotDot|QDir::AllDirs);
    model->setRootPath("");
    ui->treeView->setModel(model);
    connect(ui->ok, SIGNAL(clicked()),SLOT(on_ok_clicked()));
    connect(ui->cancel, SIGNAL(clicked()), SLOT(close()));
    connect(ui->treeView, SIGNAL(clicked(QModelIndex)),
            SLOT(on_treeView_clicked(QModelIndex)));
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_ok_clicked()
{
    emit FilePath(ui->lineEdit->text());
    close();
}

void Dialog::on_cancel_clicked()
{
    close();
}

void Dialog::on_treeView_clicked(const QModelIndex &index)
{
    QFileInfo fileInfo=model->fileInfo(index);

    ui->lineEdit->setText(fileInfo.absoluteFilePath());

}
