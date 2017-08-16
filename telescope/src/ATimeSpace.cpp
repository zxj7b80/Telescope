/*
 * ATimeSpace.cpp
 *
 */
#include "ADefine.h"
#include "ATimeSpace.h"
#include "AMath.h"

namespace AstroUtil
{
///////////////////////////////////////////////////////////////////////////////
ATimeSpace::ATimeSpace()
{
}

ATimeSpace::~ATimeSpace()
{
}

void ATimeSpace::Hour2HMS(const double hour, int &hh, int &mm, double &ss)
{
	double t = hour;
	hh = (int) t;
	t = (t - hh) * 60;
	mm = (int) t;
	ss = (t - mm) * 60;
}

double ATimeSpace::HMS2Hour(const int hh, const int mm, const double ss)
{
	return (hh + mm / 60.0 + ss/ 3600.0);
}

void ATimeSpace::Deg2DMS(const double deg, int &dd, int &mm, double &ss, int &sign)
{
	double t;
	sign = deg < 0 ? -1 : 1;
	dd = (int) (t = deg * sign);
	t = (t - dd) * 60;
	mm = (int) t;
	ss = (t - mm) * 60;
}

double ATimeSpace::DMS2Deg(const int sign, const int dd, const int mm, const double ss)
{
	return (dd + mm / 60.0 + ss / 3600.0) * sign;
}

bool ATimeSpace::RAStr2Dbl(const char *pszVal, double &val)
{
	if (pszVal == NULL || strlen(pszVal) == 0) return false;
	int len = strlen(pszVal);
	char *buff = new char[len + 1];
	char ch;
	int i, ii;
	double temp;
	bool bDot = false;
	int part = 0;

	for (i = 0, ii = 0, val = 0; i < len; ++i) {// scan input string
		if ((ch = pszVal[i]) >= '0' && ch <= '9') {// is digit
			if (ii == 2 && !bDot) {// scan two sequential digits
				if (part < 2) {
					buff[ii] = 0;
					ii = 0;
					temp = atof(buff);
					if (part == 0 && temp >= 0 && temp < 24) // hour complete
						val = temp;
					else if (part == 1 && temp >= 0) // minute complete
						val += temp / 60.0;
					else return false;

					part++;
				}
				else {
					bDot = true;
					buff[ii++] = '.';
				}
			}
			buff[ii++] = ch;
		}
		else if (ch == ':' || ch ==' ') {// scan separator
			if (bDot || part == 2) // more separator in second section
				return false;
			if (ii != 0) {
				buff[ii] = 0;
				ii = 0;
				temp = atoi(buff);
				if (part == 0 && temp >= 0 && temp < 24) val = temp;
				else if (part == 1 && temp >= 0)         val += temp / 60.0;
				else                                     return false;
			}
			part++;
		}
		else if (ch == '.') {
			if(bDot) break;
			bDot = true;
			buff[ii++] = '.';
		}
		else break;
	}

	buff[ii] = 0;
	if (ii > 1 || (ii == 1 && buff[0] != '.')) {
		temp = atof(buff);
		if (part == 0 && temp >= 0 && temp < 24) val = temp;
		else if (part == 1 && temp >= 0)         val += temp / 60;
		else if (part == 2 && temp >= 0)         val += temp / 3600;
	}
	delete []buff;

	return (part <= 2);
}

bool ATimeSpace::DECStr2Dbl(const char *pszVal, double &val)
{
	if(pszVal == NULL || strlen(pszVal) == 0) return false;
	int len = strlen(pszVal);
	char *buff;
	int i, ii;
	char ch;
	double temp;
	bool bDot = false;
	bool bFlag = false;
	int part = 0;

	i = 0;
	switch (ch = pszVal[0])
	{
	case '+':
		i = 1;
		break;
	case '-':
		bFlag = true;
		i = 1;
		break;
	default:
		if(!isdigit(ch) && ch != '.' && ch != ':' && ch != ' ') return false;
		break;
	}

	buff = new char[len + 1];
	for (ii = 0, temp = 0; i < len; ++i) {
		if ((ch = pszVal[i]) >= '0' && ch <= '9') {
			if (ii == 2 && !bDot) {
				if (part < 2) {
					buff[ii] = 0;
					ii = 0;
					temp = atof(buff);

					if(part == 0)                   val = temp;
					else if (part == 1 && val >= 0) val += temp / 60.0;
					else                            return false;

					part++;
				}
				else {
					bDot = true;
					buff[ii++] = '.';
				}
			}

			buff[ii++] = ch;
		}
		else if (ch == ':' || ch ==' ') {
			if (bDot || part == 2) // more separator in second section
				return false;
			if (ii != 0) {
				buff[ii] = 0;
				ii = 0;
				temp = atoi(buff);
				if (part == 0)                  val = temp;
				else if (part == 1 && val >= 0) val += temp / 60.0;
				else                            return false;
			}
			part++;
		}
		else if (ch == '.') {
			if (bDot) return false;
			bDot = true;
			buff[ii++] = '.';
		}
		else return false;
	}

	buff[ii] = 0;
	if (ii > 1 || (ii == 1 && buff[0] != '.')) {
		temp = atof(buff);
		if(part == 0)                   val = temp;
		else if (part == 1 && val >= 0) val += temp / 60;
		else if (part == 2 && val >= 0) val += temp / 3600;
	}
	if(bFlag) val = -val;

	delete []buff;
	return (part <= 2);
}

void ATimeSpace::RADbl2Str(const double val, char *pszVal)
{
	int hh, mm;
	double ss;
	Hour2HMS(val, hh, mm, ss);
	sprintf(pszVal, "%02d:%02d:%06.3f", hh, mm, ss);
}

void ATimeSpace::DECDbl2Str(const double val, char *pszVal)
{
	int dd, mm, sign;
	double ss;
	Deg2DMS(val, dd, mm, ss, sign);
	if (sign == 1) sprintf(pszVal, "+%02d:%02d:%05.2f", dd, mm, ss);
	else           sprintf(pszVal, "-%02d:%02d:%05.2f", dd, mm, ss);
}

double ATimeSpace::GetEpoch(const int year, const int month, const int day, const double hour)
{
	return 2000 + (JulianDay(year, month, day, hour) - 2451545) / 365.25;
}

double ATimeSpace::GetEpoch(double mjd)
{
	return 2000 + (mjd - 51544.5) / 365.25;
}

double ATimeSpace::Epoch2JC(double epoch)
{
	return (epoch - 2000) * 0.01;
}

double ATimeSpace::JulianDay(int year, int month, int day, double hour)
{
	int a = (14 - month) / 12;
	int y = year + 4800 - a;
	int m = month + 12 * a - 3;
	int JDN = day + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045;
	return (JDN + hour / 24 - 0.5);
}

double ATimeSpace::ModifiedJulianDay(int nYear, int nMonth, int nDay, double vHour)
{
	return (JulianDay(nYear, nMonth, nDay, vHour) - 2400000.5);
}

void ATimeSpace::JD2YMDH(double jd, int &year, int &month, int &day, double &hour)
{
	int jdn = (int) (jd + 0.5);
	int f = jdn + 1401 + (((4 * jdn + 274277) / 146097) * 3) / 4 - 38;
	int e = 4 * f + 3;
	int g = (e % 1461) / 4;
	int h = 5 * g + 2;
	day   = (h % 153) / 5 + 1;
	month = (h / 153 + 2) % 12 + 1;
	year  = e / 1461 - 4716 + (14 - month) / 12;
	hour  = (jd - jdn) * 24;
}
///////////////////////////////////////////////////////////////////////////////
void ATimeSpace::PMatEqu(double T1, double T2)
{
	double dt = T2 - T1;
	double Z, Zeta, Theta;
	const double sec = 3600;
	double c1, c2, c3;	// Z, Zeta, Theta对应的余弦项
	double s1, s2, s3;	// Z, Zeta, Theta对应的正弦项

	Zeta = ((2306.2181 + (1.39656 - 0.000139 * T1) * T1)
		+ ((0.30188 - 0.000345 * T1) + 0.017998 * dt) * dt) * dt / sec;
	Z = Zeta + ((0.7928 + 0.000411 * T1) + 0.000205 * dt) * dt * dt / sec;
	Theta = ((2004.3109 - (0.8533 + 0.000217 * T1) * T1)
		- ((0.42665 + 0.000217 * T1) + 0.041833 * dt) * dt) * dt / sec;
	Z *= GtoR;
	Zeta *= GtoR;
	Theta*= GtoR;

	c1 = cos(Z);
	c2 = cos(Theta);
	c3 = cos(Zeta);
	s1 = sin(Z);
	s2 = sin(Theta);
	s3 = sin(Zeta);

	m_matPN[0][0] = -s1 * s3 + c1 * c2 * c3;
	m_matPN[0][1] = -s1 * c3 - c1 * c2 * s3;
	m_matPN[0][2] = -c1 * s2;
	m_matPN[1][0] = c1 * s3 + s1 * c2 * c3;
	m_matPN[1][1] = c1 * c3 - s1 * c2 * s3;
	m_matPN[1][2] = -s1 * s2;
	m_matPN[2][0] = s2 * c3;
	m_matPN[2][1] = -s2 * s3;
	m_matPN[2][2] = c2;
}

void ATimeSpace::PN_Matrix(double T1, double T2)
{
	PMatEqu(T1, T2);
	EqMean2True(T2, m_matPN[0][0], m_matPN[1][0], m_matPN[2][0]);
	EqMean2True(T2, m_matPN[0][1], m_matPN[1][1], m_matPN[2][1]);
	EqMean2True(T2, m_matPN[0][2], m_matPN[1][2], m_matPN[2][2]);
}

void ATimeSpace::PError(double &X, double &Y, double &Z)
{
	double U, V, W;

	U = m_matPN[0][0] * X + m_matPN[0][1] * Y + m_matPN[0][2] * Z;
	V = m_matPN[1][0] * X + m_matPN[1][1] * Y + m_matPN[1][2] * Z;
	W = m_matPN[2][0] * X + m_matPN[2][1] * Y + m_matPN[2][2] * Z;

	X = U;
	Y = V;
	Z = W;
}

void ATimeSpace::Aberrat(double JC, double &VX, double &VY, double &VZ)
{
	double l;
	double CL;

	l = PI360 * frac(0.27908 + 100.00214 * JC);
	CL = cos(l);
	VX = -0.0000994 * sin(l);
	VY = 0.0000912 * CL;
	VZ = 0.0000395 * CL;
}

void ATimeSpace::EqTrue2Mean(double JC, double &X, double &Y, double &Z)
{
	double dx, dy, dz;

	DeltaMean2True(JC, X, Y, Z, dx, dy, dz);
	X -= dx;
	Y -= dy;
	Z -= dz;
}

void ATimeSpace::EqMean2True(double JC, double &X, double &Y, double &Z)
{
	double dx, dy, dz;

	DeltaMean2True(JC, X, Y, Z, dx, dy, dz);
	X += dx;
	Y += dy;
	Z += dz;
}

void ATimeSpace::DeltaMean2True(double JC, double X, double Y, double Z, double &dx, double &dy, double &dz)
{
	double LS;
	double D;
	double F;
	double N;
	double EPS;
	double DPSI;
	double DEPS;
	double c;
	double s;

	LS = PI360 * frac(0.993133 + 99.997306 * JC);	// 平近点角
	D = PI360 * frac(0.827362 + 1236.853087 * JC);	//
	F = PI360 * frac(0.259089 + 1342.227826 * JC);
	N = PI360 * frac(0.347346 - 5.372447 * JC);
	EPS = 0.4090928 - 0.00022696 * JC;
	DPSI = (-17.2 * sin(N) - 1.319 * sin(2 * (F - D + N))
		- 0.227 * sin(2 * (F + N)) + 0.206 * sin(2 * N) + 0.143 * sin(LS)) / 3600 * GtoR;
	DEPS = (9.203 * cos(N) + 0.574 * cos(2 * (F - D + N))
		+ 0.098 * cos(2 * (F + N)) - 0.09 * cos(2 * N)) / 3600 * GtoR;
	c = DPSI * cos(EPS);
	s = DPSI * sin(EPS);
	dx = -(c * Y + s * Z);
	dy = (c * X - DEPS * Z);
	dz = (s * X + DEPS * Y);
}

void ATimeSpace::Actual2Epoch(double E0, double ra_a, double de_a, double E1, double &ra_e, double &de_e)
{
	double r;
	double X, Y, Z;
	double VX, VY, VZ;
	double T0 = Epoch2JC(E0);
	double T1 = Epoch2JC(E1);

	Aberrat(T0, VX, VY, VZ);
	Sphere2Cart(1.0, ra_a, de_a, X, Y, Z);
	X -= VX;
	Y -= VY;
	Z -= VZ;
	EqTrue2Mean(T0, X, Y, Z);
	PMatEqu(T0, T1);
	PError(X, Y, Z);
	Cart2Sphere(X, Y, Z, r, ra_e, de_e);
}

void ATimeSpace::Epoch2Actual(double E0, double ra_e, double de_e, double E1, double &ra_a, double &de_a)
{
	double r;
	double X, Y, Z;
	double VX, VY, VZ;
	double T0 = Epoch2JC(E0);
	double T1 = Epoch2JC(E1);

	PN_Matrix(T0, T1);
	Aberrat(T1, VX, VY, VZ);
	Sphere2Cart(1.0, ra_e, de_e, X, Y, Z);
	PError(X, Y, Z);
	X += VX;
	Y += VY;
	Z += VZ;
	Cart2Sphere(X, Y, Z, r, ra_a, de_a);
}

double ATimeSpace::GetGMST0(const double mjd)
{
	double t = (mjd - 51544.5) / 36525;
	double gmst0;

	gmst0 = reduce(100.46061837 + (36000.770053608 + (0.000387933 - t / 38710000) * t) * t, 360.0);
	return (gmst0 * GtoR);
}

double ATimeSpace::LocalMeanSiderialTime(const double mjd, const double longitude)
{
	double t = (mjd - 51544.5) / 36525;
	double gmst;	// 格林威治平恒星时
	double lmst;	// 本地平恒星时

	gmst = 280.46061837 + (13185000.77005374225 + (0.000387933 - t / 38710000) * t) * t;
	lmst = gmst * GtoR + longitude;

	return reduce(lmst, PI360);
}

void ATimeSpace::Eq2AltAzi(double ha, double dec, double latitude, double &azi, double &alt)
{
	double sinz, cosz;
	double sina, cosa;

	sinz = sin(latitude) * sin(dec) + cos(latitude) * cos(dec) * cos(ha);
	cosz = sqrt(1 - sinz * sinz);
	alt = atan2(sinz, cosz);

	sina = cos(dec) * sin(ha);
	cosa = sin(latitude) * cos(dec) * cos(ha) - cos(latitude) * sin(dec);
	azi = reduce(atan2(sina, cosa) + API, PI360);
}

void ATimeSpace::AltAzi2Eq(double azi, double alt, double latitude, double &ha, double &dec)
{
	double z, sinz, cosz;
	double sint, cost;

	z = API * 0.5 - alt;
	sinz = sin(latitude) * cos(z) + cos(latitude) * sin(z) * cos(azi);
	cosz = sqrt(1 - sinz * sinz);
	dec = atan2(sinz, cosz);

	sint = -1 * sin(z) * sin(azi);
	cost = cos(latitude) * cos(z) - sin(latitude) * sin(z) * cos(azi);
	ha = reduce(atan2(sint, cost), PI360);
}

double ATimeSpace::ParAngle(double ha, double dec, double latitude, char mode)
{
	double y = sin(ha);
	double x = tan(latitude) * cos(dec) - sin(dec) * cos(ha);
	double q = atan2(y, x);
	double azi(0), alt(0);
	if(mode != 'N') {
		Eq2AltAzi(ha, dec, latitude, azi, alt);
		if(mode == '+') q += alt;
		else if(mode == '-') q -= alt;
	}

	return reduce(q, PI360);
}

double ATimeSpace::RefractReal(const double vAlt, const double pressure, const double temperature)
{
	double arcdeg = vAlt + 10.3 * GtoR / (vAlt * RtoG + 5.11);
	double R = 1.02 * cot(arcdeg) * GtoR / 60;
	double scale = pressure * 283 / 1010 / (273 + temperature);
	return scale * R;
}

double ATimeSpace::RefractVisual(const double vAlt, const double pressure, const double temperature)
{
	double arcdeg = vAlt + 7.31 * GtoR / (vAlt * RtoG + 4.4);
	double R = cot(arcdeg) * GtoR / 60;
	double scale = pressure * 283 / 1010 / (273 + temperature);
	return scale * R;
}

double ATimeSpace::AirMass(const double vAlt)
{
	double x = vAlt + 244.0 * GtoR / (165 + 47 * pow(vAlt * RtoG, 1.1));
	return 1.0 / sin(x);
}
///////////////////////////////////////////////////////////////////////////////
}
