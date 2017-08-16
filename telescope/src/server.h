//
// C++ Interface: server.h
//
// Description:
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
		QTcpSocket *tcp_socket;
		void  ConnectServer(QString str);
	protected:
		void incomingConnection ( int socket_descriptor );
	private:
		MainWindow *m_parent;
		
               //   QTcpSocket *clientConnection;
		int isPackageBroken(QDataStream&);
                void slotReadMessage();
	        quint16 m_BlockSize;
                QByteArray readDataIntoBuffer(int maxSize);
	private slots:
		void readData();
		bool Login();
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
