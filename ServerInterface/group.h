#ifndef GROUP_H
#define GROUP_H

#include "customlist.h"
#include "iperf.h"
#include <QObject>
#include <QDebug>
#include <QThread>
#include <QMap>
#include <QList>
#include <QUuid>
#include <QPair>
#include <QTimer>
#include <QMutex>

class Group : public QThread
{
    Q_OBJECT
public:
    explicit Group(QString uuid, QString owner, QString name, QString discription);

    QString uuid, name, discription, owner, fullName;

    bool state;

    void run();
    void stop();
    void del();
    void update(QString name, QString discription);

    QString createIperf(QString server, QString client);
    bool removeIperf(QString name);
    void updateIperf(QString uuid, QString server, QString client);
    void startStopIperf(QString uuid);
    QStringList getIperfs();
    int countRunningIperfs();
    int countIperfs();

private:
    QMap<QString, Iperf*> _iperfs; //iperf's uuid, iperf
    QMap<QString,CustomList*> _infoMap;

    QTimer *infoTimer;
    QMutex mutex;

signals:
    void sendInfo(QMap<QString,CustomList*> info, QString to);

public slots:
    void onInfoTimerTimeout();
    void onStopInfoTimer();
    void onSend(QString info);
};

#endif // GROUP_H
