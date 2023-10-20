#ifndef CUSTOMLIST_H
#define CUSTOMLIST_H

#include <QStringList>
#include <QString>

class CustomList : public QStringList
{

public:
    CustomList();

    void write(QString element);

private:
    int _count = 0;
};

#endif // CUSTOMLIST_H
