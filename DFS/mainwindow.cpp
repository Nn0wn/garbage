#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QString>
#include <QTextStream>
#include <QMessageBox>
#include "graphwidget.h"
#include "inctree.h"
#include "node.h"
#include <QWidget>

INCTR* tree= new INCTR;
int peaks_i;
bool gravity = false;
int* path;


int Factor(int size)
{
    if(size==1)
        return 1;
    size*=Factor(size-1);
    return size;
}

int analyser(QString str, int k)
{
    int peaks=0;
    int com=0;
    while(str[k]==' ' || str[k]=='\t' || str[k]=='\r' || str[k]=='\n')
        k++;
    if(str[k]!='\0')
        peaks++;
    while(str[k]!='\0')
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

int* DFS(INCTR* tree, int tree_size, int i, int* path)
{
    int step=0, level=1;
    INCTR* temp=new INCTR;
    INCTR* dfs_tree=new INCTR;
    INCTR* last_prev=new INCTR;
    dfs_tree=&tree[i];
    path[step]=dfs_tree->turn;
    dfs_tree->grey=true;
    dfs_tree->level=level;
    level++;
    for(int j=0; j<tree_size; j++)
    {
        temp=&tree[j];
        while(temp->next)
        {
            temp=temp->next;
            if(dfs_tree->turn==temp->turn)
                temp->grey=true;
        }
    }
    last_prev=dfs_tree;
    while(dfs_tree&&(dfs_tree->next || dfs_tree->prev))
    {
        if(dfs_tree->next)
        {
            if(dfs_tree->next->grey!=true)
            {
                for(int j=0; j<tree_size; j++)
                {
                    temp=&tree[j];
                    while(temp->next)
                    {
                        temp=temp->next;
                        if(dfs_tree->next->turn==temp->turn)
                            temp->grey=true;
                    }
                }
                dfs_tree=&tree[dfs_tree->next->turn-1];
                dfs_tree->prev=last_prev;
                last_prev=dfs_tree;
                i=dfs_tree->turn-1;
                dfs_tree->grey=true;
                dfs_tree->level=level;
                level++;
                step++;
                path[step]=dfs_tree->turn;

            }
            else
            {
                if(dfs_tree->next->grey!=true)
                    dfs_tree->next->prev=dfs_tree->prev;
                dfs_tree=dfs_tree->next;
            }
        }
        else
        {
            dfs_tree=dfs_tree->prev->prev;
            last_prev=dfs_tree;
            level--;
            step++;
            if(dfs_tree)
                path[step]=dfs_tree->turn;
            else
                path[step]=0;
        }
    }
    return path;
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
                    temp2->prev=&peaks[i];
                    temp=temp2;
                    k++;
                }
                j=0;
            }
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
        return tree;
    }
    tree=delete_tree(tree);
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
    return peaks;
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
    tree=delete_tree(tree);
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
    GraphWidget *widget = new GraphWidget(this, tree, &gravity, peaks_i, 0, nullptr);
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
    int num=pow(2,peaks_i)+20;
    path=new int[num];
    INCTR* temp=new INCTR;
    for(int j=0; j<peaks_i; j++)
    {
        temp=&tree[j];
        temp->prev=nullptr;
        while(temp->next)
        {
            temp=temp->next;
            temp->grey=false;
        }
    }
    if(ui->textEdit_4->toPlainText()!="")
    {
        int chosen_peak = ui->textEdit_4->toPlainText().toInt();
        if(chosen_peak<peaks_i)
        {
            path=DFS(tree, peaks_i, chosen_peak-1, path);
            QMainWindow* temp = new QMainWindow;
            GraphWidget* widget = new GraphWidget(this, tree, &gravity, peaks_i, 1000, path);
            temp->setWindowTitle("Graph output");
            temp->setCentralWidget(widget);
            temp->show();
            temp->setAttribute(Qt::WA_DeleteOnClose);

        }
        else
        {
            ui->textEdit_3->clear();
            ui->textEdit_3->setText("Wrong peak");
            return;
        }
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    if(ui->textEdit_2->toPlainText()!="")
    {
        if(ui->textEdit_4->toPlainText()!="")
        {
            int chosen_peak = ui->textEdit_4->toPlainText().toInt();
            if(chosen_peak<peaks_i)
            {
                INCTR* temp=new INCTR;
                for(int j=0; j<peaks_i; j++)
                {
                    temp=&tree[j];
                    temp->prev=nullptr;
                    while(temp->next)
                    {
                        temp=temp->next;
                        temp->grey=false;
                    }
                }
                path=new int[Factor(peaks_i)];
                path=DFS(tree, peaks_i, chosen_peak-1, path);
            }
            else
            {
                ui->textEdit_3->clear();
                ui->textEdit_3->setText("Wrong peak");
                return;
            }
        }
        else
        {
            ui->textEdit_3->clear();
            ui->textEdit_3->setText("Choose any peak");
            return;
        }
        QString name=ui->textEdit_2->toPlainText();
        int min=INT_MAX;
        for(int j=0; j<peaks_i; j++)
        {
            if(name==tree[j].name)
                min=tree[j].level-1;
        }
        if(min!=INT_MAX)
            ui->textEdit_3->setText("Distance to a peak "+name+" is "+QString::number(min));
        else
            ui->textEdit_3->setText("There isn't such an element");
    }
    else
    {
        ui->textEdit_3->clear();
        ui->textEdit_3->setText("Write the name of the peak");
        return;
    }

}

void MainWindow::on_pushButton_3_clicked()
{
    if(!path)
    {
        if(ui->textEdit_4->toPlainText()!="")
        {
            int num=pow(2,peaks_i)+20;
            path=new int[num];
            INCTR* temp=new INCTR;
            for(int j=0; j<peaks_i; j++)
            {
                temp=&tree[j];
                temp->prev=nullptr;
                while(temp->next)
                {
                    temp=temp->next;
                    temp->grey=false;
                }
            }
            int chosen_peak = ui->textEdit_4->toPlainText().toInt();
            if(chosen_peak<peaks_i)
            {
                path=DFS(tree, peaks_i, chosen_peak-1, path);
                QMainWindow* temp = new QMainWindow;
                GraphWidget* widget = new GraphWidget(this, tree, &gravity, peaks_i, 1, path);
                temp->setWindowTitle("Graph output");
                temp->setCentralWidget(widget);
                temp->show();
                temp->setAttribute(Qt::WA_DeleteOnClose);
            }
            else
            {
                ui->textEdit_3->clear();
                ui->textEdit_3->setText("Wrong peak");
                return;
            }
        }
        else
        {
            ui->textEdit_3->clear();
            ui->textEdit_3->setText("Choose any peak");
            return;
        }
    }
    else
    {
        QMainWindow* temp = new QMainWindow;
        GraphWidget* widget = new GraphWidget(this, tree, &gravity, peaks_i, 1, path);
        temp->setWindowTitle("Graph output");
        temp->setCentralWidget(widget);
        temp->show();
        temp->setAttribute(Qt::WA_DeleteOnClose);
    }
}

void MainWindow::on_actionAbout_triggered()
{
    HelpBrowser* browser = new HelpBrowser(":/", "index.htm");
    browser->resize(800, 600);
    browser->show();
}
