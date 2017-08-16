/*
 * class ATimeSpace 常用天文时空相关转换接口
 * Version：0.2
 *
 * 功能列表:
 * 1. 赤经/赤纬格式转换: 字符串<->实数
 * 2. 时间/角度格式转换: 实数<->时/度, 分, 秒
 *
 * 创建时间: 2012年3月1日
 * 完成时间: 2012年3月10日
 * 作者: 卢晓猛, lxm@nao.cas.cn
 */

#ifndef _ATIMESPACE_H_
#define _ATIMESPACE_H_

namespace AstroUtil {
///////////////////////////////////////////////////////////////////////////////
class ATimeSpace
{
public:
	ATimeSpace();
	virtual ~ATimeSpace();

protected:
	// 赤道坐标系岁差与章动系数, 3x3矩阵
	double m_matPN[3][3];

// Function
public:
    /*
	 * \fn bool RAStr2Dbl(const char *pszVal, double &val)
	 * \brief Transfer R.A. from string to double
	 * \param[in] pszVal R.A. in string style
	 * \param[out] val    R.A. in double, in hour
	 * \return
	 * if pszVal format is right then transfer it to be double and return true, or else return false
	 * \note
	 * pszVal could be the following style:
	 * HH:MM:SS.SS, colon could be omitted or replaced by space, and both MM and SS could be omitted too.
	 */
	bool RAStr2Dbl(const char *pszVal, double &val);
    /*
	 * \fn bool DECStr2Dbl(const char *pszVal, double &val)
	 * \brief Transfer DEC. from string to double
	 * \param[in] pszVal DEC. in string style
	 * \param[out] val    DEC. in double, in degree
	 * \return
	 * if pszVal format is right then transfer it to be double and return true, or else return false
	 * \note
	 * pszVal could be the following style:
	 * sDD:MM:SS.SS, colon could be omitted or replaced by space, and both MM and SS could be omitted too.
	 * 's' means plus or minus sign
	 */
	bool DECStr2Dbl(const char *pszVal, double &val);
    /*
	 * \fn bool RADbl2Str(const double val, char *pszVal)
	 * \brief Transfer R.A. from double to string
	 * \param[in]    val      R.A. in double, in hour
	 * \param[out] pszVal R.A. in string style
	 * \return
	 * regulate val to be between 0 and 24 and transfer it to be string style
	 * \note
	 * pszVal could be the following style:
	 * HH:MM:SS.SS, colon could be omitted or replaced by space, and both MM and SS could be omitted too.
	 */
	void RADbl2Str(const double val, char *pszVal);
    /*
	 * \fn bool DECDbl2Str(const double val, char *pszVal)
	 * \brief Transfer DEC. from double to string
	 * \param[in]    val      DEC. in double, in hour
	 * \param[out] pszVal DEC in string style
	 * \return
	 * regulate val to be between -90 and +90 and transfer it to be string style
	 * \note
	 * pszVal could be the following style:
	 * sDD:MM:SS.SS, colon could be omitted or replaced by space, and both MM and SS could be omitted too.
	 * 's' means plus or minus sign
	 */
	void DECDbl2Str(const double val, char *pszVal);
    /*
	 * \fn void Hour2HMS(const double hour, int &hh, int &mm, double &ss)
	 * \brief resolve double hour to hour, minute and second
	 * \param[in] hour double time, in hour
	 * \param[out] hh  hour
	 * \param[out] mm  minute
	 * \param[out] ss  second
	 */
	void Hour2HMS(const double hour, int &hh, int &mm, double &ss);
    /*
	 * \fn double HMS2Hour(const int hh, const int mm, const double ss)
	 * \brief construct hour, minute and second to double hour
	 * \param[in] hh  hour
	 * \param[in] mm  minute
	 * \param[in] ss  second
	 * \return
	 * double hour
	 */
	double HMS2Hour(const int hh, const int mm, const double ss);
    /*
	 * \fn void Deg2DMS(const double deg, int &dd, int &mm, double &ss, int &sign)
	 * \brief resolve double degree to degree, minute and second
	 * \param[in] deg double degree, in degree
	 * \param[out] dd  degree
	 * \param[out] mm  minute
	 * \param[out] ss  second
	 * \param[out] sign plus or minus sign. if deg is less than 0 then sign is -1, or else 1
	 */
	void Deg2DMS(const double deg, int &dd, int &mm, double &ss, int &sign);
    /*
	 * \fn double DMS2Deg(const int sign, const int dd, const int mm, const double ss)
	 * \brief construct degree, minute and second to double degree
	 * \param[in] sign plus or minus sign. if deg is less than 0 then sign is -1, or else 1
	 * \param[in] dd  degree
	 * \param[in] mm  minute
	 * \param[in] ss  second
	 * \return
	 * double degree
	 */
	double DMS2Deg(const int sign, const int dd, const int mm, const double ss);


    /*
	 * \brief calculate epoch referred by year\month\day\hour
	 */
	double GetEpoch(const int year, const int month, const int day, const double hour = 0.0);
    /*
	 * @brief calculate epoch from modified julian day
	 */
	double GetEpoch(double mjd);
    /* \fn double Epoch2JC(const double epoch)
	 *  \brief 根据历元计算对应的儒略纪元.
	 *  \param[in] epoch 由函数GetEpoch()计算或者用户输入的历元
	 *  \return epoch对应的儒略纪元. 从J2000开始计算
     */
	double Epoch2JC(double epoch);
    /*
	 * @brief 由格里高利历计算儒略日
	 *  \param[in] year  年
	 *  \param[in] month 月
	 *  \param[in] day   日
	 *  \param[in] hour  当日小时数
	 * @return
	 * 公元历对应的儒略日
	 */
	double JulianDay(int year, int month, int day, double hour);
    /* \fn double ModifiedJulianDay(int nYear, int nMonth, int nDay, double vHour)
	 *  \brief 计算修正儒略日
	 *  \param[in] nYear 年
	 *  \param[in] nMonth 月
	 *  \param[in] nDay 日
	 *  \param[in] vHour 时
	 *  \return 年月日时对应的修正儒略日
	 **/
	double ModifiedJulianDay(int nYear, int nMonth, int nDay, double vHour);
    /*
	 * @brief 由儒略日计算对应的公元历
	 * @param jd    儒略日
	 * @param year  年
	 * @param month 月
	 * @param day   日
	 * @param hour  当日小时数
	 */
	void JD2YMDH(double jd, int &year, int &month, int &day, double &hour);


    /* \fn void PMatEqu(double T1, double T2)
	 *  \brief 根据儒略世纪计算岁差系数.
	 *  \param[in] T1 儒略世纪
	 *  \param[in] T2 儒略世纪
	 *
	 * \note
	 * 如果已经知道T1历元对应的赤道坐标位置, 需要计算T2历元时的坐标位置. 使用本函数
	 * 计算历元变化对目标位置的影响.<br>
	 * 本函数计算的结果, 并不是已经分解到赤经/赤纬轴的坐标修正量, 而是岁差系数.<br>
	 * 本函数计算的结果, 存储在类的矩阵m_matPN中。
	 **/
	void PMatEqu(double T1, double T2);
    /* \fn void PN_Matrix(double T1, double T2)
	 *  \brief 计算从平历元T1到真历元T2的岁差系数.
	 *  \param[in] T1 儒略世纪
	 *  \param[in] T2 儒略世纪
	 **/
	void PN_Matrix(double T1, double T2);
    /* \fn void PError(double& X, double& Y, double &Z)
	 *  \brief 计算岁差带来的偏差
	 *  \param X X轴坐标
	 *  \param Y Y轴坐标
	 *  \param Z Z轴坐标
	 **/
	void PError(double &X, double &Y, double &Z);
    /* \fn void Aberrat(double JC, double& VX, double& VY, double& VZ)
	 *  \brief 赤道坐标中, 地球运动速度矢量
	 *  \param[in] JC 儒略世纪
	 *  \param[out] VX X轴速度, 单位：光速
	 *  \param[out] VY Y轴速度, 单位: 光速
	 *  \param[out] VZ Z轴速度, 单位: 光速
	 **/
	void Aberrat(double JC, double &VX, double &VY, double &VZ);
    /* \fn void EqTrue2Mean(double JC, double& X, double& Y, double& Z)
	 *  \brief 从真坐标转换为平坐标
	 *  \param[in] JC 儒略世纪
	 *  \param X X轴坐标
	 *  \param Y Y轴坐标
	 *  \param Z Z轴坐标
	 **/
	void EqTrue2Mean(double JC, double &X, double &Y, double &Z);
    /* \fn void EqMean2True(double JC, double& X, double& Y, double& Z)
	 *  \brief 从平坐标转换为真坐标
	 *  \param[in] JC 儒略世纪
	 *  \param X X轴坐标
	 *  \param Y Y轴坐标
	 *  \param Z Z轴坐标
	 **/
	void EqMean2True(double JC, double &X, double &Y, double &Z);
    /*  \fn void DeltaMean2True(double JC, double X, double Y, double Z, double& dx, double& dy, double& dz)
	 *   \brief 真坐标与平坐标的差值
	 *   \param[in] JC 儒略世纪
	 *   \param[in] X 笛卡尔(直角)坐标系X轴坐标
	 *   \param[in] Y 笛卡尔(直角)坐标系Y轴坐标
	 *   \param[in] Z 笛卡尔(直角)坐标系Z轴坐标
	 *   \param[out] dx X轴差值
	 *   \param[out] dy Y轴差值
	 *   \param[out] dz Z轴差值
	 **/
	void DeltaMean2True(double JC, double X, double Y, double Z, double &dx, double &dy, double &dz);


    /* \fn void Actual2Epoch(double E0, double ra_a, double de_a, double E1, double &ra_e, double &de_e)
	 *  \brief 从当前历元坐标转换为指定历元坐标
	 *  \param[in]  E0   当前西元历元
	 *  \param[in]  ra_a 当前历元下的赤经, 量纲: 弧度
	 *  \param[in]  de_a 当前历元下的赤纬, 量纲: 弧度
	 *  \param[in]  E1   指定西元历元
	 *  \param[out] ra_e 指定历元下的赤经, 量纲: 弧度
	 *  \param[out] de_e 指定历元下的赤纬, 量纲: 弧度
	 **/
	void Actual2Epoch(double E0, double ra_a, double de_a, double E1, double &ra_e, double &de_e);
    /* \fn void Epoch2Actual(double T0, double ra_e, double de_e, double T1, double &ra_a, double &de_a)
	 *  \brief 从指定历元坐标转换为当前历元坐标
	 *  \param[in]  E0   指定西元历元
	 *  \param[in]  ra_a 指定历元下的赤经, 量纲: 弧度
	 *  \param[in]  de_a 指定历元下的赤纬, 量纲: 弧度
	 *  \param[in]  E1   西元历元
	 *  \param[out] ra_e 当前指定历元下的赤经, 量纲: 弧度
	 *  \param[out] de_e 当前指定历元下的赤纬, 量纲: 弧度
	 **/
	void Epoch2Actual(double E0, double ra_e, double de_e, double E1, double &ra_a, double &de_a);


    /*
	 * \fn double GetGMST0(const double mjd)
	 * \brief 根据修正儒略日计算格林威治0时的平恒星时.
	 * \param[in] mjd 修正儒略日
	 * \return 以弧度为单位的平恒星时
	 */
	double GetGMST0(const double mjd);
    /*
	 * \fn double LocalMeanSiderialTime(const double mjd)
	 * \brief 根据修正儒略日计算本地平恒星时.
	 * \param[in] mjd 由ModifiedJulianDay计算得到的修正儒略日
	 * \param[in] longitude 地理经度, 量纲: 弧度
	 * \return 以弧度为单位的本地平恒星时
	 */
	double LocalMeanSiderialTime(const double mjd, const double longitude);


    /*
	 * \fn void Eq2AltAzi(double ha, double dec, double latitude, double &azi, double &alt)
	 * \brief 赤道坐标转换为地平坐标
	 * \param[in]  ha  时角, 量纲: 弧度
	 * \param[in]  dec 赤纬, 量纲: 弧度
	 * \param[in]  latitude 地理纬度, 量纲: 弧度
	 * \param[out] azi 方位角, 量纲: 弧度, 北零点
	 * \param[out] alt 俯仰角, 量纲: 弧度
	 **/
	void Eq2AltAzi(double ha, double dec, double latitude, double &azi, double &alt);
    /*
	 * \fn void AltAzi2Eq(double azi, double alt, double latitude, double &ha, double &dec)
	 * \brief 地平坐标转换为赤道坐标
	 * \param[in] azi 方位角, 量纲: 弧度, 北零点
	 * \param[in] alt 俯仰角, 量纲: 弧度
	 * \param[in] latitude 地理纬度, 量纲: 弧度
	 * \param[out] ha  时角, 量纲: 弧度
	 * \param[out] dec 赤纬, 量纲: 弧度
	 **/
	void AltAzi2Eq(double azi, double alt, double latitude, double &ha, double &dec);
    /*
	 * \fn double ParAngle(double ha, double dec, double latitude, char mode)
	 * \brief 计算地平式望远镜抵消地平自转, 消转器的转动弧度
	 * \param[in] ha   时角, 量纲: 弧度
	 * \param[in] dec  赤纬, 量纲: 弧度
	 * \param[in] latitude 地理纬度, 量纲: 弧度
	 * \param[in] mode 卡塞格林焦点, mode='N'
	 *                 耐氏焦点, 东边焦点mode='+'
	 *                 西边焦点mode='-'
	 * \return 抵消地球自转需要的弧度
	 **/
	double ParAngle(double ha, double dec, double latitude, char mode);
    /* \fn double RefractReal(const double vAlt, const double pressure, const double temperature)
	 *  \brief 计算大气折射
	 *  \param[in]  vAlt        真高度角, 量纲: 弧度
	 *  \param[in]  presure     气压, 量纲: 毫巴
	 *  \param[in]  temperature 温度, 量纲: 摄氏度
	 *  \return
	 *  大气折射, 量纲: 弧度
	 *  \note
	 *  毫巴: 100帕
	 **/
	double RefractReal(const double vAlt, const double pressure, const double temperature);
    /* \fn double RefractVisual(const double vAlt, const double pressure, const double temperature)
	 *  \brief 计算大气折射
	 *  \param[in]  vAlt        视高度角, 量纲: 弧度
	 *  \param[in]  presure     气压, 量纲: 毫巴
	 *  \param[in]  temperature 温度, 量纲: 摄氏度
	 *  \return
	 *  大气折射, 量纲: 弧度
	 *  \note
	 *  毫巴: 100帕
	 **/
	double RefractVisual(const double vAlt, const double pressure, const double temperature);
    /* \fn double AirMass(const double vAlt)
	 *  \brief 计算大气质量
	 *  \param[in] vAlt 视高度角, 量纲: 弧度
	 *  \return 大气质量
	 *  \note
	 *  大气质量计算公式采用Pickering(2002)模型, 其公式为:<br>
	 *  \f$X = \frac{1}{sin\left(h + \frac{244}{165 + 47 \times \mbox{h}^{1.1}}\right)} \f$<br>
	 *  上式中, h为视高度角, 量纲为角度
	 **/
	double AirMass(const double vAlt);
};
///////////////////////////////////////////////////////////////////////////////
}

#endif /* _ATIMESPACE_H_ */
