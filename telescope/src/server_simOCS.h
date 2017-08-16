

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
#include "simulationOCS.h"
class SimulationOCS;
class ServerSimOCS:public QTcpServer
{
		Q_OBJECT
	public:
		ServerSimOCS ( SimulationOCS *parent );
		virtual ~ServerSimOCS();
		void writeData(QString data);
		void  ConnectServer(QString str);
		QTcpSocket *tcp_socket;
	protected:
		void incomingConnection ( int socket_descriptor );
	private:
		SimulationOCS *m_parent;
		
               //   QTcpSocket *clientConnection;
		int isPackageBroken(QDataStream&);
                 void slotReadMessage();
	        quint16 m_BlockSize;
                QByteArray readDataIntoBuffer(int maxSize);


	private slots:
		void readData();
		void showError(QAbstractSocket::SocketError socketError);
		bool Login();
	signals:
		void newMessage(QString hash);

};

// class ServerThread:public QThread
// {
// 		Q_OBJECT
// 	public:
// 		ServerThread ( int socketDescriptor );
// 		virtual ~ServerThread();
// 	protected:
// 		virtual void run();
// 	private:
// 		int socket_fd;
// };

#endif
