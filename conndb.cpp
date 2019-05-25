#include "conndb.h"
#include <QMessageBox>

QSqlDatabase ConnDB::db;

bool ConnDB::initDB()
{
    db = QSqlDatabase::addDatabase("QMYSQL");

//    db.setHostName("localhost");
//    db.setPort(3306);
//    db.setDatabaseName("mei2");
//    db.setUserName("tangjun");
//    db.setPassword("123456");

    db.setHostName("123.57.243.85");
    db.setPort(3307);
    db.setUserName("tangjun");
    db.setPassword("123456");
    db.setDatabaseName("testTangJun");

    if (!db.open()) {
        QMessageBox::warning(nullptr, QObject::tr("Warning"), QObject::tr("Failed to connect database!"), QMessageBox::Cancel);
        return false;
    }

    return true;
}
