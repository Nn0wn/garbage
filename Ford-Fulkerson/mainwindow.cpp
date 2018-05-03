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

int Ford_Fulkeson(INCTR* tree, int* path, int start, int finish)
{
    int step=0, cur_flow=0, cur_turn=-1, flow_points_step=0, flow_max=0, flow_min=INT_MAX, k=0, path_step=0;
    while(true)
    {
        //flow_max bug
        cur_turn=start;
        int* prev_turn=new int[peaks_i-1];
        int* flow_points=new int[peaks_i-1];
        int* part_path=new int [peaks_i-1];
        for(int i=0; i<peaks_i; i++)
        {
            flow_points[i]=-1;
            part_path[i]=0;
            prev_turn[i]=0;
        }
        while(abs(cur_turn)!=finish)
        {
            INCTR* temp=&tree[abs(cur_turn)-1];
            flow_points_step=0;
            cur_turn=0;
            part_path[path_step]=temp->turn;
            path_step++;
            if(prev_turn!=0)
            {
                for(int i=0; i<peaks_i; i++)
                {
                    if(tree[i].turn==prev_turn[path_step-1])
                    {
                        temp->prev=&tree[i];
                        i=peaks_i;
                    }
                }
            }
            prev_turn[path_step-1]=temp->turn;
            temp->grey=true;
            for(int i=0; i< peaks_i; i++)
            {
                INCTR* temp_grey=&tree[i];
                while(temp_grey->next)
                {
                    temp_grey=temp_grey->next;
                    if(temp_grey->turn==temp->turn)
                        temp_grey->grey=true;
                }
            }
            while(temp->next)
            {
                temp=temp->next;
                if(temp->grey!= true && temp->weight-temp->weight_cur>cur_flow)
                {
                    cur_flow=temp->weight-temp->weight_cur;
                    cur_turn=temp->turn;
                }
            }
            for(int i=0; i< peaks_i; i++)
            {
                if(tree[i].grey!=true)
                {
                    temp=&tree[i];
                    while(temp->next)
                    {
                        temp=temp->next;
                        if(temp->turn==prev_turn[path_step-1] && temp->weight_cur>cur_flow)
                        {
                            cur_flow=temp->weight_cur;
                            cur_turn=-temp->turn;
                        }
                    }
                }
            }
            if(cur_flow==0 || cur_turn==-1)
            {
                if(temp->turn!=start)
                {
                    temp=temp->prev;
                    flow_points_step--;
                    path_step--;
                }
                else
                {
                    return flow_max;
                }
            }
            else
            {
                flow_points[flow_points_step]=cur_flow;
                flow_points_step++;
                cur_flow=0;
            }
        }
        part_path[path_step]=finish;
        while(flow_points[k]!=-1)
        {
            if(flow_points[k]<flow_min)
                flow_min=flow_points[k];
            k++;
        }
        flow_max+=flow_min;
        for(int i=0; i<peaks_i; i++)
        {
            INCTR* temp_turn;
            if(part_path[i]==0 || part_path[i+1]==0)
                i=peaks_i;
            for(int j=0; j<peaks_i; j++)
            {
                if(tree[j].turn==part_path[i])
                {
                    temp_turn=&tree[j];
                    while(temp_turn->next && temp_turn->turn!=part_path[i+1])
                    {
                        temp_turn=temp_turn->next;
                    }
                    if(temp_turn->turn==part_path[i+1])
                        temp_turn->weight_cur+=flow_min;
                }
                else/*(tree[j].turn==part_path[i])*/
                {
                    temp_turn=&tree[j];
                    while(temp_turn->next && temp_turn->turn!=-part_path[i+1])
                    {
                        temp_turn=temp_turn->next;
                    }
                    if(temp_turn->turn==-part_path[i+1])
                        temp_turn->weight_cur-=flow_min;
                }
            }
        }
        for(int i=0; i< peaks_i; i++)
        {
            INCTR* temp_grey=&tree[i];
            temp_grey->grey=false;
            while(temp_grey->next)
            {
                temp_grey=temp_grey->next;
                temp_grey->grey=false;
            }
        }
    }
//    int step = 0;
//    int* ranges = new int[peaks_i];
//    bool change = false;
//    ranges[peak]=0;
//    for(int i=0; i<peaks_i; i++)
//    {
//        if(i!=peak)
//            ranges[i]=INT_MAX;
//    }
//    INCTR* temp=new INCTR;
//    for(int i=0; i<peaks_i-1; i++)
//    {
//        for(int j=0; j<peaks_i; j++)
//        {
//            temp=&tree[j];
//            if(ranges[temp->turn-1]!=INT_MAX)
//            {
//                while(temp->next)
//                {
//                    temp=temp->next;
//                    if(ranges[temp->turn-1]==INT_MAX)
//                    {
//                        ranges[temp->turn-1]=temp->weight+ranges[j];
//                        path[step]=tree[j].turn;
//                        path[step+1]=temp->turn;
//                        step+=2;
//                        change=true;
//                    }
//                    else
//                    {
//                        if(ranges[temp->turn-1]>(temp->weight+ranges[j]))
//                        {
//                                ranges[temp->turn-1]=temp->weight+ranges[j];
//                                path[step]=tree[j].turn;
//                                path[step+1]=temp->turn;
//                                step+=2;
//                                change=true;
//                        }
//                    }
//                }
//            }
//        }
//        if(change == false)
//        {
//            i=peaks_i-2;
//            path[step]=0;
//            path[step+1]=0;
//            //step++;
//        }
//        else
//            change=false;
//    }
//    return ranges;
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
    QString temp_w;
    int i=0, j=0, k=0;
    while(str[k]!='\0')
    {
        while(str[k]!=',' && str[k]!='\r' && str[k]!='\0')
        {
            peaks[i].name[j]=str[k];
            peaks[i].turn=i+1;
            k++;
            j++;
            if(str[k]==':')
            {
                k++;
                j=0;
                while(str[k]!=',' && str[k]!='\r' && str[k]!='\0')
                {
                    temp_w[j]=str[k];
                    j++;
                    k++;
                }
                peaks[i].weight=temp_w.toInt();
                temp_w.clear();
            }
        }
        j=0;
        if(str[k]!='\0' && str[k]!='\r')
        {
            k++;
            INCTR* temp=new INCTR;
            peaks[i].next=temp;
            //peaks[i].outdeg++;
            temp->next=NULL;
            //temp->intdeg++;
            temp->prev=&peaks[i];
            while(str[k]!=',' && str[k]!='\r' && str[k]!='\0')
            {
                while(str[k]!=',' && str[k]!='\r' && str[k]!='\0')
                {
                    temp->name[j]=str[k];
                    j++;
                    k++;
                    if(str[k]==':')
                    {
                        k++;
                        j=0;
                        while(str[k]!=',' && str[k]!='\r' && str[k]!='\0')
                        {
                            temp_w[j]=str[k];
                            j++;
                            k++;
                        }
                        temp->weight=temp_w.toInt();
                        temp_w.clear();
                    }
                }
                if(str[k]!='\0' && str[k]!='\r')
                {
                    INCTR* temp2=new INCTR;
                    //peaks[i].outdeg++;
                    temp->next=temp2;
                    temp2->next=NULL;
                    //temp2->intdeg++;
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
                    //peaks[i].intdeg+=temp->intdeg;
                    temp->turn=peaks[i].turn;
                }
                while(temp->next)
                {
                    temp=temp->next;
                    if(peaks[i].name==temp->name)
                    {
                        //peaks[i].intdeg+=temp->intdeg;
                        temp->turn=peaks[i].turn;
                    }
                }
                //delete temp;
            }
        }
        //peaks[i].deg=peaks[i].intdeg+peaks[i].outdeg;
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
    //int* ranges = new int[peaks_i];
    int flow_max=0;
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
        QString peak_s = ui->textEdit_4->toPlainText();
        int chosen_peak = -1;
        for(int i=0; i<peaks_i; i++)
        {
            if(peak_s==tree[i].name)
            {
                chosen_peak = tree[i].turn;
            }
        }
        if(chosen_peak!=-1)
        {
            flow_max=Ford_Fulkeson(tree, path, chosen_peak, /*temp*/6);
//            ui->textEdit_3->clear();
//            ui->textEdit_3->setText("Distance to a peak ["+ tree[chosen_peak-1].name+"] is "+QString::number(ranges[chosen_peak-1]));
//            for(int i=0; i<peaks_i; i++)
//            {
//                if(i==chosen_peak-1)
//                    continue;
//                if(ranges[i]==INT_MAX)
//                    ui->textEdit_3->append("Distance to a peak ["+ tree[i].name+"] is infinity");
//                else
//                    ui->textEdit_3->append("Distance to a peak ["+ tree[i].name+"] is "+QString::number(ranges[i]));
//            }
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
