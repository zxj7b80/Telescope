//diag_azalt.cpp
#include "diag_azalt.h"
#include "signs.h"
#define Column_AzaltDiagTable 			6
#define Row_AzaltDiagTable 			16

#define AzaltDiagTimerShowClock    		100				//time for timer_show,100ms
//#define DeroTimerSaveClock    			1000				//time for timer_save_az,1000ms

#define AzDiagCommandToUmac    			"m1262m1264m1362m1364m1462m1464m1562m1564m1662m1664m1762m1764m1862m1864m1962m1964m162m164"  //Az command to umac
#define AltDiagCommandToUmac    		"m2162m2164m2262m2264m2362m2364m2462m2464m2562m2564m2662m2664m2762m2764m2862m2864m262m264"  //Alt command to umac

#define ClearAzaltDiagToUmac		        "m1262=0m1264=0m1362=0m1364=0m1462=0m1464=0m1562=0m1564=0m1662=0m1664=0m1762=0m1764=0m1862=0m1864=0m1962=0m1964=0m162=0m164=0m2162=0m2164=0m2262=0m2264=0m2362=0m2364=0m2462=0m2464=0m2562=0m2564=0m2662=0m2664=0m2762=0m2764=0m2862=0m2864=0m262=0m264=0"

#define Col_counts_single			0
#define Col_position_single			1
#define Col_counts_all				2				//sum by UMAC
#define Col_position_all			3				//sum by UMAC
#define Col_error				4				 
#define ColTime					5

#define RowAz_1					0
#define RowAz_2					1
#define RowAz_3					2
#define RowAz_4					3
#define RowAz_5					4
#define RowAz_6					5
#define RowAz_7					6
#define RowAz_8					7	
			 
#define RowAlt_1				8
#define RowAlt_2				9
#define RowAlt_3				10
#define RowAlt_4				11
#define RowAlt_5				12
#define RowAlt_6				13
#define RowAlt_7				14
#define RowAlt_8				15	

#define FILENAME_Az				"AzReadingHead"
#define FILENAME_Alt				"AltReadingHead"
 

sAzAlt azDiag, altDiag;
extern FlagTCS flagtcs;

DiagAzalt::DiagAzalt ( MainWindow *parent )
{
 	m_parent = parent;
	setupUi ( this );
	SQUMAC::instance_p();
	iniTableWidget();
 	flagtcs.IsOpened_diag_azalt=true;
	timer_show_AzaltDiag = new QTimer;
	connect ( timer_show_AzaltDiag,SIGNAL ( timeout() ),this,SLOT ( ShowData() ) );
        timer_show_AzaltDiag->start ( AzaltDiagTimerShowClock ); 
}

/*  function: save data to a file
//input:
 int num       //matrix Az.AzData rows,#define N 300
double ( *p ) [5]:   save data, Az.AzData[N][5] ,AltData[N][5]
QString DataTime[]:save time
QString FileName: file name

*/

bool DiagAzalt::DataSave ( double ( *p ) [8],QString DataTime[],QString FileName,int num)
{
	char s_0[480] ;
	char s_1[60],s_2[60],s_3[60],s_4[60],s_5[60],s_6[60],s_7[60],s_8[60];

	int i,j;
 
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

bool DiagAzalt::Save(double head[3],double CalData[5],QString FileName,int PointerLast,int* Pointer,double Head[Q][8],QString DataTime[Q])
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
	       	if ( radioButton_AzaltDiagSave->isChecked() )
			DataSave ( Head ,DataTime,FileName,Q);
		*Pointer =0;
	}
 	return true;

}
DiagAzalt::~DiagAzalt()
{
// 	qDebug ( "diag azalt" );
// 	timer_show->deleteLater();
// 	timer_save_az->deleteLater();
// 	deleteLater();
     flagtcs.IsOpened_diag_azalt=false;
}
void DiagAzalt::iniTableWidget()
{
	///////////////table setup////////////////////////
	AzaltDiagTableWidget->setColumnCount ( Column_AzaltDiagTable );
	AzaltDiagTableWidget->setRowCount ( Row_AzaltDiagTable);
	QStringList labels_Hor,labels_ver;
	AzaltDiagTableWidget->setSortingEnabled ( false );
	labels_ver  << tr ( "Az_1" ) << tr ( "Az_2" )<<tr ( "Az_3" )<<tr ( "Az_4" )<<tr ( "Az_5" )<<tr ( "Az_6" )<<tr ( "Az_7" )<<tr ( "Az_8" )<<tr ( "Alt_1" )<<tr ( "Alt_2" )<<tr ( "Alt_3" )<<tr ( "Alt_4" )<<tr ( "Alt_5" )<<tr ( "Alt_6" )<<tr ( "Alt_7" )<<tr ( "Alt_8" );
	labels_Hor<< tr ( "Single Counts(cts)" ) << tr ( "Position(°)" )<<tr ( "SUM(cts)" )<<tr ( "Position(°)" )<<tr ( "Error('')" )<<tr ( "Time" );
	AzaltDiagTableWidget->setHorizontalHeaderLabels ( labels_Hor );
	AzaltDiagTableWidget->setVerticalHeaderLabels ( labels_ver );
        AzaltDiagTableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	AzaltDiagTableWidget->verticalHeader()->setResizeMode(QHeaderView::Stretch); 
}


void DiagAzalt::on_ClearPushBtn_clicked()
{
	QString str;
	char comm[5000];
	sprintf ( comm,ClearAzaltDiagToUmac );
	SQUMAC::instance_p()->QcommCmdPMAC ( comm,str );
}

void DiagAzalt::CommandToUMAC_Azalt ( char cmd[],int n, double (*p)[9])
{
	QString str;
	 							 
				 
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
//	UMAC_ETHERNET=true;
	for(int i=0;i<n/2;i++)
		* ( * ( p) +i )=( list[i*2+0].toDouble() +list[i*2+1].toDouble() ) /3072;	



}


void DiagAzalt::FreshAzaltDiagTable (int row,int col,QString str)
{
        QTableWidgetItem *AzaltDiagItem = new QTableWidgetItem ( str);
	AzaltDiagTableWidget->setItem ( row, col, AzaltDiagItem );
}

void DiagAzalt::CalData(double head[9],double cts_single,double cts_all,double (*p)[17])
{
 	double PosRealAll;									//real position,degree
	double PosReal[8];
        double HeadErr[8];	
	for(int i=0;i<8;i++)
		PosReal[i]=head[i]*cts_single/3600;						//Head_1 degree,position, degree

	PosRealAll=head[8]*cts_all/3600;							//All readingheads,position, degree
	for(int i=0;i<8;i++)
		HeadErr[i]=(PosReal[i]-PosRealAll)*3600;

	for(int i=0;i<8;i++)
		* ( * ( p) +i )=PosReal[i];
	
	* ( * ( p) +8 )=PosRealAll;

        for(int i=0;i<8;i++)
		* ( * ( p) +i+9 )=HeadErr[i];
}

void DiagAzalt::ShowData()
{	
	QString str;
	char comm[50000];
	int n;

	n=18;
	sprintf ( comm,AzDiagCommandToUmac);	
	double	AzHead[9],AzData[17];								//save Az readingheads
	CommandToUMAC_Azalt ( comm,n,&AzHead);
        qDebug()<<AzHead[0];
    CalData(AzHead,CTS_single,CTS,&AzData);
        Save(AzHead,AzData,FILENAME_Az,azDiag.Pointer,&azDiag.Pointer,azDiag.Head,azDiag.DataTime);
  
	str=QString ( "%1" ).arg ( AzHead[0],0,'f',2 );						//Az_1 counts
	FreshAzaltDiagTable(RowAz_1,Col_counts_single,str);
	str=QString ( "%1" ).arg ( AzHead[1],0,'f',2 );						//Az_2 counts
	FreshAzaltDiagTable(RowAz_2,Col_counts_single,str);
	str=QString ( "%1" ).arg ( AzHead[2],0,'f',2 );						//Az_3 counts
	FreshAzaltDiagTable(RowAz_3,Col_counts_single,str);
	str=QString ( "%1" ).arg ( AzHead[3],0,'f',2 );						//Az_4 counts
	FreshAzaltDiagTable(RowAz_4,Col_counts_single,str);
	str=QString ( "%1" ).arg ( AzHead[4],0,'f',2 );						//Az_5 counts
	FreshAzaltDiagTable(RowAz_5,Col_counts_single,str);
	str=QString ( "%1" ).arg ( AzHead[5],0,'f',2 );						//Az_6 counts
	FreshAzaltDiagTable(RowAz_6,Col_counts_single,str);
	str=QString ( "%1" ).arg ( AzHead[6],0,'f',2 );						//Az_7 counts
	FreshAzaltDiagTable(RowAz_7,Col_counts_single,str);
	str=QString ( "%1" ).arg ( AzHead[7],0,'f',2 );						//Az_8 counts
	FreshAzaltDiagTable(RowAz_8,Col_counts_single,str);
	str=QString ( "%1" ).arg ( AzHead[8],0,'f',2 );						//Az_all counts
	FreshAzaltDiagTable(RowAz_1,Col_counts_all,str);

	str=QString ( "%1" ).arg ( AzData[0],0,'f',5 );						//Az_1 position
	FreshAzaltDiagTable(RowAz_1,Col_position_single,str);
	str=QString ( "%1" ).arg ( AzData[1],0,'f',5 );	
	FreshAzaltDiagTable(RowAz_2,Col_position_single,str);					//Az_2 position
	str=QString ( "%1" ).arg ( AzData[2],0,'f',5 );						
	FreshAzaltDiagTable(RowAz_3,Col_position_single,str);					//Az_3 position
	str=QString ( "%1" ).arg ( AzData[3],0,'f',5 );						
	FreshAzaltDiagTable(RowAz_4,Col_position_single,str);					//Az_4 position
	str=QString ( "%1" ).arg ( AzData[4],0,'f',5 );						//Az_5 position
	FreshAzaltDiagTable(RowAz_5,Col_position_single,str);
	str=QString ( "%1" ).arg ( AzData[5],0,'f',5 );	
	FreshAzaltDiagTable(RowAz_6,Col_position_single,str);					//Az_6 position
	str=QString ( "%1" ).arg ( AzData[6],0,'f',5 );						
	FreshAzaltDiagTable(RowAz_7,Col_position_single,str);					//Az_7 position
	str=QString ( "%1" ).arg ( AzData[7],0,'f',5 );						
	FreshAzaltDiagTable(RowAz_8,Col_position_single,str);					//Az_8 position
	str=QString ( "%1" ).arg ( AzData[8],0,'f',5 );						
	FreshAzaltDiagTable(RowAz_1,Col_position_all,str);					//Az_8 position

	str=QString ( "%1" ).arg ( AzData[0],0,'f',5 );						//Az_1 position
	FreshAzaltDiagTable(RowAz_1,Col_position_single,str);
	str=QString ( "%1" ).arg ( AzData[1],0,'f',5 );	
	FreshAzaltDiagTable(RowAz_2,Col_position_single,str);					//Az_2 position
	str=QString ( "%1" ).arg ( AzData[2],0,'f',5 );						
	FreshAzaltDiagTable(RowAz_3,Col_position_single,str);					//Az_3 position
	str=QString ( "%1" ).arg ( AzData[3],0,'f',5 );						
	FreshAzaltDiagTable(RowAz_4,Col_position_single,str);					//Az_4 position
	str=QString ( "%1" ).arg ( AzData[4],0,'f',5 );						//Az_5 position
	FreshAzaltDiagTable(RowAz_5,Col_position_single,str);
	str=QString ( "%1" ).arg ( AzData[5],0,'f',5 );	
	FreshAzaltDiagTable(RowAz_6,Col_position_single,str);					//Az_6 position
	str=QString ( "%1" ).arg ( AzData[6],0,'f',5 );						
	FreshAzaltDiagTable(RowAz_7,Col_position_single,str);					//Az_7 position
	str=QString ( "%1" ).arg ( AzData[7],0,'f',5 );						
	FreshAzaltDiagTable(RowAz_8,Col_position_single,str);					//Az_8 position
	str=QString ( "%1" ).arg ( AzData[8],0,'f',5 );						
	FreshAzaltDiagTable(RowAz_1,Col_position_all,str);					//Az_all position

	str=QString ( "%1" ).arg ( AzData[9],0,'f',5 );						//Az_1 error
	FreshAzaltDiagTable(RowAz_1,Col_error,str);
	str=QString ( "%1" ).arg ( AzData[10],0,'f',5 );	
	FreshAzaltDiagTable(RowAz_2,Col_error,str);						//Az_2 error
	str=QString ( "%1" ).arg ( AzData[11],0,'f',5 );						
	FreshAzaltDiagTable(RowAz_3,Col_error,str);						//Az_3 error
	str=QString ( "%1" ).arg ( AzData[12],0,'f',5 );						
	FreshAzaltDiagTable(RowAz_4,Col_error,str);						//Az_4 error
	str=QString ( "%1" ).arg ( AzData[13],0,'f',5 );					//Az_5 error
	FreshAzaltDiagTable(RowAz_5,Col_error,str);
	str=QString ( "%1" ).arg ( AzData[14],0,'f',5 );	
	FreshAzaltDiagTable(RowAz_6,Col_error,str);						//Az_6 error
	str=QString ( "%1" ).arg ( AzData[15],0,'f',5 );						
	FreshAzaltDiagTable(RowAz_7,Col_error,str);						//Az_7 error
	str=QString ( "%1" ).arg ( AzData[16],0,'f',5 );						
	FreshAzaltDiagTable(RowAz_8,Col_error,str);						//Az_8 error


	//Alt
	n=18;
	sprintf ( comm,AltDiagCommandToUmac);	
	double	AltHead[9],AltData[17];								//save Alt readingheads
	CommandToUMAC_Azalt ( comm,n,&AltHead);
    CalData(AltHead,CTS_single,CTS,&AltData);
        Save(AltHead,AltData,FILENAME_Alt,altDiag.Pointer,&altDiag.Pointer,altDiag.Head,altDiag.DataTime);


	QDateTime dt=QDateTime::currentDateTime();
	QTime time=dt.time();
	QString dtstr;
	dtstr=dt.toString ( "yyyy-MM-dd hh:mm:ss.zzz" );
  	FreshAzaltDiagTable(RowAz_1,ColTime,dtstr);						//time


	str=QString ( "%1" ).arg ( AltHead[0],0,'f',2 );					//Alt_1 counts
	FreshAzaltDiagTable(RowAlt_1,Col_counts_single,str);
	str=QString ( "%1" ).arg ( AltHead[1],0,'f',2 );					//Alt_2 counts
	FreshAzaltDiagTable(RowAlt_2,Col_counts_single,str);
	str=QString ( "%1" ).arg ( AltHead[2],0,'f',2 );					//Alt_3 counts
	FreshAzaltDiagTable(RowAlt_3,Col_counts_single,str);
	str=QString ( "%1" ).arg ( AltHead[3],0,'f',2 );					//Alt_4 counts
	FreshAzaltDiagTable(RowAlt_4,Col_counts_single,str);
	str=QString ( "%1" ).arg ( AltHead[4],0,'f',2 );					//Alt_5 counts
	FreshAzaltDiagTable(RowAlt_5,Col_counts_single,str);
	str=QString ( "%1" ).arg ( AltHead[5],0,'f',2 );					//Alt_6 counts
	FreshAzaltDiagTable(RowAlt_6,Col_counts_single,str);
	str=QString ( "%1" ).arg ( AltHead[6],0,'f',2 );					//Alt_7 counts
	FreshAzaltDiagTable(RowAlt_7,Col_counts_single,str);
	str=QString ( "%1" ).arg ( AltHead[7],0,'f',2 );					//Alt_8 counts
	FreshAzaltDiagTable(RowAlt_8,Col_counts_single,str);
	str=QString ( "%1" ).arg ( AltHead[8],0,'f',2 );					//Alt_all counts
	FreshAzaltDiagTable(RowAlt_1,Col_counts_all,str);

	str=QString ( "%1" ).arg ( AltData[0],0,'f',5 );					//Alt_1 position
	FreshAzaltDiagTable(RowAlt_1,Col_position_single,str);
	str=QString ( "%1" ).arg ( AltData[1],0,'f',5 );	
	FreshAzaltDiagTable(RowAlt_2,Col_position_single,str);					//Alt_2 position
	str=QString ( "%1" ).arg ( AltData[2],0,'f',5 );						
	FreshAzaltDiagTable(RowAlt_3,Col_position_single,str);					//Alt_3 position
	str=QString ( "%1" ).arg ( AltData[3],0,'f',5 );						
	FreshAzaltDiagTable(RowAlt_4,Col_position_single,str);					//Alt_4 position
	str=QString ( "%1" ).arg ( AltData[4],0,'f',5 );					//Alt_5 position
	FreshAzaltDiagTable(RowAlt_5,Col_position_single,str);
	str=QString ( "%1" ).arg ( AltData[5],0,'f',5 );	
	FreshAzaltDiagTable(RowAlt_6,Col_position_single,str);					//Alt_6 position
	str=QString ( "%1" ).arg ( AltData[6],0,'f',5 );						
	FreshAzaltDiagTable(RowAlt_7,Col_position_single,str);					//Alt_7 position
	str=QString ( "%1" ).arg ( AltData[7],0,'f',5 );						
	FreshAzaltDiagTable(RowAlt_8,Col_position_single,str);					//Alt_8 position
	str=QString ( "%1" ).arg ( AltData[8],0,'f',5 );						
	FreshAzaltDiagTable(RowAlt_1,Col_position_all,str);					//Alt_8 position

	str=QString ( "%1" ).arg ( AltData[0],0,'f',5 );					//Alt_1 position
	FreshAzaltDiagTable(RowAlt_1,Col_position_single,str);
	str=QString ( "%1" ).arg ( AltData[1],0,'f',5 );	
	FreshAzaltDiagTable(RowAlt_2,Col_position_single,str);					//Alt_2 position
	str=QString ( "%1" ).arg ( AltData[2],0,'f',5 );						
	FreshAzaltDiagTable(RowAlt_3,Col_position_single,str);					//Alt_3 position
	str=QString ( "%1" ).arg ( AltData[3],0,'f',5 );						
	FreshAzaltDiagTable(RowAlt_4,Col_position_single,str);					//Alt_4 position
	str=QString ( "%1" ).arg ( AltData[4],0,'f',5 );					//Alt_5 position
	FreshAzaltDiagTable(RowAlt_5,Col_position_single,str);
	str=QString ( "%1" ).arg ( AltData[5],0,'f',5 );	
	FreshAzaltDiagTable(RowAlt_6,Col_position_single,str);					//Alt_6 position
	str=QString ( "%1" ).arg ( AltData[6],0,'f',5 );						
	FreshAzaltDiagTable(RowAlt_7,Col_position_single,str);					//Alt_7 position
	str=QString ( "%1" ).arg ( AltData[7],0,'f',5 );						
	FreshAzaltDiagTable(RowAlt_8,Col_position_single,str);					//Alt_8 position
	str=QString ( "%1" ).arg ( AltData[8],0,'f',5 );						
	FreshAzaltDiagTable(RowAlt_1,Col_position_all,str);					//Alt_all position

	str=QString ( "%1" ).arg ( AltData[9],0,'f',5 );					//Alt_1 error
	FreshAzaltDiagTable(RowAlt_1,Col_error,str);
	str=QString ( "%1" ).arg ( AltData[10],0,'f',5 );	
	FreshAzaltDiagTable(RowAlt_2,Col_error,str);						//Alt_2 error
	str=QString ( "%1" ).arg ( AltData[11],0,'f',5 );						
	FreshAzaltDiagTable(RowAlt_3,Col_error,str);						//Alt_3 error
	str=QString ( "%1" ).arg ( AltData[12],0,'f',5 );						
	FreshAzaltDiagTable(RowAlt_4,Col_error,str);						//Alt_4 error
	str=QString ( "%1" ).arg ( AltData[13],0,'f',5 );					//Alt_5 error
	FreshAzaltDiagTable(RowAlt_5,Col_error,str);
	str=QString ( "%1" ).arg ( AltData[14],0,'f',5 );	
	FreshAzaltDiagTable(RowAlt_6,Col_error,str);						//Alt_6 error
	str=QString ( "%1" ).arg ( AltData[15],0,'f',5 );						
	FreshAzaltDiagTable(RowAlt_7,Col_error,str);						//Alt_7 error
	str=QString ( "%1" ).arg ( AltData[16],0,'f',5 );						
	FreshAzaltDiagTable(RowAlt_8,Col_error,str);						//Alt_8 error

}
