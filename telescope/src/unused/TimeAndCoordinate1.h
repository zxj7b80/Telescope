class TimeAndCoordinate
{
public:
	void Actual2Epoch(const double inEpoch, double inRA, double inDEC,double& outRA, double& outDEC) ;
        void Epoch2Actual(double inRA,double inDEC,double outEpoch,double &outRA, double &outDEC);
        double GetEpoch(int Year,int Month,int Day,int Hour,int Minute,int Second,int MicroSec);
        double GetLMST(double Lgt,double Lat,int Year,int Month,int Day,int Hour,int Minute,int Second,int MicroSec);
        void AltAzi2Eq(double Azi, double Alt, double LMST, double Lgt, double Lat, double &RA, double &DEC);
        void Eq2AltAzi(double RA,double DEC,double LMST,double Lgt,double Lat,int Focus,double &Azi,double &Alt,double &Rot);

				/** 由视高度角计算真高度角, 适用于光学波段(黄光)
				 * \param[out] refraction  蒙气差, 量纲: 弧度
				 * \param[in]  vAlt        视高度角, 量纲: 弧度
				 * \param[in]  presure     气压, 量纲: 毫巴
				 * \param[in]  temperature 温度, 量纲: 摄氏度
				 * \return
				 * 真高度角, 量纲: 弧度
				 * \note
				 * 毫巴: 100帕
				 **/
        double AltVisual2Real(double &refraction, const double vAlt, const double presure, const double temperature);
};

