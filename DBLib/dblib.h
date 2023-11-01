#ifndef DBLIB_H
#define DBLIB_H

#include "dblib_global.h"
#include "settingslib.h"

#include <QObject>
#include <QDebug>
#include <QMap>
#include <QJsonObject>
#include <QJsonArray>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>

class DBLIBSHARED_EXPORT DBLib : public QObject
{
    Q_OBJECT

public:
    DBLib(QObject *parent = nullptr);
    static QSqlDatabase db;

    static void connect(QJsonObject props);
    static QJsonObject get(QString dbName, QString SQL, QJsonArray injections);
    static bool insert(QString dbName, QString tableName, QJsonObject injections);
    static bool update(QString dbName, QString tableName, QJsonObject values, QString filter);
};

#endif // DBLIB_H
