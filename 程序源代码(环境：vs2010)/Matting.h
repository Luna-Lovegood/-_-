/*
 * Matting.h
 *
 *  Created on: Aug 17, 2013
 *      Author: zhonghua
 */

#ifndef MATTING_H_
#define MATTING_H_

#include "def.h"
#include "Statistics.h"

class Matting {
public:
	Matting();
	virtual ~Matting();
	static int train(const Mat& image, const Mat& profile);

	// ================================================
	// static method
	// ================================================
	static int mat(const Statistics& stat, const Mat& ori, Mat& output, 
		Mat& predict_raw, Mat& predict_result);

	static double grabCut(const Mat& ori, const Mat& min, const Mat& trimap, 
		const Rect& boundRect, const vector<Point>& contour, Mat& output);

	static void dump_training_results();

	static double evaluate(const Mat& ground_truth, const Mat& result);

};

#endif /* MATTING_H_ */
