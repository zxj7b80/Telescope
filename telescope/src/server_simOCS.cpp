//
// C++ Implementation: server_simOCS.cpp
//
// Description: just for simulation OCS

#include "server_simOCS.h"
#include "main_window.h"
#include "signs.h"
extern OcsCOM OcsCom;
extern int LocalhostPort;	//Port of Mount(Az/Alt/Dero)
extern QString LocalhostIP; //IP of Mount(Az/Alt/Dero)

QString CommandToMount;
ServerSimOCS::ServerSimOCS ( SimulationOCS *parent )
{
	m_parent = parent;
	tcp_socket = new QTcpSocket ( this );
//	connect( tcp_socket,SIGNAL(connected()),this,SLOT(showConnected()));
	connect ( tcp_socket, SIGNAL ( readyRead() ), this, SLOT ( readData() ) );
	connect ( tcp_socket, SIGNAL ( error ( QAbstractSocket::SocketError ) ),this, SLOT ( showError ( QAbstractSocket::SocketError ) ) );
//	connect(parent->timer,SIGNAL(timeout()),this,SLOT(writeData()));
	connect ( this, SIGNAL ( newMessage ( QString ) ),m_parent, SLOT ( refreshOCStable ( QString ) ) );
	connect(tcp_socket, SIGNAL(connected()), this, SLOT(Login()));
}

ServerSimOCS::~ServerSimOCS()
{
	delete tcp_socket;
}

int ServerSimOCS::isPackageBroken ( QDataStream& in )
{
	quint16 block_size;
	if ( tcp_socket->bytesAvailable() < ( int ) sizeof ( quint16 ) ) //You must make sure that enough data is available before attempting to read it using operator>>().
		return 1;
	in >> block_size;
//	qDebug() << block_size;
	if ( tcp_socket->bytesAvailable() < block_size )
		return 1;//some error happen here.
	return 0;
}

void ServerSimOCS::incomingConnection ( int socket_descriptor )
{
	tcp_socket->setSocketDescriptor ( socket_descriptor );
	qDebug ( "server incoming" );
//	ServerThread *thread = new ServerThread ( socketDescriptor );
//	connect ( thread, SIGNAL ( finished() ), thread, SLOT ( deleteLater() ) );
//	thread->start();
}
void ServerSimOCS::readData()
{
//	QHash<QString,QString> hash;
	QString hash;
//	QByteArray data = tcp_socket->readAll();
//	QDataStream in ( &data ,QIODevice::ReadOnly );
	QDataStream in ( tcp_socket /*,QIODevice::ReadWrite */ );
	in.setVersion ( QDataStream::Qt_4_0 );
	if ( isPackageBroken ( in ) )
	{
		throw QString ( "Package Broken." );
	}

	in >> hash;

/////////////////////////////////////////////////////////////////////////////////
//slotReadMessage();

//	hash="yangshihai";
	//m_parent->showInfoOCS(hash);
	//m_parent->refreshOCStable(hash);
	emit newMessage ( hash );

	qDebug() <<"get Azalt back:"<<hash;
}


void ServerSimOCS::ConnectServer(QString str)
{qDebug()<<"ConnectServer"<<str<<LocalhostIP<<LocalhostPort;
   	tcp_socket->connectToHost (LocalhostIP,LocalhostPort);
 
}
bool ServerSimOCS::Login()
{
	QString data=CommandToMount;
	QByteArray cmdSend;
	QDataStream out(&cmdSend,QIODevice::ReadWrite);
	out.setVersion ( QDataStream::Qt_4_0 );
	out << (quint16)0;
	out << data;
	out.device()->seek(0);
	out << ( quint16 ) ( cmdSend.size() - sizeof ( quint16 ) );
	qDebug()<<"login"<<data;
qDebug() << cmdSend;
	if ( (tcp_socket->write(cmdSend))==-1)  
	    return false;  
	 
        tcp_socket->close();
         return true; 
 
}


void ServerSimOCS::writeData ( QString data )
{
	QByteArray response;
	QDataStream out ( &response,QIODevice::WriteOnly );
	out.setVersion ( QDataStream::Qt_4_0 );
//	out << (quint16)0;
	out << data;
//	out.device()->seek(0);
//	out << ( quint16 ) ( response.size() - sizeof ( quint16 ) );
	tcp_socket->write ( response );
}

void ServerSimOCS::showError ( QAbstractSocket::SocketError socketError )
{
	switch ( socketError )
	{
		case QAbstractSocket::RemoteHostClosedError:
			break;
		case QAbstractSocket::HostNotFoundError:
			QMessageBox::information ( m_parent, tr ( "SimOCS Client" ),
			                           tr ( "The host of Mount was not found. Please check the "
			                                "host name and port settings." ) );
			break;
		case QAbstractSocket::ConnectionRefusedError:
			QMessageBox::information ( m_parent, tr ( "SimOCS Client" ),
			                           tr ( "The connection was refused by the peer. "
			                                "Make sure the Mount server is running, "
			                                "and check that the host name and port "
			                                "settings are correct." ) );
			break;
		default:
			QMessageBox::information ( m_parent, tr ( "SimOCS Client" ),
			                           tr ( "The following error occurred: %1." )
			                           .arg ( tcp_socket->errorString() ) );
	}

}

void ServerSimOCS::slotReadMessage()
{
	QByteArray  m_Buffer;
	QString nextFortune;
	m_BlockSize = 0;
	m_Buffer.clear();
	m_Buffer=readDataIntoBuffer ( 1000 );
	nextFortune=m_Buffer;
	qDebug() <<nextFortune;
	OcsCom.CommandFromOCS=nextFortune;

}
//int DirectAzalt::readDataIntoBuffer ( int maxSize )
QByteArray ServerSimOCS::readDataIntoBuffer ( int maxSize )
{
	QByteArray  m_Buffer;
	QByteArray   datas ( "hello" );
	char *getValue=NULL;
	char *tmp;
	quint16 numBytesRead =0;
	quint16 numBytesAvailable =0;

	if ( tcp_socket->bytesAvailable() < maxSize  &&  maxSize > MaxBufferSize )
		return 0;

	numBytesAvailable=tcp_socket->bytesAvailable();
	getValue = ( char* ) malloc ( sizeof ( char ) *numBytesAvailable );


	//clientConnection->read ( getValue,numBytesAvailable );
	tcp_socket->read ( getValue,numBytesAvailable );
	memcpy ( &numBytesRead,getValue,sizeof ( quint16 ) );			//get the datas length
	tmp = getValue;

	for ( int i=0;i<numBytesAvailable;i++ )
	{
		m_Buffer.append ( *tmp++ );
	}

	if ( getValue ) free ( getValue );

	tcp_socket->disconnectFromHost();
	return m_Buffer;
}

