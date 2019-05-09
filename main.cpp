#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include "logindlg.h"
//#include "chosysdlg.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
//    QApplication a(argc, argv);
//    LogInDlg logInDlg;
//    ChoSysDlg choSysDlg;

//    if (logInDlg.exec() == QDialog::Accepted)
//    {
//        if (choSysDlg.exec() == QDialog::Accepted){
//            MainWindow classSys;
//            classSys.show();
//            return a.exec();
//        }
//    }
//    else return 0;


    QApplication a(argc, argv);
    qDebug() << "show login dialog";
    LogInDlg logInDlg;
    logInDlg.show();
    return a.exec();
}
