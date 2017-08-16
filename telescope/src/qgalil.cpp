//
// C++ Implementation: qgalil
//

//
#include "qgalil.h"
#include "Galil.h"
QGalil::QGalil ( const std::string address )
{
	try
	{
		galil = new Galil ( address );
	}
	catch ( std::string str )
	{
		throw QString::fromStdString ( str+"Net connection to Galil error!" );
	}
	timer = new QTimer;
	connect ( timer,SIGNAL ( timeout() ),this,SLOT ( throwGalilInfo() ) );
	timer->start ( 1000 );
}

QGalil::~QGalil()
{
	timer->deleteLater();
	delete galil;
	qDebug("QGalil");
}

void QGalil::throwGalilInfo()
{
	std::vector<char> g;
	g= galil->record ();//default is "QR" which means syn;
//	qDebug() << g.size();
	emit galilInfoThrown ( g );
}

std::string QGalil::command ( const std::string& command, const std::string& terminator, const std::string& ack, bool trim )  //send a command (e.g. "MG _RPX") to the controller and get the response
{
	try
	{
		return galil->command ( command,terminator,ack,trim );
	}
	catch ( std::string str )
	{
		throw QString::fromStdString ( str+"Net connection to Galil error!" );
	}
}

double QGalil::commandValue ( const std::string& command ) //convenience method that converts response from string to numerical value
{
	try
	{
		return galil->commandValue ( command );
	}
	catch ( std::string str )
	{
		throw QString::fromStdString ( str+"Net connection to Galil error!" );
	}
}

void QGalil::programDownload ( const std::string& program )   //DL download a controller program from an in-memory buffer
{

	try
	{
		galil->programDownload ( program );
	}
	catch ( std::string str )
	{
		throw QString::fromStdString ( str+"Net connection to Galil error!" );
	}
}

void QGalil::programDownloadFile ( const std::string& file ) //DL download a controller program from a disk file
{

	try
	{
		galil->programDownloadFile ( file );
	}
	catch ( std::string str )
	{
		throw QString::fromStdString ( str+"Net connection to Galil error!" );
	}
}

std::vector<std::string> QGalil::sources() //returns list of sources (_RPA...) supported by this controller, which are fed to sourceValue(), source(), and setSource()
{

	try
	{
		return galil->sources();
	}
	catch ( std::string str )
	{
		throw QString::fromStdString ( str+"Net connection to Galil error!" );
	}
}

std::vector<char> QGalil::record ( const std::string& method ) //reads DR packet OR sends QR and reads response
{

	try
	{
		return galil->record ( method );
	}
	catch ( std::string str )
	{
		throw QString::fromStdString ( str+"Net connection to Galil error!" );
	}
}

double QGalil::sourceValue ( const std::vector<char>& record, const std::string& source ) //get the value for one particular source (e.g. _RPA is 1000)
{

	try
	{
		return galil->sourceValue ( record,source );
	}
	catch ( std::string str )
	{
		throw QString::fromStdString ( str+"Net connection to Galil error!" );
	}
}

std::string  QGalil::source ( const std::string& field, const std::string& source )   //get e.g. the description string for one particular source (e.g. _RPA is "Axis A reference position").  Fields are "Description", "Units", & "Scale"
{

	try
	{
		return galil->source ( field,source );
	}
	catch ( std::string str )
	{
		throw QString::fromStdString ( str+"Net connection to Galil error!" );
	}
}
