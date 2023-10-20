#include "group.h"


Group::Group(QString uuid, QString owner, QString name, QString discription)
{
    this->uuid = uuid;
    this->owner = owner;
    this->name = name;
    this->discription = discription;
    fullName = name + "_" + uuid + "_";
    state = false;

    infoTimer = new QTimer();
    connect(infoTimer, &QTimer::timeout, this, &Group::onInfoTimerTimeout);
#ifdef QT_DEBUG
        qDebug() << "Создана группа " << uuid;
#endif
}

void Group::run()
{
    qDebug() << "Запуск " << uuid;
    state = true;
    QMap<QString, Iperf*>::const_iterator i = _iperfs.constBegin();
    while (i != _iperfs.constEnd()) {
        i.value()->start();
        i++;
    }
}

void Group::stop()
{
#ifdef QT_DEBUG
            qDebug() << "Остановка группы:  " << uuid;
#endif
    QMap<QString, Iperf*>::const_iterator i = _iperfs.constBegin();
    while (i != _iperfs.constEnd()) {
        i.value()->stop();
    }
    wait();
}

void Group::del()
{
#ifdef QT_DEBUG
            qDebug() << "Удаление группы:  " << uuid;
#endif
    QMap<QString, Iperf*>::const_iterator i = _iperfs.constBegin();
    while (i != _iperfs.constEnd()) {
        i.value()->deleteLater();
    }
    this->deleteLater();
}

void Group::update(QString name, QString discription)
{
#ifdef QT_DEBUG
    qDebug() << "Новое именя и описание: " << name << ", " << discription;
#endif
    this->name = name;
    this->discription = discription;
    fullName = name + "_" + uuid + "_";
}

QString Group::createIperf(QString server, QString client)
{
    QString uuid        = QUuid::createUuid().toString(QUuid::WithoutBraces);
    CustomList *list    = new CustomList();
    Iperf *iperf        = new Iperf(uuid, server, client);

    connect(iperf, &Iperf::send, this, &Group::onSend);
    connect(iperf, &Iperf::finished, iperf, &Iperf::deleteLater);
    connect(iperf, &Iperf::stopInfoTimer, this, &Group::onStopInfoTimer);

#ifdef QT_DEBUG
    qDebug() << "Cоздан перф: " << uuid;
#endif

    _iperfs.insert(uuid, iperf);
    _infoMap.insert(uuid, list);
    return uuid;
}

bool Group::removeIperf(QString uuid)
{
   QMap<QString, Iperf*>::const_iterator i = _iperfs.constBegin();
    while (i != _iperfs.constEnd()) {
        if(i.value()->uuid == uuid){
            _iperfs.remove(uuid);
            i.value()->deleteLater();

#ifdef QT_DEBUG
    qDebug() << "Yдален перф: " << uuid ;
#endif
            return true;
        }
        i++;
    }
#ifdef QT_DEBUG
    qDebug() << "Перф не был найден: " << uuid ;
#endif
    return false;
}


void Group::startStopIperf(QString uuid)
{
    if(!_iperfs.contains(uuid)){
#ifdef QT_DEBUG
        qDebug() << "Перф не был найден " << uuid;
#endif
        return;
    }

    if(_iperfs[uuid]->getState())
    {
#ifdef QT_DEBUG
        qDebug() << "Остановка перфа " << uuid;
#endif
        _iperfs[uuid]->stop();
        if(countRunningIperfs() == 0)
        {
            state = false;
            wait();
        }
    }
    else
    {
#ifdef QT_DEBUG
        qDebug() << "Запуск перфа " << uuid;
#endif

//        infoTimer->start(10000);
        _iperfs[uuid]->start();

        state = true;
    }
}

QStringList Group::getIperfs()
{
    return _iperfs.keys();
}

int Group::countRunningIperfs()
{
    int count = 0;
    QMap<QString, Iperf*>::const_iterator i = _iperfs.constBegin();
    while (i != _iperfs.constEnd()) {
        if(i.value()->getState())
            count++;
        i++;
    }
    return count;
}

int Group::countIperfs()
{
    return _iperfs.size();
}

void Group::onInfoTimerTimeout()
{

#ifdef QT_DEBUG
    qDebug() << "время отправки инормации!" << uuid;
#endif
    mutex.lock();
    QMap<QString, CustomList*>::const_iterator i = _infoMap.constBegin();
    while (i != _infoMap.constEnd()) {
        qDebug() <<"==================";
        for(int k = 0; k < i.value()->size(); k++){
            qDebug() << i.value()->value(k);
        }
        qDebug() <<"==================";
        i++;
    }

//    emit sendInfo(_infoMap, owner);
    mutex.unlock();
}

void Group::onStopInfoTimer()
{
#ifdef QT_DEBUG
qDebug() << "Остановка таймера на информацию" << uuid;
#endif
    if(infoTimer->isActive())
        infoTimer->stop();
}

void Group::onSend(QString info)
{
    qDebug() << "info " << info;
    Iperf *iperf = qobject_cast<Iperf *>(sender());
    _infoMap[iperf->uuid]->write(info);
}
