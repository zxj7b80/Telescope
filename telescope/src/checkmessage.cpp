#include "checkmessage.h"
#include "QtGui"

checkmessage::checkmessage()
{
}

QString checkmessage::get_itemvalue(QString message, QString item)
{
    int i_bindex,i_eindex;

    item = "<"+ item + "=";
    i_bindex = message.indexOf(item);
    if (i_bindex== -1) return NULL; //can't find item

    i_bindex = i_bindex + item.size() ;
    i_eindex = message.indexOf(">",i_bindex);
    if(i_eindex==-1) return NULL; //can't find item

    return message.mid(i_bindex,i_eindex - i_bindex );
}

QString checkmessage::get_receiveuid(QString message)
{
    int i_bindex,i_eindex,i;
    QString item = "<RelatedCmdCUID=";
    i_bindex = message.indexOf(item);
    if (i_bindex== -1) return NULL; //can't find item

    i_bindex = i_bindex + item.size() ;
    i_eindex = i_bindex;
    for(i=0;i<5;i++)
    {
        i_eindex = message.indexOf(".",i_eindex) + 1;
    }
    return message.mid(i_bindex,i_eindex - i_bindex - 1);
}

QString checkmessage::get_receivestate(QString message)
{
    QString item = "bExeStatus";
    QString item1 = "ExeStatus";
    if(get_itemvalue(message,item) == "true")//start or done
    {
        return get_itemvalue(message,item1);
    }
    else
    {
        if(get_itemvalue(message,"RV")=="0")
        {
            return "INFO";
        }
        else
        {
            return "ERROR";
        }
    }
}
