#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include "logindlg.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LogInDlg logInDlg;
    logInDlg.show();

    return a.exec();
}
