/********************************************************************************
** Form generated from reading UI file 'QtGuiApplication5.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTGUIAPPLICATION5_H
#define UI_QTGUIAPPLICATION5_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtGuiApplication5Class
{
public:
    QWidget *centralWidget;
    QCheckBox *checkBox;
    QSlider *hSliderDegree;
    QLabel *label;
    QPushButton *ButtonProcess;
    QLabel *labelThread;
    QPushButton *ButtonShow;
    QLabel *labelInImage;
    QComboBox *combBoxFunction;
    QLabel *labeThreadCount;
    QPushButton *ButtonSave;
    QLabel *labelOutImage;
    QLabel *labelThread_2;
    QComboBox *combBoxModel;
    QSlider *hSliderThread;
    QPushButton *ButtonApply;
    QTextBrowser *textBrowser;
    QPushButton *ButtonOpen;

    void setupUi(QMainWindow *QtGuiApplication5Class)
    {
        if (QtGuiApplication5Class->objectName().isEmpty())
            QtGuiApplication5Class->setObjectName(QStringLiteral("QtGuiApplication5Class"));
        QtGuiApplication5Class->resize(812, 519);
        centralWidget = new QWidget(QtGuiApplication5Class);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        checkBox = new QCheckBox(centralWidget);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setGeometry(QRect(569, 11, 121, 16));
        QFont font;
        font.setPointSize(11);
        checkBox->setFont(font);
        hSliderDegree = new QSlider(centralWidget);
        hSliderDegree->setObjectName(QStringLiteral("hSliderDegree"));
        hSliderDegree->setGeometry(QRect(56, 52, 101, 20));
        hSliderDegree->setMinimum(1);
        hSliderDegree->setMaximum(100);
        hSliderDegree->setSingleStep(1);
        hSliderDegree->setOrientation(Qt::Horizontal);
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(-3, -8, 851, 531));
        label->setStyleSheet(QStringLiteral("background-image: url(:/QtGuiApplication5/1.jpg);"));
        ButtonProcess = new QPushButton(centralWidget);
        ButtonProcess->setObjectName(QStringLiteral("ButtonProcess"));
        ButtonProcess->setGeometry(QRect(710, 5, 85, 28));
        QFont font1;
        font1.setPointSize(10);
        ButtonProcess->setFont(font1);
        labelThread = new QLabel(centralWidget);
        labelThread->setObjectName(QStringLiteral("labelThread"));
        labelThread->setGeometry(QRect(388, 12, 45, 17));
        QFont font2;
        font2.setPointSize(9);
        labelThread->setFont(font2);
        ButtonShow = new QPushButton(centralWidget);
        ButtonShow->setObjectName(QStringLiteral("ButtonShow"));
        ButtonShow->setEnabled(true);
        ButtonShow->setGeometry(QRect(530, 48, 80, 28));
        ButtonShow->setFont(font1);
        labelInImage = new QLabel(centralWidget);
        labelInImage->setObjectName(QStringLiteral("labelInImage"));
        labelInImage->setGeometry(QRect(10, 88, 385, 300));
        labelInImage->setStyleSheet(QStringLiteral("background-color: rgb(135, 253, 255);"));
        labelInImage->setScaledContents(true);
        combBoxFunction = new QComboBox(centralWidget);
        combBoxFunction->addItem(QString());
        combBoxFunction->addItem(QString());
        combBoxFunction->addItem(QString());
        combBoxFunction->setObjectName(QStringLiteral("combBoxFunction"));
        combBoxFunction->setGeometry(QRect(110, 8, 115, 24));
        labeThreadCount = new QLabel(centralWidget);
        labeThreadCount->setObjectName(QStringLiteral("labeThreadCount"));
        labeThreadCount->setGeometry(QRect(550, 10, 16, 16));
        QFont font3;
        font3.setPointSize(12);
        font3.setBold(false);
        font3.setUnderline(false);
        font3.setWeight(50);
        labeThreadCount->setFont(font3);
        ButtonSave = new QPushButton(centralWidget);
        ButtonSave->setObjectName(QStringLiteral("ButtonSave"));
        ButtonSave->setGeometry(QRect(710, 48, 85, 28));
        ButtonSave->setFont(font1);
        labelOutImage = new QLabel(centralWidget);
        labelOutImage->setObjectName(QStringLiteral("labelOutImage"));
        labelOutImage->setGeometry(QRect(415, 88, 385, 300));
        labelOutImage->setStyleSheet(QStringLiteral("background-color:rgb(255, 2, 57)"));
        labelOutImage->setScaledContents(true);
        labelThread_2 = new QLabel(centralWidget);
        labelThread_2->setObjectName(QStringLiteral("labelThread_2"));
        labelThread_2->setGeometry(QRect(10, 54, 45, 17));
        labelThread_2->setFont(font);
        combBoxModel = new QComboBox(centralWidget);
        combBoxModel->addItem(QString());
        combBoxModel->addItem(QString());
        combBoxModel->addItem(QString());
        combBoxModel->addItem(QString());
        combBoxModel->setObjectName(QStringLiteral("combBoxModel"));
        combBoxModel->setGeometry(QRect(250, 8, 115, 24));
        hSliderThread = new QSlider(centralWidget);
        hSliderThread->setObjectName(QStringLiteral("hSliderThread"));
        hSliderThread->setGeometry(QRect(442, 10, 100, 19));
        hSliderThread->setMinimum(1);
        hSliderThread->setMaximum(8);
        hSliderThread->setPageStep(8);
        hSliderThread->setOrientation(Qt::Horizontal);
        ButtonApply = new QPushButton(centralWidget);
        ButtonApply->setObjectName(QStringLiteral("ButtonApply"));
        ButtonApply->setGeometry(QRect(620, 48, 85, 28));
        ButtonApply->setFont(font1);
        textBrowser = new QTextBrowser(centralWidget);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        textBrowser->setGeometry(QRect(10, 398, 791, 111));
        textBrowser->setFont(font);
        textBrowser->setStyleSheet(QStringLiteral("background-color: rgb(219, 245, 255);"));
        ButtonOpen = new QPushButton(centralWidget);
        ButtonOpen->setObjectName(QStringLiteral("ButtonOpen"));
        ButtonOpen->setGeometry(QRect(10, 7, 75, 27));
        ButtonOpen->setFont(font1);
        QtGuiApplication5Class->setCentralWidget(centralWidget);
        label->raise();
        checkBox->raise();
        hSliderDegree->raise();
        ButtonProcess->raise();
        labelThread->raise();
        ButtonShow->raise();
        labelInImage->raise();
        combBoxFunction->raise();
        labeThreadCount->raise();
        ButtonSave->raise();
        labelOutImage->raise();
        labelThread_2->raise();
        combBoxModel->raise();
        hSliderThread->raise();
        ButtonApply->raise();
        textBrowser->raise();
        ButtonOpen->raise();

        retranslateUi(QtGuiApplication5Class);

        QMetaObject::connectSlotsByName(QtGuiApplication5Class);
    } // setupUi

    void retranslateUi(QMainWindow *QtGuiApplication5Class)
    {
        QtGuiApplication5Class->setWindowTitle(QApplication::translate("QtGuiApplication5Class", "QtGuiApplication5", nullptr));
        checkBox->setText(QApplication::translate("QtGuiApplication5Class", "\345\276\252\347\216\257\345\244\204\347\220\206100\346\254\241", nullptr));
        label->setText(QString());
        ButtonProcess->setText(QApplication::translate("QtGuiApplication5Class", "\345\244\204\347\220\206", nullptr));
        labelThread->setText(QApplication::translate("QtGuiApplication5Class", "<html><head/><body><p align=\"center\"><span style=\" font-size:11pt;\">\347\272\277\347\250\213\346\225\260</span></p></body></html>", nullptr));
        ButtonShow->setText(QApplication::translate("QtGuiApplication5Class", "\346\230\276\347\244\272\345\216\237\345\233\276", nullptr));
        labelInImage->setText(QApplication::translate("QtGuiApplication5Class", "<html><head/><body><p align=\"center\">\345\210\235\345\247\213\345\233\276\347\211\207</p></body></html>", nullptr));
        combBoxFunction->setItemText(0, QApplication::translate("QtGuiApplication5Class", "\351\253\230\346\226\257\345\231\252\345\243\260", nullptr));
        combBoxFunction->setItemText(1, QApplication::translate("QtGuiApplication5Class", "\345\233\276\345\203\217\347\274\251\346\224\276", nullptr));
        combBoxFunction->setItemText(2, QApplication::translate("QtGuiApplication5Class", "\345\233\276\345\203\217\346\227\213\350\275\254", nullptr));

        labeThreadCount->setText(QApplication::translate("QtGuiApplication5Class", "1", nullptr));
        ButtonSave->setText(QApplication::translate("QtGuiApplication5Class", "\344\277\235\345\255\230", nullptr));
        labelOutImage->setText(QApplication::translate("QtGuiApplication5Class", "<html><head/><body><p align=\"center\">\345\244\204\347\220\206\345\220\216\347\232\204\345\233\276\347\211\207</p></body></html>", nullptr));
        labelThread_2->setText(QApplication::translate("QtGuiApplication5Class", "<html><head/><body><p align=\"center\"><span style=\" font-size:11pt;\">\345\217\202\346\225\260</span></p></body></html>", nullptr));
        combBoxModel->setItemText(0, QApplication::translate("QtGuiApplication5Class", "\345\215\225\347\272\277\347\250\213\346\250\241\345\274\217", nullptr));
        combBoxModel->setItemText(1, QApplication::translate("QtGuiApplication5Class", "\345\244\232\347\272\277\347\250\213\346\250\241\345\274\217", nullptr));
        combBoxModel->setItemText(2, QApplication::translate("QtGuiApplication5Class", "OpenMP\346\250\241\345\274\217", nullptr));
        combBoxModel->setItemText(3, QApplication::translate("QtGuiApplication5Class", "OpenCL\346\250\241\345\274\217", nullptr));

        ButtonApply->setText(QApplication::translate("QtGuiApplication5Class", "\345\272\224\347\224\250", nullptr));
        ButtonOpen->setText(QApplication::translate("QtGuiApplication5Class", "\346\211\223\345\274\200\346\226\207\344\273\266", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtGuiApplication5Class: public Ui_QtGuiApplication5Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTGUIAPPLICATION5_H
