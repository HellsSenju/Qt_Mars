#include "settingslib.h"

SettingsLib::SettingsLib(QObject *parent) : QObject(parent)
{
}

QString SettingsLib::getField(QString field)
{
    return SettingsLib::setting.value(field).toString();
}
