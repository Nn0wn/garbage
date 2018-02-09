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

BT* tree;

bool gravity = false;


int* analyser(QString str, int k, int* arr)
{
    int* temp;
    int i=0, j=0;
    bool flag_word=false;
    bool flag_dotcom=false;
    bool flag_num=true;
    while(str[k]!='\0')
    {
        flag_dotcom=false;
        flag_num=true;
        while(str[k]==" ")
            k++;
        while(str[k]!=";" && str[k]!=" " && str[k]!='\0')
        {
            flag_word=true;
            k++;
        }
        if(str[k]==";")
        {
            flag_dotcom=true;
            k++;
        }
        if(str[k]<'0' || str[k]>'9')
            flag_num=false;
        while(str[k]!=" " && str[k]!='\0')
        {
            if(str[k]<'0' || str[k]>'9')
                flag_num=false;
            k++;
        }
        if(str[k]!=" " && str[k]!='\0')
            flag_num=false;
        if(flag_dotcom==false || flag_word==false || flag_num==false)
        {
            arr[j]=i;
            j++;
        }
        i++;
        while(str[k]==" ")
            k++;
    }
//    temp=new int[j];
//    for(i=0;i<j;i++)
//        temp[i]=arr[i];
//    temp[j]=-1;
//    delete []arr;
//    arr=temp;
//    temp=0;
//    delete temp;
    arr[j]=-1;
    return arr;
}

int strsize(QString str)
{
    bool flag=false;
    int size=0;
    int i=0;
    while(i<str.size())
    {
        if(str[i]!=' ' && flag!=true)
        {
            flag=true;
            size++;
        }
        if(str[i]==" ")
            flag=false;
        i++;
    }
    return size;
}

BT* Read(QString str, int i, QTableWidget* tw, int* without_nums){
    int j=0, k=0, t=0, p=0;
    int *arr;
    int str_size=strsize(str)+1;
    arr=new int[str_size];
    QStringList str_l=str.split(" ", QString::SkipEmptyParts);
    QString str_i, str_new;
    if(without_nums[str_l.size()]!=-1)
    {
        for(i=i;i<str_l.size();i++)
        {
            while(i==without_nums[k] && without_nums[k]!=-1 && i<str_l.size())
            {
                i++;
                k++;
            }
            if(i>=str_l.size())
                break;
            QByteArray str_b=str_l.at(i).toLatin1();
            char* str_ch=str_b.data();
            QString arr_ch2;
            QString arr_ch;
            char* end=strtok(str_ch, " ;");
            while(end!=NULL)
            {
                QString end_s(end);
                arr_ch=end_s;
                end=strtok(NULL, " ;");
                QString end_s2(end);
                arr_ch2=end_s2;
                arr[j]=arr_ch2.toInt();
                end=strtok(NULL, " ;");
            }
            tw->insertRow(j);
            tw->setItem(j, 0, new QTableWidgetItem(arr_ch2));
            tw->setItem(j, 1, new QTableWidgetItem(arr_ch));
            if(j<str_size)
                j++;
        }
    }
    if(without_nums[0]!=-1)
    {
        for(i=0; i<str_l.size(); i++)
        {
            while(i!=without_nums[t] && i<str_l.size())
                i++;
            if(without_nums[t]!=-1)
            t++;
            if(i>=str_l.size())
                break;
            str_new=str_l[i];
            for(p=0; p<str_new.size(); p++)
                if(str_new[p]==";")
                    str_new.truncate(p);
            int numb=0;
            for(p=0; p<j; p++)
            {
                if(numb==arr[p])
                {
                    numb++;
                    p=-1;
                }
            }
            arr[j]=numb;
            tw->insertRow(j);
            tw->setItem(j, 0, new QTableWidgetItem(str_i.number(numb)));
            tw->setItem(j, 1, new QTableWidgetItem(str_new));
            if(j<str_size)
                j++;
            arr[j]=-1;
            if(t>=k && k!=0)
                i=str_l.size();
        }
    }
    k=0;
    p=0;
    if(without_nums[str_l.size()]!=-1)
    {
        for(i=0;i<str_l.size();i++)
        {
            while(i==without_nums[k] && without_nums[k]!=-1)
            {
                i++;
                k++;
            }
            if(i>=str_l.size())
                break;
            str_new=str_l[i];
            for(j=0;j<str_new.size();j++)
                if(str_new[j]==";")
                    str_new.truncate(j);
            tree=insertrand(tree, NULL, arr[p], str_new);
            if(p<str_size)
                p++;
        }
    }
    k=0;
    if(without_nums[0]!=-1)
    {
        for(i=0;i<str_l.size();i++)
        {
            while(i!=without_nums[k])
                i++;
            if(without_nums[k]!=-1)
            k++;
            if(i>=str_l.size())
                break;
            str_new=str_l[i];
            for(j=0;j<str_new.size();j++)
                if(str_new[j]==";")
                    str_new.truncate(j);
            tree=insertrand(tree, NULL, arr[p], str_new);
            if(without_nums[k]==-1)
                i=str_l.size();
            if(p<str_size)
                p++;
        }
    }
    //delete []without_nums;
    delete []arr;
    fixRoots(tree, nullptr);
    return tree;
}

BT* ReadFile(QString fileName, QTextEdit* te, QTableWidget* tw){
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
    int* without_numbers = new int[strsize(temp)+1];
    return Read(temp, i, tw, analyser(temp, i, without_numbers));
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tree = nullptr;

    this->createUI(QStringList()<<trUtf8("Key")
                               <<trUtf8("Element"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createUI(const QStringList &headers)
{
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setShowGrid(true);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->hideColumn(3);
}

void MainWindow::on_action_7_triggered()
{
    tree=delete_tree(tree);
    exit(0);
}

void MainWindow::on_action_5_triggered()
{
   int i=0;
   tree=delete_tree(tree);
   ui->tableWidget->clearContents();
   while(ui->tableWidget->rowCount()!=0)
   {
       ui->tableWidget->removeRow(0);
       i++;
   }

   QString fileName = QFileDialog::getOpenFileName(this, "Open file");
   tree = ReadFile(fileName, ui->textEdit, ui->tableWidget);
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

void MainWindow::on_action_11_triggered()
{
    ui->textEdit_3->clear();
    if(ui->textEdit_2->toPlainText()=="")
    {
        ui->textEdit_3->setPlainText("No info");
        return;
    }
    QString points_arr=ui->textEdit_2->toPlainText();
    QStringList points_arr_l=points_arr.split(" ", QString::SkipEmptyParts);
    BT* temp=Find(tree, points_arr_l.first().toInt());
    if(temp!=nullptr)
    {
        ui->textEdit_3->insertPlainText(temp->info);
        return;
    }
    else
    {
        ui->textEdit_3->setText("ERROR, no such element");
        return;
    }
}

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
    int i=0;
    int point_i;
    QString point;
    point=ui->textEdit_4->toPlainText();
    point_i=point.toInt();
    tree=remove(tree, point_i);
    QTableWidgetItem* widg_i=new QTableWidgetItem;
    while(i<ui->tableWidget->rowCount())
    {
        widg_i=ui->tableWidget->item(i,0);
        if(point==widg_i->data(0))
        {
            ui->tableWidget->removeRow(i);
            i=ui->tableWidget->rowCount();
        }
        i++;
    }
    fixRoots(tree, nullptr);
    /*if(tree!=nullptr)
    {
        GraphWidget *widget = new GraphWidget(this, tree, &gravity);
        QMainWindow* temp = new QMainWindow;
        temp->setCentralWidget(widget);
        temp->setWindowTitle("Graph output");
        //temp->show();
    }*/
}

void MainWindow::on_pushButton_3_clicked()
{
    QString element=ui->textEdit_5->toPlainText();
    int* arr=new int[strsize(element)+1];
    if(analyser(element, 0, arr)[0]!=-1)
    {
        ui->textEdit_3->setPlainText("You entered wrong element");
        return;
    }
    QStringList list=element.split(";", QString::SkipEmptyParts);
    tree=insertrand(tree, nullptr, list[1].toInt(), list[0]);
    fixRoots(tree, nullptr);
    int rows=ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(rows);
    ui->tableWidget->setItem(rows, 0, new QTableWidgetItem(list[1]));
    ui->tableWidget->setItem(rows, 1, new QTableWidgetItem(list[0]));
    /*if(tree!=nullptr)
    {
        GraphWidget *widget = new GraphWidget(this, tree, &gravity);
        QMainWindow* temp = new QMainWindow;
        temp->setCentralWidget(widget);
        temp->setWindowTitle("Graph output");
        temp->show();
    }*/
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->textEdit_3->clear();
    if(ui->textEdit_2->toPlainText()=="")
    {
        ui->textEdit_3->setPlainText("No info");
        return;
    }
    QString points_arr=ui->textEdit_2->toPlainText();
    QStringList points_arr_l=points_arr.split(" ", QString::SkipEmptyParts);
    BT* temp=Find(tree, points_arr_l.first().toInt());
    if(temp!=nullptr)
    {
        ui->textEdit_3->insertPlainText(temp->info);
        return;
    }
    else
    {
        ui->textEdit_3->setText("ERROR, no such element");
        return;
    }
}
