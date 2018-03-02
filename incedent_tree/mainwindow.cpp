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

INCTR* tree= new INCTR;
int peaks_i;
bool gravity = false;


int GhouilaHouriCheck(INCTR* peaks, int size)
{
    for(int i=0; i<size; i++)
    {
        if(peaks[i].intdeg<size/2 || peaks[i].outdeg<size/2)
            return 1;
    }
    return 0;
}

int Factor(int size)
{
    if(size==1)
        return 1;
    size*=Factor(size-1);
    return size;
}

int** Gamilton(INCTR* genparent, INCTR* parent, int peaks_i, int step, int circle, int** gampath)
{
    bool flag=false;
    INCTR* temp= new INCTR;
    temp=parent;
    while(temp->next)
    {
        temp=temp->next;
        flag=false;
        for(int j=0; j<peaks_i; j++)
        {
            if(step!=peaks_i)
            {
                if(temp->turn==tree[j].turn /*&& j!=peaks_i-1*/)
                {
                    for(int k=0; k<step; k++)
                    {
                        if(gampath[circle][k]==tree[j].turn)
                        {
                            if(!temp->next)
                            {
                                gampath[/*circle*/0][peaks_i+1]=circle;
                                gampath[/*circle*/0][peaks_i+2]=step-1;
                                return gampath;
                            }
                            else
                            {
                                flag=true;
                            }
                        }
                    }
                    if(flag!=true)
                    {
                        flag=false;
                        gampath[circle][step]=temp->turn;
                        //if(step<peaks_i-1)
                        step++;
                        gampath=Gamilton(genparent, &tree[j], peaks_i, step, circle, gampath);
                        step=gampath[/*circle*/0][peaks_i+2];
                        circle=gampath[/*circle*/0][peaks_i+1];
                    }
                }
            }
            else
            {
                if(temp->turn==genparent->turn)
                {
                    //gampath[circle][step]=temp->turn;
                    circle++;
//                    if(temp->next)
//                    {
                        gampath[circle]=new int[peaks_i+10];
                        for(int i=0; i<step-1; i++)
                            gampath[circle][i]=gampath[circle-1][i];
//                    }
                    step-=1;
                    gampath[circle-1][peaks_i]=-1;
                    gampath[/*circle-1*/0][peaks_i+1]=circle;
                    gampath[/*circle-1*/0][peaks_i+2]=step;
                    return gampath;
                }
                else
                {
                    while(temp->next)
                    {
                        temp=temp->next;
                        if(temp->turn==genparent->turn)
                        {
                            //gampath[circle][step]=temp->turn;
                            circle++;
//                            if(temp->next)
//                            {
                                gampath[circle]=new int[peaks_i+10];
                                for(int i=0; i<step-1; i++)
                                    gampath[circle][i]=gampath[circle-1][i];
//                            }
                            step-=1;
                            gampath[circle-1][peaks_i]=-1;
                            gampath[/*circle-1*/0][peaks_i+1]=circle;
                            gampath[/*circle-1*/0][peaks_i+2]=step;
                            return gampath;
                        }
                    }
                    step-=1;
                    gampath[/*circle*/0][peaks_i+1]=circle;
                    gampath[/*circle*/0][peaks_i+2]=step;
                    return gampath;
                }
            }

        }
    }
    //if(!temp->next)
    gampath[/*circle-1*/0][peaks_i+2]=step-1;
    //delete temp;
    return gampath;
}


int analyser(QString str, int k)
{
    int peaks=0;
    int com=0;
    while(str[k]==' ' || str[k]=='\t' || str[k]=='\r' || str[k]=='\n')
        k++;
    if(str[k]!='\0')
        peaks++;
    while(str[k]!='\0' /*&& com<3*/)
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
    if(str[k]!='\t' && str[k]!=' ' && str[k]!='\n' && str[k]!='\r')
        flag=true;
    if(flag==true)
    {
        new_str[i]=str[k];
        i++;
    }
    for(k=1; k<str.size(); k++)
    {
        if(str[k-1]=='\n' || str[k-1]==',' || str[k]=='\n' || str[k]==',')
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

INCTR* Read(QString str, INCTR* peaks, int size)
{
    int i=0, j=0, k=0;
    while(str[k]!='\0')
    {
        while(str[k]!=',' && str[k]!='\r' && str[k]!='\0')
        {
            peaks[i].name[j]=str[k];
            peaks[i].turn=i+1;
            k++;
            j++;
        }
        j=0;
        if(str[k]!='\0' && str[k]!='\r')
        {
            k++;
            INCTR* temp=new INCTR;
            peaks[i].next=temp;
            peaks[i].outdeg++;
            temp->next=NULL;
            temp->intdeg++;
            temp->prev=&peaks[i];
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
                    peaks[i].outdeg++;
                    temp->next=temp2;
                    temp2->next=NULL;
                    temp2->intdeg++;
                    temp2->prev=temp;
                    temp=temp2;
                    k++;
                }
                j=0;
            }
            //delete temp;
        }
        i++;
        k++;
    }
    for(i=0; i<size; i++)
    {
        for(j=0; j<size; j++)
        {
            if(peaks[j].next)
            {
                INCTR* temp=new INCTR;
                temp=peaks[j].next;
                if(peaks[i].name==temp->name)
                {
                    peaks[i].intdeg+=temp->intdeg;
                    temp->turn=peaks[i].turn;
                }
                while(temp->next)
                {
                    temp=temp->next;
                    if(peaks[i].name==temp->name)
                    {
                        peaks[i].intdeg+=temp->intdeg;
                        temp->turn=peaks[i].turn;
                    }
                }
                //delete temp;
            }
        }
        peaks[i].deg=peaks[i].intdeg+peaks[i].outdeg;
    }
    return peaks;
}


INCTR* ReadFile(QString fileName, QTextEdit* te){
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
    peaks_i=analyser(temp, i);
    if(peaks_i==-1)
        return nullptr;
    temp=corrector(temp);
    INCTR* peaks = new INCTR[peaks_i];
    peaks=Read(temp, peaks, peaks_i);
//    BT* temp_tree=new BT;
//    //for(int k=0; k<peaks[0].name.size(); k++)
//    temp_tree->info=peaks[0].name;
//    return ConstTree(temp_tree, peaks, peaks_i, 0);
    return peaks;
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
    tree=delete_tree(tree, peaks_i);
    exit(0);
}

void MainWindow::on_action_5_triggered()
{
   tree=delete_tree(tree, peaks_i);
   QString fileName = QFileDialog::getOpenFileName(this, "Open file");
   tree = ReadFile(fileName, ui->textEdit);
   ui->action_3->setEnabled(true);
}

void MainWindow::on_action_6_triggered()
{
    QString text=ui->textEdit->toPlainText();
    if(analyser(text, text.size())==1)
    {
        ui->textEdit_3->setText("ERROR, You entered wrong tree!");
        return;
    }
    if(tree==nullptr)
    {
        ui->textEdit_3->setText("ERROR, no info for graph construction!");
        return;
    }
    GraphWidget *widget = new GraphWidget(this, tree, &gravity, peaks_i, nullptr);
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

void MainWindow::on_pushButton_clicked()
{
    int step=1, circle=0, i=0;
    int** gampath=new int*[100];
    for(i=0; i<log(Factor(peaks_i)); i++)
    {
        gampath[i]=nullptr;
        gampath[i]=new int[100];

    }
    if(GhouilaHouriCheck(tree, peaks_i)!=0)
    {
        ui->textEdit_3->setText("Wrong graph");
        return;
    }
    i=0;
    for(int i=0; i<peaks_i; i++)
    {
//        if(circle==0)
//            gampath[circle]=new int[peaks_i+30];
//        else
//            gampath[circle]=new int[peaks_i+10];
        gampath[circle][0]=tree[i].turn;
        gampath=Gamilton(&tree[i], &tree[i], peaks_i, step, circle, gampath);
        //gampath[4]=new int [peaks_i];
        while(gampath[circle] && gampath[circle][peaks_i]==-1)
            circle++;
    }
    gampath[circle]=NULL;
    ui->textEdit_3->clear();
    for(int i=0; i<circle-1; i++)
    {
        ui->textEdit_2->append(QString::number(i+1)+')'+'\t'+'|');
        for(int j=0; j<peaks_i; j++)
        {
            for(int k=0; k<peaks_i; k++)
            {
                if(gampath[i][j]==tree[k].turn)
                    ui->textEdit_2->insertPlainText(QString::number(tree[k].turn)+'|');
            }
        }
        ui->textEdit_2->append((QString)'\n');
    }
    if(ui->textEdit_4->toPlainText()!="")
    {
        int chosen_circ = ui->textEdit_4->toPlainText().toInt();
        if(chosen_circ<circle)
        {
            GraphWidget* widget = new GraphWidget(this, tree, &gravity, peaks_i, gampath[chosen_circ-1]);
            QMainWindow* temp = new QMainWindow;
            temp->setCentralWidget(widget);
            temp->setWindowTitle("Graph output");
            temp->show();
        }
        else
        {
            ui->textEdit_2->setText("Wrong cycle");
            return;
        }
    }
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
