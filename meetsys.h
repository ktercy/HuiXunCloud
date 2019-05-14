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

private slots:
    void on_btnBusiLogIn_clicked();

    void on_btnBusiRegi_clicked();

private:
    Ui::MeetSys *ui;
    int userID; //普通用户ID
    int busiUserID; //商家用户ID
};

#endif // MEETSYS_H
