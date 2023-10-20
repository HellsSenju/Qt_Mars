#include "dblib.h"


DBLib::DBLib(QObject *parent) : QObject(parent)
{
}

void DBLib::connect(QJsonObject props)
{
    if(props.isEmpty()){
        db = QSqlDatabase::addDatabase("QPSQL", SettingsLib::getField("DB/dbName"));
        db.setHostName(SettingsLib::getField("DB/hostName"));
        db.setDatabaseName(SettingsLib::getField("DB/dbName"));
        db.setUserName(SettingsLib::getField("DB/userName"));
        db.setPassword(SettingsLib::getField("DB/password"));

    }
    else{
        db = QSqlDatabase::addDatabase("QPSQL", props.value("dbName").toString());
        db.setHostName(props.value("hostName").toString());
        db.setDatabaseName(props.value("dbName").toString());
        db.setUserName(props.value("userName").toString());
        db.setPassword(props.value("password").toString());
    }

    if(db.open()) qDebug() << "data base open";
    else qDebug() << db.lastError().text();
}

QJsonObject DBLib::get(QString dbName, QString SQL, QJsonArray injections)
{
    QSqlQuery q = QSqlQuery(db.database(dbName));
    QJsonObject obj;
    if(!q.prepare(SQL)) {
        obj.insert("error", q.lastError().text());
        return obj;
    }

    for(int i = 0; i < injections.size(); i++)
    {
        QJsonValue value = injections[i];
        if(value.isString())
            q.addBindValue(value.toString());
        if(value.isDouble())
            q.addBindValue(value.toDouble());
    }

    if(!q.exec()){
        obj.insert("error", q.lastError().text());
        return obj;
    }

    for(int j = 0; q.next(); j++){
        QJsonObject buff;

        for(int i = 0; i < q.record().count(); i++){
            buff.insert(q.record().fieldName(i), q.record().value(i).toJsonValue());
        }
        obj.insert(QString::number(j), buff);
    }

    return obj;
}

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
