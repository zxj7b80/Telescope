#include "writelog.h"
#include <QtGui>
#include "signs.h"

QString  Gregular_log_file_path ;
QString  Gerror_log_file_path ;
QFile   *GRegularLogFile;
QFile   *GErrorLogFile;
QString  Gregular_log_name;
QString  Gerror_log_name;
QString  GUser;


writelog::writelog()
{
    Gregular_log_file_path = SMySetting::instance_p()->value( "misc/LogDirectory" ).toString();
    Gerror_log_file_path = SMySetting::instance_p()->value ( "misc/LogDirectory" ).toString();
    GUser = "Mount";
}

void writelog::writeregular(QString action,QString message)
{
    checkregularfilename();
    QString writeLine;
    QString s_datetime = QDateTime::currentDateTime().toString("yyyyMMdd.hh:mm:ss.zzz");

    writeLine = QString("%1%2%3%4").arg(GUser,-10)
            .arg(action,-20)
            .arg(s_datetime,-15)
            .arg(message,-100);
    QTextStream out(GRegularLogFile);
    out << writeLine << "\n" ;
}

void writelog::writeerror(QString action,QString message)
{
    checkerrorfilename();
    QString writeLine;
    QString s_datetime = QDateTime::currentDateTime().toString("yyyyMMdd.hh:mm:ss.zzz");

    writeLine = QString("%1%2%3%4").arg(GUser,-10)
            .arg(action,-20)
            .arg(s_datetime,-25)
            .arg(message,-100);
    QTextStream out(GErrorLogFile);
    out << writeLine << "\n";
}

void writelog::checkregularfilename()
{
    QString s_date = QDateTime::currentDateTime().toString("yyyyMMdd");
    if(Gregular_log_name.contains(s_date)) return;
    GRegularLogFile->close();
    delete GRegularLogFile;

    Gregular_log_name = Gregular_log_file_path + "/" + s_date + "regular.log";
    GRegularLogFile = new QFile(Gregular_log_name);

    if(!GRegularLogFile->open(QIODevice::Append))
    {
        QMessageBox::critical(0, "open file error",
                              "Can't open regular log file.\n"
                              "Click Cancel to exit.", QMessageBox::Cancel);
        
        return;
    }
}

void writelog::checkerrorfilename()
{
    QString s_date = QDateTime::currentDateTime().toString("yyyyMMdd");
    if(Gerror_log_name.contains(s_date)) return;
    GErrorLogFile->close();
    delete GErrorLogFile;

    Gerror_log_name = Gerror_log_file_path + "/" + s_date + "error.log";
    GErrorLogFile = new QFile(Gerror_log_name);

    if(GErrorLogFile->open(QIODevice::Append))
    {
        QMessageBox::critical(0, "open file error",
                              "Can't open error log file.\n"
                              "Click Cancel to exit.", QMessageBox::Cancel);
        return;
    }
}

writelog::~writelog()
{
    
}

void writelog::createlogfile()
{
    QString s_date = QDateTime::currentDateTime().toString("yyyyMMdd");

    Gregular_log_name = Gregular_log_file_path + "/" + s_date + "regular.log";

    Gerror_log_name = Gerror_log_file_path + "/" + s_date + "error.log";

    GRegularLogFile = new QFile(Gregular_log_name);
    if(!GRegularLogFile->open(QIODevice::Append))
    {
        QMessageBox::critical(0, "open file error",
                              "Can't open regular log file.\n"
                              "Click Cancel to exit.", QMessageBox::Cancel);
    }

    GErrorLogFile = new QFile(Gerror_log_name);
    if(!GErrorLogFile->open(QIODevice::Append))
    {
        QMessageBox::critical(0, "open file error",
                              "Can't open error log file.\n"
                              "Click Cancel to exit.", QMessageBox::Cancel);
    }
}
