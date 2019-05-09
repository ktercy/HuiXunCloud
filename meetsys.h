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

    void displayMeetRooms();

public slots:
    void getUsID(int usID);

private:
    Ui::MeetSys *ui;
    int userID;
};

#endif // MEETSYS_H
