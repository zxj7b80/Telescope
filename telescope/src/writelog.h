#ifndef WRITELOG_H
#define WRITELOG_H

#include <QString>
#include <QFile>


class writelog
{
public:
    writelog();
    ~writelog();
    void writeregular(QString action,QString message);
    void writeerror(QString action,QString message);
    void createlogfile();

private:
    void checkregularfilename();
    void checkerrorfilename();
    
};

#endif // WRITELOG_H
