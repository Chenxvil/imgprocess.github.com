#pragma once

#include <QtWidgets/QMainWindow>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QTime>
#include <QDir>
#include <QFile>
#include "myThread.h"
#include "ui_QtGuiApplication5.h"
using namespace cv;
using namespace std;
static const double PI = 3.14159265358979323846;

class QtGuiApplication4 : public QMainWindow
{
	Q_OBJECT

public:
	QtGuiApplication4(QWidget *parent = Q_NULLPTR);

private slots:
	void LoadSourImage();
	void ProcessImage();
	void ApplyImage();
	void ShowImage();
	void SaveImage();
	void ControlSetting0();
	void ControlSetting1();
	//void TscaleProcess(Mat srcimage, Mat& dstimage, double scale, int num, int sum);
private:
	Ui::QtGuiApplication5Class ui;
	QImage sourImage;
	QImage procImage;
	QString sourPath;
	Mat realImage;
	myThread *mThread[8];
	QImage cvMat2QImage(const Mat& src);
	Mat QImage2cvMat(QImage src);
	void gaussNoiseProcess(Mat& src, int k);
	Mat ScaleProcess(Mat &srcimage, double scale);
	Mat RotatProcess(Mat &srcimage, double degree);
	inline double generateGaussianNoise();
	inline double BiCubicNum(double x);
	void mpscaleProcess(Mat &image, double scale, int sum);
	void mprotatProcess(Mat &srcimage, double degree, int sum);
	void mpgaussNoiseProcess(Mat& image, int k, int sum);
	void mmTscaleProcess(Mat &image, double scale, int sum);
	void mmTrotatProcess(Mat &srcimage, double degree, int sum);
	void mmTgaussNoiseProcess(Mat& image, int k, int sum);
};
