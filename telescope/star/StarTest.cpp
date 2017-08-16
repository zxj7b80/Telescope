// StarTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>
//#include<string.h>
//#include <time.h>
#include<math.h>
#include "astro.h"
#include "circum.h"

/* Delaunay arguments, in arc seconds; they differ slightly from ELP82B */
static double delaunay[5][4] = {
    {485866.733,  1717915922.633, 31.310,  0.064}, /* M', moon mean anom */
    {1287099.804, 129596581.224,  -0.577, -0.012}, /* M, sun mean anom */
    {335778.877,  1739527263.137, -13.257, 0.011}, /* F, moon arg lat */
    {1072261.307, 1602961601.328, -6.891,  0.019}, /* D, elong moon sun */
    {450160.280,  -6962890.539,   7.455,   0.008}, /* Om, moon l asc node */
};

/* multipliers for Delaunay arguments */
static short multarg[NUT_SERIES][5] = {
	/* bounds:  -2..3, -2..2, -2/0/2/4, -4..4, 0..2 */
    {0, 0, 0, 0, 1},
    {0, 0, 0, 0, 2},
    {-2, 0, 2, 0, 1},
    {2, 0, -2, 0, 0},
    {-2, 0, 2, 0, 2},
    {1, -1, 0, -1, 0},
    {0, -2, 2, -2, 1},
    {2, 0, -2, 0, 1},
    {0, 0, 2, -2, 2},
    {0, 1, 0, 0, 0},
    {0, 1, 2, -2, 2},
    {0, -1, 2, -2, 2},
    {0, 0, 2, -2, 1},
    {2, 0, 0, -2, 0},
    {0, 0, 2, -2, 0},
    {0, 2, 0, 0, 0},
    {0, 1, 0, 0, 1},
    {0, 2, 2, -2, 2},
    {0, -1, 0, 0, 1},
    {-2, 0, 0, 2, 1},
    {0, -1, 2, -2, 1},
    {2, 0, 0, -2, 1},
    {0, 1, 2, -2, 1},
    {1, 0, 0, -1, 0},
    {2, 1, 0, -2, 0},
    {0, 0, -2, 2, 1},
    {0, 1, -2, 2, 0},
    {0, 1, 0, 0, 2},
    {-1, 0, 0, 1, 1},
    {0, 1, 2, -2, 0},
    {0, 0, 2, 0, 2},
    {1, 0, 0, 0, 0},
    {0, 0, 2, 0, 1},
    {1, 0, 2, 0, 2},
    {1, 0, 0, -2, 0},
    {-1, 0, 2, 0, 2},
    {0, 0, 0, 2, 0},
    {1, 0, 0, 0, 1},
    {-1, 0, 0, 0, 1},
    {-1, 0, 2, 2, 2},
    {1, 0, 2, 0, 1},
    {0, 0, 2, 2, 2},
    {2, 0, 0, 0, 0},
    {1, 0, 2, -2, 2},
    {2, 0, 2, 0, 2},
    {0, 0, 2, 0, 0},
    {-1, 0, 2, 0, 1},
    {-1, 0, 0, 2, 1},
    {1, 0, 0, -2, 1},
    {-1, 0, 2, 2, 1},
    {1, 1, 0, -2, 0},
    {0, 1, 2, 0, 2},
    {0, -1, 2, 0, 2},
    {1, 0, 2, 2, 2},
    {1, 0, 0, 2, 0},
    {2, 0, 2, -2, 2},
    {0, 0, 0, 2, 1},
    {0, 0, 2, 2, 1},
    {1, 0, 2, -2, 1},
    {0, 0, 0, -2, 1},
    {1, -1, 0, 0, 0},
    {2, 0, 2, 0, 1},
    {0, 1, 0, -2, 0},
    {1, 0, -2, 0, 0},
    {0, 0, 0, 1, 0},
    {1, 1, 0, 0, 0},
    {1, 0, 2, 0, 0},
    {1, -1, 2, 0, 2},
    {-1, -1, 2, 2, 2},
    {-2, 0, 0, 0, 1},
    {3, 0, 2, 0, 2},
    {0, -1, 2, 2, 2},
    {1, 1, 2, 0, 2},
    {-1, 0, 2, -2, 1},
    {2, 0, 0, 0, 1},
    {1, 0, 0, 0, 2},
    {3, 0, 0, 0, 0},
    {0, 0, 2, 1, 2},
    {-1, 0, 0, 0, 2},
    {1, 0, 0, -4, 0},
    {-2, 0, 2, 2, 2},
    {-1, 0, 2, 4, 2},
    {2, 0, 0, -4, 0},
    {1, 1, 2, -2, 2},
    {1, 0, 2, 2, 1},
    {-2, 0, 2, 4, 2},
    {-1, 0, 4, 0, 2},
    {1, -1, 0, -2, 0},
    {2, 0, 2, -2, 1},
    {2, 0, 2, 2, 2},
    {1, 0, 0, 2, 1},
    {0, 0, 4, -2, 2},
    {3, 0, 2, -2, 2},
    {1, 0, 2, -2, 0},
    {0, 1, 2, 0, 1},
    {-1, -1, 0, 2, 1},
    {0, 0, -2, 0, 1},
    {0, 0, 2, -1, 2},
    {0, 1, 0, 2, 0},
    {1, 0, -2, -2, 0},
    {0, -1, 2, 0, 1},
    {1, 1, 0, -2, 1},
    {1, 0, -2, 2, 0},
    {2, 0, 0, 2, 0},
    {0, 0, 2, 4, 2},
    {0, 1, 0, 1, 0}
};

/* amplitudes which  have secular terms; in 1/NUT_SCALE arc seconds
 * {index, constant dPSI, T/10 in dPSI, constant in dEPS, T/10 in dEPS}
 */
static long ampsecul[][5] = {
    {0  ,-171996 ,-1742 ,92025 ,89},
    {1  ,2062    ,2     ,-895  ,5},
    {8  ,-13187  ,-16   ,5736  ,-31},
    {9  ,1426    ,-34   ,54    ,-1},
    {10 ,-517    ,12    ,224   ,-6},
    {11 ,217     ,-5    ,-95   ,3},
    {12 ,129     ,1     ,-70   ,0},
    {15 ,17      ,-1    ,0     ,0},
    {17 ,-16     ,1     ,7     ,0},
    {30 ,-2274   ,-2    ,977   ,-5},
    {31 ,712     ,1     ,-7    ,0},
    {32 ,-386    ,-4    ,200   ,0},
    {33 ,-301    ,0     ,129   ,-1},
    {37 ,63      ,1     ,-33   ,0},
    {38 ,-58     ,-1    ,32    ,0},
    /* termination */  { -1, }
};

/* amplitudes which only have constant terms; same unit as above
 * {dPSI, dEPS}
 * indexes which are already in ampsecul[][] are zeroed
 */
static short ampconst[NUT_SERIES][2] = {
    {0,0},
    {0,0},
    {46,-24},
    {11,0},
    {-3,1},
    {-3,0},
    {-2,1},
    {1,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {48,1},
    {-22,0},
    {0,0},
    {-15,9},
    {0,0},
    {-12,6},
    {-6,3},
    {-5,3},
    {4,-2},
    {4,-2},
    {-4,0},
    {1,0},
    {1,0},
    {-1,0},
    {1,0},
    {1,0},
    {-1,0},
    {0,0},
    {0,0},
    {0,0},
    {0,0},
    {-158,-1},
    {123,-53},
    {63,-2},
    {0,0},
    {0,0},
    {-59,26},
    {-51,27},
    {-38,16},
    {29,-1},
    {29,-12},
    {-31,13},
    {26,-1},
    {21,-10},
    {16,-8},
    {-13,7},
    {-10,5},
    {-7,0},
    {7,-3},
    {-7,3},
    {-8,3},
    {6,0},
    {6,-3},
    {-6,3},
    {-7,3},
    {6,-3},
    {-5,3},
    {5,0},
    {-5,3},
    {-4,0},
    {4,0},
    {-4,0},
    {-3,0},
    {3,0},
    {-3,1},
    {-3,1},
    {-2,1},
    {-3,1},
    {-3,1},
    {2,-1},
    {-2,1},
    {2,-1},
    {-2,1},
    {2,0},
    {2,-1},
    {1,-1},
    {-1,0},
    {1,-1},
    {-2,1},
    {-1,0},
    {1,-1},
    {-1,1},
    {-1,1},
    {1,0},
    {1,0},
    {1,-1},
    {-1,0},
    {-1,0},
    {1,0},
    {1,0},
    {-1,0},
    {1,0},
    {1,0},
    {-1,0},
    {-1,0},
    {-1,0},
    {-1,0},
    {-1,0},
    {-1,0},
    {-1,0},
    {1,0},
    {-1,0},
    {1,0}
};

static double vx_earth[][3] = {
	//earth l, T^0 
	{ 175347045.7, 0, 0 },
	{ 3341656.5, 4.66925680, 6283.07584999 },
	{ 34894.3, 4.626102, 12566.151700 },
	{ 3417.6, 2.82887, 3.52312 },
	{ 3497.1, 2.74412, 5753.38488 },
	{ 3135.9, 3.62767, 77713.77147 },
	{ 2676.2, 4.41808, 7860.41939 },
	{ 2342.7, 6.13516, 3930.20970 },
	{ 1273.2, 2.03710, 529.69097 },
	{ 1324.3, 0.74246, 11506.76977 },
	{ 901.9, 2.0451, 26.2983 },
	{ 1199.2, 1.10963, 1577.34354 },
	{ 857.2, 3.5085, 398.1490 },
	{ 779.8, 1.1788, 5223.6939 },
	{ 990.2, 5.2327, 5884.9268 },
	{ 753.1, 2.5334, 5507.5532 },
	{ 505.3, 4.5829, 18849.2275 },
	{ 492.4, 4.2051, 775.5226 },
	{ 356.7, 2.9195, 0.0673 },
	{ 284.1, 1.8987, 796.2980 },
	{ 242.8, 0.3448, 5486.7778 },
	{ 317.1, 5.8490, 11790.6291 },
	{ 271.0, 0.3149, 10977.0788 },
	{ 206.2, 4.8065, 2544.3144 },
	{ 205.4, 1.8695, 5573.1428 },
	{ 202.3, 2.4577, 6069.7768 },
	{ 126.2, 1.0830, 20.7754 },
	{ 155.5, 0.8331, 213.2991 },
	{ 115.1, 0.6454, 0.9803 },
	{ 102.9, 0.6360, 4694.0030 },
	{ 101.7, 4.2668, 7.1135 },
	{ 99.2, 6.210, 2146.165 },
	{ 132.2, 3.4112, 2942.4634 },
	{ 97.6, 0.681, 155.420 },
	{ 85.1, 1.299, 6275.962 },
	{ 74.7, 1.755, 5088.629 },
	{ 101.9, 0.9757, 15720.8388 },
	{ 84.7, 3.671, 71430.696 },
	{ 73.5, 4.679, 801.821 },
	{ 73.9, 3.503, 3154.687 },
	{ 78.8, 3.037, 12036.461 },
	{ 79.6, 1.808, 17260.155 },
	{ 85.8, 5.983, 161000.686 },
	{ 57.0, 2.784, 6286.599 },
	{ 61.1, 1.818, 7084.897 },
	{ 69.6, 0.833, 9437.763 },
	{ 56.1, 4.387, 14143.495 },
	{ 62.4, 3.978, 8827.390 },
	{ 51.1, 0.283, 5856.478 },
	{ 55.6, 3.470, 6279.553 },
	{ 41.0, 5.368, 8429.241 },
	{ 51.6, 1.333, 1748.016 },
	{ 52.0, 0.189, 12139.554 },
	{ 49.0, 0.487, 1194.447 },
	{ 39.2, 6.168, 10447.388 },
	{ 35.6, 1.776, 6812.767 },
	{ 36.8, 6.041, 10213.286 },
	{ 36.6, 2.570, 1059.382 },
	{ 33.3, 0.593, 17789.846 },
	{ 36.0, 1.709, 2352.866 },
	{ 40.9, 2.399, 19651.048 },
	{ 30.0, 2.740, 1349.867 },
	{ 30.4, 0.443, 83996.847 },
	{ 23.7, 0.485, 8031.092 },
	{ 23.6, 2.065, 3340.612 },
	{ 21.1, 4.148, 951.718 },
	{ 24.7, 0.215, 3.590 },
	{ 25.4, 3.165, 4690.480 },
	{ 22.8, 5.222, 4705.732 },
	{ 21.4, 1.426, 16730.464 },
	{ 21.9, 5.556, 553.569 },
	{ 17.5, 4.561, 135.065 },
	{ 19.9, 5.222, 12168.003 },
	{ 19.9, 5.775, 6309.374 },
	{ 20.3, 0.371, 283.859 },
	{ 14.4, 4.193, 242.729 },
	{ 16.2, 5.988, 11769.854 },
	{ 15.1, 4.196, 6256.778 },
	{ 19.1, 3.822, 23581.258 },
	{ 18.9, 5.386, 149854.400 },
	{ 14.3, 3.724, 38.028 },
	{ 17.9, 2.215, 13367.973 },
	{ 12.1, 2.622, 955.600 },
	{ 11.3, 0.177, 4164.312 },
	{ 14.0, 4.401, 6681.225 },
	{ 13.6, 1.889, 7632.943 },
	{ 12.5, 1.131, 5.523 },
	{ 10.5, 5.359, 1592.596 },
	{ 9.8, 1.00, 11371.70 },
	{ 9.2, 4.57, 4292.33 },
	{ 10.3, 6.200, 6438.496 },
	{ 12.0, 1.004, 632.784 },
	{ 10.8, 0.327, 103.093 },
	{ 8.4, 4.54, 25132.30 },
	{ 10.0, 6.029, 5746.271 },
	{ 8.4, 3.30, 7234.79 },
	{ 8.0, 5.82, 28.45 },
	{ 10.5, 0.939, 11926.254 },
	{ 7.7, 3.12, 7238.68 },
	{ 9.4, 2.62, 5760.50 },
	{ 8.1, 6.11, 4732.03 },
	{ 9.2, 0.48, 522.58 },
	{ 9.8, 5.24, 27511.47 },
	{ 7.9, 1.00, 5643.18 },
	{ 8.1, 6.27, 426.60 },
	{ 9.0, 5.34, 6386.17 },
	{ 8.6, 4.17, 7058.60 },
	{ 6.3, 4.72, 6836.65 },
	{ 7.6, 3.97, 11499.66 },
	{ 7.8, 2.96, 23013.54 },
	{ 7.3, 0.61, 11513.88 },
	{ 6.5, 5.79, 18073.70 },
	{ 7.2, 4.00, 74.78 },
	{ 7.3, 4.39, 316.39 },
	{ 7.1, 0.32, 263.08 },
	{ 6.6, 3.66, 17298.18 },
	{ 6.8, 5.91, 90955.55 },
	// 117 terms retained, 442 terms dropped, error 0.3" 

	// earth l, T^1 
	{ 628331966747.5, 0, 0 },
	{ 206058.9, 2.6782346, 6283.0758500 },
	{ 4303.4, 2.63513, 12566.15170 },
	{ 425.3, 1.5905, 3.5231 },
	{ 109.0, 2.9662, 1577.3435 },
	{ 93.5, 2.592, 18849.228 },
	{ 119.3, 5.7956, 26.2983 },
	{ 72.1, 1.138, 529.691 },
	{ 67.8, 1.875, 398.149 },
	{ 67.3, 4.409, 5507.553 },
	{ 59.0, 2.888, 5223.694 },
	{ 56.0, 2.175, 155.420 },
	{ 45.4, 0.398, 796.298 },
	{ 36.4, 0.466, 775.523 },
	{ 29.0, 2.647, 7.114 },
	{ 19.1, 1.846, 5486.778 },
	{ 20.8, 5.341, 0.980 },
	{ 18.5, 4.969, 213.299 },
	{ 16.2, 0.032, 2544.314 },
	{ 17.3, 2.991, 6275.962 },
	{ 15.8, 1.430, 2146.165 },
	{ 14.6, 1.205, 10977.079 },
	{ 11.9, 3.258, 5088.629 },
	{ 11.5, 2.075, 4694.003 },
	{ 9.7, 4.24, 1349.87 },
	{ 10.0, 1.303, 6286.599 },
	{ 9.5, 2.70, 242.73 },
	{ 12.5, 2.834, 1748.016 },
	{ 11.8, 5.274, 1194.447 },
	{ 8.6, 5.64, 951.72 },
	{ 10.6, 0.766, 553.569 },
	{ 7.6, 5.30, 2352.87 },
	{ 5.8, 1.77, 1059.38 },
	{ 6.4, 2.65, 9437.76 },
	{ 5.2, 5.66, 71430.70 },
	{ 5.3, 0.91, 3154.69 },
	{ 6.1, 4.67, 4690.48 },
	{ 4.3, 0.24, 6812.77 },
	{ 5.0, 1.42, 6438.50 },
	{ 4.3, 0.77, 10447.39 },
	{ 5.2, 1.85, 801.82 },
	{ 3.7, 2.00, 8031.09 },
	{ 3.6, 2.43, 14143.50 },
	{ 3.4, 3.86, 1592.60 },
	{ 3.4, 0.89, 12036.46 },
	{ 3.2, 3.19, 4705.73 },
	{ 3.2, 0.62, 8429.24 },
	{ 4.1, 5.24, 7084.90 },
	{ 3.0, 6.07, 4292.33 },
	{ 2.9, 2.32, 20.36 },
	{ 3.5, 4.80, 6279.55 },
	{ 2.9, 1.43, 5746.27 },
	{ 2.7, 4.80, 7234.79 },
	{ 2.5, 6.22, 6836.65 },
	{ 2.7, 0.93, 5760.50 },
	{ 3.2, 3.40, 7632.94 },
	{ 2.3, 5.00, 17789.85 },
	{ 2.1, 3.96, 10213.29 },
	{ 2.1, 2.22, 5856.48 },
	{ 2.3, 5.67, 11499.66 },
	{ 2.1, 5.20, 11513.88 },
	{ 1.9, 0.53, 3340.61 },
	{ 1.9, 4.74, 83996.85 },
	{ 2.1, 2.55, 25132.30 },
	{ 1.8, 1.47, 4164.31 },
	{ 1.8, 3.02, 5.52 },
	{ 2.0, 0.91, 6256.78 },
	{ 2.1, 2.27, 522.58 },
	{ 1.8, 3.03, 5753.38 },
	{ 1.6, 6.12, 5216.58 },
	{ 1.6, 4.64, 3.29 },
	// 71 terms retained, 270 terms dropped, error 0.056"*T  

	// earth l, T^2  
	{ 52918.9, 0, 0 },
	{ 8719.8, 1.07210, 6283.07585 },
	{ 309.1, 0.8673, 12566.1517 },
	{ 27.3, 0.053, 3.523 },
	{ 16.3, 5.188, 26.298 },
	{ 15.8, 3.685, 155.420 },
	{ 9.5, 0.76, 18849.23 },
	{ 8.9, 2.06, 77713.77 },
	{ 7.0, 0.83, 775.52 },
	{ 5.1, 4.66, 1577.34 },
	{ 4.1, 1.03, 7.11 },
	{ 3.5, 5.14, 796.30 },
	{ 3.2, 6.05, 5507.55 },
	{ 3.0, 1.19, 242.73 },
	{ 2.9, 6.12, 529.69 },
	{ 3.8, 3.44, 5573.14 },
	{ 2.7, 0.31, 398.15 },
	{ 2.4, 4.38, 5223.69 },
	{ 2.5, 2.28, 553.57 },
	{ 2.1, 3.75, 0.98 },
	{ 1.7, 0.90, 951.72 },
	{ 1.5, 5.76, 1349.87 },
	{ 1.2, 2.97, 2146.17 },
	{ 1.4, 4.36, 1748.02 },
	{ 1.3, 3.72, 1194.45 },
	{ 1.3, 2.95, 6438.50 },
	{ 1.0, 5.99, 6286.60 },
	{ 0.9, 4.80, 5088.63 },
	{ 0.8, 3.31, 213.30 },
	{ 1.1, 1.27, 161000.69 },
	{ 0.8, 3.42, 5486.78 },
	{ 1.0, 0.60, 3154.69 },
	{ 0.9, 5.23, 7084.90 },
	{ 0.6, 1.60, 2544.31 },
	{ 0.7, 3.43, 4694.00 },
	{ 0.6, 2.48, 10977.08 },
	{ 0.7, 6.19, 4690.48 },
	{ 0.6, 1.98, 801.82 },
	{ 0.5, 1.44, 6836.65 },
	{ 0.5, 2.34, 1592.60 },
	{ 0.5, 1.31, 4292.33 },
	{ 0.4, 0.04, 7234.79 },
	{ 0.5, 3.81, 149854.40 },
	{ 0.4, 4.94, 7632.94 },
	// 44 terms retained, 98 terms dropped, error 0.011"*T^2  

	// earth l, T^3  
	{ 289.2, 5.8438, 6283.0758 },
	{ 35.0, 0, 0 },
	{ 16.8, 5.488, 12566.152 },
	{ 3.0, 5.20, 155.42 },
	{ 1.3, 4.72, 3.52 },
	{ 0.6, 5.97, 242.73 },
	{ 0.7, 5.30, 18849.23 },
	{ 0.4, 3.79, 553.57 },
	// 8 terms retained, 14 terms dropped, error 0.005"*T^3  

	// earth l, T^4  
	{ 114.1, 3.1416, 0 },
	{ 7.7, 4.13, 6283.08 },
	{ 0.8, 3.84, 12566.15 },
	{ 0.4, 0.42, 155.42 },
	// 4 terms retained, 7 terms dropped, error 0.00032"*T^4  

	// earth l, T^5  
	{ 0.9, 3.14, 0 },
	{ 0.2, 2.77, 6283.08 },
	{ 0.1, 2.01, 155.42 },
	// 3 terms retained, 2 terms dropped, error 0.00023"*T^5  
	// end earth l  

	// earth b, T^0  
	{ 279.6, 3.1987, 84334.6616 },
	{ 101.6, 5.4225, 5507.5532 },
	{ 80.4, 3.880, 5223.694 },
	{ 43.8, 3.704, 2352.866 },
	{ 31.9, 4.000, 1577.344 },
	{ 22.7, 3.985, 1047.747 },
	{ 16.4, 3.565, 5856.478 },
	{ 18.1, 4.984, 6283.076 },
	{ 14.4, 3.703, 9437.763 },
	{ 14.3, 3.411, 10213.286 },
	{ 11.2, 4.828, 14143.495 },
	{ 10.9, 2.086, 6812.767 },
	{ 9.7, 3.47, 4694.00 },
	{ 10.4, 4.057, 71092.881 },
	{ 8.8, 4.44, 5753.38 },
	{ 8.4, 4.99, 7084.90 },
	{ 6.9, 4.33, 6275.96 },
	{ 9.1, 1.14, 6620.89 },
	{ 7.2, 3.60, 529.69 },
	{ 7.7, 5.55, 167621.58 },
	// 20 terms retained, 164 terms dropped, error 0.15"  

	// earth b, T^1  
	{ 9.0, 3.90, 5507.55 },
	{ 6.2, 1.73, 5223.69 },
	{ 3.8, 5.24, 2352.87 },
	{ 2.8, 2.47, 1577.34 },
	{ 1.8, 0.42, 6283.08 },
	// 5 terms retained, 94 terms dropped, error 0.018"*T  

	// earth b, T^2  
	{ 1.7, 1.63, 84334.66 },
	{ 0.5, 2.41, 1047.75 },
	// 2 terms retained, 47 terms dropped, error 0.0035"*T^2  

	// earth b, T^3  
	// 0 terms retained, 11 terms dropped, error 4.5e-05"*T^3  

	// earth b, T^4  
	// 0 terms retained, 5 terms dropped, error 1.7e-05"*T^4  
	// end earth b  

	// earth r, T^0  
	{ 100013988.8, 0, 0 },
	{ 1670699.6, 3.09846351, 6283.07584999 },
	{ 13956.0, 3.055246, 12566.151700 },
	{ 3083.7, 5.19847, 77713.77147 },
	{ 1628.5, 1.17388, 5753.38488 },
	{ 1575.6, 2.84685, 7860.41939 },
	{ 924.8, 5.4529, 11506.7698 },
	{ 542.4, 4.5641, 3930.2097 },
	{ 472.1, 3.6610, 5884.9268 },
	{ 328.8, 5.8998, 5223.6939 },
	{ 346.0, 0.9637, 5507.5532 },
	{ 306.8, 0.2987, 5573.1428 },
	{ 174.8, 3.0119, 18849.2275 },
	{ 243.2, 4.2735, 11790.6291 },
	{ 211.8, 5.8471, 1577.3435 },
	{ 185.8, 5.0219, 10977.0788 },
	{ 109.8, 5.0551, 5486.7778 },
	{ 98.3, 0.887, 6069.777 },
	{ 86.5, 5.690, 15720.839 },
	{ 85.8, 1.271, 161000.686 },
	{ 62.9, 0.922, 529.691 },
	{ 57.1, 2.014, 83996.847 },
	{ 64.9, 0.273, 17260.155 },
	{ 49.4, 3.245, 2544.314 },
	{ 55.7, 5.242, 71430.696 },
	{ 42.5, 6.011, 6275.962 },
	{ 47.0, 2.578, 775.523 },
	{ 39.0, 5.361, 4694.003 },
	{ 44.7, 5.537, 9437.763 },
	{ 35.7, 1.675, 12036.461 },
	{ 31.9, 0.184, 5088.629 },
	{ 31.8, 1.778, 398.149 },
	{ 33.2, 0.244, 7084.897 },
	{ 38.2, 2.393, 8827.390 },
	{ 28.5, 1.213, 6286.599 },
	{ 37.5, 0.830, 19651.048 },
	{ 37.0, 4.901, 12139.554 },
	{ 34.5, 1.843, 2942.463 },
	{ 26.3, 4.589, 10447.388 },
	{ 24.6, 3.787, 8429.241 },
	{ 23.6, 0.269, 796.298 },
	{ 27.8, 1.899, 6279.553 },
	{ 23.9, 4.996, 5856.478 },
	{ 20.3, 4.653, 2146.165 },
	{ 23.3, 2.808, 14143.495 },
	{ 22.1, 1.950, 3154.687 },
	{ 19.5, 5.382, 2352.866 },
	{ 18.0, 0.199, 6812.767 },
	{ 17.2, 4.433, 10213.286 },
	{ 16.2, 5.232, 17789.846 },
	{ 17.3, 6.152, 16730.464 },
	{ 13.8, 5.190, 8031.092 },
	{ 18.8, 0.673, 149854.400 },
	{ 18.3, 2.253, 23581.258 },
	{ 13.6, 3.685, 4705.732 },
	{ 13.1, 0.653, 13367.973 },
	{ 10.4, 4.333, 11769.854 },
	{ 10.0, 4.201, 6309.374 },
	{ 10.2, 1.594, 4690.480 },
	{ 7.6, 2.63, 6256.78 },
	{ 9.7, 3.68, 27511.47 },
	{ 6.7, 0.56, 3340.61 },
	{ 8.7, 6.06, 1748.02 },
	{ 7.8, 3.67, 12168.00 },
	{ 6.6, 5.66, 11371.70 },
	{ 7.7, 0.31, 7632.94 },
	{ 6.6, 3.14, 801.82 },
	{ 7.5, 5.65, 11926.25 },
	{ 6.9, 2.92, 6681.22 },
	{ 6.8, 1.42, 23013.54 },
	{ 6.5, 2.65, 19804.83 },
	// 71 terms retained, 455 terms dropped, error 1.1e-06 a.u.  

	//earth r, T^1  
	{ 103018.6, 1.1074897, 6283.0758500 },
	{ 1721.2, 1.06442, 12566.15170 },
	{ 702.2, 3.1416, 0 },
	{ 32.3, 1.022, 18849.228 },
	{ 30.8, 2.844, 5507.553 },
	{ 25.0, 1.319, 5223.694 },
	{ 18.5, 1.424, 1577.344 },
	{ 10.1, 5.914, 10977.079 },
	{ 8.6, 0.27, 5486.78 },
	{ 8.7, 1.42, 6275.96 },
	{ 5.1, 1.69, 5088.63 },
	{ 5.0, 6.01, 6286.60 },
	{ 4.7, 5.99, 529.69 },
	{ 4.4, 0.52, 4694.00 },
	{ 3.9, 4.75, 2544.31 },
	{ 3.8, 5.07, 796.30 },
	{ 4.1, 1.08, 9437.76 },
	{ 3.5, 0.02, 83996.85 },
	{ 3.4, 0.95, 71430.70 },
	{ 3.2, 6.16, 2146.17 },
	{ 3.4, 5.41, 775.52 },
	{ 2.9, 5.48, 10447.39 },
	{ 2.5, 0.24, 398.15 },
	{ 2.2, 4.95, 6812.77 },
	{ 2.2, 0.42, 8031.09 },
	{ 2.8, 3.42, 2352.87 },
	{ 2.6, 6.13, 6438.50 },
	{ 1.9, 5.31, 8429.24 },
	{ 2.4, 3.09, 4690.48 },
	{ 1.7, 1.54, 4705.73 },
	{ 2.2, 3.69, 7084.90 },
	{ 2.1, 1.28, 1748.02 },
	{ 1.8, 3.23, 6279.55 },
	{ 1.6, 4.10, 11499.66 },
	{ 1.6, 5.54, 3154.69 },
	{ 1.8, 1.82, 7632.94 },
	// 36 terms retained, 256 terms dropped, error 2.2e-07 a.u.*T  

	// earth r, T^2  
	{ 4359.4, 5.78455, 6283.07585 },
	{ 123.6, 5.5793, 12566.1517 },
	{ 12.3, 3.142, 0 },
	{ 8.8, 3.63, 77713.77 },
	{ 5.7, 1.87, 5573.14 },
	{ 3.3, 5.47, 18849.23 },
	{ 1.5, 4.48, 5507.55 },
	{ 1.0, 2.81, 5223.69 },
	{ 0.9, 3.11, 1577.34 },
	{ 1.1, 2.84, 161000.69 },
	{ 0.6, 5.47, 775.52 },
	{ 0.6, 1.38, 6438.50 },
	{ 0.5, 4.42, 6286.60 },
	{ 0.4, 0.90, 10977.08 },
	{ 0.4, 3.20, 5088.63 },
	{ 0.5, 3.66, 7084.90 },
	{ 0.5, 5.39, 149854.40 },
	// 17 terms retained, 122 terms dropped, error 3.9e-08 a.u.*T^2  

	// earth r, T^3  
	{ 144.6, 4.2732, 6283.0758 },
	{ 6.7, 3.92, 12566.15 },
	{ 0.8, 0, 0 },
	{ 0.2, 3.73, 18849.23 },
	// 4 terms retained, 23 terms dropped, error 1.1e-08 a.u.*T^3  

	// earth r, T^4  
	{ 3.9, 2.56, 6283.08 },
	{ 0.3, 2.27, 12566.15 },
	{ 0.1, 3.44, 5573.14 },
	// 3 terms retained, 7 terms dropped, error 2.1e-09 a.u.*T^4  

	// earth r, T^5  
	{ 0.1, 1.22, 6283.08 },
	// 1 terms retained, 2 terms dropped, error 2.4e-09 a.u.*T^5  
	// end earth  
};

static int vn_earth[][3] = {
	//addresses for earth l, b, r   
	   { 0, 247, 274, },// T^0
	{ 117, 267, 345, },//T^1   
	 { 188, 272, 381, },// T^2  
	{ 232, 274, 398, },// T^3  
	{ 240, 0, 402, },// T^4   
	{ 244, 0, 405, },// T^5   
	{ 247, 0, 406, },// end   
	{ 0, }// termination  
};

void refract(double pr, double tr, double ta, double *aa)
{
	#define	MAXRERR	degrad(0.1/3600.)	/* desired accuracy, rads */

	double d, t, t0, a;

	/* first guess of error is to go backwards.
	 * make use that we know delta-apparent is always < delta-true.
	 */
	unrefract (pr, tr, ta, &t);
	d = 0.8*(ta - t);
	t0 = t;
	a = ta;

	/* use secant method to discover a value that unrefracts to ta.
	 * max=7 ave=2.4 loops in hundreds of test cases.
	 */
	while (1) {
	    a += d;
	    unrefract (pr, tr, a, &t);
	    if (fabs(ta-t) <= MAXRERR)
		break;
	    d *= -(ta - t)/(t0 - t);
	    t0 = t;
	}

	*aa = a;

#undef	MAXRERR
}

void unrefract(double pr, double tr, double aa, double *ta)
{
	#define	LTLIM	14.5
    #define	GELIM	15.5

	double aadeg = raddeg(aa);

	if (aadeg < LTLIM)
	    unrefractLT15 (pr, tr, aa, ta);
	else if (aadeg >= GELIM)
	    unrefractGE15 (pr, tr, aa, ta);
	else {
	    /* smooth blend -- important for inverse */
	    double taLT, taGE, p;

	    unrefractLT15 (pr, tr, aa, &taLT);
	    unrefractGE15 (pr, tr, aa, &taGE);
	    p = (aadeg - LTLIM)/(GELIM - LTLIM);
	    *ta = taLT + (taGE - taLT)*p;
	}
}

void unrefractLT15(double pr, double tr, double aa, double *ta)
{
	double aadeg = raddeg(aa);
	double r, a, b;

	a = ((2e-5*aadeg+1.96e-2)*aadeg+1.594e-1)*pr;
	b = (273+tr)*((8.45e-2*aadeg+5.05e-1)*aadeg+1);
	r = degrad(a/b);

	*ta  =  (aa < 0 && r < 0) ? aa : aa - r;
}
void unrefractGE15(double pr, double tr, double aa, double *ta)
{
	double r;
	
	r = 7.888888e-5*pr/((273+tr)*tan(aa));
	*ta  =  aa - r;
}

void ma(double lat_, double ha, double dec, double *alt,
    double *az, double *fpa)
{
	static double last_lat = -3434, slat_a, clat_a;
	double cosa, a_2, sinB, cosB, cosZe, ze, sinAz, sinFpa;

	if (lat_ != last_lat) {
	    slat_a = sin(lat_+25*PI/180);
	    clat_a = cos(lat_+25*PI/180);
	    last_lat = lat_;
	}

	cosa=-clat_a*sin(dec)+ slat_a*cos(dec)*cos(ha);
	a_2=acos(cosa)/2;
	sinB=cos(dec)*sin(ha)/sin(a_2*2);
	cosB=cos(asin(sinB));
	cosZe=sin(25*PI/180)*cos(a_2)+cos(25*PI/180)*sin(a_2)*cosB;
	ze=acos(cosZe);
	sinAz=cos(dec)*sin(ha)/2/cos(a_2)/sin(ze);
	sinFpa=(slat_a*tan(a_2)*cosB-clat_a)*sin(ha);

	*alt=PI/2-ze;
	if(sinAz > 0.99999) *az=PI*3/2;
	else if(sinAz < -0.99999) *az=PI/2;
	else *az=asin(sinAz)+PI;
	*fpa=asin(sinFpa);
}

void hadec_aa(double lat_, double ha, double dec, double *alt,
    double *az)
{
	static double last_lat = -3434, slat, clat;
	double cap, B;

	if (lat_ != last_lat) {
	    slat = sin(lat_);
	    clat = cos(lat_);
	    last_lat = lat_;
	}

	solve_sphere (-ha, PI/2-dec, slat, clat, &cap, &B);
	*az = B;
	*alt = PI/2 - acos(cap);
}

void aa_hadec(double lat_, double alt, double az, double *ha,
    double *dec)
{
	static double last_lat = -3434, slat, clat;
	double cap, B;

	if (lat_ != last_lat) {
	    slat = sin(lat_);
	    clat = cos(lat_);
	    last_lat = lat_;
	}

	solve_sphere (-az, PI/2-alt, slat, clat, &cap, &B);
	*ha = B;
	*dec = PI/2 - acos(cap);
	if (*ha > PI)
	    *ha -= 2*PI;
}
void solve_sphere(double A, double b, double cc, double sc,
    double *cap, double *Bp)
{
	double cb = cos(b), sb = sin(b);
	double cA = cos(A);
	double ca;
	double B;

	ca = cb*cc + sb*sc*cA;
	if (ca >  1.0) ca =  1.0;
	if (ca < -1.0) ca = -1.0;
	if (cap)
	    *cap = ca;

	if (!Bp)
	    return;

	if (cc > .99999) {
	    /* as c approaches 0, B approaches pi - A */
	    B = PI - A;
	} else if (cc < -.99999) {
	    /* as c approaches PI, B approaches A */
	    B = A;
	} else {
	    /* compute cB and sB and remove common factor of sa from quotient.
	     * be careful where B causes atan to blow.
	     */
	    double sA = sin(A);
	    double x, y;

	    y = sA*sb*sc;
	    x = cb - ca*cc;
	
	    if (fabs(x) < 1e-5)
		B = y < 0 ? 3*PI/2 : PI/2;
	    else
		B = atan2 (y, x);
	}

	*Bp = B;
	range (Bp, 2*PI);
}

void now_lst(double mjd0_,double lng_, double *lstp)
{
	static double last_mjd = -23243, last_lng = 121212, last_lst;
	double eps, deps, dpsi;

	if (last_mjd == mjd0_ && last_lng == lng_) {
	    *lstp = last_lst;
	    return;
	}

	//utc_gst (mjd_day(mjd), mjd_hr(mjd), &lst);
	
	double T, x;

	T = (mjd0_  - J2000)/36525.0;
	//Greenwich Mean Sidereal Time at 0h UT
	x = 24110.54841 +
		(8640184.812866 + (0.093104 - 6.2e-6 * T) * T) * T;
	x /= 3600.0;
	range(&x, 24.0);
	


	//x += radhr(lng_);

	obliquity_p03(mjd0_, &eps);
	nutation(mjd0_, &deps, &dpsi);
	x += radhr(dpsi*cos(eps+deps));

	/*utc_gst (mjd_day(mjd), mjd_hr(mjd), &lst);
	lst += radhr(lng);

	obliquity(mjd, &eps);
	nutation(mjd, &deps, &dpsi);
	lst += radhr(dpsi*cos(eps+deps));

	range (&lst, 24.0);*/
	range (&x, 24.0);

	last_mjd = mjd0_;
	last_lng = lng_;
	*lstp = last_lst = x;
}

void cal_mjd(int mn, double dy, int yr, double *mjd_)
{
    static double last_mjd, last_dy;
	static int last_mn, last_yr;
	int b, d, m, y;
	long c;

	if (mn == last_mn && yr == last_yr && dy == last_dy) {
	    *mjd_ = last_mjd;
	    return;
	}

	m = mn;
	y = (yr < 0) ? yr + 1 : yr;
	if (mn < 3) {
	    m += 12;
	    y -= 1;
	}

	if (yr < 1582 || (yr == 1582 && (mn < 10 || (mn == 10 && dy < 15))))
	    b = 0;
	else {
	    int a;
	    a = y/100;
	    b = 2 - a + a/4;
	}

	if (y < 0)
	    c = (long)((365.25*y) - 0.75) - 694025L;
	else
	    c = (long)(365.25*y) - 694025L;

	d = (int)(30.6001*(m+1));

	*mjd_ = b + c + d + dy - 0.5;

	last_mn = mn;
	last_dy = dy;
	last_yr = yr;
	last_mjd = *mjd_;	
}

void mjd_cal(double mjd_, int *mn, double *dy, int *yr)
{
	static double last_mjd, last_dy;
	static int last_mn, last_yr;
	double d, f;
	double i, a, b, ce, g;

	/* we get called with 0 quite a bit from unused epoch fields.
	 * 0 is noon the last day of 1899.
	 */
	if (mjd_ == 0.0) {
	    *mn = 12;
	    *dy = 31.5;
	    *yr = 1899;
	    return;
	}

	if (mjd_ == last_mjd) {
	    *mn = last_mn;
	    *yr = last_yr;
	    *dy = last_dy;
	    return;
	}

	d = mjd_ + 0.5;
	i = floor(d);
	f = d-i;
	if (f == 1) {
	    f = 0;
	    i += 1;
	}

	if (i > -115860.0) {
	    a = floor((i/36524.25)+.99835726)+14;
	    i += 1 + a - floor(a/4.0);
	}

	b = floor((i/365.25)+.802601);
	ce = i - floor((365.25*b)+.750001)+416;
	g = floor(ce/30.6001);
	*mn = (int)(g - 1);
	*dy = ce - floor(30.6001*g)+f;
	*yr = (int)(b + 1899);

	if (g > 13.5)
	    *mn = (int)(g - 13);
	if (*mn < 2.5)
	    *yr = (int)(b + 1900);
	if (*yr < 1)
	    *yr -= 1;

	last_mn = *mn;
	last_dy = *dy;
	last_yr = *yr;
	last_mjd = mjd_;
}
void mjd_year(double mjd_, double *yr)
{
	static double last_mjd, last_yr;
	int m, y;
	double d;
	double e0, e1;	/* mjd of start of this year, start of next year */

	if (mjd_ == last_mjd) {
	    *yr = last_yr;
	    return;
	}

	mjd_cal (mjd_, &m, &d, &y);
	if (y == -1) y = -2;
	cal_mjd (1, 1.0, y, &e0);
	cal_mjd (1, 1.0, y+1, &e1);
	*yr = y + (mjd_ - e0)/(e1 - e0);

	last_mjd = mjd_;
	last_yr = *yr;
}
// DeltaT = Ephemeris Time - Universal Time
/* calculate  DeltaT = ET - UT1 in seconds.  Describes the irregularities
 * of the Earth rotation rate in the ET time scale.
 */
double deltat(double mjd_)
{
	double Y, B;
	static double ans;
	static double lastmjd = -10000;

	if (mjd_ == lastmjd) {
	    return(ans);
	}
	lastmjd = mjd_;

	Y = 2000.0 + (mjd_ - J2000)/365.25;
	B = Y - 2006.0;
	ans = 7060 + B * (7060  - 6900);
	ans *= 0.01;
	return(ans);
}

void range(double *v, double r)
{
	*v -= r*floor(*v/r);
}

void precess_p03(double mjd1, double mjd2, double *ra, double *dec)
{
	static double last_mjd1 = -213.432, last_from;
	static double last_mjd2 = -213.432, last_to;
	double zeta_A, z_A, theta_A;
	double T;
	double A, B, C;
	double alpha, delta;
	double alpha_in, delta_in;
	double from_equinox, to_equinox;
	double alpha2000, delta2000;

	/* convert mjds to years;
	 * avoid the remarkably expensive calls to mjd_year()
	 */
	if (last_mjd1 == mjd1)
	    from_equinox = last_from;
	else {
	    mjd_year (mjd1, &from_equinox);
	    last_mjd1 = mjd1;
	    last_from = from_equinox;
	}
	if (last_mjd2 == mjd2)
	    to_equinox = last_to;
	else {
	    mjd_year (mjd2, &to_equinox);
	    last_mjd2 = mjd2;
	    last_to = to_equinox;
	}

	/* convert coords in rads to degs */
	alpha_in = raddeg(*ra);
	delta_in = raddeg(*dec);

	/* precession progresses about 1 arc second in .047 years */
	/* From from_equinox to 2000.0 */
	if (fabs (from_equinox-2000.0) > .02) {
	    T = (from_equinox - 2000.0)/100.0;

	    zeta_A  = (2.650545+2306.083227* T + 0.2988499* T*T 
+ 0.01801828* T*T*T - 0.000005971* T*T*T*T-0.0000003173* T*T*T*T*T)/3600.;

	    z_A     = (-2.650545+2306.077181* T + 1.0927348* T*T 
+ 0.01826837* T*T*T - 0.000028596* T*T*T*T - 0.0000002904* T*T*T*T*T)/3600.;

	    theta_A = (2004.191903* T - 0.4294934* T*T 
- 0.04182264* T*T*T - 0.000007089* T*T*T*T - 0.0000001274* T*T*T*T*T)/3600.;


	    A = DSIN(alpha_in - z_A) * DCOS(delta_in);
	    B = DCOS(alpha_in - z_A) * DCOS(theta_A) * DCOS(delta_in)
	      + DSIN(theta_A) * DSIN(delta_in);
	    C = -DCOS(alpha_in - z_A) * DSIN(theta_A) * DCOS(delta_in)
	      + DCOS(theta_A) * DSIN(delta_in);

	    alpha2000 = DATAN2(A,B) - zeta_A;
	    range (&alpha2000, 360.0);
	    delta2000 = DASIN(C);
	} else {
	    /* should get the same answer, but this could improve accruacy */
	    alpha2000 = alpha_in;
	    delta2000 = delta_in;
	};


	/* From 2000.0 to to_equinox */
	if (fabs (to_equinox - 2000.0) > .02) {
	    T = (to_equinox - 2000.0)/100.0;

	    zeta_A  = (2.650545+2306.083227* T + 0.2988499* T*T 
+ 0.01801828* T*T*T - 0.000005971* T*T*T*T-0.0000003173* T*T*T*T*T)/3600.;

	    z_A     = (-2.650545+2306.077181* T + 1.0927348* T*T 
+ 0.01826837* T*T*T - 0.000028596* T*T*T*T - 0.0000002904* T*T*T*T*T)/3600.;

	    theta_A = (2004.191903* T - 0.4294934* T*T 
- 0.04182264* T*T*T - 0.000007089* T*T*T*T - 0.0000001274* T*T*T*T*T)/3600.;

	    A = DSIN(alpha2000 + zeta_A) * DCOS(delta2000);
	    B = DCOS(alpha2000 + zeta_A) * DCOS(theta_A) * DCOS(delta2000)
	      - DSIN(theta_A) * DSIN(delta2000);
	    C = DCOS(alpha2000 + zeta_A) * DSIN(theta_A) * DCOS(delta2000)
	      + DCOS(theta_A) * DSIN(delta2000);

	    alpha = DATAN2(A,B) + z_A;
	    range(&alpha, 360.0);
	    delta = DASIN(C);
	} else {
	    /* should get the same answer, but this could improve accruacy */
	    alpha = alpha2000;
	    delta = delta2000;
	};

	*ra = degrad(alpha);
	*dec = degrad(delta);
}

void obliquity_p03(double mjd_, double *eps)
{
	static double last_mjd = -213.432, last_from;
	double from_equinox;
	double T;

	/* convert mjds to years;
	 * avoid the remarkably expensive calls to mjd_year()
	 */
	if (last_mjd == mjd_)
	    from_equinox = last_from;
	else {
	    mjd_year (mjd_, &from_equinox);
	    last_mjd = mjd_;
	    last_from = from_equinox;
	}
	/* precession progresses about 1 arc second in .047 years */
	/* From from_equinox to 2000.0 */
	if (fabs (from_equinox-2000.0) > .02) {
	    T = (from_equinox - 2000.0)/100.0;

	    *eps  = (84381.406000 - 46.836769* T - 
			0.0001831* T*T 
			+ 0.00200340* T*T*T - 0.000000576* T*T*T*T - 
			0.0000000434* T*T*T*T*T)/3600./180.*M_PI;

	} else {
	    /* should get the same answer, but this could improve accruacy */
	    *eps  = (84381.406000)/3600./180.*M_PI;

	};
}

void sphcart(double l, double b, double r, double *x, double *y,
    double *z)
{
	double rcb = r * cos(b);

	*x = rcb * cos(l);
	*y = rcb * sin(l);
	*z = r * sin(b);
}

void cartsph(double x, double y, double z, double *l, double *b,
    double *r)
 {
	  double rho = x*x + y*y;

	if (rho > 1e-35) {	/* standard case: off axis */
	    *l = atan2(y, x);
	    range (l, 2*PI);
	    *b = atan2(z, sqrt(rho));
	    *r = sqrt(rho + z*z);
	} else {		/* degenerate case; avoid math error */
	    *l = 0.0;
	    if (z == 0.0)
		*b = 0.0;
	    else
		*b = (z > 0.0) ? PI/2. : -PI/2.;
	    *r = fabs(z);
	}
 }

void nutation(double mjd_, double *deps, double *dpsi)
{
	static double lastmjd = -10000, lastdeps, lastdpsi;
	double T, T2, T3, T10;			/* jul cent since J2000 */
	double prec;				/* series precis in arc sec */
	int i, isecul;				/* index in term table */
	static double delcache[5][2*NUT_MAXMUL+1];
			/* cache for multiples of delaunay args
			 * [M',M,F,D,Om][-min*x, .. , 0, .., max*x]
			 * make static to have unfilled fields cleared on init
			 */

	if (mjd_ == lastmjd) {
	    *deps = lastdeps;
	    *dpsi = lastdpsi;
	    return;
	}

	prec = 0.0;

#if 0	/* this is if deps should contain a precision value */
	prec =* deps;
	if (prec < 0.0 || prec > 1.0)	/* accept only sane value */
		prec = 1.0;
#endif

	/* augment for abundance of small terms */
	prec *= NUT_SCALE/10;

	T = (mjd_ - J2000)/36525.;
	T2 = T * T;
	T3 = T2 * T;
	T10 = T/10.;

	/* calculate delaunay args and place in cache */
	for (i = 0; i < 5; ++i) {
	    double x;
	    short j;

	    x = delaunay[i][0] +
		delaunay[i][1] * T +
		delaunay[i][2] * T2 +
		delaunay[i][3] * T3;

	    /* convert to radians */
	    x /= SECPERCIRC;
	    x -= floor(x);
	    x *= 2.*PI;

	    /* fill cache table */
	    for (j = 0; j <= 2*NUT_MAXMUL; ++j)
		delcache[i][j] = (j - NUT_MAXMUL) * x;
	}

	/* find dpsi and deps */
	lastdpsi = lastdeps = 0.;
	for (i = isecul = 0; i < NUT_SERIES ; ++i) {
	    double arg = 0., ampsin, ampcos;
	    short j;

	    if (ampconst[i][0] || ampconst[i][1]) {
		/* take non-secular terms from simple array */
		ampsin = ampconst[i][0];
		ampcos = ampconst[i][1];
	    } else {
		/* secular terms from different array */
		ampsin = ampsecul[isecul][1] + ampsecul[isecul][2] * T10;
		ampcos = ampsecul[isecul][3] + ampsecul[isecul][4] * T10;
		++isecul;
	    }

	    for (j = 0; j < 5; ++j)
		arg += delcache[j][NUT_MAXMUL + multarg[i][j]];

	    if (fabs(ampsin) >= prec)
		lastdpsi += ampsin * sin(arg);

	    if (fabs(ampcos) >= prec)
		lastdeps += ampcos * cos(arg);

	}

	/* convert to radians.
	 */
	lastdpsi = degrad(lastdpsi/3600./NUT_SCALE);
	lastdeps = degrad(lastdeps/3600./NUT_SCALE);

	lastmjd = mjd_;
	*deps = lastdeps;
	*dpsi = lastdpsi;
}
void nut_eq(double mjd_, double *ra, double *dec)
{
	static double lastmjd = -10000;
	static double a[3][3];		/* rotation matrix */
	double xold, yold, zold, x, y, z;

	if (mjd_ != lastmjd) {
	    double epsilon, dpsi, deps;
	    double se, ce, sp, cp, sede, cede;

	    obliquity_p03(mjd_, &epsilon);
	    nutation(mjd_, &deps, &dpsi);

	    /* the rotation matrix a applies the nutation correction to
	     * a vector of equatoreal coordinates Xeq to Xeq' by 3 subsequent
	     * rotations:  R1 - from equatoreal to ecliptic system by
	     * rotation of angle epsilon about x, R2 - rotate ecliptic
	     * system by -dpsi about its z, R3 - from ecliptic to equatoreal
	     * by rotation of angle -(epsilon + deps)
	     *
	     *	Xeq' = A * Xeq = R3 * R2 * R1 * Xeq
	     * 
	     *		[ 1       0          0    ]
	     * R1 =	[ 0   cos(eps)   sin(eps) ]
	     *		[ 0  - sin(eps)  cos(eps) ]
	     * 
	     *		[ cos(dpsi)  - sin(dpsi)  0 ]
	     * R2 =	[ sin(dpsi)   cos(dpsi)   0 ]
	     *		[      0           0      1 ]
	     * 
	     *		[ 1         0                 0         ]
	     * R3 =	[ 0  cos(eps + deps)  - sin(eps + deps) ]
	     *		[ 0  sin(eps + deps)   cos(eps + deps)  ]
	     * 
	     * for efficiency, here is a explicitely:
	     */
	    
	    se = sin(epsilon);
	    ce = cos(epsilon);
	    sp = sin(dpsi);
	    cp = cos(dpsi);
	    sede = sin(epsilon + deps);
	    cede = cos(epsilon + deps);

	    a[0][0] = cp;
	    a[0][1] = -sp*ce;
	    a[0][2] = -sp*se;

	    a[1][0] = cede*sp;
	    a[1][1] = cede*cp*ce+sede*se;
	    a[1][2] = cede*cp*se-sede*ce;

	    a[2][0] = sede*sp;
	    a[2][1] = sede*cp*ce-cede*se;
	    a[2][2] = sede*cp*se+cede*ce;

	    lastmjd = mjd_;
	}

	sphcart(*ra, *dec, 1.0, &xold, &yold, &zold);
	x = a[0][0] * xold + a[0][1] * yold + a[0][2] * zold;
	y = a[1][0] * xold + a[1][1] * yold + a[1][2] * zold;
	z = a[2][0] * xold + a[2][1] * yold + a[2][2] * zold;
	cartsph(x, y, z, ra, dec, &zold);	/* radius should be 1.0 */
	if (*ra < 0.) *ra += 2.*PI;		/* make positive for display */
}

void sunpos(double mjd_, double *lsn, double *rsn, double *bsn)
{
	static double last_mjd = -3691, last_lsn, last_rsn, last_bsn;
	double ret[3];

	if (mjd_ == last_mjd) {
	    *lsn = last_lsn;
	    *rsn = last_rsn;
	    if (bsn) *bsn = last_bsn;
	    return;
	}

	//vsop87(mjd, SUN, 0.0, ret);	/* full precision earth pos */
	double t[6];			// powers of time 
    double t_abs[6];		// powers of abs(time) 
    int i, cooidx, alpha;			// misc indexes 

    for (i = 0; i < 3; ++i) ret[i] = 0.0;

    // time and its powers 
    t[0] = 1.0;
    t[1] = (mjd_ - J2000)/365250.0;
    for (i = 2; i <= 5; ++i) t[i] = t[i-1] * t[1];
    t_abs[0] = 1.0;
    for (i = 1; i <= 5; ++i) t_abs[i] = fabs(t[i]);

    // do the term summation; first the spatial dimensions 
    for (cooidx = 0; cooidx < 3; ++cooidx) {

	// then the powers of time  
		for (alpha = 0; vn_earth[alpha+1][cooidx] ; ++alpha) {
			double  term= 0.0;
			 
			for (i = vn_earth[alpha][cooidx]; i < vn_earth[alpha+1][cooidx]; ++i) {
			double a, b, c, arg;

			a = vx_earth[i][0];		
			b = vx_earth[i][1];
			c = vx_earth[i][2];

			arg = b + c * t[1];
			term += a * cos(arg);
			}

			ret[cooidx] += t[alpha] * term;
		} // alpha  
    } // cooidx  

    for (i = 0; i < 3; ++i) ret[i] /= VSOP_ASCALE;

    // reduce longitude to 0..2pi  
    ret[0] -= floor(ret[0]/(2.*PI)) * (2.*PI);

    // reduction from dynamical equinox of VSOP87 to FK5;
      
    double L1, c1, s1;
	L1 = ret[0] - degrad(13.97 * t[1] - 0.031 * t[2]);
	c1 = cos(L1); s1 = sin(L1);
	ret[0] += degrad(-0.09033 + 0.03916 * (c1 + s1) * tan(ret[1]))/3600.0;
	ret[1] += degrad(0.03916 * (c1 - s1))/3600.0;


	*lsn = ret[0] - PI;		/* revert to sun pos */
	range (lsn, 2*PI);		/* normalise */

	last_lsn = *lsn;		/* memorise */
	last_rsn = *rsn = ret[2];
	last_bsn = -ret[1];
	last_mjd = mjd_;

	if (bsn) *bsn = last_bsn;
}

void ab_eq(double mjd_, double lsn, double *ra, double *dec)
{
	static double lastmjd = -10000;
	static double eexc;	/* earth orbit excentricity */
	static double leperi;	/* ... and longitude of perihelion */
	static char dirty = 1;	/* flag for cached trig terms */

	if (mjd_ != lastmjd) {
	    double T;		/* centuries since J2000 */

	    T = (mjd_ - J2000)/36525.;
	    eexc = 0.016708617 - (42.037e-6 + 0.1236e-6 * T) * T;
	    leperi = degrad(102.93735 + (0.71953 + 0.00046 * T) * T);
	    lastmjd = mjd_;
	    dirty = 1;
	}

	double sr, cr, sd, cd, sls, cls;/* trig values coords */
	static double cp, sp, ce, se;	/* .. and perihel/eclipic */
	double dra, ddec;		/* changes in ra and dec */

	if (dirty) {
		double eps;

		cp = cos(leperi);
		sp = sin(leperi);
		obliquity_p03(mjd_, &eps);
		se = sin(eps);
		ce = cos(eps);
		dirty = 0;
	}

	sr = sin(*ra);
	cr = cos(*ra);
	sd = sin(*dec);
	cd = cos(*dec);
	sls = sin(lsn);
	cls = cos(lsn);

	dra = ABERR_CONST/cd * ( -(cr * cls * ce + sr * sls) +
			eexc * (cr * cp * ce + sr * sp));

	ddec = se/ce * cd - sr * sd;	/* tmp use */
	ddec = ABERR_CONST * ( -(cls * ce * ddec + cr * sd * sls) +
			eexc * (cp * ce * ddec + cr * sd * sp) );
	
	*ra += dra;
	range (ra, 2*PI);
	*dec += ddec;    
}

int main(int argc, char* argv[])
{
	double mjd0,mjd1;//mjd0 modified julian date, counted from J1900.0
	double lsn,rsn;//true geoc lng of sun,dist from sun to earth
	double ra,dec;//ra and dec at epoch of date
	//double ra1,dec1,mjd2,dltra,dltdec;//ra and dec at epoch of date
	double ha,alt,az,fpa,ha1,dec1;//hour angle,altitude and azimuth
	double utc,deltaUT,lst0,lst;
	Now np;


	//strcpy(bp,"Spica,f|M|B1,13:25:11.6,11:9:41,0.98,2000");
	ra= hrrad(9+42./60.+55.5/3600.);
	dec= degrad(39+47.0/60.0+15.18/3600.);
	

	cal_mjd(4,24,2007,&mjd0);

	SYSTEMTIME time1;
	GetSystemTime(&time1);
	utc=time1.wHour*3600+time1.wMinute*60.+time1.wSecond+time1.wMilliseconds/1000.0;
	deltaUT=deltat(mjd0);
	mjd1=mjd0+(utc+deltaUT)/86400.0;
	
	np.n_lat=degrad(40+23./60.+36./3600.);
	np.n_lng=degrad(117+34./60.+30./3600.);
	np.n_temp=10;
	np.n_pressure=1010;
	
	precess_p03(J2000,mjd1,&ra,&dec);
	
	sunpos(mjd1,&lsn,&rsn,NULL);

	nut_eq(mjd1,&ra,&dec);
	

	ab_eq(mjd1,lsn,&ra,&dec);
	
///////////////////////////////////////////////////////////
	now_lst(mjd0,np.n_lng,&lst0);
	lst = 1.002737909*(utc)/3600.0 + 
		lst0+ radhr(np.n_lng);
	range (&lst, 24.0);
	ha=hrrad(lst)-ra;

	hadec_aa(np.n_lat,ha,dec,&alt,&az);//calculate alt az from ha dec
	printf("alt=%f\n",alt*180/PI);
	printf("az=%f\n",az*180/PI);
	

	refract(np.n_pressure,np.n_temp,alt,&alt);
	printf("alt_r=%f\n",alt*180/PI);
	

	aa_hadec(np.n_lat,alt,az,&ha1,&dec1);
	printf("ha=%f\n",ha*180/PI);
	printf("ha1=%f\n",ha1*180/PI);
	printf("dec=%f\n",dec*180/PI);
	printf("dec1=%f\n",dec1*180/PI);
	

	ma(np.n_lat, ha1, dec1, &alt, &az, &fpa);
//obj_cir(&np,&op);




	printf("JD=%f\n",mjd0+MJD0);
	printf("lst0=%f\n",lst0);
	printf("lst=%f\n",lst);//*3600
	
	printf("alt_ma=%f\n",alt*180/PI);
	printf("az_ma=%f\n",az*180/PI);
	printf("fpa=%f\n",fpa*180/PI);
	
	return 0;
}

