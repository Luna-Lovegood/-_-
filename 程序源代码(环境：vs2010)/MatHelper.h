/*
 * MatHelper.h
 *
 *  Created on: Aug 24, 2013
 *      Author: zhonghua
 */

#ifndef MATHELPER_H_
#define MATHELPER_H_

#include "def.h"

class MatHelper {
public:
	static Mat resize(const Mat& ori, int long_edge);
	static Mat resize(const Mat& ori, int width, int height);

	static Mat read_image(const string& string, int long_edge=0);

	static Mat read_image_ch(const string& string, int ch);
};

#endif /* MATHELPER_H_ */
