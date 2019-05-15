#include "mybtn.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDateTime>

MyBtn::MyBtn(BtnFunType funType, QWidget *parent)
    : QPushButton(parent), funcType(funType)
{
    connect(this, SIGNAL(clicked()), this, SLOT(clickFunc()));
}

void MyBtn::setMeetRoomID(int mrID)
{
    meetRoomID = mrID;
}

void MyBtn::setMeetID(int mrID)
{
    meetID = mrID;
}

int MyBtn::getMeetRoomID()
{
    return meetRoomID;
}

int MyBtn::getMeetD()
{
    return meetID;
}

void MyBtn::clickFunc()
{
    switch (funcType) {
        case meet_will_held: {
            cancelMeet();
            break;
        }
        case mr_online_check: {
            checkMeet();
            break;
        }
        case mr_online_status: {
            confirmMeet();
            break;
        }
        case normal: {
        //什么都不做
        }
        defualt: {
            qDebug() << "MyBtn error!";
        }
    }
}

void MyBtn::cancelMeet()
{
    //连接用户数据库（本地测试数据库）
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setPort(3306);
    db.setDatabaseName("mei2");
    db.setUserName("tangjun");
    db.setPassword("123456");
    if (!db.open()) {   //打开数据库，如果出错，则弹出警告窗口
        QMessageBox::warning(this, tr("Warning"), tr("Failed to connect database!"), QMessageBox::Yes);
        QSqlDatabase::removeDatabase(db.connectionName());   //移除连接
        return;
    }

    QSqlQuery query(db);
    QString sql = QString("update meet_app set pay = 0, operate = 0 where meet_id = %1").arg(meetID);
    query.exec(sql);

    db.close();
    QSqlDatabase::removeDatabase(db.connectionName());
}

void MyBtn::checkMeet()
{

}

void MyBtn::confirmMeet()
{

}
