#ifndef CHECKMESSAGE_H
#define CHECKMESSAGE_H

#include <QString>

class checkmessage
{
public:
    checkmessage();
    QString get_itemvalue(QString message, QString item);
    QString get_receiveuid(QString message);
    QString get_receivestate(QString message);
};

#endif // CHECKMESSAGE_H
