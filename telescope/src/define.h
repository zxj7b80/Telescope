 #ifndef DEFINE_H
#define DEFINE_H

#include <QString>

//#define IP_UMAC                 "192.6.94.5"
//#define CTS_ADC                 1.09459459459459
//#define CTS_DERO                19.775390625
//#define CTS_M3                  19.775390625
//#define CTS_M3_MM               0.00001525878907
//#define CTS_M3_ROT              6.63220919952 //0.648
#define N_NUMBER                300
#define TIMER_100MS             100
#define TIMER_1000MS            1000
/*
#define ENABLE_DERO             "enable plc 20"
#define DISENABLE_DERO          "enable plc 21"
#define ENABLE_ADC1             "enable plc 22"
#define DISENABLE_ADC1          "enable plc 23"
#define ENABLE_ADC2             "enable plc 24"
#define DISENABLE_ADC2          "enable plc 25"*/
#define ENABLE_DERO             "#2$"
#define DISENABLE_DERO          "#2k"
#define ENABLE_ADC1             "#3$"
#define DISENABLE_ADC1          "#3k"
#define ENABLE_ADC2             "#4$"
#define DISENABLE_ADC2          "#4k"

#define ENABLE_M3               "P706=1P701=0enable plc10"
#define DISENABLE_M3            "P706=0disable plc10"
#define HOME_ADC1               "#3HM"
#define HOME_ADC2               "#4HM"
#define HOME_DERO               "#2HM"
#define HOME_M3                 "#1HM"
#define UNLOCK_TRANS_M3         "M7026=1"
#define LOCK_TRANS_M3           "M7026=0"
#define SET_ADC1_POS            "#3J=%f"
#define SET_ADC2_POS            "#4J=%f"
#define SET_DERO_POS            "#2J=%f"
#define SET_M3_POS              "#1J=%f"
#define SET_ADC1_VEL            "I322=%f"
#define SET_ADC2_VEL            "I422=%f"
#define SET_DERO_VEL            "I222=%f"
#define SET_M3_VEL              "I122=%f"
#define SET_M3_VEL_ROT          "I722=%f"
#define SET_ADC1_STOP           "#3j/"
#define SET_ADC2_STOP           "#4j/"
#define SET_DERO_STOP           "#2j/"
#define SET_M3_TRANS_STOP       "#1K"
#define SET_M3_ROT_STOP         "#7K"
#define SET_M3_ROT_DIR_P        "#7J+"
#define SET_M3_ROT_DIR_N        "#7J-"
#define SET_FIBER_SWITCH_ON     "M7025=1"
#define SET_FIBER_SWITCH_OFF    "M7025=0"
#define SET_FIBER_DIR_P         "#8J:400"
#define SET_FIBER_DIR_N         "#8J:-400"
#define SET_M3_AUTO_1           "#7J:-95000P705=1"
#define SET_M3_AUTO_2           "#7J:95000P705=2"
#define FLAG_CHECK_ADC_HOME     "M334M345M434M445"
#define FLAG_CHECK_ADC_AMP      "M339M439"
#define FLAG_CHECK_ADC_LOOP     "M338M438"
#define FLAG_CHECK_ADC_ERROR    "M342M442"
#define FLAG_CHECK_DERO_M3_HOME "M234M245M134M145"
#define FLAG_CHECK_DERO_M3_AMP  "M239M139"
#define FLAG_CHECK_DERO_M3_LOOP "M238M138"
#define FLAG_CHECK_DERO_ERROR   "M242"
#define FLAG_CHECK_M3_BRAKE     "M7026"
#define FLAG_CHECK_M3_ROT_POS   "M7000M7001P703"
#define FLAG_CHECK_M3_ROT       "M740"
#define FLAG_CHECK_FIBER_SWITCH "M7025"
#define GET_ADC1_INFO           "M361M362M364M365M374"
#define GET_ADC2_INFO           "M461M462M464M465M474"
#define GET_DERO_INFO           "M261M262M264M265M274"
#define GET_M3_INFO             "M161M162M164M165M174"
#define GET_M3_INFO_ROT         "M761M762M764M765M774"

/*struct FlagTCS
{
    bool IsOpened_tracking;         //tracking dialog status:active(true) or not(false)
    bool IsOpened_terminal;         //terminal dialog active(true) or not(false)
    bool IsOpened_derotator;        //derotator dialog active(true) or not(false)
    bool IsOpened_setup;            //setup dialog active(true) or not(false)
    bool IsOpened_adc;              //adc dialog active(true) or not(false)

    bool flag_adc1_home;
    bool flag_adc2_home;
    bool flag_adc1_amp;
    bool flag_adc2_amp;
    bool flag_adc1_soft_limit_n;
    bool flag_adc1_soft_limit_p;
    bool flag_adc2_soft_limit_n;
    bool flag_adc2_soft_limit_p;
    bool flag_adc1_error;
    bool flag_adc2_error;
    bool flag_adc1_loop_status;
    bool flag_adc2_loop_status;

    bool flag_dero_home;
    bool flag_m3_trans_home;
    bool flag_dero_amp;
    bool flag_m3_trans_amp;
    bool flag_dero_soft_limit_n;
    bool flag_dero_soft_limit_p;
    bool flag_dero_error;
    bool flag_m3_trans_brake;
    bool flag_m3_trans_safe_pos;
    bool flag_dero_loop_status;
    bool flag_m3_trans_loop_status;
    bool flag_m3_rot_pos_p;
    bool flag_m3_rot_pos_n;
    bool flag_m3_rot_pos;
    bool flag_fiber_switch;
};
*/
struct StatusADC
{
        double  AdcPosReal_Degree;
        double  AdcData[N_NUMBER][6];
        QString AdcDataTime[N_NUMBER];
        double  AdcSeconds;
        int 	AdcPointer;
        double  Adc_point;
        double  Adc_point_speed;
        double	AdcHead[N_NUMBER][8];
        double  AdcVelocity;
};

struct StatusDero
{
        double  DeroPosReal_Degree;
        double  DeroData[N_NUMBER][6];
        QString DeroDataTime[N_NUMBER];
        double  DeroSeconds;
        int 	DeroPointer;
        double  Dero_point;
        double  Dero_point_speed;
        double	DeroHead[N_NUMBER][8];
        double  DeroVelocity;
};

struct StatusM3
{
        double  M3PosReal_Degree;
        double  M3Data[N_NUMBER][6];
        QString M3DataTime[N_NUMBER];
        double  M3Seconds;
        int 	M3Pointer;
        double  M3_point;
        double  M3_point_speed;
        double	M3Head[N_NUMBER][8];
        double  M3Velocity;
};

#endif // DEFINE_H
