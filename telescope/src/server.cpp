//
// C++ Implementation: server.cpp
//
// Description:  for localhost

//
//
#include "server.h"
#include "main_window.h"
#include "signs.h"
extern OcsCOM OcsCom;
extern int OcsPort;		//Port of OCS (TCP/IP)
extern QString OcsIP; 		//IP of OCS(TCP/IP)
QString	DataBackToOcs;
Server::Server(MainWindow *parent)
{
	m_parent = parent;
	tcp_socket = new QTcpSocket(this);
//	connect( tcp_socket,SIGNAL(connected()),this,SLOT(showConnected()));
	connect ( tcp_socket, SIGNAL ( readyRead() ), this, SLOT ( readData() ) );
 	connect ( tcp_socket, SIGNAL ( error ( QAbstractSocket::SocketError ) ),this, SLOT ( showError ( QAbstractSocket::SocketError ) ) );
//	connect(parent->timer,SIGNAL(timeout()),this,SLOT(writeData()));
	connect(tcp_socket, SIGNAL(connected()), this, SLOT(Login()));
}

Server::~Server()
{
	delete tcp_socket;
}

int Server::isPackageBroken(QDataStream& in)
{
	quint16 block_size;
	if ( tcp_socket->bytesAvailable() < ( int ) sizeof ( quint16 ) )//You must make sure that enough data is available before attempting to read it using operator>>().
			return 1;
	in >> block_size;
//	qDebug() << block_size;
	if ( tcp_socket->bytesAvailable() < block_size )
		return 1;//some error happen here.
	return 0;
}

void Server::incomingConnection ( int socket_descriptor )
{
	tcp_socket->setSocketDescriptor (  socket_descriptor);
	qDebug("server incoming");
//	ServerThread *thread = new ServerThread ( socketDescriptor );
//	connect ( thread, SIGNAL ( finished() ), thread, SLOT ( deleteLater() ) );
//	thread->start();
}
void Server::readData()
{
//	QHash<QString,QString> hash;
	QString hash;
//	QByteArray data = tcp_socket->readAll();
//	QDataStream in ( &data ,QIODevice::ReadOnly );
	QDataStream in ( tcp_socket /*,QIODevice::ReadWrite */);
	in.setVersion ( QDataStream::Qt_4_0 );
	if(isPackageBroken(in))
	{
		throw QString("Package Broken.");
	}
	
	in >> hash;
//	qDebug() << hash["CMD"];

/////////////////////////////////////////////////////////////////////////////////
//slotReadMessage();



////////////////////////////////////////////////////////////////////////////////


	
	m_parent->showInfoOCS(hash);
qDebug()<<"get Azalt back:";
}

void Server::ConnectServer(QString str)
{qDebug()<<"ConnectOcsServer"<<str<<OcsIP<<OcsPort;
   	tcp_socket->connectToHost (OcsIP,OcsPort);

 
}
bool Server::Login()
{
	QString data=DataBackToOcs;
	QByteArray cmdSend;
	QDataStream out(&cmdSend,QIODevice::ReadWrite);
	out.setVersion ( QDataStream::Qt_4_0 );
	out << (quint16)0;
	out << data;
	out.device()->seek(0);
	out << ( quint16 ) ( cmdSend.size() - sizeof ( quint16 ) );
qDebug()<<"login"<<data;
	if ( (tcp_socket->write(cmdSend))==-1)  
	    return false;  
	 
        tcp_socket->close();
         return true; 
 
}

void Server::writeData(QString data)
{
	static int i = 0;
	QByteArray response;
	QDataStream out(&response,QIODevice::WriteOnly);
	out.setVersion ( QDataStream::Qt_4_0 );
//	out << (quint16)0;
	out << data;
//	out.device()->seek(0);
//	out << ( quint16 ) ( response.size() - sizeof ( quint16 ) );
	tcp_socket->write(response);
}

void Server::showError ( QAbstractSocket::SocketError socketError )
{
	switch ( socketError )
	{
		case QAbstractSocket::RemoteHostClosedError:
			break;
		case QAbstractSocket::HostNotFoundError:
			QMessageBox::information ( m_parent, tr ( "Mount Client" ),
			                           tr ( "The host of OCS was not found. Please check the "
			                                "host name and port settings." ) );
			break;
		case QAbstractSocket::ConnectionRefusedError:
			QMessageBox::information ( m_parent, tr ( "Mount Client" ),
			                           tr ( "The connection was refused by the peer. "
			                                "Make sure the OCS server is running, "
			                                "and check that the host name and port "
			                                "settings are correct." ) );
			break;
		default:
			QMessageBox::information ( m_parent, tr ( "Mount Client" ),
			                           tr ( "The following error occurred: %1." )
			                           .arg ( tcp_socket->errorString() ) );
	}

}

ServerThread::ServerThread ( int socketDescriptor ) :socket_fd ( socketDescriptor )
{

}

ServerThread::~ServerThread()
{

}

void ServerThread::run()
{	
	QString str;
	qDebug("bg");
	QTcpSocket tcpSocket;
	if ( !tcpSocket.setSocketDescriptor ( socket_fd ) )
	{
		return;
	}

	QByteArray block;
	block = tcpSocket.readAll();
	QDataStream out ( &block, QIODevice::ReadWrite );
	out.setVersion ( QDataStream::Qt_4_0 );
// 	out << ( quint16 ) 5;
// 	out << QString("This is message....ddd");
// 	out.device()->seek ( 0 );
// 	qDebug() << block.size();
// 	out << ( quint16 ) ( block.size() - sizeof ( quint16 ) );
// 	tcpSocket.write ( block );
// 	tcpSocket.disconnectFromHost();
// 	tcpSocket.waitForDisconnected();
	out >> str;
//	qDebug() << str;

}



void Server::slotReadMessage()
{
        QByteArray  m_Buffer;
	QString nextFortune;
	m_BlockSize = 0;
	m_Buffer.clear();
	m_Buffer=readDataIntoBuffer ( 1000 );
	nextFortune=m_Buffer;
        qDebug() <<nextFortune;
         OcsCom.CommandFromOCS=nextFortune;
// 	 
// 
// 
//  

}
//int DirectAzalt::readDataIntoBuffer ( int maxSize )
QByteArray Server::readDataIntoBuffer ( int maxSize )
{  QByteArray  m_Buffer;

	QByteArray   datas ( "hello" );

	char *getValue=NULL;
	char *tmp;
	quint16 numBytesRead =0;
	quint16 numBytesAvailable =0;


//	if ( clientConnection->bytesAvailable() < maxSize  &&  maxSize > MaxBufferSize )

if ( tcp_socket->bytesAvailable() < maxSize  &&  maxSize > MaxBufferSize )
		return 0;

	//numBytesAvailable=clientConnection->bytesAvailable();
         numBytesAvailable=tcp_socket->bytesAvailable();
	getValue = ( char* ) malloc ( sizeof ( char ) *numBytesAvailable );


	//clientConnection->read ( getValue,numBytesAvailable );
       tcp_socket->read ( getValue,numBytesAvailable );
	memcpy ( &numBytesRead,getValue,sizeof ( quint16 ) );//get the datas length

	tmp = getValue;

	for ( int i=0;i<numBytesAvailable;i++ )
	{
		m_Buffer.append ( *tmp++ );
	}

	//strcpy ( temp,getValue );

	if ( getValue ) free ( getValue );

	//clientConnection->disconnectFromHost();
         tcp_socket->disconnectFromHost();
	//return m_Buffer.size();
      return m_Buffer;
}

