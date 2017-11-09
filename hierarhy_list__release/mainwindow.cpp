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




//int* graph(int k, int* roads1, int* roads2, int* result, int j, int y, int towns);
int analyser(QString buffer, int length);

//int roads(int n);

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


typedef char* base;      // базовый тип элементов (атомов)
struct s_expr;

struct  two_ptr
{
    s_expr *hd;
    s_expr *tl;
};
struct s_expr
{
    bool tag; // true: atom, false: pair
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



//void print_s_expr( node* s );

lisp head(const lisp s);
lisp tail(const lisp s);
lisp cons(const lisp h, const lisp t);
lisp make_atom(const base x, int flag);
bool isAtom(const lisp s);
bool isNull(const lisp s);
void destroy(lisp s);
base getAtom(const lisp s);
void read_lisp(lisp& y, char* buffch);
void read_elem(char prev, lisp& y, base elem, char *buffch, int i);
void read_seq(lisp& y, char *buffch, int i);
void write_lisp(const lisp x, char *arr, int i);
void write_seq(const lisp x, char *arr, int i);

void read_lisp(lisp &y, char *buffch)
{
    int i=0;
    base elem = new char;
    //while(buffch[i]!=' ')
    do
    {
        elem[i]=buffch[i];
        i++;
    }while((buffch[i]==' ')&&(buffch[i-1]!='\0'));
    read_elem(elem[i-1], y,  elem, buffch, i-1);
} //end read_lisp

lisp head(const lisp s)
{// PreCondition: not null (s)
    if (s != NULL)
    {
        if (!isAtom(s))
            return s->node.pair.hd;
        else
        {
            //cout << "Error: Head(atom) \n";
            //system("Pause");
            exit(1);
        }
    }
    else
    {
        //cout << "Error: Head(nil) \n";
        //system("Pause");
        exit(1);
    }
}

bool isNull(const lisp s)
{
    return s == NULL;
}

lisp tail(const lisp s)
{// PreCondition: not null (s)
    if (s != NULL)
    {
        if (!isAtom(s))
            return s->node.pair.tl;
        else
        {
            cout << "Error: Tail(atom) \n";
            system("Pause");
            exit(1);
        }
    }
    else
    {
        cout << "Error: Tail(nil) \n";
        system("Pause");
        exit(1);
    }
}


void read_elem(char prev, lisp& y, base elem, char* buffch, int i)
{ //prev - ранее прочитанный символ}
    if (prev == ')')
    {
        //cout << " ! List.Error 1 " << endl;
        //system("Pause");
        //exit(1);
        return;
    }
    else
    {
        if (prev != '(')
            y = make_atom(elem, 0);
        else
            read_seq(y, buffch, i);
    }
} //end read_s_expr

void read_seq(lisp& y, char *buffch, int i)
{
    base elem = new char;
    lisp part1;
    lisp part2;
    if (buffch[i]=='\0')
    {
        //cout << " ! List.Error 2 " << endl;
        //system("Pause");
        //exit(1);
        return;
    }
    else
    {
        while (buffch[i] != ' ')
        {
            elem[i]=buffch[i];
            i++;
        }
        if (elem[i] == ')')
            y = NULL;
        else
        {
            read_elem(elem[i-1], part1, elem, buffch, i-1);
            read_seq(part2, buffch, i);
            y = cons(part1, part2);
        }
    }
} //end read_seq

lisp make_atom(const base x, int flag)
{
    lisp s = NULL;
    int flag2 = 0;
    s = new s_expr;
    s->tag = true;
//    if (x == x1)
//    {
//        s->node.atom = zam;
//        flag2 = 1;
//        find_zam = true;
//    }
//    else
        s->node.atom = x;
    if (flag == 1)
    {
        N++;
        //if (flag2 == 0)
            //cout << "Шаг " << N << ". Был рассмотрен атом " << x << endl;
        //else
            //cout << "Шаг " << N << ". Был рассмотрен атом " << x << " и заменён на " << zam << endl;
    }

    return s;
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_Enter_clicked()
{
    lisp array1;
    char* arr=new char;
    int i = 0;
    QByteArray buffb = buff.toLatin1();
    char* buffch = buffb.data();
    int length=buff.size();
    analyser(buff, length);
    read_lisp(array1, buffch);
    write_lisp(array1, arr, i);
    ui->textEdit->setText(arr);

   //QList<QString> list= buff.split(" ", QString::SkipEmptyParts);
    //int size = list.size();

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
// PreCondition: not isAtom (t)
{
    lisp p = NULL;;
    if (isAtom(t))
    {
        exit(1);
        //cout << "Error: Tail(nil) \n";
        //system("Pause");
        //exit(1);
    }
    else
    {
        p = new s_expr;
        if (p == NULL)
        {
            exit(1);
            //cout << "Memory not enough\n";
            //system("Pause");
            //exit(1);
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

void write_lisp(const lisp x, char* arr, int i)
{//пустой список выводится как ()
    if (isNull(x)){
        arr[i]=' ';
        arr[i+1]='(';
        arr[i+2]=')';
    }
    else
    {
        if (isAtom(x)){
            while(x->node.atom[i]!='\0'){
            arr[i]=x->node.atom[i];
            i++;
            }
        }
        else
        { //непустой список}
            arr[i]=' ';
            i++;
            arr[i]='(';
            i++;
            write_seq(x, arr, i);
            arr[i]=' ';
            i++;
            arr[i]=')';
        }
    }
} // end write_lisp
  //..............................................................................
void write_seq(const lisp x, char* arr, int i)
{//выводит последовательность элементов списка без обрамляющих его скобок
    if (!isNull(x))
    {
        write_lisp(head(x), arr, i);
        write_seq(tail(x), arr, i);
    }
}

//int* graph(int k, int* roads1, int* roads2, int* result, int j, int y, int towns)
//{
//    int t=0;
//    for(int i=0; i<towns; i++)
//    {
//        if((roads1[i]==k)&&(y!=roads2[i]))
//        {
//            y=roads1[i];
//            t=roads2[i];
//            while((result[j]!=0)&&(result[j]<towns))
//                j++;
//            result[j]=t;
//            j++;
//            graph(t, roads1, roads2, result, j, y, towns);
//        }
//        else if((roads2[i]==k)&&(y!=roads1[i]))
//        {
//            y=roads2[i];
//            t=roads1[i];
//            while((result[j]!=0)&&(result[j]<towns))
//                j++;
//            result[j]=t;
//            j++;
//            graph(t, roads1, roads2, result, j, y, towns);
//        }
//    }
//    return result;
//}

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
    int p=input.size();
    if(p==0)
    {
        input.close();
        return buff;
    }
    buff=stream.readAll();
    input.close();
    flag=1;
    return buff;
}
