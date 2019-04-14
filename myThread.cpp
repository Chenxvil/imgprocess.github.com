#include "myThread.h"
myThread::myThread()
{
}

void myThread::setScaleProcess(Mat src, Mat& dst, double scale, int n, int s)
{
	this->srcimage = src;
	this->dstimage = &dst;
	this->factor = scale;
	this->num = n;
	this->sum = s;
}

//void myThread::run()
//{
//	cout << num;
//	//mTscaleProcess();
//}
void myThread::mTscaleProcess(Mat srcimage, Mat& dstimage, double factor, int num, int sum)
{

	int nRows = cvRound((srcimage.rows - 8)*factor);
	int nCols = cvRound((srcimage.cols - 8)*factor);
	nRows /= sum;
	double k_i[4], k_j[4];
	double msum = 0, srcx, srcy, srcx_dec, srcy_dec;
	int srcx_int, srcy_int;
	if (srcimage.type() == CV_8UC3)
	{
		for (int i = nRows * num; i < nRows * (num + 1); i++)
		{
			for (int j = 0; j < nCols; j++)
			{
				srcx = i / factor;
				srcy = j / factor;
				srcx_int = floor(srcx);
				srcy_int = floor(srcy);
				srcx_dec = srcx - srcx_int;
				srcy_dec = srcy - srcy_int;


				k_i[0] = BiCubicNum(1 + srcx_dec);
				k_i[1] = BiCubicNum(srcx_dec);
				k_i[2] = BiCubicNum(1 - srcx_dec);
				k_i[3] = BiCubicNum(2 - srcx_dec);

				k_j[0] = BiCubicNum(1 + srcy_dec);
				k_j[1] = BiCubicNum(srcy_dec);
				k_j[2] = BiCubicNum(1 - srcy_dec);
				k_j[3] = BiCubicNum(2 - srcy_dec);
				for (int rgb = 0; rgb < 3; rgb++)
				{
					for (int m = 0; m < 4; m++)
					{
						for (int n = 0; n < 4; n++)
						{
							msum += k_i[m] * k_j[n] * srcimage.at<Vec3b>(srcx_int + m + 1, srcy_int + n + 1)[rgb];
						}
					}
					dstimage.at<Vec3b>(i, j)[rgb] = msum;
					msum = 0;
				}

			}
		}
	}
	else if (srcimage.type() == CV_8UC1)
	{
		for (int i = nRows * num; i < nRows * (num + 1); i++)
		{
			for (int j = 0; j < nCols; j++)
			{
				srcx = i / factor;
				srcy = j / factor;
				srcx_int = floor(srcx);
				srcy_int = floor(srcy);
				srcx_dec = srcx - srcx_int;
				srcy_dec = srcy - srcy_int;


				k_i[0] = BiCubicNum(1 + srcx_dec);
				k_i[1] = BiCubicNum(srcx_dec);
				k_i[2] = BiCubicNum(1 - srcx_dec);
				k_i[3] = BiCubicNum(2 - srcx_dec);

				k_j[0] = BiCubicNum(1 + srcy_dec);
				k_j[1] = BiCubicNum(srcy_dec);
				k_j[2] = BiCubicNum(1 - srcy_dec);
				k_j[3] = BiCubicNum(2 - srcy_dec);

				for (int m = 0; m < 4; m++)
				{
					for (int n = 0; n < 4; n++)
					{
						msum += k_i[m] * k_j[n] * srcimage.at<uchar>(srcx_int + m + 1, srcy_int + n + 1);
					}
					dstimage.at<Vec3b>(i, j) = msum;
					msum = 0;
				}
			}
		}
	}

}

void myThread::mTrotatProcess(Mat srcimage, Mat& dstimage, double xi, int num, int sum)
{
	int rows = dstimage.rows / sum;
	double k_i[4], k_j[4];
	double msum = 0, srcx, srcy, srcx_dec, srcy_dec;
	int srcx_int, srcy_int;
	for (int i = rows * num; i < rows * (num + 1); i++)
	{
		for (int j = 0; j < dstimage.cols; j++)
		{
			srcx = (i - dstimage.rows * 0.5) * cos(xi) - (j - dstimage.cols * 0.5) * sin(xi) + srcimage.rows * 0.5;
			srcy = (i - dstimage.rows * 0.5) * sin(xi) + (j - dstimage.cols * 0.5) * cos(xi) + srcimage.cols * 0.5;

			if (srcx < 0 || srcy < 0 || srcx >= srcimage.rows - 4 || srcy >= srcimage.cols - 4)
			{
				for (int rgb = 0; rgb < 3; rgb++)
					dstimage.at<Vec3b>(i, j)[rgb] = 0;
				continue;
			}
			srcx_int = floor(srcx);
			srcy_int = floor(srcy);
			srcx_dec = srcx - srcx_int;
			srcy_dec = srcy - srcy_int;


			k_i[0] = BiCubicNum(1 + srcx_dec);
			k_i[1] = BiCubicNum(srcx_dec);
			k_i[2] = BiCubicNum(1 - srcx_dec);
			k_i[3] = BiCubicNum(2 - srcx_dec);

			k_j[0] = BiCubicNum(1 + srcy_dec);
			k_j[1] = BiCubicNum(srcy_dec);
			k_j[2] = BiCubicNum(1 - srcy_dec);
			k_j[3] = BiCubicNum(2 - srcy_dec);
			for (int rgb = 0; rgb < 3; rgb++)
			{
				for (int m = 0; m < 4; m++)
				{
					for (int n = 0; n < 4; n++)
					{
						msum += k_i[m] * k_j[n] * srcimage.at<Vec3b>(srcx_int + m + 1, srcy_int + n + 1)[rgb];
					}
				}
				dstimage.at<Vec3b>(i, j)[rgb] = msum;
				msum = 0;
			}

		}
	}
}

void myThread::mTgaussNoiseProcess(Mat &srcimage, int k, int num, int sum)
{
	int rows = srcimage.rows / sum;
	if (srcimage.type() == CV_8UC1)
	{
		for (int i = rows * num; i < rows * (num + 1); i++)
		{
			for (int j = 0; j < srcimage.cols; j++)
			{
				double temp = (srcimage.at<uchar>(i, j) + k * generateGaussianNoise());
				srcimage.at<uchar>(i, j) = (temp > 0 && temp < 255) ? temp : (temp > 255 ? 255 : 0);
			}
		}
	}
	else if (srcimage.type() == CV_8UC3)
	{
		for (int i = rows * num; i < rows * (num + 1); i++)
		{
			for (int j = 0; j < srcimage.cols; j++)
			{
				double temp[3];
				for (int rgb = 0; rgb < 3; rgb++)
				{
					temp[rgb] = srcimage.at<Vec3b>(i, j)[rgb] + k * generateGaussianNoise();
					srcimage.at<Vec3b>(i, j)[rgb] = (temp[rgb] > 0 && temp[rgb] < 255) ? temp[rgb] : (temp[rgb] > 255 ? 255 : 0);
				}
			}
		}
	}
}
