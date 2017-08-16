class TimeAndCoordinate
{
public:
	void Actual2Epoch(const double inEpoch, double inRA, double inDEC,double& outRA, double& outDEC) ;
        void Epoch2Actual(double inRA,double inDEC,double outEpoch,double &outRA, double &outDEC);
        double GetEpoch(int Year,int Month,int Day,int Hour,int Minute,int Second,int MicroSec);
        double GetLMST(double Lgt,double Lat,int Year,int Month,int Day,int Hour,int Minute,int Second,int MicroSec);
        void AltAzi2Eq(double Azi, double Alt, double LMST, double Lgt, double Lat, double &RA, double &DEC);
        void Eq2AltAzi(double RA,double DEC,double LMST,double Lgt,double Lat,int Focus,double &Azi,double &Alt,double &Rot);

				/** ���Ӹ߶ȽǼ�����߶Ƚ�, �����ڹ�ѧ����(�ƹ�)
				 * \param[out] refraction  ������, ����: ����
				 * \param[in]  vAlt        �Ӹ߶Ƚ�, ����: ����
				 * \param[in]  presure     ��ѹ, ����: ����
				 * \param[in]  temperature �¶�, ����: ���϶�
				 * \return
				 * ��߶Ƚ�, ����: ����
				 * \note
				 * ����: 100��
				 **/
        double AltVisual2Real(double &refraction, const double vAlt, const double presure, const double temperature);
};

