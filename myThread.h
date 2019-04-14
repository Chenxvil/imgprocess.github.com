#ifndef MYTHREND_H
#define MYTHREAD_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include<QThread>
#include<qreadwritelock.h>
#include<cmath>

using namespace cv;
using namespace std;
class myThread : public QThread
{
	Q_OBJECT
public:
	myThread();
	//myThread(int num, int sum);
	void setScaleProcess(Mat srcimage, Mat& dstimage, double scale, int num, int sum);
	void mTscaleProcess(Mat srcimage, Mat& dstimage, double scale, int num, int sum);
	//void mTscaleProcess();
	void mTrotatProcess(Mat srcimage, Mat& dstimage, double xi, int num, int sum);
	void mTgaussNoiseProcess(Mat & srcimage, int k, int num, int sum);
	//void run();

private:
	Mat srcimage;
	Mat* dstimage;
	int sum;
	int num;
	double factor;
	//	QReadWriteLock lock;
	inline double BiCubicNum(double x)
	{
		double const a = -0.5;
		double abs_x = abs(x);
		if (abs_x <= 1)
			return  1.5  * pow(abs_x, 3) - 2.5 * x*x + 1;
		if (1 < x && x < 2)
			return -0.5 * pow(abs_x, 3) + 2.5 * x*x - 4 * abs_x + 2;
		return 0;
	}
	inline double generateGaussianNoise()
	{
		static const double PI = 3.1415926;
		double x1, x2;

		double rand1 = rand() / ((double)RAND_MAX);
		double rand2 = rand() / ((double)RAND_MAX);
		if (rand1 < 1e-100) rand1 = 1e-100;
		x1 = sqrt(-2 * log(rand1));
		x2 = cos(rand2 * 2 * PI);

		return x1 * x2;
	}
};

#endif // !MYTHREND#pragma once
