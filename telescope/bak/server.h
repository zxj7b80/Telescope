//
// C++ Interface: server
//
// Description:
//

//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SERVER_H
#define SERVER_H
#include <QtGui>
#include <QtNetwork>
#define MaxBufferSize 1024
class MainWindow;
class Server:public QTcpServer
{
		Q_OBJECT
	public:
		Server ( MainWindow *parent );
		virtual ~Server();
		void writeData(QString data);
	protected:
		void incomingConnection ( int socket_descriptor );
	private:
		MainWindow *m_parent;
		QTcpSocket *tcp_socket;
               //   QTcpSocket *clientConnection;
		int isPackageBroken(QDataStream&);
                 void slotReadMessage();
	        quint16 m_BlockSize;
                QByteArray readDataIntoBuffer(int maxSize);
	private slots:
		void readData();
		
		void showError(QAbstractSocket::SocketError socketError);
};

class ServerThread:public QThread
{
		Q_OBJECT
	public:
		ServerThread ( int socketDescriptor );
		virtual ~ServerThread();
	protected:
		virtual void run();
	private:
		int socket_fd;
};

#endif
