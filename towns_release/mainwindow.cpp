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

int* graph(int k, int* roads1, int* roads2, int* result, int j, int y, int towns);

int roads(int n);

QString buff;

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


void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_Enter_clicked()
{
    int starting_point=0, arrival_point=0, towns=0, j=0, y=0, z=0, d=0, i=0;
    QString listString;
    QList<QString> list= buff.split(" ", QString::SkipEmptyParts);
    int size = list.size();
    for(i=0; i<size; i++)
    ui->textEdit->append(list[i]);
    i=0;
    towns=list[i].toInt();
    d=roads(towns);
    //printf("%d", d);
    int* roads1=new int[d];
    int* roads2=new int[d];
    int* result=new int[d*d];
    memset(roads1, 0, 4*d);
    memset(roads2, 0, 4*d);
    memset(result, 0, 4*d);
    for(i=0; i<(size-3)/2; i++)
    {
        roads1[i]=list[2*i+1].toInt();
        roads2[i]=list[2*(i+1)].toInt();
    }
    starting_point=list[2*i+1].toInt();
    arrival_point=list[2*i+2].toInt();
    graph(starting_point, roads1, roads2, result, j, y, towns);
    for(int i=0; i<d*d; i++)
    {
        if(result[i]>towns)
            result[i]=0;
    }
    for(int i=0; i<d*d; i++)
    {
        if(result[i]==arrival_point)
        {
            ui->textEdit->setPlainText("There is a road between these towns");
            z=1;
        }
    }
    if(z!=1)
        ui->textEdit->setPlainText("There is no road between these towns");
    delete(roads1);
    delete(roads2);
    delete(result);
}

int roads(int n)
{
   if(n>0)
   {
       n=(n-1)+roads(n-1);
   }
   return n;
}

int* graph(int k, int* roads1, int* roads2, int* result, int j, int y, int towns)
{
    int t=0;
    for(int i=0; i<towns; i++)
    {
        if((roads1[i]==k)&&(y!=roads2[i]))
        {
            y=roads1[i];
            t=roads2[i];
            while((result[j]!=0)&&(result[j]<towns))
                j++;
            result[j]=t;
            j++;
            graph(t, roads1, roads2, result, j, y, towns);
        }
        else if((roads2[i]==k)&&(y!=roads1[i]))
        {
            y=roads2[i];
            t=roads1[i];
            while((result[j]!=0)&&(result[j]<towns))
                j++;
            result[j]=t;
            j++;
            graph(t, roads1, roads2, result, j, y, towns);
        }
    }
    return result;
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
        return buff;
    }
    QTextStream stream(&input);

    buff=stream.readAll();
    input.close();
    return buff;
}
