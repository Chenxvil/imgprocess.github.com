#pragma once
#include<iostream>
#include<CL\cl.h>
#include"QtGuiApplication4.h"
#include<math.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#define PI 3.1415926
using namespace cv;
class Transform
{
public:
	Mat scaleProcess(Mat* src, float scale);
	Mat rotatProcess(Mat* src, float degree);
	Mat gaussNoiseProcess(Mat* src, int factor);
private:
	void scale_cl(int* in, int* out, int w, int h, float scale);
	void rotat_cl(int* in, int* out, int h, int w, int nh, int nw, float degree);
	void gaussnoise_cl(int* in, int* out, int heigh, int width, int factor, float* rand1, float* rand2);
	char *LoadProgSource(const char* cFilename, const char* cPreamble, size_t* szFinalLength);
	size_t RoundUP(int x1, int x2);
};