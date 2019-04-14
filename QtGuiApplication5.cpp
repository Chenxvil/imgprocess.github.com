#include "QtGuiApplication4.h"
#include "Transform.h"
#include <ctime>

using namespace cv;
using namespace std;

QtGuiApplication4::QtGuiApplication4(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.ButtonShow->setEnabled(false);
	/*ui.hSliderDegree->setEnabled(false);*/
	ui.hSliderThread->setEnabled(false);
	connect(ui.ButtonOpen, SIGNAL(clicked()), this, SLOT(LoadSourImage()));
	connect(ui.ButtonProcess, SIGNAL(clicked()), this, SLOT(ProcessImage()));
	connect(ui.ButtonSave, SIGNAL(clicked()), this, SLOT(SaveImage()));
	connect(ui.ButtonApply, SIGNAL(clicked()), this, SLOT(ApplyImage()));
	connect(ui.ButtonShow, SIGNAL(clicked()), this, SLOT(ShowImage()));
	connect(ui.combBoxFunction, SIGNAL(currentIndexChanged(int)), this, SLOT(ControlSetting0()));
	connect(ui.combBoxModel, SIGNAL(currentIndexChanged(int)), this, SLOT(ControlSetting1()));
}
void QtGuiApplication4::LoadSourImage()
{
	ui.labelInImage->clear();
	QString fileName = QFileDialog::getOpenFileName(this);
	sourPath = fileName;
	if (fileName.isEmpty())
	{
		ui.textBrowser->append("Error: Open file failed, please check the file path!\n");
		return;
	}
	else
	{
		QImage Img(fileName);
		sourImage = Img.copy();
		ui.labelInImage->setPixmap(QPixmap::fromImage(Img));
	}
}

void QtGuiApplication4::SaveImage()
{
	if (procImage.isNull())
		ui.textBrowser->append("There is no any process, please process the image first!\n");

	QString fileName = QFileDialog::getSaveFileName(this);//QFileDialog::getOpenFileName(this);

	procImage.save(fileName);
}

void QtGuiApplication4::ProcessImage()
{

	int intFunc = ui.combBoxFunction->currentIndex();
	int intMode = ui.combBoxModel->currentIndex();
	int threadSum = 0;
	double runTime;
	QString runPara;
	QImage temp;//= (sourImage.copy())
	Mat matSourImage = imread(sourPath.toStdString());//QImage2cvMat(sourImage);
	if (matSourImage.empty())
	{
		ui.textBrowser->append("Warning: There is currently no input image, please select an image!\n");
		return;
	}
	if (intMode == 0)
	{
		switch (intFunc)
		{
		case 0:
		{
			int temp = ui.hSliderDegree->value()*1.28;
			runTime = clock();
			if (ui.checkBox->isChecked() == true)
				for (int i = 0; i < 99; i++) {
					gaussNoiseProcess(matSourImage, temp);
				}
			gaussNoiseProcess(matSourImage, temp);
			runTime = clock() - runTime;
			runPara = "The current Gaussian coefficient is :";
			runPara.append(QString::number(temp));
			break;
		}
		case 1:
		{
			int temp = ui.hSliderDegree->value() / 5;
			double scale = temp * 0.1 + 0.5;
			runTime = clock();
			if (ui.checkBox->isChecked() == true)
				for (int i = 0; i < 99; i++) {
					ScaleProcess(matSourImage, scale);
				}
			matSourImage = ScaleProcess(matSourImage, scale);
			runTime = clock() - runTime;
			runPara = "The current zoom scale is:";
			runPara.append(QString::number(scale));
			matSourImage.copyTo(realImage);
			break;
		}
		case 2:
		{
			int temp = ui.hSliderDegree->value();
			double degree = temp * 3.6;
			runTime = clock();
			if (ui.checkBox->isChecked() == true)
				for (int i = 0; i < 99; i++)
					RotatProcess(matSourImage, degree);
			matSourImage = RotatProcess(matSourImage, degree);
			runTime = clock() - runTime;
			runPara = "The current rotation degree is:";
			runPara.append(QString::number(degree));
		}
		default:
			break;
		}
	}
	if (intMode == 1)
	{
		threadSum = ui.hSliderThread->value();
		switch (intFunc)
		{
		case 0:
		{
			int temp = ui.hSliderDegree->value()*1.28;
			runTime = clock();
			if (ui.checkBox->isChecked() == true)
				for (int i = 0; i < 99; i++)
					mmTgaussNoiseProcess(matSourImage, temp, threadSum);
			mmTgaussNoiseProcess(matSourImage, temp, threadSum);
			runTime = clock() - runTime;
			runPara = "The current Gaussian coefficient is :";
			runPara.append(QString::number(temp));
			break;
		}
		case 1:
		{
			int temp = ui.hSliderDegree->value() / 5;
			double scale = temp * 0.1 + 0.5;
			runTime = clock();
			if (ui.checkBox->isChecked() == true)
				for (int i = 0; i < 99; i++)
					mmTscaleProcess(matSourImage, scale, threadSum);
			mmTscaleProcess(matSourImage, scale, threadSum);
			runTime = clock() - runTime;
			runPara = "The current zoom factor is:";
			runPara.append(QString::number(scale));
			matSourImage.copyTo(realImage);
			break;
		}
		case 2:
		{
			int temp = ui.hSliderDegree->value();
			double degree = temp * 3.6;
			runTime = clock();
			if (ui.checkBox->isChecked() == true)
				for (int i = 0; i < 99; i++)
					mmTrotatProcess(matSourImage, degree, threadSum);
			mmTrotatProcess(matSourImage, degree, threadSum);
			runTime = clock() - runTime;
			runPara = "The current rotation coefficient is:";
			runPara.append(QString::number(degree));
		}
		default:
			break;
		}
	}
	if (intMode == 2)
	{
		switch (intFunc)
		{

		case 0:
		{
			int temp = ui.hSliderDegree->value()*1.28;
			runTime = clock();

			if (ui.checkBox->isChecked() == true)
			{
				for (int i = 0; i < 99; i++)
					mpgaussNoiseProcess(matSourImage, temp, 5);
			}
			mpgaussNoiseProcess(matSourImage, temp, 5);
			runTime = clock() - runTime;
			runPara = "The current Gaussian coefficient is :";
			runPara.append(QString::number(temp));
			break;
		}


		case 1:
		{
			int temp = ui.hSliderDegree->value() / 5;
			double scale = temp * 0.1 + 0.5;
			runTime = clock();
			if (ui.checkBox->isChecked() == true)
				for (int i = 0; i < 99; i++)
					mpscaleProcess(matSourImage, scale, 5);
			mpscaleProcess(matSourImage, scale, 5);
			runTime = clock() - runTime;
			runPara = "The current zoom scale is:";
			runPara.append(QString::number(scale));
			matSourImage.copyTo(realImage);
			break;
		}
		case 2:
		{
			int temp = ui.hSliderDegree->value();
			double degree = temp * 3.6;
			runTime = clock();
			if (ui.checkBox->isChecked() == true)
				for (int i = 0; i < 99; i++)
					mprotatProcess(matSourImage, degree, 5);
			mprotatProcess(matSourImage, degree, 5);
			runTime = clock() - runTime;
			runPara = "The current rotation degree is:";
			runPara.append(QString::number(degree));
		}
		default:
			break;
		}
	}
	if (intMode == 3)
	{
		Transform tf;
		switch (intFunc)
		{
		case 0:
		{
			int temp = ui.hSliderDegree->value()*1.28;
			runTime = clock();
			if (ui.checkBox->isChecked() == true)
				for (int i = 0; i < 99; i++)
					tf.gaussNoiseProcess(&matSourImage, temp);
			matSourImage = tf.gaussNoiseProcess(&matSourImage, temp);
			runTime = clock() - runTime;
			runPara = "The current Gaussian coefficient is :";
			runPara.append(QString::number(temp));
			break;
		}
		case 1:
		{
			int temp = ui.hSliderDegree->value() / 5;
			double scale = temp * 0.1 + 0.5;
			runTime = clock();
			if (ui.checkBox->isChecked() == true)
				for (int i = 0; i < 99; i++) {
					tf.scaleProcess(&matSourImage, scale);
				}
			matSourImage = tf.scaleProcess(&matSourImage, scale);
			runTime = clock() - runTime;
			runPara = "The current zoom scale is:";
			runPara.append(QString::number(scale));
			matSourImage.copyTo(realImage);
			break;
		}
		case 2:
		{
			int temp = ui.hSliderDegree->value();
			double degree = temp * 3.6;
			runTime = clock();
			if (ui.checkBox->isChecked() == true)
				for (int i = 0; i < 99; i++)
					tf.rotatProcess(&matSourImage, degree);
			matSourImage = tf.rotatProcess(&matSourImage, degree);
			runTime = clock() - runTime;
			runPara = "The current rotation degree is:";
			runPara.append(QString::number(degree));
		}
		default:
			break;
		}
	}
	temp = cvMat2QImage(matSourImage);
	procImage = temp.copy();
	QString s0 = "Information: ";
	s0.append(runPara);
	QString s1 = "Runtime: ";
	s1.append(QString::number(runTime));
	s1.append("ms.");
	ui.textBrowser->append(s0);
	ui.textBrowser->append(s1);
	if (intMode == 1)
	{
		QString s2 = "RunThreadCount: ";
		s2.append(QString::number(threadSum));
		ui.textBrowser->append(s2);
		ui.textBrowser->append("\n");
	}
	else
	{
		ui.textBrowser->append("\n");
	}
	ui.labelOutImage->setPixmap(QPixmap::fromImage(temp));

}

void QtGuiApplication4::ApplyImage()
{
	sourImage = procImage.copy();
	/*imwrite("./temp.jpg",sourImage)
	sourPath = */

	ui.labelInImage->setPixmap(QPixmap::fromImage(sourImage));
	procImage.save("./temp.jpg");
	sourPath = "./temp.jpg";
	ui.labelOutImage->clear();

}

void QtGuiApplication4::ShowImage()
{
	if (realImage.empty())
	{
		ui.textBrowser->append("Warning: Please click the Show Original button after processing!\n");
		return;
	}
	imshow("原图", realImage);
}

void QtGuiApplication4::ControlSetting0()
{
	ui.ButtonShow->setEnabled(false);
	int intFunc = ui.combBoxFunction->currentIndex();
	if (intFunc == 1)
		ui.ButtonShow->setEnabled(true);
}
void QtGuiApplication4::ControlSetting1()
{
	ui.hSliderThread->setEnabled(false);
	int intMode = ui.combBoxModel->currentIndex();
	if (intMode == 1)
		ui.hSliderThread->setEnabled(true);
}

inline double QtGuiApplication4::generateGaussianNoise()
{
	static const double TWO_PI = 6.2831853071795864769252866;
	static double x1, x2;

	double rand1 = rand() / ((double)RAND_MAX);
	double rand2 = rand() / ((double)RAND_MAX);
	if (rand1 < 1e-100) rand1 = 1e-100;
	x1 = sqrt(-2 * log(rand1));
	x2 = cos(rand2 * TWO_PI);

	return x1 * x2;
}
void QtGuiApplication4::gaussNoiseProcess(Mat & src, int k)
{
	if (src.type() == CV_8UC1)
	{
		for (int i = 0; i < src.rows; i++)
		{
			for (int j = 0; j < src.cols; j++)
			{
				double temp = (src.at<uchar>(i, j) + k * generateGaussianNoise());
				src.at<uchar>(i, j) = (temp > 0 && temp < 255) ? temp : (temp > 255 ? 255 : 0);
			}
		}
	}
	else if (src.type() == CV_8UC3)
	{
		for (int i = 0; i < src.rows; i++)
		{
			for (int j = 0; j < src.cols; j++)
			{
				double temp[3];
				for (int rgb = 0; rgb < 3; rgb++)
				{
					temp[rgb] = src.at<Vec3b>(i, j)[rgb] + k * generateGaussianNoise();
					src.at<Vec3b>(i, j)[rgb] = (temp[rgb] > 0 && temp[rgb] < 255) ? temp[rgb] : (temp[rgb] > 255 ? 255 : 0);
				}
			}
		}
	}
}

double QtGuiApplication4::BiCubicNum(double x)
{
	double const a = -0.5;
	double abs_x = abs(x);
	if (abs_x <= 1)
		return  1.5  * pow(abs_x, 3) - 2.5 * x*x + 1;
	if (1 < x && x < 2)
		return -0.5 * pow(abs_x, 3) + 2.5 * x*x - 4 * abs_x + 2;
	return 0;
}
Mat QtGuiApplication4::ScaleProcess(Mat &srcimage, double scale)
{

	int nRows = cvRound(srcimage.rows*scale);
	int nCols = cvRound(srcimage.cols*scale);
	Mat dstimage(nRows, nCols, srcimage.type());

	copyMakeBorder(srcimage, srcimage, 4, 4, 4, 4, BorderTypes::BORDER_REPLICATE);

	double k_i[4], k_j[4];
	double sum = 0, srcx, srcy, srcx_dec, srcy_dec;
	int srcx_int, srcy_int;
	for (int i = 0; i < dstimage.rows; i++)
	{
		for (int j = 0; j < dstimage.cols; j++)
		{
			srcx = i / scale;
			srcy = j / scale;
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
						sum += k_i[m] * k_j[n] * srcimage.at<Vec3b>(srcx_int + m + 1, srcy_int + n + 1)[rgb];
					}
				}
				dstimage.at<Vec3b>(i, j)[rgb] = sum;
				sum = 0;
			}

		}
	}
	Mat mask(srcimage, Rect(4, 4, srcimage.cols - 8, srcimage.rows - 8));
	srcimage = mask.clone();
	return dstimage;
}
Mat QtGuiApplication4::RotatProcess(Mat &srcimage, double degree)
{
	double xi = degree / (180 / PI);
	int nRows = ceil(abs(srcimage.rows*cos(xi)) + abs(srcimage.cols*sin(xi)));
	int nCols = ceil(abs(srcimage.cols*cos(xi)) + abs(srcimage.rows*sin(xi)));
	Mat dstimage(nRows, nCols, srcimage.type());

	copyMakeBorder(srcimage, srcimage, 4, 4, 4, 4, BorderTypes::BORDER_REPLICATE);

	double k_i[4], k_j[4];
	double sum = 0, srcx, srcy, srcx_dec, srcy_dec;
	int srcx_int, srcy_int;
	for (int i = 0; i < dstimage.rows; i++)
	{
		for (int j = 0; j < dstimage.cols; j++)
		{
			srcx = (i - nRows * 0.5) * cos(xi) - (j - nCols * 0.5) * sin(xi) + srcimage.rows * 0.5;
			srcy = (i - nRows * 0.5) * sin(xi) + (j - nCols * 0.5) * cos(xi) + srcimage.cols * 0.5;

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
						// cout<<srcx_int + m + 1<<" "<<srcy_int + n + 1<<endl;
						sum += k_i[m] * k_j[n] * srcimage.at<Vec3b>(srcx_int + m + 1, srcy_int + n + 1)[rgb];
					}
				}
				dstimage.at<Vec3b>(i, j)[rgb] = sum;
				sum = 0;
			}

		}
	}
	Mat mask(srcimage, Rect(4, 4, srcimage.cols - 8, srcimage.rows - 8));
	srcimage = mask.clone();
	return dstimage;
}
//void QtGuiApplication4::TscaleProcess(Mat srcimage, Mat& dstimage, double scale, int num, int sum)
//{
//
//	int nRows = cvRound((srcimage.rows - 8)*scale);
//	int nCols = cvRound((srcimage.cols - 8)*scale);
//	nRows /= sum;
//	double k_i[4], k_j[4];
//	double msum = 0, srcx, srcy, srcx_dec, srcy_dec;
//	int srcx_int, srcy_int;
//	if (srcimage.type() == CV_8UC3)
//	{
//		for (int i = nRows * num; i < nRows * (num + 1); i++)
//		{
//			for (int j = 0; j < nCols; j++)
//			{
//				srcx = i / scale;
//				srcy = j / scale;
//				srcx_int = floor(srcx);
//				srcy_int = floor(srcy);
//				srcx_dec = srcx - srcx_int;
//				srcy_dec = srcy - srcy_int;
//
//
//				k_i[0] = BiCubicNum(1 + srcx_dec);
//				k_i[1] = BiCubicNum(srcx_dec);
//				k_i[2] = BiCubicNum(1 - srcx_dec);
//				k_i[3] = BiCubicNum(2 - srcx_dec);
//
//				k_j[0] = BiCubicNum(1 + srcy_dec);
//				k_j[1] = BiCubicNum(srcy_dec);
//				k_j[2] = BiCubicNum(1 - srcy_dec);
//				k_j[3] = BiCubicNum(2 - srcy_dec);
//				for (int rgb = 0; rgb < 3; rgb++)
//				{
//					for (int m = 0; m < 4; m++)
//					{
//						for (int n = 0; n < 4; n++)
//						{
//							msum += k_i[m] * k_j[n] * srcimage.at<Vec3b>(srcx_int + m + 1, srcy_int + n + 1)[rgb];
//						}
//					}
//					dstimage.at<Vec3b>(i, j)[rgb] = msum;
//					msum = 0;
//				}
//
//			}
//		}
//	}
//	else if (srcimage.type() == CV_8UC1)
//	{
//		for (int i = nRows * num; i < nRows * (num + 1); i++)
//		{
//			for (int j = 0; j < nCols; j++)
//			{
//				srcx = i / scale;
//				srcy = j / scale;
//				srcx_int = floor(srcx);
//				srcy_int = floor(srcy);
//				srcx_dec = srcx - srcx_int;
//				srcy_dec = srcy - srcy_int;
//
//
//				k_i[0] = BiCubicNum(1 + srcx_dec);
//				k_i[1] = BiCubicNum(srcx_dec);
//				k_i[2] = BiCubicNum(1 - srcx_dec);
//				k_i[3] = BiCubicNum(2 - srcx_dec);
//
//				k_j[0] = BiCubicNum(1 + srcy_dec);
//				k_j[1] = BiCubicNum(srcy_dec);
//				k_j[2] = BiCubicNum(1 - srcy_dec);
//				k_j[3] = BiCubicNum(2 - srcy_dec);
//
//				for (int m = 0; m < 4; m++)
//				{
//					for (int n = 0; n < 4; n++)
//					{
//						msum += k_i[m] * k_j[n] * srcimage.at<uchar>(srcx_int + m + 1, srcy_int + n + 1);
//					}
//					dstimage.at<Vec3b>(i, j) = msum;
//					msum = 0;
//				}
//			}
//		}
//	}
//}

void QtGuiApplication4::mpscaleProcess(Mat &image, double scale, int sum)
{
	int nRows = cvRound((image.rows)*scale);
	int nCols = cvRound((image.cols)*scale);
	Mat dstimage(nRows, nCols, image.type());
	copyMakeBorder(image, image, 4, 4, 4, 4, BorderTypes::BORDER_REPLICATE);

#pragma omp parallel for
	for (int i = 0; i < sum; i++)
	{
		//TscaleProcess(image, dstimage, scale, i, sum);
		mThread[i] = new myThread();
		mThread[i]->start();
		mThread[i]->mTscaleProcess(image, dstimage, scale, i, sum);
	}
	for (int i = 0; i < sum; i++)
	{
		mThread[i]->terminate();
		mThread[i]->wait();
		delete mThread[i];
	}
	Mat mask(image, Rect(4, 4, image.cols - 8, image.rows - 8));
	image = mask.clone();
	dstimage.copyTo(image);
}
void QtGuiApplication4::mprotatProcess(Mat &srcimage, double degree, int sum)
{
	double xi = degree / (180 / PI);
	int nRows = ceil(abs(srcimage.rows*cos(xi)) + abs(srcimage.cols*sin(xi)));
	int nCols = ceil(abs(srcimage.cols*cos(xi)) + abs(srcimage.rows*sin(xi)));
	Mat dstimage(nRows, nCols, srcimage.type());

	copyMakeBorder(srcimage, srcimage, 4, 4, 4, 4, BorderTypes::BORDER_REPLICATE);

#pragma omp parallel for
	for (int i = 0; i < sum; i++)
	{
		mThread[i] = new myThread();
		mThread[i]->start();
		mThread[i]->mTrotatProcess(srcimage, dstimage, xi, i, sum);
	}
	for (int i = 0; i < sum; i++)
	{
		mThread[i]->terminate();
		mThread[i]->wait();
		delete mThread[i];
	}
	Mat mask(srcimage, Rect(4, 4, srcimage.cols - 8, srcimage.rows - 8));
	srcimage = mask.clone();
	dstimage.copyTo(srcimage);
}
void QtGuiApplication4::mpgaussNoiseProcess(Mat& image, int k, int sum)
{
#pragma omp parallel for
	for (int i = 0; i < sum; i++)
	{
		mThread[i] = new myThread();
		mThread[i]->start();
		mThread[i]->mTgaussNoiseProcess(image, k, i, sum);
	}
	for (int i = 0; i < sum; i++)
	{
		mThread[i]->terminate();
		mThread[i]->wait();
		delete mThread[i];
	}
}

void QtGuiApplication4::mmTscaleProcess(Mat &image, double scale, int sum)
{
	int nRows = cvRound((image.rows)*scale);
	int nCols = cvRound((image.cols)*scale);
	Mat dstimage(nRows, nCols, image.type());
	copyMakeBorder(image, image, 4, 4, 4, 4, BorderTypes::BORDER_REPLICATE);
	for (int i = 0; i < sum; i++)
	{
		mThread[i] = new myThread();
		//	mThread[i]->setScaleProcess(image, dstimage, scale, i, sum);

	}
	for (int i = 0; i < sum; i++)
	{
		mThread[i]->start();
		mThread[i]->mTscaleProcess(image, dstimage, scale, i, sum);
	}
	for (int i = 0; i < sum; i++)
	{
		mThread[i]->terminate();
		mThread[i]->wait();
		delete mThread[i];
	}
	Mat mask(image, Rect(4, 4, image.cols - 8, image.rows - 8));
	image = mask.clone();
	dstimage.copyTo(image);
}
void QtGuiApplication4::mmTrotatProcess(Mat &srcimage, double degree, int sum)
{
	double xi = degree / (180 / PI);
	int nRows = ceil(abs(srcimage.rows*cos(xi)) + abs(srcimage.cols*sin(xi)));
	int nCols = ceil(abs(srcimage.cols*cos(xi)) + abs(srcimage.rows*sin(xi)));
	Mat dstimage(nRows, nCols, srcimage.type());

	copyMakeBorder(srcimage, srcimage, 4, 4, 4, 4, BorderTypes::BORDER_REPLICATE);

	for (int i = 0; i < sum; i++)
	{
		// mThread[i]->start();
		mThread[i] = new myThread();
	}
	for (int i = 0; i < sum; i++)
	{
		mThread[i]->mTrotatProcess(srcimage, dstimage, xi, i, sum);
	}
	for (int i = 0; i < sum; i++)
	{
		mThread[i]->terminate();
		mThread[i]->wait();
		delete mThread[i];
	}
	Mat mask(srcimage, Rect(4, 4, srcimage.cols - 8, srcimage.rows - 8));
	srcimage = mask.clone();
	dstimage.copyTo(srcimage);
}
void QtGuiApplication4::mmTgaussNoiseProcess(Mat& image, int k, int sum)
{
	for (int i = 0; i < sum; i++)
	{
		// mThread[i]->start();
		mThread[i] = new myThread();
	}
	for (int i = 0; i < sum; i++)
	{
		mThread[i]->mTgaussNoiseProcess(image, k, i, sum);
	}
	for (int i = 0; i < sum; i++)
	{
		mThread[i]->terminate();
		mThread[i]->wait();
		delete mThread[i];
	}
}

QImage QtGuiApplication4::cvMat2QImage(const cv::Mat& mat)
{
	switch (mat.type())
	{
	case CV_32F:
	{
		Mat temp;
		mat.convertTo(temp, CV_8UC2, 255, 0);
		Mat src_imag = Mat::zeros(mat.size(), CV_32FC1);
		Mat planes[] = { mat,src_imag };
		Mat src_merge;
		merge(planes, 2, src_merge);
		QImage image(src_merge.data, src_merge.cols, src_merge.rows, src_merge.step, QImage::Format_RGB888);
		return image.rgbSwapped();
	}
	// 8-bit, 4 channel
	case CV_8UC4:
	{
		QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB32);
		return image;
	}

	// 8-bit, 3 channel
	case CV_8UC3:
	{
		QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
		return image.rgbSwapped();
	}

	// 8-bit, 1 channel
	case CV_8UC1:
	{
		static QVector<QRgb>  sColorTable;
		// only create our color table once
		if (sColorTable.isEmpty())
		{
			for (int i = 0; i < 256; ++i)
				sColorTable.push_back(qRgb(i, i, i));
		}
		QImage image(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
		image.setColorTable(sColorTable);
		return image;
	}

	default:
		qDebug("Image format is not supported: depth=%d and %d channels\n", mat.depth(), mat.channels());
		break;
	}
	return QImage();
}

Mat QtGuiApplication4::QImage2cvMat(QImage image)
{
	switch (image.format())
	{
		// 8-bit, 4 channel
	case QImage::Format_RGB32:
	{
		return cv::Mat(image.height(), image.width(), CV_8UC4, const_cast<uchar*>(image.bits()), image.bytesPerLine()).clone();
		// mat;
	}

	// 8-bit, 3 channel
	case QImage::Format_RGB888:
	{
		//由于Qimage 和mat RGB不一样 需要转换  QImage是rgb排列而Mat是bgr排列
		QImage swapped = image.rgbSwapped();
		return cv::Mat(swapped.height(), swapped.width(), CV_8UC3, const_cast<uchar*>(swapped.bits()), swapped.bytesPerLine()).clone();
	}

	// 8-bit, 1 channel
	case QImage::Format_Indexed8:
	{
		cv::Mat  mat(image.height(), image.width(), CV_8UC1, const_cast<uchar*>(image.bits()), image.bytesPerLine());

		return mat;
	}

	default:
		qDebug("Image format error: depth=%d , %d format\n", image.depth(), image.format());
		break;
	}

	return Mat();
}

