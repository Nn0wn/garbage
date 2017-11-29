#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <cstring>
#include <fstream>
#include <QFile>
#include <QDir>
#include "dialog.h"
#include <QTextStream>
#include <QMessageBox>
#include <QClipboard>

using namespace std;

QString buff;
int flag=0;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

typedef struct elem
{
    QChar liter;
    elem* next;
}elem;

elem* pop(elem* head);

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_Enter_clicked()
{
    if(buff.isEmpty())
    {
        ui->textEdit->setText("The file is empty");
        return;
    }
    QFile output("C:/MASM/output.txt");
    if(!output.open(QFile::WriteOnly|QFile::Text|QFile::Append))
    {
        QMessageBox::information(this, "Error", "Path is not correct!");
        return;
    }
    QTextStream writestream(&output);
    int i=0;
    ui->textEdit->clear();
    while(buff[i]!='\0'){
        elem* head=new elem;
        elem* tmp=new elem;
        head->liter=buff[i];
        head->next=tmp;
        tmp->next=NULL;
        i++;
        while((buff[i+1]!='\n')&&(buff[i+1]!='\0'))
        {
            elem* tmp_next=new elem;
            tmp->liter=buff[i];
            tmp->next=tmp_next;
            tmp=tmp_next;
            tmp_next->next=NULL;
            tmp->next=NULL;
            i++;
        }
        tmp->liter=buff[i];
        tmp->next=NULL;
        i+=2;
        while(head->liter!='\0')
        {
            QChar c=pop(head)->liter;
            ui->textEdit->insertPlainText(c);
            writestream<<c;
        }
        ui->textEdit->insertPlainText("\n");
        writestream<<"\n";
    }
    output.close();
}

elem* pop(elem* head)
{
    if(head==NULL)
        return head;
    elem* tmp=new elem;
    if((head->next==NULL)){
        tmp->liter=head->liter;
        head->liter='\0';
    }
    else
    {
        while(head->next->next)
            head=head->next;
        tmp=head->next;
        head->next=NULL;
    }
    return tmp;
}

void MainWindow::on_actionOpen_file_triggered()
{
        Dialog *wnd = new Dialog(this);
        wnd->show();

        connect(wnd, SIGNAL(FilePath(QString)), this, SLOT(ReadToFile(QString)));
}

QString MainWindow::ReadToFile(const QString &FilePath)
{
    QFile input(FilePath);
    if(!input.open(QFile::ReadOnly|QFile::Text))
    {
        QMessageBox::information(this, "Error", "Path is not correct!");        
        return '\0';
    }
    QTextStream stream(&input);
    int p=input.size();
    if(p==0)
    {
        input.close();
        return '\0';
    }
    buff=stream.readAll();
            ui->textEdit_2->setText(buff);
    input.close();
    flag=1;
    return buff;
}
