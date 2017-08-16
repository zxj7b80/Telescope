//diag_dero.cpp
#include "diag_dero.h"
#include "signs.h"
#define Column_DeroDiagTable 			5
#define Row_DeroDiagTable 			5

#define DeroDiagTimerShowClock    		100				//time for timer_show,100ms
#define DeroTimerSaveClock    			1000				//time for timer_save_az,1000ms

#define DeroDiagCommandToUmac    			"M262M264"	//C1 command to umac
#define ADC1DiagCommandToUmac    			"M362M364"		//N2 command to umac
#define ADC2DiagCommandToUmac    			"M462M464"		//N2 command to umac
#define ClearDeroDiagToUmac		        "m262=0m264=0m362=0m364=0m462=0m464=0"

#define Row_counts_single			0
#define Row_position_single			1
#define Row_counts_all				2				//sum by UMAC
#define Row_position_all			3				//sum by UMAC
#define Row_error				4				 

#define ColC1_1					0
#define ColC1_2					1
#define ColN2					2
#define ColN1					3
#define ColTime					4

#define FILENAME_C1				"C1ReadingHead"
#define FILENAME_N2				"N2ReadingHead"
#define FILENAME_N1				"N1ReadingHead"

sDero c1Diag;
sADC ADC2Diag, ADC1Diag;

extern FlagTCS flagtcs;
DiagDero::DiagDero ( MainWindow *parent )
{

 	m_parent = parent;
// 	timer_show=new QTimer;
	setupUi ( this );
	SQUMAC::instance_p();
	iniTableWidget();
 	flagtcs.IsOpened_diag_dero=true;
	timer_show_DeroDiag = new QTimer;
	connect ( timer_show_DeroDiag,SIGNAL ( timeout() ),this,SLOT ( ShowData() ) );
        timer_show_DeroDiag->start ( DeroDiagTimerShowClock ); 

// 	timer_save_DeroDiag = new QTimer;
// 	connect ( timer_save_DeroDiag,SIGNAL ( timeout() ),this,SLOT ( save_DeroDiag() ) );
// 	timer_save_DeroDiag->start ( DeroTimerSaveClock );

// 	timer_save_alt = new QTimer;
// 	connect ( timer_save_alt,SIGNAL ( timeout() ),this,SLOT ( Save_alt() ) );
// //	connectDevice();
// //	initUi();
// 	flag_save=false;
// 	flag_save_alt=false;
// 
// 	QDir dir;
// 	dir.mkpath ( "/home/telescope/telescope" );
// 	path = QDir::currentPath();
//       
}

/*  function: save data to a file
//input:
 int num       //matrix Az.AzData rows,#define N 300
double ( *p ) [5]:   save data, Az.AzData[N][5] ,AltData[N][5]
QString DataTime[]:save time
QString FileName: file name

*/

bool DiagDero::DataSave ( double ( *p ) [8],QString DataTime[],QString FileName,int num)
{
	char s_0[480] ;
	char s_1[60],s_2[60],s_3[60],s_4[60],s_5[60],s_6[60],s_7[60],s_8[60];

        int i;
 
	////////////// save file////////////////////
	QDateTime dt=QDateTime::currentDateTime();
	QTime time=dt.time();
	QString dtstr;
	dtstr=dt.toString ( "MM_dd_yyyy" );
	QString fileName =SMySetting::instance_p()->value ( "misc/DataDirectory" ).toString()+FileName+dtstr+".txt";
	QFile file ( fileName );
	if ( !file.open ( QFile::Append|QFile::Truncate ) )
	{
		QMessageBox::warning ( this,
		                       tr ( "Warn" ),
		                       tr ( "open error!" ),
		                       QMessageBox::Yes );
		return false;
	}
	QTextStream out ( &file );

        for ( i=0;i<Q;i++ )
	{
		out<<"\n";
		sprintf ( s_1,"  %10.2f",* ( * ( p+i ) +0 )  );				//head_1  counts
		sprintf ( s_2,"  %10.2f",* ( * ( p+i ) +1 )  );				//head_2  counts
		sprintf ( s_3,"  %10.2f",* ( * ( p+i ) +2)   );				//head_all  counts
		sprintf ( s_4,"  %10.6f",* ( * ( p+i ) +3 )  );				//head_1   position
		sprintf ( s_5,"  %10.6f",* ( * ( p+i ) +4 )  );				//head_2   position
		sprintf ( s_6,"  %10.6f",* ( * ( p+i ) +5 )  );				//head_all  position
		sprintf ( s_7,"  %10.2f",* ( * ( p+i ) +6 )  );				//head_1  error
		sprintf ( s_8,"  %10.2f",* ( * ( p+i ) +7 )  );				//head_2  error

		sprintf ( s_0,"%s%s%s%s%s%s%s%s   ",s_1,s_2,s_3,s_4,s_5,s_6,s_7,s_8 );
 		out<<s_0+DataTime[i];

	}
 
	file.close();
	return true;
}

bool DiagDero::Save(double head[3],double CalData[5],QString FileName,int PointerLast,int* Pointer,double Head[Q][8],QString DataTime[Q])
{
 	QDateTime dt=QDateTime::currentDateTime();
	QTime time=dt.time();
	QString dtstr;
	dtstr=dt.toString ( "yyyy-MM-dd hh:mm:ss.zzz" );

	if ( *Pointer <= Q-1 )
	{
                for(int i=0;i<3;i++)
		 	Head[*Pointer][i]=head[i];
		for(int j=0;j<5;j++)
		 	Head[ *Pointer][j+3]=CalData[j];		 
		
		DataTime[*Pointer]=dtstr;
		*Pointer=PointerLast+1;
	}
	else
	{	
	       	if ( radioButton_DeroDiagSave->isChecked() )
			DataSave ( Head ,DataTime,FileName,Q);
		*Pointer =0;
	}
 	return true;

}
DiagDero::~DiagDero()
{
// 	qDebug ( "diag azalt" );
// 	timer_show->deleteLater();
// 	timer_save_az->deleteLater();
// 	deleteLater();
flagtcs.IsOpened_diag_dero=false;
}
void DiagDero::iniTableWidget()
{
//tableWidget = new QTableWidget(1, 2);
	///////////////table setup////////////////////////

	DeroDiagTableWidget->setColumnCount ( Column_DeroDiagTable );
	DeroDiagTableWidget->setRowCount ( Row_DeroDiagTable);
	QStringList labels_Hor,labels_ver;
	DeroDiagTableWidget->setSortingEnabled ( false );
	labels_Hor << tr ( "C1-1" ) << tr ( "C1-2" )<<tr ( "N2" )<<tr ( "N1" )<<tr ( "Time" );
	labels_ver << tr ( "Single Counts(cts)" ) << tr ( "Position(°)" )<<tr ( "SUM(cts)" )<<tr ( "Position(°)" )<<tr ( "Error('')" );
	DeroDiagTableWidget->setHorizontalHeaderLabels ( labels_Hor );
	DeroDiagTableWidget->setVerticalHeaderLabels ( labels_ver );
//  	DeroDiagTableWidget->setColumnWidth ( column_NetTable_1, width_NetTable_1 );		//1# column width
// 	DeroDiagTableWidget->setColumnWidth ( column_NetTable_2, width_NetTable_2 );		//2# column width
        DeroDiagTableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	DeroDiagTableWidget->verticalHeader()->setResizeMode(QHeaderView::Stretch);
}

/*
void DiagDero::closeEvent ( QCloseEvent *event )
{
	parentWidget()->hide();
	event->ignore();
}*/



void DiagDero::on_ClearPushBtn_clicked()
{
	QString str;
	char comm[1024];
	sprintf ( comm,ClearDeroDiagToUmac );
	SQUMAC::instance_p()->QcommCmdPMAC ( comm,str );
}

void DiagDero::CommandToUMAC_Dero ( char cmd[],int n, double (*p)[3])
{
	QString str;
        //double PosReal1,PosReal2,PosRealAll;	 						//real position,degree
        //double HeadErr1,HeadErr2;
				 
	SQUMAC::instance_p()->QcommCmdPMAC ( cmd,str );
	QStringList  list = str.split ( QRegExp ( "\\s+" ),QString::SkipEmptyParts );
	int it=0;
	for ( QStringList::iterator point = list.begin(); point != list.end();++it, ++point )
		{}
	if ( it<n )
	{
//		UMAC_ETHERNET=false;
		return;
	};
	UMAC_ETHERNET=true;
	for(int i=0;i<n/2;i++)
		* ( * ( p) +i )=( list[i*2+0].toDouble() +list[i*2+1].toDouble() ) /3072;			//  data[p][i]

// 	* ( * ( p) +0 )=( list[0].toDouble() +list[1].toDouble() ) /3072;
// 	* ( * ( p) +1 )=( list[2].toDouble() +list[3].toDouble() ) /3072;
// 	* ( * ( p) +2 )=( list[4].toDouble() +list[5].toDouble() ) /3072;


}


void DiagDero::FreshDeroDiagTable (int row,int col,QString str)
{
        QTableWidgetItem *DeroDiagItem = new QTableWidgetItem ( str);
	DeroDiagTableWidget->setItem ( row, col, DeroDiagItem );
}

void DiagDero::CalData(double head[3],double cts_single,double cts_all,double (*p)[5])
{
 	double PosReal1,PosReal2,PosRealAll;	 						//real position,degree
	double HeadErr1,HeadErr2;	
	PosReal1=head[0]*cts_single/3600;							//C1_1 degree,position, degree
	PosReal2=head[1]*cts_single/3600;							//C1_2 degree,position, degree
	PosRealAll=head[2]*cts_all/3600;							//All readingheads,position, degree

	HeadErr1=(PosReal1-PosRealAll)*3600;
	HeadErr2=(PosReal2-PosRealAll)*3600;
        * ( * ( p) +0 )=PosReal1;
	* ( * ( p) +1 )=PosReal2;
	* ( * ( p) +2 )=PosRealAll;
	* ( * ( p) +3 )=HeadErr1;
	* ( * ( p) +4 )=HeadErr2;	

}



void DiagDero::ShowData()
{	
        /*QString str;
	char comm[50000];
	int n;

        n=2;
        sprintf ( comm,DeroDiagCommandToUmac);
	double	C1Head[3],C1Data[5];								//save C1 readingheads
	CommandToUMAC_Dero ( comm,n,&C1Head);
        CalData(C1Head,CTS_DERO_single,CTS_Dero,&C1Data);
	Save(C1Head,C1Data,FILENAME_C1,c1Diag.c1Pointer,&c1Diag.c1Pointer,c1Diag.C1Head,c1Diag.C1DataTime);	
   
        n=4;
	sprintf ( comm,N2DiagCommandToUmac);	
	double	N2Head[3],N2Data[5];								//save N2 readingheads
 	CommandToUMAC_Dero ( comm,n,&N2Head);
  	CalData(N2Head,CTS_N2,CTS_N2,&N2Data);	
  	Save(N2Head,N2Data,FILENAME_N2,n2Diag.n2Pointer,&n2Diag.n2Pointer,n2Diag.N2Head,n2Diag.N2DataTime);	
 	
        n=4;
	sprintf ( comm,N1DiagCommandToUmac);	
	double	N1Head[3],N1Data[5];								//save N2 readingheads
	CommandToUMAC_Dero ( comm,n,&N1Head);
 	CalData(N1Head,CTS_N1,CTS_N1,&N1Data);	
	Save(N2Head,N1Data,FILENAME_N1,n1Diag.n1Pointer,&n1Diag.n1Pointer,n1Diag.N1Head,n1Diag.N1DataTime);	

	str=QString ( "%1" ).arg ( C1Head[0],0,'f',2 );						//C1_1 counts
	FreshDeroDiagTable(Row_counts_single,ColC1_1,str);
	str=QString ( "%1" ).arg ( C1Head[1],0,'f',2 );						//C1_2counts
	FreshDeroDiagTable(Row_counts_single,ColC1_2,str);
	str=QString ( "%1" ).arg ( C1Head[2],0,'f',2 );						//C1_all counts
	FreshDeroDiagTable(Row_counts_all,ColC1_1,str);
	FreshDeroDiagTable(Row_counts_all,ColC1_2,str);	
	str=QString ( "%1" ).arg ( C1Data[0],0,'f',5 );						//C1_1 position
	FreshDeroDiagTable(Row_position_single,ColC1_1,str);
	str=QString ( "%1" ).arg ( C1Data[1],0,'f',5 );						//C1_2 position
	FreshDeroDiagTable(Row_position_single,ColC1_2,str);
	str=QString ( "%1" ).arg ( C1Data[2],0,'f',5 );						//C1_all position
	FreshDeroDiagTable(Row_position_all,ColC1_1,str);
	FreshDeroDiagTable(Row_position_all,ColC1_2,str);
 	str=QString ( "%1" ).arg ( C1Data[3],0,'f',2 );						//C1_1  error
	FreshDeroDiagTable(Row_error,ColC1_1,str);
  	str=QString ( "%1" ).arg ( C1Data[4],0,'f',2 );						//C1_2  error
	FreshDeroDiagTable(Row_error,ColC1_2,str);

	str=QString ( "%1" ).arg ( N2Head[0],0,'f',2 );						//N2 counts
	FreshDeroDiagTable(Row_counts_single,ColN2,str);
	FreshDeroDiagTable(Row_counts_all,ColN2,str);						//N2 counts
 	str=QString ( "%1" ).arg ( N2Data[0],0,'f',5 );						//N2 position
	FreshDeroDiagTable(Row_position_single,ColN2,str);					
	FreshDeroDiagTable(Row_position_all,ColN2,str);	
	str=QString ( "%1" ).arg ( N2Data[3],0,'f',2 );						//N2  error
	FreshDeroDiagTable(Row_error,ColN2,str);

	str=QString ( "%1" ).arg ( N1Head[0],0,'f',2 );						//N1 counts
	FreshDeroDiagTable(Row_counts_single,ColN1,str);
	FreshDeroDiagTable(Row_counts_all,ColN1,str);						//N1 counts
 	str=QString ( "%1" ).arg ( N1Data[0],0,'f',5 );						//N1 position
	FreshDeroDiagTable(Row_position_single,ColN1,str);					
	FreshDeroDiagTable(Row_position_all,ColN1,str);	
	str=QString ( "%1" ).arg ( N1Data[3],0,'f',2 );						//N1  error
	FreshDeroDiagTable(Row_error,ColN1,str);

	QDateTime dt=QDateTime::currentDateTime();
	QTime time=dt.time();
	QString dtstr;
	dtstr=dt.toString ( "yyyy-MM-dd hh:mm:ss.zzz" );
  	QTableWidgetItem *DeroDiagItem1 = new QTableWidgetItem ( dtstr );			//time
        DeroDiagTableWidget->setItem ( Row_counts_single, ColTime, DeroDiagItem1 );*/
}
