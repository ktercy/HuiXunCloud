#include "meetsys.h"
#include "ui_meetsys.h"
#include "lwibomeet.h"
#include <QSqlDatabase> //连接数据库
#include <QSqlQuery>    //执行sql语句
#include <QDebug>
#include <QSqlError>    //sql语句错误
#include <QMessageBox>  //警告窗口
#include <QStringList>  //存储会议室名称
#include <QListWidgetItem>

MeetSys::MeetSys(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MeetSys)
{
    ui->setupUi(this);
    qDebug() << "this is meetsys window!";
    displayMeetRooms();
}

MeetSys::~MeetSys()
{
    delete ui;
}

void MeetSys::displayMeetRooms()
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

    //从数据库中查询所有会议室的名称和地址
    QString sql = QString("select meetroom_id, meetroom_name, meetroom_addr from meet_room");
    QSqlQuery query(db);
    query.exec(sql);
//    QStringList meetNames;

    if (!query.first()){
        QMessageBox::warning(this, tr("warning"), tr("query error!"));
    }else {
        //将所有会议室到名称和地址地址放到QListWidget中去
        for (int i = 0; i < query.size(); i++){
            QListWidgetItem *item = new QListWidgetItem;
            item->setSizeHint(QSize(500,60));
            if(i%2 == 0) {
                item->setBackgroundColor(QColor(240, 240, 240));
            }
            ui->lwBoMeet->addItem(item);
            lwiBoMeet *boMeetItem = new lwiBoMeet;
            boMeetItem->getLabMeetRoomName()->setText(query.value(1).toString().trimmed());
            boMeetItem->getLabMeetRoomAddr()->setText(query.value(2).toString().trimmed());
            ui->lwBoMeet->setItemWidget(item, boMeetItem);
//            meetNames << query.value(1).toString().trimmed();
            query.next();
        }
    }

//    ui->lwBoMeet->addItems(meetNames);  //将会议室名称导入到窗口中的listWidget中去
    //    for(int i = 0; i< meetNames.size();++i)
    //    {
    //        QString tmp = meetNames.at(i);
    //        qDebug()<< i << ". "<< tmp;
    //    }

}

void MeetSys::getUsID(int usID)
{
    userID = usID;
}
