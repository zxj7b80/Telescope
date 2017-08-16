/*
  C++ Implementation: sign.h
 function: define variables , structures and functions

*/

#ifndef SIGNS_H
#define SIGNS_H

#include <QValidator>
#include <QMutex>
#include "singleton.h"
#include "qgalil.h"
#include <QFile>

#include <stdint.h>

/************************Macro defination for infos of QSettings in .conf file*********/
#define VEL_FAST      		0x00
#define VEL_MID       		0x01
#define VEL_SLOW      		0x02
#define VEL_SPEC      		0x03
#define ACC_DEFAULT   		0x00
#define ACC_SPEC      		0x01

#define STEP1         		0x00
#define STEP01        		0x01
#define STEP001       		0x02
#define STEP_SPEC     		0x03

/****************************************/
#define CTS           		0.048677885 			//AZ encoder ratio
#define CTS_az                  0.183958793 			//AZ encoder ratio
#define CTS_alt                 0.214592275 			//AZ encoder ratio
#define CTS_single 		0.012169471 				//AZ single encoder ratio
//#define CTS_Az        		0.048677885 			//AZ encoder ratio
//#define CTS_Az_single 		0.012169471 				//AZ single encoder ratio
//#define CTS_Alt        		0.048677885 			//Alt encoder ratio
//#define CTS_Alt_single 		0.012169471 				//Alt single encoder ratio
//#define CTSAlt        		0.048677885   			//Alt encoder ratio
#define CTS_Dero        	1.094594595       //0.04119873    //Dero encoder ratio 65536*480
//#define CTS_C1_single 		0.225    		//C1 encoder ratio(single readinghead)
#define CTS_ADC	      	        1.094594595			//ADC encoder ratio 1184000
//#define CTS_DERO                19.775390625
//#define CTS_M3                  19.775390625
#define CTS_M3_TRANS            9.88769531088
#define CTS_M3_MM               0.00001525878907
#define CTS_M3_ROT              6.63220919952


#define MAXA_Az             180000           //a=2degree/s, Vmax=10degree/s ,Az max angle ,arc second
#define MAXA_Alt            45000            //a=2degree/s, Vmax=5degree/s, Alt max angle ,arc second
#define MAXA_Dero           9000             //a=0.4degree/s, Vmax=1degree/s, Dero max angle ,arc second

#define TableNumMAX 		180000
#define TableLineTime		100                              //100ms for a line of the star table
//#define	TrackPeriod		    100				//tracking period:100ms
//#define	TrackPeriod		1000				//tracking period:1s
//#define NumPerGroup             10                             //10 per group
#define PosixTClock          	200000000			//100 * 1000 * 1000;   100ms;
#define LinesPerSecond          10				//in table ,100lines=1s
#define LinesEvery100ms    	1				//in table ,10lines=100ms
#define TargetNumPerSecond      10				//100 lines per second
#define TarNumPerSecond_UMAC    1				//1 lines per second(UMAC)

#define SoftwareDelayAz		0			         //ms   1790
#define SoftwareDelayAlt	0				//ms
#define SoftwareDelayDero	0				//ms  2000

#define UmacStartP1_Az       	4000				//P parameters for Az tracking
#define UmacStartP2_Az       	4010				//P parameters for Az tracking
#define UmacStartP1_Alt       	5000				//P parameters for Alt tracking
#define UmacStartP2_Alt       	5010				//P parameters for Alt tracking
#define UmacStartP1_Dero     	6000				//P parameters for Dero tracking,group 1#
#define UmacStartP2_Dero     	6010				//P parameters for Dero tracking,group 2#

#define Q          		300     			//matrix C1Data/N2Data/N1Data rows
#define N 			300 				//matrix AzData/AltData rows
#define M 			300				//tracking data

#define M3_TR_EN     		0x0A
#define M3_TR_MO     	 	10.2
#define MMTOCTS(X)   	 	(((X)*M3_TR_EN*300)/M3_TR_MO)

#define IP_LOCAL        	"192.6.94.15"
#define IP_UMAC     		"192.6.94.5" 			//AZ,ALT,E1,DERO,DOME

/*
* Command from OCS/TCS to Mount(Az/Alt)
*/
#define TCSEMERGENSTOP	    "2.0.0.2.1"

#define TCSCHECKAZ	    "2.1.0.1.1"
#define TCSPOINTAZ          "2.1.0.2.1"
#define TCSSTOPAZ           "2.1.0.3.1"
#define TCSHOMEAZ           "2.1.0.4.1"
#define TCSENABLEAZ         "2.1.0.7.1"
#define TCSDISABLEAZ        "2.1.0.8.1"
#define TCSAZERROR	    "2.1.1.5.1"
#define TCSAZLIMIT	    "2.1.1.6.1"
//#define TCSWORKINGTIMEAZ	"2.1.0.9.1"

#define TCSCHECKALT	    "2.2.0.1.1"
#define TCSPOINTALT         "2.2.0.2.1"
#define TCSSTOPALT          "2.2.0.3.1"
#define TCSHOMEALT          "2.2.0.4.1"
#define TCSENABLEALT        "2.2.0.7.1"
#define TCSDISABLEALT       "2.2.0.8.1"
#define TCSALTERROR	    "2.2.1.5.1"
#define TCSALTLIMIT	    "2.2.1.6.1"
#define TCSALTLOCK          "2.2.0.9.1"
#define TCSALTUNLOCK        "2.2.0.10.1"
//#define TCSWORKINGTIMEALT	"2.2.0.11.1"

//Command from OCS/TCS to Mount(Cover)
#define TCSCOVEROPEN        "2.8.0.1.1"
#define TCSCOVERCLOSE       "2.8.0.2.1"
#define TCSCOVERSTOP        "2.8.0.3.1"

/*
* Status from Mount to OCS/TCS (Az/Alt)
*/


/*
* Command from OCS/TCS to Mount(dero)
*/
#define TCSCHECKDERO	        "2.3.0.1.1"
#define TCSPOINTDERO          	"2.3.0.2.1"
#define TCSSTOPDERO           	"2.3.0.3.1"
#define TCSHOMEDERO           	"2.3.0.4.1"
#define TCSENABLEDERO           "2.3.0.7.1"
#define TCSDISABLEDERO          "2.3.0.8.1"
#define TCSDEROERROR		"2.3.1.5.1"
#define TCSDEROLIMIT		"2.3.1.6.1"
//#define TCSWORKINGTIMEDERO		"2.3.0.9.1"

/*
* Command from OCS/TCS to Mount(adc1)
*/
#define TCSCHECKADC1		"2.5.0.1.1"
#define TCSPOINTADC1          	"2.5.0.2.1"
#define TCSSTOPADC1           	"2.5.0.3.1"
#define TCSHOMEADC1           	"2.5.0.4.1"
#define TCSENABLEADC1           "2.5.0.7.1"
#define TCSDISABLEADC1          "2.5.0.8.1"
#define TCSADC1ERROR		"2.5.1.5.1"
#define TCSADC1LIMIT		"2.5.1.6.1"
//#define TCSWORKINGTIMEADC1		"2.5.0.9.1"
/*
* Command from OCS/TCS to Mount(adc2)
*/
#define TCSCHECKADC2		"2.6.0.1.1"
#define TCSPOINTADC2          	"2.6.0.2.1"
#define TCSSTOPADC2           	"2.6.0.3.1"
#define TCSHOMEADC2           	"2.6.0.4.1"
#define TCSENABLEADC2           "2.6.0.7.1"
#define TCSDISABLEADC2          "2.6.0.8.1"
#define TCSADC2ERROR		"2.6.1.5.1"
#define TCSADC2LIMIT		"2.6.1.6.1"
//#define TCSWORKINGTIMEADC2		"2.6.0.9.1"


#define TCSCHECKM3	        "2.7.0.1.1"
#define TCSPOINTM3T           	"2.7.0.2.1"
#define TCSSTOPM3T          	"2.7.0.3.1"
#define TCSHOMEM3T         	"2.7.0.4.1"
#define TCSENABLEM3          	"2.7.0.7.1"
#define TCSDISABLEM3            "2.7.0.8.1"
#define TCSLOCKM3T		"2.7.0.11.1"
#define TCSUNLOCKM3T		"2.7.0.12.1"
#define TCSM3ERROR		"2.7.1.5.1"
#define TCSM3LIMIT		"2.7.1.6.1"

#define TCSPOINTM3R           	"2.7.0.9.1"
#define TCSSTOPM3R          	"2.7.0.10.1"
#define TCSPOINTFL           	"2.7.0.13.1"
#define TCSSTOPFL          	"2.7.0.14.1"

//by zxj---Terminal
#define TCSSTARTTERMINAL        "2.9.0.1.1"

/*
* Command from OCS/TCS to Tracking
*/
#define TCSSTARTTRACKING    	"2.4.0.1.1"			//TcsStartTracking
#define TCSSTOPTRACKING     	"2.4.0.2.1"			//TcsStopTracking
//#define TCSGETTRACKINGPOS     		"2.6.0.3.1"			//TcsStopTracking
#define TCSGETPOSITION		"2.4.0.3.1"			//Tcs get current position of Az, Alt, dero,adc1, adc2
/*
* Park
*/
/*#define	TCSPARKAZ				"8.6.0.8.1"
#define	TCSPARKALT				"8.6.0.8.1"
#define	TCSPARKDERO				"8.6.0.8.1"
#define	TCSPARKADC1				"8.6.0.8.1"
#define	TCSPARKADC2				"8.6.0.8.1"
*/

/*
* Hydrostatic
*/
//#define TCSHYDRORUN			"2.7.0.1.1"			//Tcs start the hydrostatic
//#define TCSHYDROERROR			"2.7.0.2.1"			//hydrostatic error
//#define TCSHYDROSTOP			"2.7.0.3.1"			//Tcs stop the hydrostatic
//#define TCSWORKINGTIMEHYDRO		"2.7.0.9.1"			//Record working time from the beginning of enable time


/*
* Paddle
*/
//#define TCSHANDRUN			"2.8.1.1.1"			//Run the handpaddle
//#define TCSHANDSTOP			"2.8.1.2.1"			//Stop the handpaddle

/*
* Guiding
*/
/*#define	TCSCHECKGUIDE			"3.6.0.1.1"			//Check guiding
#define	TCSOPENGUIDE			"3.6.0.2.1"
#define	TCSAUTOGUIDING			"3.6.0.3.1"
#define	TCSSTARTGUIDE			"3.6.0.4.1"
#define	TCSSTOPGUIDE			"3.6.0.5.1"
#define	TCSGUIDEERROR			"3.6.0.6.1"
#define	TCSWORKINGTIMEGUIDE		"3.6.0.7.1"
#define	TCSCLOSEGUIDE			"3.6.0.8.1"
*/
/*
* Emergency Stop
*/
//#define	TCSEMERGENCYSTOP		"9.6.0.8.1"
//#define	TCSRELEASEEMERGENCYSTOP		"9.6.0.9.1"




//#define AzTrackParam		"I530=99000I531=1000I532=1000I533=200I535=1000I568=615"
//#define AzPointParam		"I530=49000I531=1200I532=1200I533=200I535=1000I568=615"
#define AzTrackParam		"I2130=88000I2131=375I2132=500I2133=1000I2134=0I2168=620"
#define AzPointParam		"I2130=88000I2131=375I2132=500I2133=1000I2134=0I2168=620"

//#define AzGoParam		"I530=88000I531=2000I532=2000I533=5000I535=0I536=0I537=0I538=0I539=0I568=0I519=0.5I520=50I521=2000I519=0.1"
//#define AzGoParam_Fast "I530=88000I531=2000I532=2000I533=5000I535=0I536=0I537=0I538=0I539=0I568=0I519=0.5I520=50I521=2000I519=0.1"

//#define AltTrackParam		"I630=65000I631=1000I632=1000I633=300I635=5000I668=500"
#define AltTrackParam		"I1730=118000I1731=350I1732=830I1733=1200I1734=0I1735=0I1768=545"
#define AltTrackParam2		"I1730=118000I1731=350I1732=830I1733=1200I1734=0I1735=0I1768=545"
#define AltPointParam		"I1730=118000I1731=350I1732=830I1733=1200I1734=0I1735=0I1768=545"

//#define AltGoParam_Fast	        "I630=90000I631=500I632=3000I633=5000I635=0I636=0I637=0I638=0I639=0I668=350I617=0.1I619=0.1I620=50I621=2000"
//#define AltGoParam		"I630=-70000I631=2000I632=3000I633=5000I635=0I636=0I637=0I638=0I639=0I668=0I617=0.1I619=0.1I620=50I621=2000"

#define DeroTrackParam  		"I230=88000I231=2100I232=5000I233=5000I235=0I236=0I237=0I238=0I239=0I268=0I217=0.1I220=50I221=2000I219=0.1"
#define DeroPointParam  		"I230=88000I231=2100I232=5000I233=5000I235=0I236=0I237=0I238=0I239=0I268=0I217=0.1I220=50I221=2000I219=0.1"
//#define DeroGoParamFast_1  	"I230=88000I231=2100I232=5000I233=5000I235=0I236=0I237=0I238=0I239=0I268=0I217=0.1I220=50I221=2000I219=0.1"
//#define DeroGoParamFast_2  	"I230=88000I231=2100I232=5000I233=5000I235=0I236=0I237=0I238=0I239=0I268=0I217=0.1I220=50I221=2000I219=0.1"

#define ADC2TrackParam     	"I430=88000I431=2000I432=2000I433=5000I435=0I436=0I437=0I438=0I439=0I468=0I419=0.5I420=50I421=2000I419=0.1"
#define ADC2PointParam		"I430=88000I431=2000I432=2000I433=5000I435=0I436=0I437=0I438=0I439=0I468=0I419=0.5I420=50I421=2000I419=0.1"
//#define ADC2GoParamFast_1     	"I430=88000I431=2000I432=2000I433=5000I435=0I436=0I437=0I438=0I439=0I468=0I419=0.5I420=50I421=2000I419=0.1"
//#define ADC2GoParamFast_2		"I430=88000I431=2000I432=2000I433=5000I435=0I436=0I437=0I438=0I439=0I468=0I419=0.5I420=50I421=2000I419=0.1"

#define ADC1TrackParam      	"I330=60000I331=14000I332=14000I333=2000I335=100I337=0I339=0I368=0I321=4000I317=0.0768"
#define ADC1PointParam		"I330=60000I331=14000I332=14000I333=2000I335=100I337=0I339=0I368=0I321=4000I317=0.0768"
//#define ADC1GoParamFast_1      	"I330=60000I331=14000I332=14000I333=2000I335=100I337=0I339=0I368=0I321=4000I317=0.0768"
//#define ADC1GoParamFast_2		"I330=60000I331=14000I332=14000I333=2000I335=100I337=0I339=0I368=0I321=4000I317=0.0768"

#define PI			3.1415926535897932384626433832795

#define	ClearAzFlag			"P17=0"

#define	CheckDeroERROR			"M242M241"
#define	CheckADC1ERROR			"M342M341"
#define	CheckADC2ERROR			"M442M441"
#define	CheckDeroAMPLIFIER		"M254"
#define	CheckADC1AMPLIFIER		"M354"
#define	CheckADC2AMPLIFIER		"M454"
#define	CheckDeroLIMIT			"M231M232"
#define	CheckADCLIMIT			"M331M332M431M432"
#define	CheckAZALTLIMIT			"M7018M7016M7014M7012"
//#define	CheckAZALTLIMIT			"M531M532M631M632"    //mx31 is positive

#define StopAllAxis			"#1j/#2j/#3j/#4j/#21j/#17j/#7j/#8j/"
#define KillAllAxis			"#1K#2K#3K#4K#21K#17K#7K#8K"

#define CmdStopAz			"#21j/"
#define CmdStopAlt			"#17j/"
#define CmdStopDero			"#2j/"
#define CmdStopADC1			"#3j/"
#define CmdStopADC2			"#4j/"
#define CmdKillAz			"#21k"
#define CmdKillAlt			"#17k"
#define CmdKillDero			"#2k"
#define CmdKillADC1			"#3k"
#define CmdKillADC2			"#4k"
#define CmdGetAzPosition		"m2161m2162m2164m2165m2174"
#define CmdGetAltPosition		"m1761m1762m1764m1765m1774"
#define CmdGetDeroPosition		"m261m262m264m265m274"
#define CmdGetADC1Position		"m361m362m364m365m374"
#define CmdGetADC2Position		"m461m462m464m465m474"
#define CmdCheckAzaltError 		"M2142M1742M2141M1741"
#define  CmdAzEnable			"ENABLE PLC11"
#define  CmdAzDisable			"ENABLE PLC13"
#define  CmdAltEnable			"ENABLE PLC12"
#define  CmdAltDisable			"ENABLE PLC14"
#define  CmdDeroEnable			"ENABLE PLC20"
#define  CmdDeroDisable			"ENABLE PLC21"
#define  CmdADC1Enable			"ENABLE PLC22"
#define  CmdADC1Disable			"ENABLE PLC23"
#define  CmdADC2Enable			"ENABLE PLC24"
#define  CmdADC2Disable			"ENABLE PLC25"
#define	 CmdM3Enable		        "ENABLE PLC26"
#define	 CmdM3Disable   		"DISABLE PLC27"
#define  CmdStartAzMotion1		"&1B1R"
#define  CmdStopAzMotion1		"&1B1A"
#define  CmdKillAzMotion1		"&1B1K"
#define  CmdStartAltMotion2		"&2B2R"
#define  CmdStopAltMotion2		"&2B2A"
#define  CmdKillAltMotion2		"&2B2K"
#define  CmdStartDeroMotion3	        "&3B3R"
#define  CmdStopDeroMotion3		"&3B3A"
#define  CmdKillDeroMotion3		"&3B3K"
#define  CmdStartADCMotion4		"&4B4R"
#define  CmdStopADCMotion4		"&4B4A"
#define  CmdKillADCMotion4		"&4B4K"
#define  CmdStartADCMotion5		"&5B5R"
#define  CmdStopADCMotion5		"&5B5A"
#define  CmdKillADCMotion5		"&5B5K"
#define  CmdClearFlagBit		"M181=0M184=0M381=MP384=0M481=0M484=0M480=0"

//#define  CmdStartAzPointing             "P181=1"
//#define  CmdStartAltPointing            "P381=1"
//#define  CmdStartDeroPointing           "P481=1"

//#define  C1Release        	        "M7024=1"
//#define  C1Break        	        "M7024=0"
//#define  N2Release        	        "M7025=1"
//#define  N2Break        	        "M7025=0"

#define  EmergencyStop          	"M7027=1"
#define  EmergencyStopRelease          	"M7027=0"
#define  EnableAmplifiersCabinet   	"M7028=1"
#define  EnableAmplifiersCabinetRelease "M7028=0"
#define  ALTLOCKbuttonDown   		"M7031=1"
#define  ALTLOCKbuttonOn		"M7031=0"
#define  ALTUNLOCKbuttonDown   		"M7032=1"
#define  ALTUNLOCKbuttonOn		"M7032=0"

/*#define	 M87_addr  			0X1F00				//index of tracking step 2  AZ
#define  M88_addr			0X1F04				//year  OF group2
#define  M89_addr			0X1F08				//month
#define  M90_addr			0X1F0C				//day
#define  M95_addr			0X1F10				//hour
#define  M96_addr			0X1F14				//minute
#define  M97_addr			0X1F18				//second
*/
#define	 M180_addr  		0x1140
#define	 M181_addr  		0x1144
#define	 M182_addr  		0x1148
#define	 M183_addr  		0x114C
#define	 M184_addr  		0x1150
#define	 M185_addr  		0x1154
#define	 M186_addr  		0x1158
#define	 M187_addr  		0x115C				//index of tracking step 1  AZ
#define  M188_addr		0X1160				//year  OF group1
#define  M189_addr		0X1164				//month
#define  M190_addr		0X1168				//day
#define  M195_addr		0X116C				//hour
#define  M196_addr		0X1170				//minute
#define  M197_addr		0X1174				//second

#define	 M280_addr  		0x1180
#define	 M281_addr  		0x1184
#define	 M282_addr  		0x1188
#define	 M283_addr  		0x118C
#define	 M284_addr  		0x1190
#define	 M285_addr  		0x1194
#define	 M286_addr  		0x1198
#define	 M287_addr  		0x119C				//index of tracking step 1  ALT
#define  M288_addr		0X11A0				//year  OF group1
#define  M289_addr		0X11A4				//month
#define  M290_addr		0X11A8				//day
#define  M295_addr		0X11AC				//hour
#define  M296_addr		0X11B0				//minute
#define  M297_addr		0X11B4				//second

#define	 M380_addr  		0x11C0
#define	 M381_addr  		0x11C4
#define	 M382_addr  		0x11C8
#define	 M383_addr  		0x11CC
#define	 M384_addr  		0x11D0
#define	 M385_addr  		0x11D4
#define	 M386_addr  		0x11D8
#define	 M387_addr  		0x11DC				//index of tracking step 1  DERO
#define  M388_addr		0X11E0				//year  OF group1
#define  M389_addr		0X11E4				//month
#define  M390_addr		0X11E8				//day
#define  M395_addr		0X11EC				//hour
#define  M396_addr		0X11F0				//minute
#define  M397_addr		0X11F4				//second

#define	 M480_addr  		0x1200
#define	 M481_addr  		0x1204
#define	 M482_addr  		0x1208
#define	 M483_addr  		0x120C
#define	 M484_addr  		0x1210
#define	 M485_addr  		0x1214
#define	 M486_addr  		0x1218
#define	 M487_addr  		0x121C				//index of tracking step 1  ALT
#define  M488_addr		0X1220				//year  OF group1
#define  M489_addr		0X1224				//month
#define  M490_addr		0X1228				//day
#define  M495_addr		0X122C				//hour
#define  M496_addr		0X1230				//minute
#define  M497_addr		0X1234				//second

#define	 M2180_addr  		0x1240
#define	 M2181_addr  		0x1244
#define	 M2182_addr  		0x1248
#define	 M2183_addr  		0x124C
#define	 M2184_addr  		0x1250
#define	 M2185_addr  		0x1254
#define	 M2186_addr  		0x1258				// index of tracking step 2  DERO
#define  M2187_addr		0X125C				//year OF group2
#define  M2188_addr		0X1260				//month
#define  M2189_addr		0X1264				//day
#define  M2190_addr		0X1268				//hour
#define	 M2195_addr  		0x126C
#define  M2196_addr		0X1270				//minute
#define  M2197_addr		0X1274				//second
#define  M2198_addr      	0X1278				// constant,lines numberOffsetDelay


#define	 M1780_addr  		0x1280
#define	 M1781_addr  		0x1284
#define	 M1782_addr  		0x1288
#define	 M1783_addr  		0x128C
#define	 M1784_addr  		0x1290
#define	 M1785_addr  		0x1294
#define	 M1786_addr  		0x1298
#define	 M1787_addr  		0x129C				//index of tracking step 1  ALT
#define  M1788_addr		0X12A0				//year  OF group1
#define  M1789_addr		0X12A4				//month
#define  M1790_addr		0X12A8				//day
#define  M1795_addr		0X12AC				//hour
#define  M1796_addr		0X12B0				//minute
#define  M1797_addr		0X12B4				//second


#define	 M2000_DeroGroup1_StartAddr  	0X1300
#define	 M2010_DeroGroup2_StartAddr  	0X1328
//#define	 M2020_DeroGroup2_StartAddr  	0X1380

#define	 M3000_ADC1Group1_StartAddr  	0X1400
#define	 M3010_ADC1Group2_StartAddr  	0X1428
//#define	 M3020_ADC1Group2_StartAddr  	0X1480

#define	 M4000_ADC2Group1_StartAddr  	0X1500
#define	 M4010_ADC2Group2_StartAddr  	0X1528
//#define	 M4020_ADC2Group2_StartAddr  	0X1580

#define	 M5000_AzGroup1_StartAddr  	    0X1600
#define	 M5010_AzGroup2_StartAddr  	    0X1628
//#define	 M5020_AzGroup2_StartAddr  	    0X1680

#define	 M6000_AltGroup1_StartAddr  	0X1700
#define	 M6010_AltGroup2_StartAddr  	0X1728
//#define	 M6020_AltGroup2_StartAddr  	0X1780

#define	 Motor1_addr 	 		0X0074
#define	 Motor2_addr 	 		0X00A4
#define	 Motor3_addr 	 		0X00D4
#define	 Motor4_addr 	 		0X0104
#define	 Motor5_addr 	 		0X0134
#define	 Motor6_addr 	 		0X0164
#define	 Motor7_addr 	 		0X0194
#define	 Motor8_addr 	 		0X01C4
#define	 Motor9_addr 	 		0X01F4
#define	 Motor21_addr 	 		0X0434
#define	 Motor17_addr 	 		0X0374


//tracking
#define TrackSaveClock    			1000			//1000ms,timer_save_track_data
//#define TrackGetDataClock		50			//timer_get_track_data,ms
#define ShowTrackInfoClock			1000			//timer_show_track_info,ms
#define ShowInfoClock				1000			//timer_ShowInfo,ms
#define TrackCheckStatusClock			3000			//timer_CheckStatus,ms
#define TrackReplyClock				5000			//timer_TrackreplyOCS,ms
#define CheckPointingORTrackingClockAz    	100			//timer_CheckPointingORTracking
#define CheckPointingORTrackingClockAlt    	100			//timer_CheckPointingORTracking
#define CheckPointingORTrackingClockDero 	100			//timer_CheckPointingORTracking
#define CalAndShowRmsClock              	10000			//timer_CalAndShowRms
#define TrackSaveLinesPerSecond			10			//10 lines per second in file

//#define TrackGetDataClock 		50			//timer_get_track_data
//#define PointingClock    		1000			//1000ms,timer_pointing


#define DirConfig 			"/home/book/workspace/QtCode/telescope/config.ini"

#define RA_Hour_BOTTOM        		0
#define RA_Hour_TOP          		24

#define RA_Minute_BOTTOM       		0
#define RA_Minute_TOP          		60

#define RA_Second_BOTTOM       		0
#define RA_Second_TOP          		60
#define RA_Second_decimals    		10

#define DEC_Degree_BOTTOM               -90
#define DEC_Degree_TOP           	90

#define DEC_Minute_BOTTOM       	-60
#define DEC_Minute_TOP         		60

#define DEC_Second_BOTTOM        	-60
#define DEC_Second_TOP          	60
#define DEC_Second_decimals     	10

#define az_offset_BOTTOM       	6	-36000
#define az_offset_TOP           	36000
#define az_offset_decimals      	4

#define alt_offset_BOTTOM       	-36000
#define alt_offset_TOP           	36000
#define alt_offset_decimals      	4

#define dero_offset_BOTTOM       	-36000
#define dero_offset_TOP           	36000
#define dero_offset_decimals      	4

#define t_offset_BOTTOM       		-60
#define t_offset_TOP           		60
#define t_offset_decimals     	 	4

#define Pressure_BOTTOM        		0
#define Pressure_TOP         		1500
#define Pressure_decimals      		4

#define Temperature_BOTTOM     		-50
#define Temperature_TOP         	50
#define Temperature_decimals      	4

/****************************UMAC time**************************************************/
#define FirstIteration			"P7983=1"   	//; Flag indicating that this is the first iteration from power up
#define SecAtPwrUp			"P7992"		//; Seconds at power up (set at power up with HMI)
#define MinAtPwrUp			"P7993"   	//; Minutes at power up (set at power up with HMI)
#define HourAtPwrUp			"P7994"		//; Hour at power up (set at power up with HMI)
#define DayAtPwrUp			"P7995"   	//; Day at power up (set at power up with HMI)
#define MonthAtPwrUp                    "P7996"   	//; Month at power up (set at power up with HMI)
#define YearAtPwrUp			"P7997"	  	//; Year at power up (set at power up with HMI)
#define ClearAllTimeBuff               	"P7983..8007=0"
#define StartUMACTime               	"ENABLE PLC2"

/*******************************          Limit   ****************************************/
#define AzSoftLimitP			180
#define AzSoftLimitN		       -180
#define AltSoftLimitP			90
#define AltSoftLimitN			0
#define DeroSoftLimitP			90
#define DeroSoftLimitN		       -90
#define ADC2SoftLimitP			180
#define ADC2SoftLimitN			0
#define ADC1SoftLimitP			180
#define ADC1SoftLimitN			0
//#define C1EscapeFromLimit             	3600		// "


//log file name
extern  QString  Gregular_log_file_path ;
extern  QString  Gerror_log_file_path ;
extern  QFile   *GRegularLogFile;
extern  QFile   *GErrorLogFile;
extern  QString  Gregular_log_name;
extern  QString  Gerror_log_name;

int sgn ( double x );

struct MOTOR
{
    long long  followingError;
    int  servoCommand;
    int servoStatus;
    int generalStatus;
    long long positionBias;
    int filteredActuralVelocity;
    long long masterPosition;
    long long netActualPosition;
};

/*struct sAZ
{
    double  azPosReal_Degree;		//real position of Az,unit:degree
 double  AzData[N][6];			//save AZ data
 QString AzDataTime[N];			//save time of AZ
 double  azSeconds;			//time,for save AZ date
  int 	azPointer;			//for save AZ date,when azPointer=N
 double  Az_point;			//AZ point target from OCS/TCS
 double  Az_point_speed;
 double	AzHead[Q][8];			//save AZ readingheads
 double  AzVelocity;			//velocity
};*/


struct sAxis
{
    double  PosReal_Degree;	//real position ,unit:degree
    double  Data[N][6];			//save  data
    QString DataTime[N];	    //save time
    double  Seconds;			//time,for save date
    int     Pointer;			//for save date,when caltPointer=N
    double  point;			    // point target from OCS/TCS
    double  point_speed;
    //double	Head[Q][8];			//save  readingheads
    double  Velocity;			//velocity
    double  error;
};

struct sDero
{
    double  PosReal_Degree;		//real position of C1,unit:degree
    double  Data[Q][6];			//save C1 data
    QString DataTime[Q];			//save time of C1
    double  Seconds;			//time,for save C1 date
    int     Pointer;			//for save C1 date,when c1Pointer=Q
    double  point;
    double  point_speed;
    //double	Head[Q][8];			//save C1 readingheads
    //double  TrackTarget;			//target of tracking
    //double  TrackTargetOffset;		// tracking target(with offset)
    double  TrackError;			//following error of tracking star
    double  Velocity;			//velocity
};

struct sADC
{
    double  PosReal_Degree;		//real position of N2,unit:degree
    double  Data[Q][6];			//save N2 data
    QString DataTime[Q];			//save time of N2
    double  Seconds;			//time,for save N2 date
    int     Pointer;			//for save N2 date,when n2Pointer=Q
    double  point;
    double  point_speed;
    //double	Head[Q][8];			//save N2 readingheads
    //double  TrackTarget;			//target of tracking
    //double  TrackTargetOffset;		// tracking target(with offset)
    double  TrackError;			//following error of tracking star
    double  Velocity;			//velocity
};

/*struct sADC1
{
        double  ADC1PosReal_Degree;		//real position of N1,unit:degree
        double  ADC1Data[Q][6];			//save N1 data
        QString ADC1DataTime[Q];			//save time of N1
        double  ADC1Seconds;			//time,for save N1 date
        int 	ADC1Pointer;			//for save N1 date,when n1Pointer=Q
        double	ADC1_point;
        double  ADC1_point_speed;
        double	ADC1Head[Q][8];			//save N1 readingheads
        double  ADC1TrackTarget;			//target of tracking
        double  ADC1TrackTargetOffset;		// tracking target(with offset)
        double  ADC1TrackError;			//following error of tracking star
        double  ADC1Velocity;			//velocity
};*/


struct DateTime
{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    int msec;
};

struct StarTarget
{
    QString cDate;
    QString cTime;
    QString cAzimuth;
    QString cAltitude;
    QString cDero;
};

struct FlagTrack
{
    bool    StarTableEnd;				//flag of startable ending
};


struct TrackStarToUmac
{
    bool    flag_BeginTrack;
    bool    flag_SendtoUMAC;
    long    N_row;
    double  Target_Group1[11];   	//command to DPRAM
    double  Target_Group2[11];
    double  Vel_Group1[10];   	//command to DPRAM
    double  Vel_Group2[10];
    double  Target_Last;
    double  Vel_Last;
    float   deltaTime;
};

struct TrackStar
{
    double ra;				//Star RA
    double dec;				//Star DEC
    double ha_star;				//Star HA
    int guidetime;

    QString cCmdtoAz;			//command parameter set (Az)
    double  AzTargetOffset;  		//AZ Real position for Error (degree)
    double  AzTarget;               	//Az target position
    //double  AzTarget_End;               	//Az target position
    double  AzPosRealForError;		//for calculation tracking error
    double  AzTrackError;			//AZ tracking error
    double  AzFolErrUMAC;			//Az following error(UMAC)
    double  AzFolErrUMACTest[11];			//Az following error(UMAC)
    double  az_guidedata;			//Az guidedata
    double  az_guidedata_last;			//Az guidedata
    double  az_guidedata_all;			//Az all guidedata
    double	az_offset;			//Az offset
    double	az_offset_last;			//Az last offset
    double	az_offset_all;			//Az all offset
    double  az_correct;			//Az correct
    double  az_correct_last;		//Az last correct
    double  az_correct_all;

    int     az_DataCount;
    double  az_point;			//Az point
    int     RowAzPoint;                    //row of startable,point line
    double  delta_azTargetDelay;            //offset of delay
    double  AZ_TrackTargetSpeed;            //target speed
    double  AZ_TrackTargetSpeedLast;            //target speed
    double  AzVelocity;			//velocity for show
    double  AzTargetUMAC;
    //int 	AzCorrectLines;			//lines for correct pointing
    double  AzPointTime;			//Az point time
    int     AzCommand;

    bool 	flag_sendAz; 			//Az has got new tracking data and can send out to UMAC
    bool    flagAzPointing;			//Az is in pointing
    bool    flagAzTracking;			//Az is in tracking
    bool    flagAzRange;			//true:AZ is in range
    bool    flagAzCorrectPoint;		//correct point target after pointing
    bool    flag_AzpointEnd;

    QString cCmdtoAlt;			//command parameter set (Alt)
    double  AltTarget;               	//Az target position
    double  AltTargetOffset;  		//Alt Real position for Error (degree)
    double  AltPosRealForError;		//for calculation tracking error
    double  AltTrackError;			//Alt tracking error
    double  AltFolErrUMAC;			//ALt following error(UMAC)
    double  AltFolErrUMACTest[11];			//Az following error(UMAC)
    double  alt_guidedata;			//Alt guidedata
    double  alt_guidedata_last;		//Alt last guidedata
    double  alt_guidedata_all;
    double	alt_offset;			//Alt offset
    double	alt_offset_last;		//Alt last offset
    double	alt_offset_all;		//Alt last offset
    double  alt_correct;		//Alt correct
    double  alt_correct_last;		//Alt last correct
    double  alt_correct_all;

    int     alt_DataCount;
    double  alt_point;			//ALt point
    int     RowAltPoint;                    //row of startable,point line
    double  delta_altTargetDelay;            //offset of delay
    double  ALT_TrackTargetSpeed;            //target speed
    double  ALT_TrackTargetSpeedLast;            //target speed
    double  AltVelocity;			//velocity for show
    double  AltTargetUMAC;
    //int 	AltCorrectLines;		//lines for correct pointing
    double  AltPointTime;			//Alt point time
    int     AltCommand;

    bool 	flag_sendAlt; 			//Alt has got new tracking data and can send out to UMAC,Alt's tracking command string  is ready
    bool    flagAltPointing;		//Alt is in pointing
    bool    flagAltTracking;		//Alt is in tracking
    bool    flagAltRange;			//true:Alt is in range
    bool    flagAltCorrectPoint;		//correct point target after pointing
    bool    flag_AltpointEnd;		//

    QString cCmdtoDero;			//command parameter set (Dero)
    double  DeroPosition;               	//Dero real position
    double  DeroTarget;               	//Dero target position
    double  DeroTargetOffset;             	//Dero target position with offset,such as guiding
    double  DeroPosRealForError;  		//Dero Real position for Error (degree)
    double  DeroTrackError;               	//Dero tracking error
    double  DeroFolErrUMAC;			// following error(UMAC)
    double  DeroFolErrUMACTest[11];
    double  dero_guidedata;			//Dero guidedata
    double  dero_guidedata_last;		//Dero last guidedata
    double  dero_guidedata_all;
    double	dero_offset;			//Dero offset
    double	dero_offset_last;		//Dero last offset
    double	dero_offset_all;
    double  dero_correct;		//Dero correct
    double  dero_correct_last;		//Dero last correct
    double  dero_correct_all;

    int     dero_DataCount;
    double  dero_point;			//Dero point
    int     RowDeroPoint;                   //row of startable,point line
    double  delta_deroTargetDelay;            //offset of delay
    double  Dero_TrackTargetSpeed;            //target speed
    double  Dero_TrackTargetSpeedLast;            //target speed
    double  DeroVelocity;			//velocity for show
    double  DeroTargetUMAC;
    //int 	DeroCorrectLines;		//lines for correct pointing
    double  DeroPointTime;			//Dero point time

    bool 	flag_sendDero;			//Dero has got new tracking data and can send out to UMAC
    bool    flagDeroPointing;		//Dero is in pointing
    bool    flagDeroTracking;		//Dero is in tracking
    bool    flagDeroRange;			//true:Dero is in range
    bool    flagDeroCorrectPoint;		//correct point target after pointing
    bool    flag_DeropointEnd;

    QString cCmdtoADC1;			//command parameter set (Dero)
    double  ADC1Position;               	//Dero real position
    double  ADC1Target;               	//Dero target position
    double  ADC1TargetOffset;             	//Dero target position with offset,such as guiding
    double  ADC1PosRealForError;  		//Dero Real position for Error (degree)
    double  ADC1TrackError;               	//Dero tracking error
    double  ADC1FolErrUMAC;			// following error(UMAC)
    double  ADC1FolErrUMACTest[11];
    double  ADC1_guidedata;			//Dero guidedata
    double  ADC1_guidedata_last;		//Dero last guidedata
    double  ADC1_guidedata_all;
    double	ADC1_offset;			//Dero offset
    double	ADC1_offset_last;		//Dero last offset
    double	ADC1_offset_all;
    double  ADC1_correct;		//Dero correct
    double  ADC1_correct_last;		//Dero last correct
    double  ADC1_correct_all;

    int     ADC1_DataCount;
    double  ADC1_point;			//Dero point
    int     RowADC1Point;                   //row of startable,point line
    double  delta_ADC1TargetDelay;            //offset of delay
    double  ADC1_TrackTargetSpeed;            //target speed
    double  ADC1Velocity;			//velocity for show
    double  ADC1TargetUMAC;
    double  ADC1PointTime;			//Dero point time

    bool 	flag_sendADC1;			//Dero has got new tracking data and can send out to UMAC
    bool    flagADC1Pointing;		//Dero is in pointing
    bool    flagADC1Tracking;		//Dero is in tracking
    bool    flagADC1Range;			//true:Dero is in range
    bool    flagADC1CorrectPoint;		//correct point target after pointing
    bool    flag_ADC1pointEnd;

    QString cCmdtoADC2;			//command parameter set (Dero)
    double  ADC2Position;               	//Dero real position
    double  ADC2Target;               	//Dero target position
    double  ADC2TargetOffset;             	//Dero target position with offset,such as guiding
    double  ADC2PosRealForError;  		//Dero Real position for Error (degree)
    double  ADC2TrackError;               	//Dero tracking error
    double  ADC2FolErrUMAC;			// following error(UMAC)
    double  ADC2FolErrUMACTest[11];
    double  ADC2_guidedata;			//Dero guidedata
    double  ADC2_guidedata_last;		//Dero last guidedata
    double  ADC2_guidedata_all;
    double	ADC2_offset;			//Dero offset
    double	ADC2_offset_last;		//Dero last offset
    double	ADC2_offset_all;
    double  ADC2_correct;		//Dero correct
    double  ADC2_correct_last;		//Dero last correct
    double  ADC2_correct_all;

    int     ADC2_DataCount;
    double  ADC2_point;			//Dero point
    int     RowADC2Point;                   //row of startable,point line
    double  delta_ADC2TargetDelay;            //offset of delay
    double  ADC2_TrackTargetSpeed;            //target speed
    double  ADC2Velocity;			//velocity for show
    double  ADC2TargetUMAC;
    double  ADC2PointTime;			//Dero point time

    bool 	flag_sendADC2;			//Dero has got new tracking data and can send out to UMAC
    bool    flagADC2Pointing;		//Dero is in pointing
    bool    flagADC2Tracking;		//Dero is in tracking
    bool    flagADC2Range;			//true:Dero is in range
    bool    flagADC2CorrectPoint;		//correct point target after pointing
    bool    flag_ADC2pointEnd;

    bool    flagZenith;			//Zenith
    bool	StarTableEnd;
};

struct FlagTCS
{
    bool 	Az_Amplifier;					//Az amplifier status:active (true)or not(false)
    bool	Az_Homed;					//Az home status:active (true)or not(false)
    bool	Az_Homing;
    bool	Az_Error;					//
    bool	Az_LoopStatus;					//
    bool	Az_Limit_P;					//
    bool	Az_Limit_N;
    //bool	Az_Hydrostatic;
    bool 	Alt_Amplifier;					//Alt amplifier status:active (true)or not(false)
    bool	Alt_Homed;					//Alt home status:active (true)or not(false)
    bool	Alt_Homing;
    bool	Alt_Error;					//
    bool	Alt_LoopStatus;					//
    bool	Alt_Limit_P;
    bool	Alt_Limit_N;
    bool	Alt_Brake;
    bool 	Alt_Release;
    bool    Mirror_Cover_left;
    bool    Mirror_Cover_right;
    bool    Mirror_Cover_stop;
    bool    Mirror_Cover_open;
    bool    Mirror_Cover_close;
    bool    Mirror_Cover_left_open;
    bool    Mirror_Cover_right_open;
    bool    Mirror_Cover_left_close;
    bool    Mirror_Cover_right_close;

    bool    Az_SoftLimit_P;					//AZ software limit +
    bool    Az_SoftLimit_N;					//AZ software limit -
    bool    Alt_SoftLimit_P;				//ALT software limit +
    bool    Alt_SoftLimit_N;				//ALT software limit -
    bool    Dero_SoftLimit_P;				//C1 software limit +
    bool    Dero_SoftLimit_N;				//C1 software limit -
    bool    ADC1_SoftLimit_P;				//N1 software limit +
    bool    ADC1_SoftLimit_N;				//N1 software limit -
    bool    ADC2_SoftLimit_P;				//N2 software limit +
    bool    ADC2_SoftLimit_N;				//N2 software limit -


    bool   	Dero_Amplifier;					//dero amplifier status:active (true)or not(false)
    bool	Dero_Homed;
    bool	Dero_Error;
    bool	Dero_LoopStatus;
    bool	Dero_Limit_P;
    bool	Dero_Limit_N;
    bool	Dero_ALTHALF;

    //bool   	Der_Status;//true: C1 c2 N1 are ready

    bool   	ADC1_Amplifier;					//N1 amplifier status:active (true)or not(false)
    bool	ADC1_Homed;
    bool	ADC1_Homing;
    bool	ADC1_Error;
    bool	ADC1_LoopStatus;
    bool	ADC1_Limit_P;
    bool	ADC1_Limit_N;
    //bool	ADC1_Brake;

    bool   	ADC2_Amplifier;					//N1 amplifier status:active (true)or not(false)
    bool	ADC2_Homed;
    bool	ADC2_Homing;
    bool	ADC2_Error;
    bool	ADC2_LoopStatus;
    bool	ADC2_Limit_P;
    bool	ADC2_Limit_N;
    //bool	ADC2_Brake;

    bool    m3_trans_home;
    bool    m3_trans_homed;
    bool    m3_trans_homing;
    bool    m3_trans_amp;
    bool    m3_trans_brake;
    bool    m3_trans_safe_pos;
    bool    m3_trans_loop_status;
    bool    m3_rot_pos_p;
    bool    m3_rot_pos_n;
    bool    m3_rot_pos;
    bool    fiber_switch;

    bool 	IsOpened_azalt;                       	//direct Azalt dialog active(true) or not(false)
    bool 	IsOpened_dero;				//direct_dero dialog status:active(true) or not(false)
    bool 	IsOpened_adc;                       	//diag Azalt dialog active(true) or not(false)
    bool	IsOpened_m2;				//diag dero dialog active(true) or not(false)
    bool  	IsOpened_tracking;				//tracking dialog status:active(true) or not(false)
    bool	IsOpened_setup;					//setup dialog active(true) or not(false)
    bool 	IsOpened_simulationOCS;				//diag dero dialog active(true) or not(false)
    bool        IsOpened_terminal;				//terminal dialog active(true) or not(false)

    bool  	flag_TrackStarByOCS_MA;				//true: OCS ask MA to begin tracking star
    bool  	flag_TrackingStarByOCS_MA;			//true：tracking
    bool 	flag_TrackStarStopByOCS_MA;			//true: OCS ask to stop tracking
    bool	flag_GetTrackingPosByOCS_MA;
    bool        flag_m3_rot_in_positon;
    /*
        bool  	flag_TcsCheckDero;				//true: OCS ask to check Derotator

        bool   	flag_PointByOCS_Dero;				//true: OCS ask derotator to begin point
        bool   	flag_PointingByOCS_Dero;			//pointing
        bool   	flag_PointStopByOCS_Dero;			//true: OCS ask derotator to stop point
        */
    bool 	flag_tracking;					//true: tracking star(Az or Alt or Dero or All),tracking thread is working
    bool 	flag_pointing;					//true: pointing
    bool 	flag_AZIMUTH;					//true: azimuth can track(tracking screen)
    bool 	flag_ALTITUDE;					//true: altitude can track(tracking screen)
    bool 	flag_DERO;					//true:   for derotator
    bool 	flag_ADC1;					//true:   for derotator
    bool 	flag_ADC2;					//true:   for derotator

    bool    AxisEnabled;
    bool    AutoCorrect;
    bool    PointModel;
    bool	flag_TcsStatus;					//AZ,Alt,DERO
    bool    OffsetPixelUnit;
    //bool	HydroPressure;
    //bool	HydroAlarm;
    bool 	AzUpdateStartTime;
    bool 	AltUpdateStartTime;
    bool	DeroUpdateStartTime;
    bool	ADC1UpdateStartTime;
    bool	ADC2UpdateStartTime;
    //bool	flag_handpaddle;
    //bool	flag_handpaddleMode;
};


struct OcsCOM					//structure of comminication with OCS/TCS
{
    QString CommandFromOCS;
    char StatusCUID_MA[100];		//2.9.0.1.1.20061123.123456.4
    char NewCUID[20];			//2.9.0.1.2.
    char StatusCUID_EmergenStop[100];
    char StatusCUID_Az[100];
    char StatusCUID_AzPoint[100];
    char StatusCUID_AzHome[100];
    char StatusCUID_Alt[100];
    char StatusCUID_AltPoint[100];
    char StatusCUID_AltHome[100];
    char StatusCUID_Dero[100];
    char StatusCUID_DeroPoint[100];
    char StatusCUID_DeroHome[100];
    char StatusCUID_ADC1[100];
    char StatusCUID_ADC1Point[100];
    char StatusCUID_ADC1Home[100];
    char StatusCUID_ADC2[100];
    char StatusCUID_ADC2Point[100];
    char StatusCUID_ADC2Home[100];
    char StatusCUID_M3T[100];
    char StatusCUID_M3TPoint[100];
    char StatusCUID_M3THome[100];
    char StatusCUID_M3R[100];
    char StatusCUID_M3RPoint[100];
    char StatusCUID_FiberLight[100];
    char StatusCUID_Track[100];
    char StatusCUID_AltLock[100];
    char StatusCUID_AltUnlock[100];
    char StatusCUID_CoverOpen[100];
    char StatusCUID_CoverClose[100];
    char StatusCUID_CoverStop[100];
    //by zxj---
    char StatusCUID_TerminalStart[100];

    bool flag_TcsEmergenStop_AzAlt;
    bool flag_TcsEmergenStop_M3;
    bool flag_TcsEmergenStop_ADC;

    bool flag_TcsEnableAz;
    bool flag_TcsDisableAz;
    bool flag_PointByOCS_AZ;		//AZ start point
    bool flag_PointingByOCS_AZ;		//AZ pointing
    bool flag_PointStopByOCS_AZ;		// stop az point
    bool flag_TcsCheckAz;			//Check Az
    bool flag_TcsHomeAz;			//Home Az
    bool flag_TcsHomingAz;			//Az is Homing by OCS/TCS
    bool flag_HomeErrorAz;
    bool flag_TcsParkAz;
    bool flag_TcsParkingAz;
    bool flag_Az_Error;
    bool flag_Az_Limit_P;
    bool flag_Az_Limit_N;

    bool flag_TcsGetAzAltPosition;

    bool flag_PointByOCS_ALT;		//ALT start point
    bool flag_PointingByOCS_ALT;		//ALT pointing
    bool flag_PointStopByOCS_ALT;		//stop alt point
    bool flag_TcsCheckAlt;			//Check Alt
    bool flag_TcsHomeAlt;			//Home Alt
    bool flag_TcsHomingAlt;			//Az is Homing by OCS/TCS
    bool flag_HomeErrorAlt;
    bool flag_TcsEnableAlt;
    bool flag_TcsDisableAlt;
    bool flag_TcsParkAlt;
    bool flag_TcsParkingAlt;
    bool flag_Alt_Error;
    bool flag_Alt_Limit_P;
    bool flag_Alt_Limit_N;
    bool flag_TcsUnlockAlt;
    bool flag_TcsLockAlt;
    bool flag_TcsCloseCover;
    bool flag_TcsOpenCover;
    bool flag_TcsStopCover;


    bool flag_TcsEnableDero;
    bool flag_TcsDisableDero;
    bool flag_TcsCheckDero;
    bool flag_TcsHomeDero;
    bool flag_TcsHomingDero;
    bool flag_HomeErrorDero;
    bool flag_PointByOCS_Dero;
    bool flag_PointingByOCS_Dero;
    bool flag_PointStopByOCS_Dero;
    bool flag_TcsParkDero;
    bool flag_TcsParkingDero;
    bool flag_DERO_Error;
    bool flag_DERO_Limit_P;
    bool flag_DERO_Limit_N;
    bool flag_TcsGetDeroPosition;

    bool flag_TcsEnableADC1;
    bool flag_TcsDisableADC1;
    bool flag_TcsCheckADC1;
    bool flag_TcsHomeADC1;
    bool flag_TcsHomingADC1;
    bool flag_HomeErrorADC1;
    bool flag_PointByOCS_ADC1;
    bool flag_PointingByOCS_ADC1;
    bool flag_PointStopByOCS_ADC1;
    bool flag_TcsParkADC1;
    bool flag_TcsParkingADC1;
    bool flag_ADC1_Error;
    bool flag_ADC1_Limit_P;
    bool flag_ADC1_Limit_N;

    bool flag_TcsEnableADC2;
    bool flag_TcsDisableADC2;
    bool flag_TcsCheckADC2;
    bool flag_TcsHomeADC2;
    bool flag_TcsHomingADC2;
    bool flag_HomeErrorADC2;
    bool flag_PointByOCS_ADC2;
    bool flag_PointingByOCS_ADC2;
    bool flag_PointStopByOCS_ADC2;
    bool flag_TcsParkADC2;
    bool flag_TcsParkingADC2;
    bool flag_ADC2_Error;
    bool flag_ADC2_Limit_P;
    bool flag_ADC2_Limit_N;

    bool flag_TcsEnableM3;
    bool flag_TcsDisableM3;
    bool flag_TcsCheckM3;
    bool flag_TcsHomeM3;
    bool flag_TcsHomingM3;
    bool flag_HomeErrorM3;
    bool flag_PointByOCS_M3T;
    bool flag_PointingByOCS_M3T;
    bool flag_PointStopByOCS_M3T;
    bool flag_TcsUnlockM3T;
    bool flag_TcsLockM3T;
    bool flag_M3_Error;
    bool flag_M3_Limit_P;
    bool flag_M3_Limit_N;

    bool flag_PointByOCS_M3R;
    bool flag_PointingByOCS_M3R;
    bool flag_PointStopByOCS_M3R;

    bool flag_PointByOCS_FL;
    bool flag_PointingByOCS_FL;
    bool flag_PointStopByOCS_FL;
    bool flag_TcsParkM3;
    bool flag_TcsParkingM3;

    int  TrackStartMode;
    int  TrackStopMode;
    bool flag_TcsGetAzWorkingTime;
    bool flag_TcsGetAltWorkingTime;
    bool flag_TcsGetC1WorkingTime;
    bool flag_TcsGetN1WorkingTime;
    bool flag_TcsGetN2WorkingTime;
    //bool flag_TcsEmergencyStop;
    //bool flag_TcsReleaseEmergencyStop;
    //bool flag_Az_Hydrostatic;
    //bool flag_Alt_Brake;
    //bool flag_DERO_Brake;
    //bool flag_ADC1_Brake;
    //bool flag_ADC2_Brake;
    //bool flag_TcsHydroRun;
    //bool flag_TcsHydroStop;
    //bool flag_TcsHandpaddleRun;
    //bool flag_TcsHandpaddleStop;
    //bool flag_TcsUnlockC1;
    //bool flag_TcsLockC1;
    //bool flag_TcsUnlockN1;
    //bool flag_TcsLockN1;
    //bool flag_TcsUnlockN2;
    //bool flag_TcsLockN2;

    QDateTime AzStartWorkingTime;
    QDateTime AltStartWorkingTime;
    QDateTime DEROStartWorkingTime;
    QDateTime ADC1StartWorkingTime;
    QDateTime ADC2StartWorkingTime;
    QDateTime AzEndWorkingTime;
    QDateTime AltEndWorkingTime;
    QDateTime DEROEndWorkingTime;
    QDateTime ADC1EndWorkingTime;
    QDateTime ADC2EndWorkingTime;
    QString	AzWorkingTime;
    QString	AltWorkingTime;
    QString	DEROWorkingTime;
    QString	ADC1WorkingTime;
    QString	ADC2WorkingTime;

    QString AzUid;
    QString AltUid;
    QString DEROUid;
    QString ADC1Uid;
    QString ADC2Uid;
    QString TrackStartUid;
    QString TrackStopUid;
    QString GetTrackPosUid;
    QString AutoGuidingUid;
    QString EmergencyUid;
};


class MyDoubleValidator : public QDoubleValidator//enhanced version of QDoubleValidator,more strict.
{
public:
    MyDoubleValidator ( double bottom, double top, int decimals, QObject* parent = 0 )
        : QDoubleValidator ( bottom, top, decimals, parent )
    {}
    QValidator::State validate ( QString & input, int &pos ) const
    {
        if ( input.isEmpty() || input == "." || input == "-"|| input == "-." )
            return Intermediate;
        if ( QDoubleValidator::validate ( input, pos ) != Acceptable )
        {
            return Invalid;
        }
        return Acceptable;
    }
};


class MySetting:public QSettings
{
    DECLARE_SINGLETON_CLASS ( MySetting )
    private:
        MySetting() :QSettings ( DirConfig,QSettings::IniFormat ) {qDebug("New MySetting");}
    virtual ~MySetting() {qDebug("Delete MySetting");}
};
typedef Singleton<MySetting> SMySetting;


class UMAC;
class QUMAC:public QObject
{
    DECLARE_SINGLETON_CLASS ( QUMAC )
    Q_OBJECT

public:
    void QcommCmdPMAC ( const char cCommand[],QString& cResponse );
    void QctrlCmdPMAC ( const int cCommand,QString& cResponse );

    //////////////////INT48////
    void QgetDataDPRAM ( long long* data, uint16_t offset, int length );

    //add
    ///I don't know delete
    void Qgetinformation ( uint16_t offset,long long & followingError, int & servoCommand, int& servoStatus, int& generalStatus, long long& positionBias, int& filteredActuralVelocity, long long& masterPosition, long long& netActualPosition );
    // 		void QsendDataDPRAM ( const char cData[],long offset );
    // 		void QgetDataDPRAM ( QString& data,long offset,int data_length );
    //add
    //////////////////////////float//////////////////////
    void QsendDataDPRAM ( const float fData[],uint16_t offset, int iDatalength );
    void QgetDataDPRAM ( float* data, uint16_t offset, int length );


    ///////////////////////////////int////////////////////
    void QsendDataDPRAM ( const int iData[], uint16_t offset, int iDatalength );
    void QgetDataDPRAM ( int* data, uint16_t offset, int length );


    //add
    void QsendDataDPRAM ( const char cData[],uint16_t offset );
    void QgetDataDPRAM ( QString& data,uint16_t offset,int data_length );
    void Qclean_close();
    QTimer *timer_directazalt;

protected:

private:
    QUMAC ( const std::string& address = IP_UMAC );
    virtual ~QUMAC();

    UMAC *umac;
    QMutex cmd_mutex;

private slots:
    void throwUMACInfo();

signals:
    void UMACInfoThrown ( QHash<QString,QString> );
};
typedef Singleton<QUMAC> SQUMAC;



template<typename T>
class Dynamic1Dim
{
public:
    Dynamic1Dim();
    ~Dynamic1Dim();
    T& operator[] ( long index );
protected:
    bool EnlargeDim ( long iSize );
public:
    T* m_pBuf;
    long m_iSize;
};

template<typename T>
Dynamic1Dim<T>::Dynamic1Dim()
{
    //数组的初始大小的1个T类型对象
    //分配一块内存其大小为T型类所占的空间

    m_pBuf = ( T* ) malloc ( sizeof ( T ) );

    //在内存空间中建立一个T型对象

    new ( m_pBuf ) T();
    m_iSize = 360000;                    //size of array
}

template<typename T>
T& Dynamic1Dim<T>::operator [] ( long index )
{
    // 如果下标是负值，抛出一个异常

    if ( index < 0 ) throw " Index shouldn\\\'\\\'t be negative";

    //检查下标是否超来数组大小，如果超过则调用EnlargeDim扩大数组

    if ( index > m_iSize - 1 )
        EnlargeDim ( index + 1 );
    return m_pBuf [index];
}



template<typename T>

bool Dynamic1Dim<T>::EnlargeDim ( long iSize )
{

    // 重新分配一块内存，其大小为扩大后T类型数组的大小
    //m_pBuf = (QString*) realloc(m_pBuf, sizeof(T) * iSize);
    m_pBuf = ( T* ) realloc ( m_pBuf, sizeof ( T ) * iSize );
    // 在扩大部分的内存空间上建立T类型的数组对象，并调用其默认构造函数
    for ( long i = m_iSize; i < iSize; i++ )
    {
        new ( &m_pBuf[i] ) T();
    }

    m_iSize = iSize;
    return true;
}




template<typename T>
Dynamic1Dim<T>::~Dynamic1Dim()
{
    // 调用T类的析构函数

    // for(long i = 0; i < m_iSize; i++)
    // {
    // m_pBuf [i].~T();
    // }

    // 释放内存空间
    free ( m_pBuf );
    //delete []m_pBuf;

}



/*class QGalil;
    class Galil8:public QGalil
    {
    //		Q_OBJECT
      DECLARE_SINGLETON_CLASS ( Galil8 );
     private:
      Galil8 ( const std::string address = IP_GALIL8 ) :QGalil ( address ) {}
      virtual ~Galil8() {}

    };
    typedef Singleton<Galil8> SGalil8;

    class Galil6_3:public QGalil
    {
    //		Q_OBJECT
      DECLARE_SINGLETON_CLASS ( Galil6_3 );
     private:
      Galil6_3 ( const std::string address = IP_GALIL6_3 ) :QGalil ( address ) {}
      virtual ~Galil6_3() {}

    };
    typedef Singleton<Galil6_3> SGalil6_3;

    class Galil6_2:public QGalil
    {
    //		Q_OBJECT
      DECLARE_SINGLETON_CLASS ( Galil6_2 );
     private:
      Galil6_2 ( const std::string address = IP_GALIL6_2 ) :QGalil ( address ) {}
      virtual ~Galil6_2() {}

    };
    typedef Singleton<Galil6_2> SGalil6_2;

    class Galil6_1:public QGalil
    {
    //		Q_OBJECT
      DECLARE_SINGLETON_CLASS ( Galil6_1 );
     private:
      Galil6_1 ( const std::string address = IP_GALIL6_1 ) :QGalil ( address ) {}
      virtual ~Galil6_1() {}

    };
    typedef Singleton<Galil6_1> SGalil6_1;*/
#endif
