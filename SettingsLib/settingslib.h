#ifndef SETTINGSLIB_H
#define SETTINGSLIB_H

#include "settingslib_global.h"

#include <QSettings>
#include <QDebug>

class SETTINGSLIBSHARED_EXPORT SettingsLib : public QObject
{
    Q_OBJECT

public:
    SettingsLib(QObject *parent = nullptr);

    static QSettings setting;
    static QString getField(QString field);
};

#endif // SETTINGSLIB_H
