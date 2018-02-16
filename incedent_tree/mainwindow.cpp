#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QString>
#include <QTextStream>
#include <QMessageBox>
#include "graphwidget.h"
#include "bintree.h"
#include "node.h"
#include <QWidget>

BT* tree= new BT;

bool gravity = false;


int analyser(QString str, int k)
{
    int peaks=0;
    int com=0;
    while(str[k]==' ' || str[k]=='\t' || str[k]=='\r' || str[k]=='\n')
        k++;
    if(str[k]!='\0')
        peaks++;
    while(str[k]!='\0' && com<3)
    {
        if(str[k]==',')
            com++;
        if(str[k]=='\n')
        {
            while(str[k+1]==' ' || str[k+1]=='\t' || str[k+1]=='\r')
                k++;
            if(str[k+1]!='\0' && str[k+1]!='\r' && str[k+1]!='\n')
                peaks++;
            com=0;
        }
        k++;
    }
    if(k!=str.size())
        return -1;
    return peaks;
}

QString corrector(QString str)
{
    int k=0, i=0;
    QString new_str;
    bool flag=false;
    for(k=0; k<str.size(); k++)
    {
        if(str[k]=='\n' || str[k]==',')
            flag=false;
        if(str[k]!='\t' && str[k]!=' ' && str[k]!='\n' && str[k]!='\r')
            flag=true;
        if(flag==true)
        {
            new_str[i]=str[k];
            i++;
        }
    }
    return new_str;
}

INCTR* Read(QString str, INCTR* peaks)
{
    int i=0, j=0, k=0;
    while(str[k]!='\0')
    {
        while(str[k]!=',' && str[k]!='\r' && str[k]!='\0')
        {
            peaks[i].name[j]=str[k];
            k++;
            j++;
        }
        j=0;
        if(str[k]!='\0' && str[k]!='\r')
        {
            k++;
            INCTR* temp=new INCTR;
            peaks[i].next=temp;
            temp->next=NULL;
            while(str[k]!=',' && str[k]!='\r' && str[k]!='\0')
            {

                while(str[k]!=',' && str[k]!='\r' && str[k]!='\0')
                {
                    temp->name[j]=str[k];
                    j++;
                    k++;
                }

                if(str[k]!='\0' && str[k]!='\r')
                {
                    INCTR* temp2=new INCTR;
                    temp->next=temp2;
                    temp2->next=NULL;
                    temp=temp2;
                    k++;
                }
                j=0;
            }
        }
        i++;
        k++;
    }
    return peaks;
}

BT* ConstTree(BT* tree,INCTR* peaks, int peaks_i, int i)
{
    int k=0, j=0;
    //tree->info=peaks[0].name;
    if(peaks[i].next)
    {
        BT* temp=new BT;
        temp->info=peaks[i].next->name;
        tree->Left=temp;
        tree->Left->Root=tree;
        tree->Left->Left=NULL;
        tree->Left->Right=NULL;
        for(j=0; j<peaks_i; j++)
        {
            if(peaks[j].name==peaks[i].next->name)
            {
                k=j;
                ConstTree(tree->Left, peaks, peaks_i, k);
            }
        }
        if(peaks[i].next->next)
        {
            BT* temp=new BT;
            temp->info=peaks[i].next->next->name;
            tree->Right=temp;
            tree->Right->Root=tree;
            tree->Right->Left=NULL;
            tree->Right->Right=NULL;
            for(j=0; j<peaks_i; j++)
            {
                if(peaks[j].name==peaks[i].next->next->name)
                {
                    k=j;
                    ConstTree(tree->Right, peaks, peaks_i, k);
                }
            }
        }
    }
    return tree;
}


BT* ReadFile(QString fileName, QTextEdit* te){
    QFile file1(fileName);
    file1.open(QIODevice::ReadOnly);
    if (!file1.isOpen()){
        QMessageBox msg;
        msg.setText("File isn't open!");
        msg.setWindowTitle("ERROR");
        msg.exec();
        return nullptr;
    }
    int i = 0;
    QTextStream file1s(&file1);
    QString temp = file1s.readAll();
    te->setPlainText(temp);
    int peaks_i=analyser(temp, i);
    if(peaks_i==-1)
        return nullptr;
    temp=corrector(temp);
    INCTR* peaks = new INCTR[peaks_i];
    peaks=Read(temp, peaks);
    BT* temp_tree=new BT;
    //for(int k=0; k<peaks[0].name.size(); k++)
    temp_tree->info=peaks[0].name;
    return ConstTree(temp_tree, peaks, peaks_i, 0);
}

//BT* ReadFile(QString fileName, QTextEdit* te){
//    QFile file1(fileName);
//    file1.open(QIODevice::ReadOnly);
//    if (!file1.isOpen()){
//        QMessageBox msg;
//        msg.setText("File isn't open!");
//        msg.setWindowTitle("ERROR");
//        msg.exec();
//        return nullptr;
//    }
//    int i = 0;
//    QTextStream file1s(&file1);
//    QString temp = file1s.readAll();
//    te->setPlainText(temp);
//    int peaks_i=analyser(temp, i);
//    //int* without_numbers = new int[strsize(temp)+1];
//    //return Read(temp, i, tw, analyser(temp, i, without_numbers));
//}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tree = nullptr;

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_action_7_triggered()
{
    tree=delete_tree(tree);
    exit(0);
}

void MainWindow::on_action_5_triggered()
{
   tree=delete_tree(tree);
   QString fileName = QFileDialog::getOpenFileName(this, "Open file");
   tree = ReadFile(fileName, ui->textEdit);
   ui->action_3->setEnabled(true);
}

void MainWindow::on_action_6_triggered()
{
    QString text=ui->textEdit->toPlainText();
//    if(analyser(text, text.size())==1)
//    {
//        ui->textEdit_3->setText("ERROR, You entered wrong tree!");
//        return;
//    }
    if(tree==nullptr)
    {
        ui->textEdit_3->setText("ERROR, no info for graph construction!");
        return;
    }
    GraphWidget *widget = new GraphWidget(this, tree, &gravity);
    QMainWindow* temp = new QMainWindow;
    temp->setCentralWidget(widget);
    temp->setWindowTitle("Graph output");
    temp->show();
}

void MainWindow::on_action_triggered()
{
    ui->checkBox->setCheckState(Qt::Checked);
    gravity = true;
}

void MainWindow::on_action_2_triggered()
{
    ui->checkBox->setCheckState(Qt::Unchecked);
    gravity = false;
}

//void MainWindow::on_action_11_triggered()
//{
//    ui->textEdit_3->clear();
//    if(ui->textEdit_2->toPlainText()=="")
//    {
//        ui->textEdit_3->setPlainText("No info");
//        return;
//    }
//    QString points_arr=ui->textEdit_2->toPlainText();
//    QStringList points_arr_l=points_arr.split(" ", QString::SkipEmptyParts);
//    BT* temp=Find(tree, points_arr_l.first().toInt());
//    if(temp!=nullptr)
//    {
//        ui->textEdit_3->insertPlainText(temp->info);
//        return;
//    }
//    else
//    {
//        ui->textEdit_3->setText("ERROR, no such element");
//        return;
//    }
//}

void MainWindow::on_checkBox_clicked()
{
    if (ui->checkBox->checkState()){
        gravity = true;
    }
    else
        gravity = false;
}

//void MainWindow::on_pushButton_clicked()
//{
//    int i=0;
//    int point_i;
//    QString point;
//    point=ui->textEdit_4->toPlainText();
//    point_i=point.toInt();
//    tree=remove(tree, point_i);
//    QTableWidgetItem* widg_i=new QTableWidgetItem;
//    while(i<ui->tableWidget->rowCount())
//    {
//        widg_i=ui->tableWidget->item(i,0);
//        if(point==widg_i->data(0))
//        {
//            ui->tableWidget->removeRow(i);
//            i=ui->tableWidget->rowCount();
//        }
//        i++;
//    }
//    fixRoots(tree, nullptr);
//    /*if(tree!=nullptr)
//    {
//        GraphWidget *widget = new GraphWidget(this, tree, &gravity);
//        QMainWindow* temp = new QMainWindow;
//        temp->setCentralWidget(widget);
//        temp->setWindowTitle("Graph output");
//        //temp->show();
//    }*/
//}

//void MainWindow::on_pushButton_3_clicked()
//{
//    QString element=ui->textEdit_5->toPlainText();
//    //int* arr=new int[strsize(element)+1];
//    //if(analyser(element, 0, arr)[0]!=-1)
//    //{
//    //    ui->textEdit_3->setPlainText("You entered wrong element");
//    //    return;
//    //}
//    QStringList list=element.split(";", QString::SkipEmptyParts);
//    tree=insertrand(tree, nullptr, list[1].toInt(), list[0]);
//    fixRoots(tree, nullptr);
//    int rows=ui->tableWidget->rowCount();
//    ui->tableWidget->insertRow(rows);
//    ui->tableWidget->setItem(rows, 0, new QTableWidgetItem(list[1]));
//    ui->tableWidget->setItem(rows, 1, new QTableWidgetItem(list[0]));
//    /*if(tree!=nullptr)
//    {
//        GraphWidget *widget = new GraphWidget(this, tree, &gravity);
//        QMainWindow* temp = new QMainWindow;
//        temp->setCentralWidget(widget);
//        temp->setWindowTitle("Graph output");
//        temp->show();
//    }*/
//}

//void MainWindow::on_pushButton_2_clicked()
//{
//    ui->textEdit_3->clear();
//    if(ui->textEdit_2->toPlainText()=="")
//    {
//        ui->textEdit_3->setPlainText("No info");
//        return;
//    }
//    QString points_arr=ui->textEdit_2->toPlainText();
//    QStringList points_arr_l=points_arr.split(" ", QString::SkipEmptyParts);
//    BT* temp=Find(tree, points_arr_l.first().toInt());
//    if(temp!=nullptr)
//    {
//        ui->textEdit_3->insertPlainText(temp->info);
//        return;
//    }
//    else
//    {
//        ui->textEdit_3->setText("ERROR, no such element");
//        return;
//    }
//}
