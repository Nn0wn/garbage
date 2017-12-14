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

int analyser(QString buffer, int length)
{
    int i=0;
    QChar open_bracket;
    int elem_counter=0;
    int* temp=new int[buffer.size()/2];
    int* bracket_iter=new int[buffer.size()/2];
    int* bracket_flag=new int[buffer.size()/2];
    for(int j=0; j<buffer.size()/2; j++)
    {
        bracket_iter[j]=0;
        bracket_flag[j]=0;
    }
    int lvl_counter=0;
    while(buffer[i]==' ')
        i++;
    if(buffer[i]!='(')
        return 1;
    temp[0]=0;
    for(i=i; i<length; i++)
    {
        open_bracket=buffer[i];
        if((open_bracket!='(')&&(open_bracket!=')'))
            elem_counter++;
        if(open_bracket=='(')
        {
            temp[lvl_counter]=elem_counter;
            bracket_flag[lvl_counter]=1;
            lvl_counter++;
            elem_counter=0;
        }
        if(open_bracket==')')
        {
            for(int j=0; j<buffer.size()/2; j++)
            {
                if(bracket_iter[j]>2)
                    return 1;
            }
            bracket_iter[lvl_counter]=0;
            lvl_counter--;
            if(bracket_flag[lvl_counter]==1)
            {
                bracket_iter[lvl_counter]++;
                bracket_flag[lvl_counter]=0;
            }
            elem_counter=temp[lvl_counter];
        }
        if(elem_counter>2)
            return 1;
        if((elem_counter>1)&&(lvl_counter==1))
            return 1;
    }
    if(lvl_counter!=0)
        return 1;
    return 0;
}

BT* Read(QString str, BT* TRoot, int* i){
    if (str[*i] == 'E'){
        (*i)++;
        return nullptr;
    }
    if(str[*i] == ' ')
        (*i)++;
    if (str[*i] == '('){
        (*i)++;
        BT* tree = ConsBT(str[(*i)++], nullptr, nullptr, TRoot);
        if(str[*i] == ' ')
            (*i)++;
        if (str[*i] == '(')
            tree->Left = Read(str, tree, i);
        if (str[*i] == 'E')
            (*i)++;
        if(str[*i] == ' ')
            (*i)++;
        if (str[*i] == '(')
            tree->Right = Read(str, tree, i);
        if (str[*i] == ')'){
            (*i)++;
            return tree;
        }
        return tree;
    }
    return nullptr;
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
    QTextStream file1s(&file1);
    QString temp = file1s.readAll();
    te->setPlainText(temp);
    int i = 0;
    return Read(temp, nullptr, &i);
}

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
    exit(0);
}

void MainWindow::on_action_5_triggered()
{
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
    QString points_arr=ui->textEdit_2->toPlainText();
    for(int i=0;i<points_arr.size();i++)
    {
    elem point=points_arr[i];
    if(point=='\0')
    {
        ui->textEdit_3->setText("ERROR, no element for matching!");
        return;
    }
    int counter=-1;
    int res = WayLength(tree, counter, point);
        ui->textEdit_3->insertPlainText(QString::number((int)res));
        ui->textEdit_3->insertPlainText(" | ");
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
