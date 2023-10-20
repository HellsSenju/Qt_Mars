#include "iperf.h"

Iperf::Iperf(QString uuid, QString ip_server, QString ip_client)
{
    this->uuid = uuid;
    _ip_server = ip_server;
    _ip_client = ip_client;
    state = false;
}

//void Iperf::run()
//{
//    qDebug() << "Запуск перфа " << uuid;

//    _server = new QWebSocket();
//    _server->open(QString("ws://%1:30005").arg(_ip_server));

//    connect(_server, &QWebSocket::textMessageReceived,
//            [=](QString info){ emit send(info); });

//    connect(_server, &QWebSocket::disconnected,
//            [=](){ _server->deleteLater(); });

////    _client = new QWebSocket();
////    connect(_client, &QWebSocket::textMessageReceived,
////            [=](QString info){ emit send(info); });

////    _client->open(QString("ws://%1:30005").arg(_ip_client));

//    sleep(5000);
//    QJsonObject serverMsg;
//    serverMsg.insert("command", "iperf -s -u -i 1 -p 30006");

//    _server->sendTextMessage(QJsonDocument(serverMsg).toJson(QJsonDocument::Compact));

////    sleep(5000);
////    QJsonObject clientMsg;
////    clientMsg.insert("command", QString("iperf -u -c %1 -p 30006 -t 1 -b 1000")
////                     .arg(_ip_server));

////    _client->sendTextMessage(QJsonDocument(clientMsg).toJson(QJsonDocument::Compact));

////    exec();
//}

void Iperf::openSockets()
{
    qDebug() << "openSockets";
    _server = new QWebSocket();
    _server->open(QString("ws://%1:30005").arg(_ip_server));
    state = false;
}

volatile bool Iperf::getState() const
{
    return state;
}

void Iperf::run()
{
    qDebug() << "Запуск " << uuid;
    pServer = new QProcess;
    pClient = new QProcess;

    pServer->start(QString("ssh user@%1 iperf -s -u -i 1 -p 30001")
                   .arg(_ip_server));

    if(pServer->waitForStarted()){
        qDebug() << "Процесс сервера запущен ";
        state = true;

//        if(pClient->startDetached(QString("ssh user@%1 iperf -u -c %2 -p 30001 -t 10000 -b 1000")
//                                  .arg(client).arg(server))){
//            qDebug() << "Процесс клиента запущен ";
//        }
//        else{
//            qDebug() << "Процесс клиента не запустился ";
//        }


        pClient->start(QString("ssh user@%1 iperf -u -c %2 -p 30001 -t 10000 -b 1000")
                                          .arg(_ip_client).arg(_ip_server));

        if(pClient->waitForStarted()){
            qDebug() << "Процесс клиента запущен ";
            pClient->closeReadChannel(QProcess::StandardOutput);
        }else{
            qDebug() << "Процесс клиента не запустился ";
        }

        while(state) {
            bool isReceived = false;

            if(pServer->waitForReadyRead()) {
//                emit send(pServer->readAll());
                isReceived = true;
            }
            else if(!isReceived) {
                if(pServer->state() == QProcess::NotRunning) {
                    state = false;
                    qDebug() << "Процесс серввера в состоянии not running";
                }
                else {
                    qDebug() << "ERROR";
                }
            }
        }
    }
    else{
        qDebug() << "Процесс сервера не запустился ";
    }

    if(pServer && pClient){

        pServer->kill();
        qDebug() << pServer->state();
        pServer->waitForFinished();
        qDebug() << pServer->state();
        pServer = 0;

        pClient->kill();
        qDebug() << pClient->state();
        pClient->waitForFinished();
        qDebug() << pClient->state();
        pClient = 0;

        qDebug() << "Процессы полностью остановлены";
        state = false;
    }
}


void Iperf::stop()
{
    qDebug() << "Остановка процессов";
    emit stopInfoTimer();
    state = false;
}

void Iperf::onError(QProcess::ProcessError error)
{
    qDebug() << error;
}
