//
// C++ Implementation: terminal
//
//
//
#include "terminal.h"
#include "signs.h"
extern FlagTCS flagtcs;

Terminal::Terminal ( MainWindow *parent )
{
    m_parent = parent;
    setupUi ( this );
    connectDevice();
    flagtcs.IsOpened_terminal=true;

    // this->setWindowState(Qt::WindowMaximized);
    // this->setGeometry(100,0,100,100);
    //	initUi();
    //	initUi2();
}

Terminal::~Terminal()
{
    qDebug ( "Close terminal" );
    deleteLater();
    flagtcs.IsOpened_terminal=false;
}

void Terminal::connectDevice()
{
    // 	connect ( SGalil8::instance_p(),SIGNAL ( galilInfoThrown ( std::vector<char> ) ),this,SLOT ( updateInterface ( std::vector<char> ) ) );
    // 	connect ( SGalil6_3::instance_p(),SIGNAL ( galilInfoThrown2 ( std::vector<char> ) ),this,SLOT ( updateInterface2 ( std::vector<char> ) ) );
    // 	SGalil8::instance_p()->programDownloadFile ( "/home/dong/telescope/bin/test01.dmc" );
    //	SGalil6_3::instance_p()->programDownloadFile ( "/home/dong/telescope/bin/test02.dmc" );
    SQUMAC::instance_p();
}

void Terminal::initUi()
{
    //	std::vector<std::string> sources = SGalil8::instance_p()->sources();   //get all sources available on this controller
    // 	for ( std::vector<std::string>::size_type i = 0; i < sources.size(); i++ )   //print the value of all sources
    // 	{
    // 		tableWidget->setItem ( i,0, new QTableWidgetItem ( QString::fromStdString ( sources[i] ) ) );
    // //		tableWidget->setItem ( i,2, new QTableWidgetItem ( QString::fromStdString ( SGalil8::instance_p()->source ( "Units", sources[i] ) ) ) );
    // //		tableWidget->setItem ( i,3, new QTableWidgetItem ( QString::fromStdString ( SGalil8::instance_p()->source ( "Description", sources[i] ) ) ) );
    // 	}

}

void Terminal::initUi2()
{
    // 	std::vector<std::string> sources = SGalil6_3::instance_p()->sources();   //get all sources available on this controller
    // 	for ( std::vector<std::string>::size_type i = 0; i < sources.size(); i++ )   //print the value of all sources
    // 	{
    // 		tableWidget_2->setItem ( i,0, new QTableWidgetItem ( QString::fromStdString ( sources[i] ) ) );
    // 		tableWidget_2->setItem ( i,2, new QTableWidgetItem ( QString::fromStdString ( SGalil6_3::instance_p()->source ( "Units", sources[i] ) ) ) );
    // 		tableWidget_2->setItem ( i,3, new QTableWidgetItem ( QString::fromStdString ( SGalil6_3::instance_p()->source ( "Description", sources[i] ) ) ) );
    // 	}

}

void Terminal::closeEvent ( QCloseEvent *event )
{
    //parentWidget()->hide();
    //event->ignore();
}

void Terminal::updateInterface ( std::vector<char> g )
{
    // 	std::vector<std::string> sources = SGalil8::instance_p()->sources();
    // 	for ( std::vector<std::string>::size_type i = 0; i < sources.size(); i++ )
    // 		tableWidget->setItem ( i,1, new QTableWidgetItem ( QString::number ( SGalil8::instance_p()->sourceValue ( g,sources[i] ) ) ) );
}

void Terminal::updateInterface2 ( std::vector<char> g )
{
    //	std::vector<std::string> sources = SGalil6_3::instance_p()->sources();
    // 	for ( std::vector<std::string>::size_type i = 0; i < sources.size(); i++ )
    // 		tableWidget_2->setItem ( i,1, new QTableWidgetItem ( QString::number ( SGalil6_3::instance_p()->sourceValue ( g,sources[i] ) ) ) );
}

void Terminal::on_pushButton_clicked()
{
    QString str;
    // 	if(radioButton_m2->isChecked())
    // 		SGalil6_3::instance_p()->command(lineEdit->text().toStdString());
    // 	else if(radioButton_m3->isChecked())
    // 		SGalil8::instance_p()->command(lineEdit->text().toStdString());
    //	else
    SQUMAC::instance_p()->QcommCmdPMAC(lineEdit->text().toAscii(),str);
    textEdit->appendPlainText(lineEdit->text()+"  "+str);
    //lineEdit->setText(tr(""));
}

void Terminal::getJogCmd(char* Jog)
{
    lineEdit->setText(Jog);
}
