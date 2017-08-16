/**
 Name        : AMath.h 常用数学函数声明文件
 Author      : Xiaomeng Lu
 Version     : 0.1
 Date        : Oct 13, 2012
 Last Date   : Oct 13, 2012
 Description : 天文数字图像处理中常用的数学函数
 **/
#ifndef _AMATH_H_
#define _AMATH_H_

#include <math.h>

namespace AstroUtil
{
// 计算实数的小数部分
#define frac(x)		((x) - floor(x))
// 将具有周期性的实数调整到一个周期内, 该周期的最小值为０
#define reduce(x, period)	((x) - floor((x) / (period)) * (period))

/*!
 * \brief 将球坐标转换为笛卡尔坐标. 默认为右手坐标系, 且XY对应球坐标基平面, Z为极轴.
 *        alpha从X轴正向逆时针增加, X轴对应0点; beta为与XY平面的夹角, Z轴正向为正.
 *        此转换中球坐标与赤道坐标系相同.
 *        若球坐标为左手坐标系, 则alpha应取坐标系数值的负值
 * \param[in] r     位置矢量的模. 对于天球坐标系取为1
 * \param[in] alpha alpha角, 量纲: 弧度
 * \param[in] beta  beta角, 量纲: 弧度
 * \param[out] x    X轴坐标
 * \param[out] y    Y轴坐标
 * \param[out] z    Z轴坐标
 **/
extern void Sphere2Cart(double r, double alpha, double beta, double& x, double& y, double& z);
/*!
 * \fn void Cart2Sphere(double r, double alpha, double beta, double& x, double& y, double& z)
 * \brief 将笛卡尔坐标转换为球坐标. 默认为右手坐标系, 且XY对应球坐标基平面, Z为极轴.
 *        alpha从X轴正向逆时针增加, X轴对应0点; beta为与XY平面的夹角, Z轴正向为正.
 *        此转换中球坐标与赤道坐标系相同.
 *        若球坐标为左手坐标系, 则alpha为坐标系数值的负值
 * \param[in] x    X轴坐标
 * \param[in] y    Y轴坐标
 * \param[in] z    Z轴坐标
 * \param[out] r     位置矢量的模. 对于天球坐标系取为1
 * \param[out] alpha alpha角, 量纲: 弧度
 * \param[out] beta  beta角, 量纲: 弧度
 **/
extern void Cart2Sphere(double x, double y, double z, double& r, double& alpha, double& beta);
}
#endif
