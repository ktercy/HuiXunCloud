#ifndef MEETSYS_H
#define MEETSYS_H

#include <QMainWindow>

namespace Ui {
class MeetSys;
}

class MeetSys : public QMainWindow
{
    Q_OBJECT

public:
    explicit MeetSys(QWidget *parent = nullptr);
    ~MeetSys();

    void displayMeetings();    //显示会议室和会议预约信息

public slots:
    void getUsID(int usID);

private:
    Ui::MeetSys *ui;
    int userID;
};

#endif // MEETSYS_H
