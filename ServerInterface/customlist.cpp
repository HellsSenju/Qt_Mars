#include "customlist.h"

CustomList::CustomList()
{
}

void CustomList::write(QString element)
{
    if(element.contains("Server") || element.contains("connected") ||
            element.contains("Interval"))
        return;

    if(_count == 5)
        _count = 0;

    if(_count >= size()){
        insert(_count, element);
        _count++;
        return;
    }

    replace(_count, element);
    _count++;
}
