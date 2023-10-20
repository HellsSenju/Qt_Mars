#include "mainwindow.h"
#include "ui_mainwindow.h"

QSqlDatabase DBLib::db;
QSettings SettingsLib::setting("/work/interfaces/ServerInterface/wsscore.ini", QSettings::IniFormat);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _lib(21098, true, false)
{
    //db
    QJsonObject object;
    DBLib::connect(object);

    QString SQL = "SELECT * FROM users";
    QJsonArray obj;

    QJsonObject fromDataBase = DBLib::get("postgres", SQL, obj);

    for(int i = 0; i < fromDataBase.size(); i++){
        QJsonObject obj = fromDataBase[QString::number(i)].toObject();
        obj.insert("userStatus", "Неактивен");
        _users.insert(obj["name"].toString(), obj);
    }
    //db end

    _lib.server->configureCommonChannels(SettingsLib::getField("CHN/commonChannels").split("_"), false);
    _lib.server->configureUsers(_users);
    _lib.server->start();
    _lib.server->help();

    ui->setupUi(this);

    connect(_lib.server, &Server::message, this, &MainWindow::onMessage);
    connect(_lib.server, &Server::operationCommand, this, &MainWindow::onOperationCommand);
    connect(_lib.server, &Server::deleteCLient, this, &MainWindow::onDeleteClient);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_sendMsgButton_clicked()
{
    _lib.server->sendToAll(ui->msgTextEdit->toPlainText());
}

void MainWindow::onMessage(QString msg)
{
    ui->textBrowser->setText(msg);
//    QString buffer = ui->textBrowser->toHtml() + "\n" + "#### " + msg;
//    ui->textBrowser->setText(buffer);
}

void MainWindow::onOperationCommand(QJsonObject msg)
{
#ifdef QT_DEBUG
    qDebug() << "operation command: " << msg;
#endif
    ui->textBrowser->setText(QJsonDocument(msg).toJson(QJsonDocument::Compact));

    QString from        = msg["from"].toString();
    QString group       = msg["content"].toObject().value("group").toString();

    switch (msg["content"].toObject().value("command").toInt())
    {
    case CreateGroup: // 0
    {
        QString uuid        = QUuid::createUuid().toString(QUuid::WithoutBraces);
        QString groupName   = msg["content"].toObject().value("name").toString();
        QString discription = msg["content"].toObject().value("discription").toString();

        Group* gr           = new Group(uuid, from, groupName, discription);

        connect(gr, &Group::sendInfo, this, &MainWindow::onSendInfo);
        connect(gr, &Group::finished, gr, &Group::deleteLater);

        _groups.insert(gr->fullName, gr);
//        _lib.server->createChannel(from, uuid + groupName, false);

        QJsonObject main;
        QJsonObject obj;

        main.   insert("command",   0);
        main.   insert("to",        from);
        obj.    insert("group",     gr->fullName);
        main.   insert("content",   obj);

        _lib.server->send(main);
    }
        break;

    case DeleteGroup: //1
    {
        if(!_groups.contains(group)){
#ifdef QT_DEBUG
            qDebug() << "Не  нашел группу";
#endif
            return;
        }

        Group *gr = _groups[group];

        if(gr->owner != from){
#ifdef QT_DEBUG
            qDebug() << "Группу пытается удалить не владелец!!!!";
#endif
            return;
        }

        _groups.remove(group);
        _lib.server->deleteChannel(from, gr->uuid + gr->name);

        QJsonObject main;
        QJsonObject obj;

        main.   insert("command",   1);
        main.   insert("to",        from);
        obj.    insert("group",     gr->fullName);
        main.   insert("content",   obj);

        gr->del();
        _lib.server->send(main);
    }
        break;

    case UpdateGroup: //2
    {
        if(!_groups.contains(group)){
#ifdef QT_DEBUG
            qDebug() << "Не  нашел группу";
#endif
            return;
        }

        Group *gr = _groups[group];

        if(gr->owner != from){
#ifdef QT_DEBUG
            qDebug() << "Группу пытается изменить не владелец!!!!";
#endif
            return;
        }

        QString newName         = msg["content"].toObject().value("newName").toString();
        QString newDiscription  = msg["content"].toObject().value("newDiscription").toString();

        //_lib.server->updateChannel(from + gr->uuid + gr->name,
//                from + gr->uuid + newName);
        gr->update(newName, newDiscription);

        QJsonObject main;
        QJsonObject obj;

        main.   insert("command",   2);
        main.   insert("to",        from);
        obj.    insert("group",     gr->fullName);
        main.   insert("content",   obj);

        qDebug() << "main" << main;

        _lib.server->send(main);
    }
        break;

    case ReadGroup: //3
    {
        if(!_groups.contains(group)){
#ifdef QT_DEBUG
            qDebug() << "Не  нашел группу";
#endif
            return;
        }

        Group *gr = _groups[group];

        if(gr->owner != from){
#ifdef QT_DEBUG
            qDebug() << "Группу пытается прочитать не владелец!!!!";
#endif
            return;
        }

        QJsonObject main;
        QJsonObject obj;
        main.   insert("command",               3);
        main.   insert("to",                    from);
        obj.    insert("uuid",                  gr->uuid);
        obj.    insert("name",                  gr->name);
        obj.    insert("discription",           gr->discription);
        obj.    insert("state",                 gr->state);
        obj.    insert("iperfsCount",           gr->countIperfs());
        obj.    insert("iperfsRunningCount",    gr->countRunningIperfs());
        obj.    insert("iperfsNotRunningCount", gr->countIperfs() - gr->countRunningIperfs());
        main.   insert("content",               obj);

        _lib.server->send(main);
    }
        break;

    case StartStopGroup: //4
    {
        if(!_groups.contains(group)){
#ifdef QT_DEBUG
            qDebug() << "Не  нашел группу";
#endif
            return;
        }

        Group *gr = _groups[group];

        if(gr->owner != from){
#ifdef QT_DEBUG
            qDebug() << "Группу пытается запустить/остановить не владелец!!!!";
#endif
            return;
        }

        if(gr->state){
            gr->stop();
            return;
        }

        QTimer *timer = new QTimer();
        QObject::connect(timer, &QTimer::timeout, gr, &Group::onInfoTimerTimeout);

        timer->start(10000);
        gr->start();

    }
        break;

    case CreateIperf: //5
    {
        if(!_groups.contains(group)){
#ifdef QT_DEBUG
            qDebug() << "Не  нашел группу";
#endif
            return;
        }

        Group *gr = _groups[group];

        if(gr->owner != from){
#ifdef QT_DEBUG
            qDebug() << "Создать перф пытается не владелец!!!!";
#endif
            return;
        }

        QJsonObject main;
        QJsonObject obj;
        main.insert("command", 5);
        main.insert("to", from);
        obj.insert("group", gr->fullName);
        obj.insert("iperf", gr->createIperf(
                    msg["content"].toObject().value("server").toString(),
                    msg["content"].toObject().value("client").toString()));
        main.insert("content", obj);
        _lib.server->send(main);
    }
        break;

    case DeleteIperf: //6
    {
        if(!_groups.contains(group)){
#ifdef QT_DEBUG
            qDebug() << "Не  нашел группу";
#endif
            return;
        }

        Group *gr = _groups[group];

        if(gr->owner != from){
#ifdef QT_DEBUG
            qDebug() << "Удалить перф пытается не владелец!!!!";
#endif
            return;
        }

        QString iperf = msg["content"].toObject().value("iperf").toString();

        if(!gr->removeIperf(iperf))
            return;

        QJsonObject main;
        QJsonObject obj;
        main.insert("command", 6);
        main.insert("to", from);
        obj.insert("group", gr->fullName);
        obj.insert("iperf", iperf);
        main.insert("content", obj);
        _lib.server->send(main);
    }
        break;

    case StartStopIperf: //7
    {
        if(!_groups.contains(group)){
#ifdef QT_DEBUG
            qDebug() << "Не  нашел группу";
#endif
            return;
        }

        Group *gr = _groups[group];

        if(gr->owner != from){
#ifdef QT_DEBUG
            qDebug() << "Запустить/остановить перф пытается не владелец!!!!";
#endif
            return;
        }

        QString iperf = msg["content"].toObject().value("iperf").toString();

        gr->startStopIperf(iperf);
    }
        break;

    case SendExisting:
    {
        QJsonObject main;
        QJsonObject content;
        QJsonObject groups;

        main.insert("command", 9);
        main.insert("to", from);

        QMap<QString, Group*>::const_iterator i = _groups.constBegin();
        while (i != _groups.constEnd()) {
            if(i.value()->owner == from){
                groups.insert(i.key(),
                              QJsonArray::fromStringList(i.value()->getIperfs()));
            }
            i++;
        }

        content.insert("groups", groups);
        main.insert("content", content);
        _lib.server->send(main);
    }
        break;

    default:
        qDebug() << "default";
        break;
    }
}

void MainWindow::onSendInfo(QMap<QString,CustomList*> info, QString to)
{
    qDebug() << "to:  " << to;
}

void MainWindow::onDeleteClient(QString uuid)
{
    QMap<QString, Group*>::const_iterator i = _groups.constBegin();
    while (i != _groups.constEnd())
    {
        if(i.value()->owner == uuid){
            i.value()->del();
            _groups.remove(i.key());
        }
        i++;
    }
}

QJsonObject MainWindow::parse(QString data)
{
    QJsonObject obj;
    QByteArray br = data.toUtf8();
    QJsonDocument doc = QJsonDocument::fromJson(br);
    if(!doc.isNull()){
        if(doc.isObject())
            obj = doc.object();
        else{
            #ifdef QT_DEBUG
            qDebug() << "--Doc is not an object";
            #endif
            return obj;
        }
    }
    else {
        #ifdef QT_DEBUG

        qDebug() << "--Invallid JSON";
        #endif
        return obj;
    }
    return obj;
}
