//
// C++ Interface: qgalil
//
// Description:
//
//

//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef QGALIL_H
#define QGALIL_H
#include <QtGui>
class Galil;
class QGalil:public QObject
{
		Q_OBJECT
	public:
		QGalil ( const std::string address );
		virtual ~QGalil();

		std::string                     command ( const std::string& command = "MG TIME", const std::string& terminator = "\r", const std::string& ack = ":", bool trim = true );   //send a command (e.g. "MG _RPX") to the controller and get the response
		double                          commandValue ( const std::string& command = "MG TIME" ); //convenience method that converts response from string to numerical value
		void                            programDownload ( const std::string& program = "MG TIME\rEN" );  //DL download a controller program from an in-memory buffer
		void                            programDownloadFile ( const std::string& file    = "program.dmc" ); //DL download a controller program from a disk file
		std::vector<std::string>        sources();                           //returns list of sources (_RPA...) supported by this controller, which are fed to sourceValue(), source(), and setSource()
		std::vector<char>               record ( const std::string& method = "QR" ); //reads DR packet OR sends QR and reads response
		double                          sourceValue ( const std::vector<char>& record,          const std::string& source = "TIME" ); //get the value for one particular source (e.g. _RPA is 1000)
		std::string                     source ( const std::string& field = "Description", const std::string& source = "TIME" );   //get e.g. the description string for one particular source (e.g. _RPA is "Axis A reference position").  Fields are "Description", "Units", & "Scale"
	private:
		Galil *galil;
		QTimer *timer;
	private slots:
		void throwGalilInfo();
	signals:
		void galilInfoThrown ( std::vector<char> );
};
#endif