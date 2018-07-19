#ifndef __DEF_H__
#define __DEF_H__

#define LONG_EDGE_PX 480
#define LONG_EDGE_TRAIN 960

#define SIGMOID(x) (1/(1+exp(-x)))

typedef unsigned char byte;

#include <iostream>

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

#pragma comment(lib, "opencv_core2410.lib")
#pragma comment(lib, "opencv_highgui2410.lib")
#pragma comment(lib, "opencv_imgproc2410.lib")



#endif
