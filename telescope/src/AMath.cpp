/**
 Name        : AMath.cpp 常用数学函数定义文件
 Author      : Xiaomeng Lu
 Version     : 0.1
 Date        : Oct 13, 2012
 Last Date   : Oct 13, 2012
 Description : 天文数字图像处理中常用的数学函数
 **/
#include "ADefine.h"
#include "AMath.h"

namespace AstroUtil
{
/*---------------------------------------------------------------------------*/
///////////////////////////////////////////////////////////////////////////////
/*------------------------------- 部分球坐标转换 -------------------------------*/
void Sphere2Cart(double r, double alpha, double beta, double& x, double& y, double& z)
{
	x = r * cos(beta) * cos(alpha);
	y = r * cos(beta) * sin(alpha);
	z = r * sin(beta);
}

void Cart2Sphere(double x, double y, double z, double& r, double& alpha, double& beta)
{
	r = sqrt(x * x + y * y + z * z);
	if (fabs(y) < AEPS && fabs(x) < AEPS) alpha = 0;
	else if ((alpha = atan2(y, x)) < 0) alpha += PI360;
	beta  = atan2(z, sqrt(x * x + y * y));
}
/*---------------------------------------------------------------------------*/
}
