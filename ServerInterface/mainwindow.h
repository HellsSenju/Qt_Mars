#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "wssc_as_library.h"
#include "settingslib.h"
#include "dblib.h"
#include "group.h"
#include "customlist.h"

#include <QMainWindow>
#include <QMap>
#include <QtCore>
#include <QtGui>
#include <QTreeWidgetItem>
#include <QAbstractSocket>
#include <QJsonArray>
#include <QUuid>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private:
    Ui::MainWindow *ui;
    WSSC_as_Library _lib;
    QMap<QString, QJsonObject> _users; // <login, info>

//    QList<Group*> _groups;
    QMap<QString,Group*> _groups; // <fullName(name_uuid_), group>

    enum Commands{
        CreateGroup,
        DeleteGroup,
        UpdateGroup,
        ReadGroup,
        StartStopGroup,
        CreateIperf,
        DeleteIperf,
        StartStopIperf,
        SendExisting
    };

    Group *findGroup(QString owner, QString uuid);
    void sendGroups(QString to);
    QJsonObject parse(QString data);

    void addRoot(QString groupUuid);
    void addChild(QTreeWidgetItem *parentGroup, QString iPerfUuid);

private slots:

    void onMessage(QString msg);
    void onOperationCommand(QJsonObject msg);
    void onSendInfo(QMap<QString, CustomList *> info, QString to);
    void onDeleteClient(QString uuid);
    void on_sendMsgButton_clicked();
};

#endif // MAINWINDOW_H
