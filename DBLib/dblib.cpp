#include "dblib.h"


DBLib::DBLib(QObject *parent) : QObject(parent)
{
}

// функция для соединения с бд
void DBLib::connect(QJsonObject props)
{
    // если нужные параметры не были переданы, то параметры подключения берутся из .ini файла
    if(props.isEmpty()){
        // добавление нового соединения с бд, где первый аргумент - тип драйвера, второй - название соединения
        db = QSqlDatabase::addDatabase("QPSQL", SettingsLib::getField("DB/dbName"));
        // задание host name для соединения (должно быть задано до открытия соединения) - ip
        db.setHostName(SettingsLib::getField("DB/hostName"));
        // название базы данных
        db.setDatabaseName(SettingsLib::getField("DB/dbName"));
        // имя пользователя
        db.setUserName(SettingsLib::getField("DB/userName"));
        //пароль
        db.setPassword(SettingsLib::getField("DB/password"));

    }
    else{
        db = QSqlDatabase::addDatabase("QPSQL", props.value("dbName").toString());
        db.setHostName(props.value("hostName").toString());
        db.setDatabaseName(props.value("dbName").toString());
        db.setUserName(props.value("userName").toString());
        db.setPassword(props.value("password").toString());
    }

    //открытие соединения
    if(db.open()) qDebug() << "data base open";
    else qDebug() << db.lastError().text();
}

//запрос на чтение
QJsonObject DBLib::get(QString dbName, QString SQL, QJsonArray injections)
{
    // экземпляр sql запроса к определенной бд
    QSqlQuery q = QSqlQuery(db.database(dbName));
    QJsonObject obj;
    // подготовка запроса к исполнению
    if(!q.prepare(SQL)) {
        obj.insert("error", q.lastError().text());
        return obj;
    }

    // добавление в запрос аргументов (для большей безопасности)
    for(int i = 0; i < injections.size(); i++)
    {
        QJsonValue value = injections[i];
        if(value.isString())
            q.addBindValue(value.toString());
        if(value.isDouble())
            q.addBindValue(value.toDouble());
    }

    // выполнение запроса
    if(!q.exec()){
        obj.insert("error", q.lastError().text());
        return obj;
    }

    //забираются данные из ответы
    for(int j = 0; q.next(); j++){
        QJsonObject buff;

        for(int i = 0; i < q.record().count(); i++){
            buff.insert(q.record().fieldName(i), q.record().value(i).toJsonValue());
        }
        obj.insert(QString::number(j), buff);
    }

    return obj;
}

//запрос на добавление
bool DBLib::insert(QString dbName, QString tableName, QJsonObject injections)
{
    QSqlQuery q = QSqlQuery(db.database(dbName));
    QString SQL = QString("INSERT INTO %1 (id, name, status) VALUES (?, ?, ?)").arg(tableName);

    if(!q.prepare(SQL)) {
        qDebug() << "prepare fasle";
        qDebug() << q.lastError().text();
        return false;
    }

    for(int i = 0; i < injections.size(); i++)
    {
        QJsonObject object = injections[QString::number(i)].toObject();
        for(QString prop : object.keys()){
            if(object[prop].isString())
                q.addBindValue(object[prop].toString());
            else if(object[prop].isDouble())
                q.addBindValue(object[prop].toDouble());
        }
    }

    if(!q.exec()){
        qDebug() << "exec fasle";
        qDebug() << q.lastError().text();
        return false;
    }
    return true;
}

// запрос на изменение
bool DBLib::update(QString dbName, QString tableName, QJsonObject values, QString filter)
{
    QSqlQuery q = QSqlQuery(db.database(dbName));
    QString SQL = QString("UPDATE %1 SET ").arg(tableName);

    for(QString prop : values.keys()){
        QString add = QString("%1 = ?").arg(prop);
        if(values.keys().indexOf(prop) != values.size()-1) add += ",";
        SQL += add;
    }

    SQL += " ";
    SQL += filter;

    if(!q.prepare(SQL)){
        qDebug() << "prepare fasle";
        qDebug() << q.lastError().text();
        return false;
    }

    for(QString prop : values.keys()){
        if(values[prop].isString())
            q.addBindValue(values[prop].toString());
        else if(values[prop].isDouble())
            q.addBindValue(values[prop].toDouble());
    }

    if(!q.exec()){
        qDebug() << "exec fasle";
        qDebug() << q.lastError().text();
        return false;
    }

    return true;
}
