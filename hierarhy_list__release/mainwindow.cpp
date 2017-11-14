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
void read_lisp(lisp& list, char* buffch);
int read_elem(char prev, lisp& list, base elem, char *buffch, int i);
int read_seq(lisp& list, char *buffch, int i);
int write_lisp(const lisp x, char *arr, int i);
int write_seq(const lisp x, char *arr, int i);

void read_lisp(lisp &list, char *buffch)
{
    int k=0;
    bool fl1=false;
    bool fl2=false;
    int i=0;
    int j=0;
    base elem = new char;
//    while(buffch[i]!='\0')
//    {
    do
    {
        if((buffch[i]!=' ')&&(buffch[i]!='(')&&(buffch[i]!=')'))
        {
            fl1=true;
//            if(fl2==true)
//            {
//                fl2=false;
//                elem[j]=' ';
//                j++;
//            }
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
           (buffch[i]==' ')/*(elem[j-1]!='(')&&(elem[j-1]!=')')*/);
        elem[j]='\0';
        k=read_elem(elem[j-1], list,  elem, buffch, i);
        if(i<k)
            i=k;
        j=0;
        i++;
//    }
} //end read_lisp




int read_elem(char prev, lisp& list, base elem, char* buffch, int i)
{ //prev - ранее прочитанный символ}
    int j=i;
    if (prev == ')')
    {
        //cout << " ! List.Error 1 " << endl;
        //system("Pause");
        //exit(1);
        return i;
    }
    else
    {
        if (prev != '(')
            list = make_atom(elem, 0);
        else
        {
            j=read_seq(list, buffch, j++);
            if(i<j)
                i=j;
        }
    }
    return i;
} //end read_s_expr

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
        //cout << " ! List.Error 2 " << endl;
        //system("Pause");
        //exit(1);
        return i;
    }
    else
    {
//        while ((buffch[i] != ' ')&&(buffch[i]!='\0'))
//        {
//            elem[j]=buffch[i];
//            i++;
//            j++;
//        }
        do
        {
            if((buffch[i]!=' ')&&(buffch[i]!='(')&&(buffch[i]!=')'))
            {
                fl1=true;
    //            if(fl2==true)
    //            {
    //                fl2=false;
    //                elem[j]=' ';
    //                j++;
    //            }
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
               (buffch[i]==' ')/*(elem[j-1]!='(')&&(elem[j-1]!=')')*/);
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
} //end read_seq

lisp make_atom(const base x, int flag)
{
    lisp new_one = NULL;
    int flag2 = 0;
    new_one = new s_expr;
    new_one->tag = true;
//    if (x == x1)
//    {
//        s->node.atom = zam;
//        flag2 = 1;
//        find_zam = true;
//    }
//    else
        new_one->node.atom = x;
    if (flag == 1)
    {
        N++;
        //if (flag2 == 0)
            //cout << "Шаг " << N << ". Был рассмотрен атом " << x << endl;
        //else
            //cout << "Шаг " << N << ". Был рассмотрен атом " << x << " и заменён на " << zam << endl;
    }

    return new_one;
}

void MainWindow::on_actionExit_triggered()
{
    close();
}

void MainWindow::on_Enter_clicked()
{
    lisp list;
    char* arr=new char[1000];
    int i = 0;
    QByteArray buffb = buff.toLatin1(); //че то с памятью
    char* buffch = buffb.data();
    int length=buff.size();
    buffch[length+1]='\0';
    analyser(buff, length);
    read_lisp(list, buffch);
    write_lisp(list, arr, i);
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

int write_lisp(const lisp x, char* arr, int i)
{
    int j=0;
    if (isNull(x)){
        arr[i]=' ';
        arr[i+1]='(';
        arr[i+2]=')';
    }
    else
    {
        if (isAtom(x)){
            while(x->node.atom[j]!='\0'){
            arr[i]=x->node.atom[j];
            i++;
            j++;
            }
            arr[i]=' ';
            i++;
        }
        else
        { //непустой список}
            arr[i]='(';
            i++;
            arr[i]=' ';
            i++;
            i=write_seq(x, arr, i);
            arr[i]=')';
            i++;
            arr[i]=' ';
            i++;
            arr[i]='\0';
        }
    }
    return i;
} // end write_lisp
  //..............................................................................
int write_seq(const lisp x, char* arr, int i)
{//выводит последовательность элементов списка без обрамляющих его скобок
    if (!isNull(x))
    {
        i=write_lisp(head(x), arr, i);
        i=write_seq(tail(x), arr, i);
    }
    return i;
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
