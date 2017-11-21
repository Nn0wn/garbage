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


typedef char* base;
struct s_expr;

struct  two_ptr
{
    s_expr *hd;
    s_expr *tl;
};
struct s_expr
{
    bool tag;
    union
    {
        base atom;
        two_ptr pair;
    } node;
};

typedef s_expr *lisp;

base x1, zam;
bool find_zam = false;
int N = 0;
lisp hier_list;

int analyser(QString buffer, int length);
lisp head(const lisp s);
lisp tail(const lisp s);
lisp cons(const lisp h, const lisp t);
lisp make_atom(const base x);
bool isAtom(const lisp s);
bool isNull(const lisp s);
void destroy(lisp s);
base getAtom(const lisp s);
void read_lisp(lisp& list, char* buffch);
int read_elem(char prev, lisp& list, base elem, char *buffch, int i);
int read_seq(lisp& list, char *buffch, int i);
int write_lisp(const lisp x, char *arr, int i);
int write_seq(const lisp x, char *arr, int i);
int change_atoms(const lisp x, const char* first_line, const char *second_line, int i);
int change_atoms_next(const lisp x, const char *first_line, const char *second_line, int i);

void read_lisp(lisp &list, char *buffch)
{
    int k=0;
    bool fl1=false;
    bool fl2=false;
    int i=0;
    int j=0;
    base elem = new char;
    do
    {
        if((buffch[i]!=' ')&&(buffch[i]!='(')&&(buffch[i]!=')'))
        {
            fl1=true;
            elem[j]=buffch[i];
            i++;
            j++;
        }
        else if((buffch[i]=='(')||(buffch[i]==')'))
        {
            fl1=false;
            fl2=true;
            elem[j]=buffch[i];
            i++;
            j++;
        }
        else
        {
            if((fl1==true)||(fl2==true))
            {
                fl2=false;
                fl1=false;
                i++;
                break;
            }
            else
                i++;
        }
    }while(((buffch[i]!='(')&&(buffch[i]!='\0')&&(buffch[i]!=')')&&(buffch[i-1]!='(')&&(buffch[i-1]!=')'))||
           (buffch[i]==' '));
        elem[j]='\0';
        k=read_elem(elem[j-1], list,  elem, buffch, i);
        if(i<k)
            i=k;
        j=0;
        i++;
}




int read_elem(char prev, lisp& list, base elem, char* buffch, int i)
{
    int j=i;
    if (prev == ')')
    {
        return i;
    }
    else
    {
        if (prev != '(')
            list = make_atom(elem);
        else
        {
            j=read_seq(list, buffch, j++);
            if(i<j)
                i=j;
        }
    }
    return i;
}

int read_seq(lisp& list, char *buffch, int i)
{
    bool fl1=false;
    bool fl2=false;
    int p=0;
    int k=0;
    int j=0;
    base elem = new char;
    lisp part1;
    lisp part2;
    if (buffch[i]=='\0')
    {
        return i;
    }
    else
    {
        do
        {
            if((buffch[i]!=' ')&&(buffch[i]!='(')&&(buffch[i]!=')'))
            {
                fl1=true;
                elem[j]=buffch[i];
                i++;
                j++;
            }
            else if((buffch[i]=='(')||(buffch[i]==')'))
            {
                fl1=false;
                fl2=true;
                elem[j]=buffch[i];
                i++;
                j++;
            }
            else
            {
                if((fl1==true)||(fl2==true))
                {
                    fl2=false;
                    fl1=false;
                    i++;
                    break;
                }
                else
                    i++;
            }
        }while(((buffch[i]!='(')&&(buffch[i]!='\0')&&(buffch[i]!=')')&&(buffch[i-1]!='(')&&(buffch[i-1]!=')'))||
               (buffch[i]==' '));
        elem[j]='\0';
        if (elem[j-1] == ')'){
            list = NULL;
            return i;
        }
        else
        {
            p=read_elem(elem[j-1], part1, elem, buffch, i);
            if(i<p)
                i=p;
            k=read_seq(part2, buffch, i);
            if(i<k)
                i=k;
            list = cons(part1, part2);
        }
    }
    return i;
}

lisp make_atom(const base x)
{
    lisp new_one = NULL;
    new_one = new s_expr;
    new_one->tag = true;
        new_one->node.atom = x;
    return new_one;
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_Enter_clicked()
{
    char* arr=new char[1000];
    int i = 0;
    QByteArray buffb = buff.toLatin1();
    char* buffch = buffb.data();
    int length=buff.size();
    analyser(buff, length);
    read_lisp(hier_list, buffch);
    write_lisp(hier_list, arr, i);
    ui->textEdit->setText(arr);
}


int analyser(QString buffer, int length)
{
    QChar open_bracket;
    int elem_counter=0;
    int* temp=new int[10];
    int lvl_counter=1;
    if(buffer[0]!='(')
        return 1;
    int i=1;
    temp[0]=0;
    for(i=i; i<length; i++)
    {
        open_bracket=buffer[i];
        if((open_bracket!='(')&&(open_bracket!=')'))
            elem_counter++;
        if(open_bracket=='(')
        {
            temp[lvl_counter]=elem_counter;
            lvl_counter++;
            elem_counter=0;
        }
        if(open_bracket==')')
        {
            lvl_counter--;
            elem_counter=temp[lvl_counter];
        }
        if(elem_counter>2)
            return 1;
    }
    if(lvl_counter!=0)
        return 1;
    return 0;
}

bool isAtom(const lisp s)
{
    if (s == NULL)
        return false;
    else
        return (s->tag);
}


lisp cons(const lisp h, const lisp t)
{
    lisp p = NULL;;
    if (isAtom(t))
    {
        exit(1);
    }
    else
    {
        p = new s_expr;
        if (p == NULL)
        {
            exit(1);
        }
        else
        {
            p->tag = false;
            p->node.pair.hd = h;
            p->node.pair.tl = t;
            return p;
        }
    }
}

lisp head(const lisp s)
{
    if (s != NULL)
    {
        if (!isAtom(s))
            return s->node.pair.hd;
        else
        {
            exit(1);
        }
    }
    else
    {
        exit(1);
    }
}

bool isNull(const lisp s)
{
    return s == NULL;
}

lisp tail(const lisp s)
{
    if (s != NULL)
    {
        if (!isAtom(s))
            return s->node.pair.tl;
        else
        {
            exit(1);
        }
    }
    else
    {
        exit(1);
    }
}

void destroy(lisp s)
{
    if (s != NULL)
    {
        if (!isAtom(s))
        {
            destroy(head(s));
            destroy(tail(s));
        }
        delete s;
        s = NULL;
    }
}


int write_lisp(const lisp x, char* arr, int i)
{
    int j=0;
    if (isNull(x)){
        arr[i]='(';
        arr[i+1]=' ';
        arr[i+2]=')';
        arr[i+3]='\0';
    }
    else
    {
        if (isAtom(x)){
            while(x->node.atom[j]!='\0')
            {
                arr[i]=x->node.atom[j];
                i++;
                j++;
            }
            arr[i]=' ';
            i++;

        }
        else
        {
            if((i-1>0)&&(arr[i-1]==' '))
                i--;
            arr[i]='(';
            i++;
            i=write_seq(x, arr, i);
            if((i-1>0)&&(arr[i-1]==' '))
                i--;
            arr[i]=')';
            i++;
            arr[i]='\0';
        }
    }
    return i;
}

int write_seq(const lisp x, char* arr, int i)
{
    if (!isNull(x))
    {
        i=write_lisp(head(x), arr, i);
        i=write_seq(tail(x), arr, i);
    }
    return i;
}

int change_atoms(const lisp x, const char *first_line, const char* second_line, int i)
{
    int j=0;
    if (isNull(x)){
        return 1;
    }
    else
    {
        if (isAtom(x)){
            if(strcmp(x->node.atom, first_line)==0)
            {
                x->node.atom=new char[sizeof(second_line)+1];
                while(second_line[j]!='\0')
                {
                    x->node.atom[j]=second_line[j];
                    j++;
                    i++;
                }
                x->node.atom[j]='\0';
            }
        }
        else
        {
            i=change_atoms_next(x, first_line, second_line, i);
        }
    }
    return i;
}

int change_atoms_next(const lisp x, const char* first_line, const char* second_line, int i)
{
    if (!isNull(x))
    {
        i=change_atoms(head(x), first_line, second_line, i);
        i=change_atoms_next(tail(x), first_line, second_line, i);
    }
    return i;
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

void MainWindow::on_pushButton_clicked()
{
    ui->textEdit->clear();
    ui->textEdit_3->clear();
    ui->textEdit_4->clear();
}
void MainWindow::on_pushButton_2_clicked()
{
    char* arr=new char[1000];
    int i=0;
    QString chaged_line=ui->textEdit_3->toPlainText();
    QString new_line=ui->textEdit_4->toPlainText();
    QByteArray b_changed_line=chaged_line.toLatin1();
    QByteArray b_new_line=new_line.toLatin1();
    change_atoms(hier_list, b_changed_line, b_new_line, i);
    write_lisp(hier_list, arr, i);
    ui->textEdit->setText(arr);
}
