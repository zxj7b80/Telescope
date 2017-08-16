#include <QThread>
#include <QTcpSocket>
#include <QtNetwork>
#define MaxBufferSize 1024
#include <QDir>
#include <QMessageBox>

#define LONG_MESSAGE_LENGTH 512


int readDataIntoBuffer ( int maxSize ,QTcpSocket *clientConnection)
{ QByteArray  m_Buffer;
//    quint16 m_BlockSize;
    char    temp[1000];

	QByteArray   datas ( "hello" );

	char *getValue=NULL;
	char *tmp;
	quint16 numBytesRead =0;
	quint16 numBytesAvailable =0;


	if ( clientConnection->bytesAvailable() < maxSize  &&  maxSize > MaxBufferSize )
		return 0;

	numBytesAvailable=clientConnection->bytesAvailable();
	getValue = ( char* ) malloc ( sizeof ( char ) *numBytesAvailable );


	clientConnection->read ( getValue,numBytesAvailable );

	memcpy ( &numBytesRead,getValue,sizeof ( quint16 ) );//get the datas length

	tmp = getValue;

	for ( int i=0;i<numBytesAvailable;i++ )
	{
		m_Buffer.append ( *tmp++ );
	}

	strcpy ( temp,getValue );

	if ( getValue ) free ( getValue );

	clientConnection->disconnectFromHost();

	return m_Buffer.size();

}





/////////////////analyse command form OCS //////////////////

////////////////////分析字符串/////////////////////////
//在字符串sourcestr中，从左边i_begin+1位开始，截取长度为i_length的字符串
//放到targetstr中
int getsubstr ( char *sourcestr, char *targetstr, int i_begin, int i_length )
{
	int i = 0;

	for ( i=0;i<i_length;i++ )
	{
		targetstr[i] = sourcestr[i + i_begin];
	}

	targetstr[i]='\0';
	return 0;
}

int strtrim ( char *sourcestr, char *targetstr ) //左
{
	int i=0, j=0 ;

	while ( ( ( sourcestr[i]<33 ) || ( sourcestr[i]>126 ) ) && ( sourcestr[i]!='\0' ) )
	{
		i++;
	}

	j = strlen ( sourcestr ) - 1;

	while ( ( ( sourcestr[j]<33 ) || ( sourcestr[j]>126 ) ) && ( sourcestr[i]!='\0' ) )
	{
		j--;
	}

	getsubstr ( sourcestr, targetstr, i, j - i + 1 );

	return 0;
}

int  strrtrim ( char *sourcestr, char *targetstr ) //右
{
	int i=0, j=0 ;

	j = strlen ( sourcestr ) - 1;

	while ( ( ( sourcestr[j]<33 ) || ( sourcestr[j]>126 ) ) && ( sourcestr[i]!='\0' ) )
	{
		j--;
	}

	getsubstr ( sourcestr, targetstr, i, j - i + 1 );

	return 0;
}

//把<command=item>中的item放到value中返回
int pr_get_commandvalue ( char *command, char *item, char *value )
{
	char *p_tempb , *p_tempe;
	int i_len,j;
	char buf[LONG_MESSAGE_LENGTH]={"<"};

	strcat ( buf,item );
	p_tempb = strstr ( command,buf );


	if ( p_tempb==NULL )
	{
		value = NULL;
		return 0;
	}
	else
	{
		p_tempb = strstr ( p_tempb,"=" );

		if ( p_tempb==NULL )
		{
			value = NULL;
			return 0;
		}
		else
		{
			p_tempe = strstr ( p_tempb,">" );
			p_tempb = p_tempb + 1;
		}
	}


	i_len = p_tempe - p_tempb;

	for ( j=0;j<i_len;j++ )
	{
		value[j] =*p_tempb;
		p_tempb++;
	}

	value[j] = '\0';
	strcpy ( buf,value );
	strtrim ( buf,value );
	//pr_strmessagebox(buf);

	return 1;

}

void slotReadMessage(QTcpSocket *clientConnection)
{      QByteArray  m_Buffer;
        quint16 m_BlockSize;
	QString nextFortune;
	m_BlockSize = 0;
	m_Buffer.clear();
	readDataIntoBuffer ( 1000,clientConnection );
	nextFortune=m_Buffer;
	//textEdit_log->append(nextFortune);
//QTableWidgetItem *item1= new QTableWidgetItem();

	//  item1->setTextAlignment(Qt::AlignRight);

//TCStableWidget->setItem(row, 1, item);
	// item->setText(filetext);
	// table->setHorizontalHeaderItem(i, item);
	// TCStableWidget->setText(row,1,m_Buffer)
	//  row++;
	//if(row>5)
	//  row=1;
//nextFortune="OCS command";

	/*QTableWidgetItem *newItem = new QTableWidgetItem ( nextFortune );
	if ( row>8 )
		OCStableWidget->insertRow ( row );
	OCStableWidget->setItem ( row, 0, newItem );
	row++;*/

	QByteArray ba;
	char* comm;
	ba= nextFortune.toLatin1();
	comm = ba.data();
	//CSendMessageTCS *sendMessageTCS ;
	//  sendMessageTCS = new CSendMessageTCS(comm);

	char request1_Mount[512];//接收到的TCS命令字符串
	strcpy ( request1_Mount,comm );
	//ParseOCSCommand ( request1_Mount );//
//             ParseOCSCommand ( request1_Mount,NewCUID,StatusCUID_MA);// 
	qDebug() <<comm;

}

