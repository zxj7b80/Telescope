#ifndef APPLICATION_H
#define APPLICATION_H

#include <QtGui>
#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QValidator>
#include <QMutex>


extern bool UMAC_ETHERNET;

class MyApplication : public QApplication
{
    Q_OBJECT
    public:
    MyApplication ( int argc, char *argv[] ) : QApplication(argc, argv){}
//    bool notify(QObject *receiver, QEvent *event)
//    {
//        try
//        {
//            return QApplication::notify(receiver, event);
//        }
//        catch(QString excepts)
//        {
//            if(UMAC_ETHERNET)
//            {
//                QMessageBox mb(QMessageBox::Warning, tr("About to close! (MyApplication)"), tr((excepts + "\n PLease close this window, check your connection to UMAC and open it again.").toAscii()), QMessageBox::Ok);
//                UMAC_ETHERNET = false;
//                qDebug() << "UMAC_ETHERNET = false";
//                if(mb.exec() == QMessageBox::Ok)
//                    return false;
//            }
//        }
//        catch(...)
//        {
//            QMessageBox mb(QMessageBox::Critical, tr("About to close!!!"), tr("Unknown excepts occurs.\n The app will quit now."), QMessageBox::Ok);
//            if(mb.exec() == QMessageBox::Ok)
//            {
//                quit();
//            }
//        }
//        return true;
//    }
};

class MyDoubleValidator : public QDoubleValidator	//enhanced version of QDoubleValidator,more strict.
{
    public:
        MyDoubleValidator(double bottom, double top, int decimals, QObject* parent = 0) : QDoubleValidator(bottom, top, decimals, parent)
        {}
        QValidator::State validate(QString & input, int &)const
        {
            const double b = bottom();
            const double t = top();
            const int d = decimals();

            QRegExp empty(QString::fromLatin1("-?\\.?"));
            if(input.contains(' '))
                return Invalid;
            if(b >= 0 && input.startsWith(QLatin1Char('-')))
                return Invalid;
            if(empty.exactMatch(input))
                return Intermediate;

            double entered = input.toDouble();
            int nume = input.count('e', Qt::CaseInsensitive);

            int i;
            if(input.contains(','))
                i = input.indexOf(',');
            else
                i = input.indexOf('.');

            if(i >= 0 && nume == 0)
            {
                i++;
                int j = i;
                while(input[j].isDigit())
                    j++;
                if (j - i > d)
                    return Invalid;
            }

            if(entered < b || entered > t)
                return Invalid;
            return Acceptable;
        }
};

#endif // APPLICATION_H
