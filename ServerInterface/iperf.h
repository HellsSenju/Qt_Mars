#ifndef IPERF_H
#define IPERF_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QUuid>
#include <QThread>
#include <QTimer>
#include <QProcess>
#include <QWebSocket>
#include <QJsonObject>
#include <QJsonDocument>

class Iperf : public QThread
{
    Q_OBJECT
public:
    explicit Iperf(QString uuid, QString ip_server, QString ip_client);

    QString uuid;

    void stop();
    void openSockets();

    volatile bool getState() const;

private:
    QWebSocket *_client;
    QWebSocket *_server;
    QProcess* pServer;
    QProcess* pClient;
    QString _ip_server;
    QString _ip_client;

    volatile bool state;

    void run();
    void startTimer();
    void setAdresses(QString _ip_server, QString _ip_client);
    void onError(QProcess::ProcessError error);

signals:
    void send(QString info);
    void stopInfoTimer();

public slots:
};

#endif // IPERF_H
