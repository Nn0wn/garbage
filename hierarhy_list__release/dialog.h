#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QFileSystemModel>
#include <QDir>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

signals:
    void FilePath(const QString &str);

private slots:
    void on_ok_clicked();

    void on_cancel_clicked();

    void on_treeView_doubleClicked(const QModelIndex &index);

private:
    Ui::Dialog *ui;
    QFileSystemModel *model;
};



#endif // DIALOG_H
